#pragma once

#include "Common.h"
#include "Message.h"

#include "string.h"

namespace R2
{
	class NetworkClient
	{
	// public:
	// 	inline static Client& Instance()
	// 	{
	// 		static Client s;
	// 		return s;
	// 	}	

	private:
		string host;
		int port;

	public:
		NetworkClient(const string& host_, int port_)
		{
			host = host_, port = port_;
		}

		void sendMessage(const unsigned char* data, size_t len, Message& reply) const
		{
			// open
			int sock = 0, valread; 
		    struct sockaddr_in serv_addr; 
		    char *hello = "Hello from client"; 
		    unsigned char buffer[1024] = {0}; 
		    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		    { 
		        throw Exception("Socket creation error"); 
		    } 
		    //
		    serv_addr.sin_family = AF_INET; 
		    serv_addr.sin_port = htons(PORT); 
		    // 
		    // Convert IPv4 and IPv6 addresses from text to binary form 
		    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
		    { 
		        throw Exception("Invalid address/ Address not supported"); 
		    } 
		   
		    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
		    { 
		        throw Exception("Connection failed"); 
		    } 
		    //
			Message msg;
			msg.attachBlob(data, len);
			while (true)
			{
				unsigned char buf[1024];
				auto bytesSent = msg.send(1024, buf);
			    send(sock, buf, bytesSent, 0 ); 
				if (msg.transferCompleted())
					break;
			}
			//
			// Message reply;
		    valread = read(sock, buffer, 1024); 
			reply.startReceiving(buffer);
			reply.receive(1024, buffer);
		    // cout << string((const char*)reply.binary()) << endl; 
			
			close(sock);
		}
	};
}