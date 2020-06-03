#pragma once

#include <string>

class Server;

class ClientSocket {
 public:
  ClientSocket(int fileDescriptor, Server &server);
  ~ClientSocket();

  int FileDescriptor() const;

  void SetPID(size_t pid);
  size_t GetPID() const;

  void Close();

  void Write(const std::string &data);
  std::string Read();

  ClientSocket(const ClientSocket &) = delete;
  ClientSocket &operator=(const ClientSocket &) = delete;

 private:
  int _fileDescriptor = -1;
  size_t _pid = -1;
  Server &_server;
};