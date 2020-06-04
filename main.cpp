
#include <iostream>
#include <vector>

#include "server/client_socket.h"
#include "server/server.h"
#include "server/socket_stream.h"
#include "text_processor.h"

int main(int argc, char const* argv[]) {
  int port = 7897;
  if (argc > 1) port = std::stoi(argv[1]);

  TextProcessor tp;

  Server server;
  server.SetPort(port);

  server.OnAccept([&](std::weak_ptr<ClientSocket> socket) {
    if (auto s = socket.lock()) {
      SocketStream is(s->FileDescriptor());
      SocketStream os(s->FileDescriptor());
    //   std::string key;
    //   is >> key;
    //   os << "ответ";
      if (/* какая-то проблема */ 0) s->Close();
    }
  });

  server.OnRead([&](std::weak_ptr<ClientSocket> socket) {
    if (auto s = socket.lock()) {
      SocketStream is(s->FileDescriptor());
      SocketStream os(s->FileDescriptor());
      std::string query;
      // is == add 4 "some text"
      is >> query;
      // is == 4 "some text"
      try {
        if (query == "show") {
          os << tp.Show();
        } else if (query == "add") {
          // считать остальное и добавить
          int n;
          is >> n;
          std::string text;  //
          std::getline(is, text, '"');
          std::getline(is, text, '"');
          tp.Add(n, text);
          os << "Done";
        } else if (query == "search") {
          int N;
          std::string word;
          is >> N;
          is >> word;
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