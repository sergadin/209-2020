#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

class ClientSocket;

class Server {
 public:
  ~Server();

  void SetBacklog(int backlog);
  void SetPort(int port);

  void Close();
  void Listen();

  template <class F>
  void OnAccept(F &&f) {
    _handleAccept = f;
  }

  template <class F>
  void OnRead(F &&f) {
    _handleRead = f;
  }

  void Close(int fileDescriptor);

 private:
  int _backlog = 1;
  int _port = -1;
  int _socket = -1;

  std::function<void(std::weak_ptr<ClientSocket> socket)> _handleAccept;
  std::function<void(std::weak_ptr<ClientSocket> socket)> _handleRead;

  std::vector<std::shared_ptr<ClientSocket>> _clientSockets;

  std::vector<int> _staleFileDescriptors;
  std::mutex _staleFileDescriptorsMutex;
};
