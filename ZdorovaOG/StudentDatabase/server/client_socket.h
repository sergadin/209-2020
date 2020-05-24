/**
 * @file
 * @brief      Заголовочный файл с описанием класса ClientSocket
 *
 * Данный файл содержит в себе описание класса ClientSocket, назначение
 * которого в передаче данных от и к клиенту.
 * Это файл из репозитория github.com/Pseudomanifold/SimpleServer со
 * стилистическим рефакторингом.
 */
#pragma once

#include <string>

class Server;

class ClientSocket {
 public:
  ClientSocket(int fileDescriptor, Server &server);
  ~ClientSocket();

  int fileDescriptor() const;

  void Close();
  void Write(const std::string &data);

  std::string Read();

  ClientSocket(const ClientSocket &) = delete;
  ClientSocket &operator=(const ClientSocket &) = delete;

 private:
  int _fileDescriptor = -1;
  Server &_server;
};