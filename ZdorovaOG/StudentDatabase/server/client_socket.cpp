#include "client_socket.h"

#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>

#include "server.h"

ClientSocket::ClientSocket(int fileDescriptor, Server &server)
    : _fileDescriptor(fileDescriptor), _server(server) {}

ClientSocket::~ClientSocket() {}

int ClientSocket::fileDescriptor() const { return _fileDescriptor; }

void ClientSocket::Close() { _server.Close(_fileDescriptor); }

void ClientSocket::Write(const std::string &data) {
  auto data_ = data + "\7";
  auto result =
      send(_fileDescriptor, reinterpret_cast<const void *>(data_.c_str()),
           data_.size(), 0);

  if (result == -1) throw std::runtime_error(std::string(strerror(errno)));
}

std::string ClientSocket::Read() {
  std::string message;

  char buffer[256] = {0};
  ssize_t numBytes = 0;

  while (true) {
    numBytes = recv(_fileDescriptor, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (message.size() > 0 && numBytes == -1) {
      std::cerr << "Socket error" << std::endl;
      break;
    }
    buffer[numBytes] = 0;
    message += buffer;
    if (message.size() > 0 && message.back() == '\7')
      break;
  }
  message.pop_back();
  return message;
}
