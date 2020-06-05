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

int ClientSocket::FileDescriptor() const { return _fileDescriptor; }

void ClientSocket::Close() { _server.Close(_fileDescriptor); }

void ClientSocket::Write(const std::string &data) {
  auto result =
      send(_fileDescriptor, reinterpret_cast<const void *>(data.c_str()),
           data.size(), 0);

  if (result == -1) throw std::runtime_error(std::string(strerror(errno)));
}

std::string ClientSocket::Read() {
  std::string message;

  char buffer[256];
  memset(buffer, 0, 256);
  ssize_t num_bytes = 0;

  while ((num_bytes = recv(_fileDescriptor, buffer, sizeof(buffer),
                           MSG_DONTWAIT)) > 0) {
    buffer[num_bytes] = 0;
    message += buffer;
  }
  return message;
}