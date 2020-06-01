#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "config/config.h"
#include "database/database.h"
#include "server/client_socket.h"
#include "server/server.h"
#include "server/socket_stream.h"

using Status = DataBase::Status;

int main(int argc, char const* argv[]) {
  int port = DEFAULT_PORT;
  if (argc > 1) port = std::stoi(argv[1]);

  DataBase db;
  db.Load("__database.csv");
  db.SetDelim(',');

  Server server;
  server.SetPort(port);

  server.OnAccept([&](std::weak_ptr<ClientSocket> socket) {
    if (auto s = socket.lock()) {
      SocketStream is(s->FileDescriptor());
      SocketStream os(s->FileDescriptor());
      std::string key;
      is >> key;
      if (key == "Xo4y_3a4eT") {
        os << "+\7" << std::flush;
        auto pid = db.RegisterUser();
        s->SetPID(pid);
      } else {
        os << "Error: incorrect key" << std::flush;
        s->Close();
      }
    }
  });
  server.OnRead([&](std::weak_ptr<ClientSocket> socket) {
    if (auto s = socket.lock()) {
      SocketStream is(s->FileDescriptor());
      SocketStream os(s->FileDescriptor());
      Status status = Status::Ok;
      try {
        status = db.Process(is, os, s->GetPID());
      } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        os << "Error:\n" << ex.what() << '\n';
      }
      while (is.get() != '\7')
        ;
      os << '\7' << std::flush;
      if (status != Status::Ok) s->Close();
      if (status == Status::Shutdown) {
        exit(0);
      }
    }
  });
  std::cout << "Server started" << std::endl;
  try {
    server.Listen();
  } catch (std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }
  return 0;
}