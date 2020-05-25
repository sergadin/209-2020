#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "config/config.h"
#include "database/database.h"
#include "server/client_socket.h"
#include "server/server.h"

enum QueryType { NewUser = 0, CloseDB = 1, Request = 2 };

QueryType GetType(std::istream& is) {
  std::string s;
  std::getline(is, s);
  if (s == "new_user") return QueryType::NewUser;
  if (s == "close_db") return QueryType::CloseDB;
  return QueryType::Request;
}

int GetUID(std::istream& is) {
  std::string s;
  std::getline(is, s);
  return std::stoi(s);
}

int main(int argc, char const* argv[]) {
  int port = DEFAULT_PORT;
  if (argc > 1) port = std::stoi(argv[1]);

  std::map<int, DataBase> dbs;
  int max_users = 0;

  Server server;
  server.SetPort(port);

  server.OnAccept([&](std::weak_ptr<ClientSocket> socket) {
    if (auto s = socket.lock()) {
      std::stringstream is(s->Read());
      std::stringstream os;

      auto qtype = GetType(is);
      switch (qtype) {
        case QueryType::NewUser: {
          auto& db = dbs[max_users];
          db.Load("__database.csv");
          db.SetDelim(',');
          s->Write(std::to_string(max_users));
          ++max_users;
          break;
        }
        case QueryType::CloseDB: {
          auto uid = GetUID(is);
          dbs.erase(uid);
          break;
        }
        case QueryType::Request: {
          auto uid = GetUID(is);
          auto& db = dbs[uid];
          try {
            while (is.peek() != EOF) {
              db.Process(is, os);
            }
            s->Write(os.str());
          } catch (std::exception& ex) {
            s->Write("Error:\n" + std::string(ex.what()));
          }
          break;
        }
        default:
          s->Write(
              "Something really bad happens. PLease contact your IT "
              "administrator.");
          s->Close();
          throw std::logic_error("Unexpected request type");
      }
      s->Close();
    }
  });
  server.Listen();
  return 0;
}