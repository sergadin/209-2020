#include <iostream>
#include <sstream>
#include <vector>

#include "server/client_socket.h"
#include "server/server.h"
#include "text_processor.h"

int main(int argc, char const* argv[]) {
  int port = 7897;
  if (argc > 1) port = std::stoi(argv[1]);

  TextProcessor tp;

  Server server;
  server.SetPort(port);

  server.OnAccept([&](std::weak_ptr<ClientSocket> socket) {
    if (auto s = socket.lock()) {
      // SocketStream os(s->FileDescriptor());
      // os << '1' << std::flush;
      s->Write("1");
    }
  });

  server.OnRead([&](std::weak_ptr<ClientSocket> socket) {
    if (auto s = socket.lock()) {
      std::stringstream is(s->Read());
      std::stringstream os;
      std::string query;
      // is == add 4 "some text"
      is >> query;
      // is == 4 "some text"
      try {
        if (query == "show") {
          os << tp.Show();
        } else if (query == "add") {
          int n;
          is >> n;
          std::string text;
          std::getline(is, text, '"');
          std::getline(is, text, '"');
          tp.Add(n, text);
          os << "Done";
        } else if (query == "search") {
          int N;
          std::string word;
          is >> N;
          std::getline(is, word, '"');
          std::getline(is, word, '"');
          tp.Search(word, N, os);
        } else if (query == "delete") {
          std::set<int> ids;
          std::string str;
          is >> str;
          while (str != "end") {
            ids.insert(std::stoi(str));
            is >> str;
          }
          tp.Delete(ids);
          os << "Done";
        } else if (query == "disconnect") {
          s->Close();
        } else if (query == "SHUTDOWN") {
          exit(0);
        } else {
          os << "RTFM!";
        }
      } catch (std::exception& ex) {
        os << ex.what();
      }
      os << "__4to-to__" << std::flush;
      s->Write(os.str());
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
