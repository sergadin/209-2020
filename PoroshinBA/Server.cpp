#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "Headbase.h"
#include "server/client_socket.h"
#include "server/server.h"
#include "server/socket_stream.h"

int main (void) {
  int port = 55432;

  DataBase db;
  db.Load("__database.csv", 0);

  Server server;
  server.SetPort(port);

  server.OnAccept([&](std::weak_ptr<ClientSocket> socket) {
    if (auto s = socket.lock()) {
      SocketStream is(s->FileDescriptor());
      SocketStream os(s->FileDescriptor());
      while (is.get() != '\7');
      os << "Connected\7" << std::flush;
      s->SetPID(db.RegisterUser());
    }
  });
  server.OnRead([&](std::weak_ptr<ClientSocket> socket) {
    if (auto s = socket.lock()) {
      SocketStream is(s->FileDescriptor());
      SocketStream os(s->FileDescriptor());
      int status = 0;
      try {
        status = db.Process(is, os, s->GetPID());
      } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        os << "Error:\n" << ex.what() << '\n';
      }
      while (is.get() != '\7');
      os << '\7' << std::flush;
      if (status != 0) s->Close();
      if (status == -1) {
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