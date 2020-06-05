#pragma once

#include <string>
#include <vector>
#include "Common.h"
#include "Message.h"
#include "NetworkClient.h"
#include "RecordSet.h"

namespace R2 
{
	using namespace std;

	class Client
	{
		NetworkClient nc;

	public:
		Client(const string& host_, int port_) : nc(host_, port_)
		{}

		inline RecordSet query(const string& q) const
		{
			Message reply;
			// unsigned char *p = new unsigned char[q.size()+1];
			// for (size_t j = 0; j <= q.size(); ++j)
			// 		p[j] = q[j];
			cout << q << endl;
			nc.sendMessage((const unsigned char*)q.c_str(), q.size()+1, reply);
			//
			cout << reply.binary() << endl;
			//
			return RecordSet(reply);
		}
	};

} // R2