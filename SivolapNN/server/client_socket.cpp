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

  num_bytes = recv(_fileDescriptor, buffer, 4, MSG_WAITALL);
  int length = static_cast<int>(static_cast<unsigned char>(buffer[0]) << 24 |
                                static_cast<unsigned char>(buffer[1]) << 16 |
                                static_cast<unsigned char>(buffer[2]) << 8 |
                                static_cast<unsigned char>(buffer[3]));
  memset(buffer, 0, 4);

  while (length != 0) {
    num_bytes =
        recv(_fileDescriptor, buffer,
             (length < sizeof(buffer) ? length : sizeof(buffer)), MSG_WAITALL);
    if (num_bytes < 0) throw std::runtime_error("Socket problems");
    buffer[num_bytes] = 0;
    length -= num_bytes;
    message += buffer;
  }
  return message;
}
