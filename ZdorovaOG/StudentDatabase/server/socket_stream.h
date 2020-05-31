#pragma once
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <istream>
#include <ostream>
#include <streambuf>

template <typename Char>
class basic_socketbuf : public std::basic_streambuf<Char> {
 public:
  typedef Char char_type;
  typedef std::basic_streambuf<char_type> buf_type;
  typedef std::basic_ostream<char_type> stream_type;
  typedef typename buf_type::int_type int_type;
  typedef typename std::basic_streambuf<Char>::traits_type traits_type;

 protected:
  static const int char_size = sizeof(char_type);
  static const int SIZE = 256;
  char_type obuf[SIZE];
  char_type ibuf[SIZE];

  int sock;

 public:
  basic_socketbuf() : sock(0) {
    buf_type::setp(obuf, obuf + (SIZE - 1));
    buf_type::setg(ibuf, ibuf, ibuf);
  }

  virtual ~basic_socketbuf() { sync(); }

  void set_socket(int sock) { this->sock = sock; }
  int get_socket() { return this->sock; }

 protected:
  int output_buffer() {
    int num = buf_type::pptr() - buf_type::pbase();
    if (send(sock, reinterpret_cast<char*>(obuf), num * char_size, 0) != num)
      return traits_type::eof();
    buf_type::pbump(-num);
    return num;
  }

  virtual int_type overflow(int_type c) {
    if (c != traits_type::eof()) {
      *buf_type::pptr() = c;
      buf_type::pbump(1);
    }

    if (output_buffer() == traits_type::eof()) return traits_type::eof();
    return c;
  }

  virtual int sync() {
    if (output_buffer() == traits_type::eof()) return traits_type::eof();
    return 0;
  }

  virtual int_type underflow() {
    if (buf_type::gptr() < buf_type::egptr()) return *buf_type::gptr();

    int num;
    if ((num = recv(sock, reinterpret_cast<char*>(ibuf), SIZE * char_size,
                    0)) <= 0)
      return traits_type::eof();

    buf_type::setg(ibuf, ibuf, ibuf + num);
    return *buf_type::gptr();
  }
};

typedef basic_socketbuf<char> SocketBuf;
typedef basic_socketbuf<wchar_t> wsocketbuf;

template <typename Char>
class basic_socketstream : public std::basic_iostream<Char> {
 public:
  typedef Char char_type;
  typedef std::basic_iostream<char_type> stream_type;
  typedef basic_socketbuf<char_type> buf_type;

 protected:
  buf_type buf;

 public:
  basic_socketstream() : stream_type(&buf) {}
  basic_socketstream(int s) : stream_type(&buf) { buf.set_socket(s); }
};

typedef basic_socketstream<char> SocketStream;
typedef basic_socketstream<wchar_t> wsocketstream;