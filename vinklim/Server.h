#pragma once

#include "Common.h"
#include "Message.h"
#include "TimetableDB.h"

namespace R2
{
	using namespace std;

	class Server
	{
		string dbPath;

	protected:
		// Server attributes

		void init(const string& dbPath)
		{
			this->dbPath = dbPath;
		}

		// Database

		string processRequest(const string& query, const string& dbPath) const
		{
			auto r = TimetableDB::Instance().processRequest(query, dbPath).serialize();
			// cout << "%" << endl;
			return r;
		}

		void readDatabase() const
		{

		}

		void saveDatabase() const
		{

		}

		// Networking

		void mainLoop() const
		{
			int listenfd, new_socket, valread; 
		    struct sockaddr_in address; 
		    int opt = 1; 
		    int addrlen = sizeof(address); 
		    unsigned char buffer[1024]; 
		    buffer[0] = 0;
		    // char *hello = "Hello from server!"; 
			//		       
		    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
		    { 
		        throw Exception("socket failed"); 
		    } 
		    //		       
		    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
		                                                  &opt, sizeof(opt))) 
		    { 
		        throw Exception("setsockopt"); 
		    } 
		    //
		    address.sin_family = AF_INET; 
		    address.sin_addr.s_addr = INADDR_ANY; 
		    address.sin_port = htons( PORT ); 
			//		       
		    if (bind(listenfd, (struct sockaddr *)&address, sizeof(address)) < 0) 
		    { 
		        throw Exception("bind failed"); 
		    } 
		    if (listen(listenfd, 10) < 0) 
		    { 
		        throw Exception("listen command failed"); 
		    } 
		    while (true)
		    {
		    	cout << "listening..." << endl;
			    new_socket = accept(listenfd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
			    if (new_socket < 0) 
			    { 
			        throw Exception("accept command failed"); 
			    } 
			    //
			    Message M;
			    //
			    bool starting = true;
			    int it = 0;
			    while (true)
			    {
			    	// cout << "S:" << it << endl;
			    	it++;
				    valread = read(new_socket, buffer, 1024);
				    if (valread == 0)
				    	break;
				    if (starting)
				    {
				    	M.startReceiving(buffer);
				    	starting = false;
				    	// cout << "msg length = " << M.length() << "   ( " << valread << " )" << endl;
				    	// cout << buffer << endl;
				    }
				    // cout << "[" << endl;
				    M.receive(1024, buffer);
				    // cout << "]" << endl;
				    if (M.transferCompleted())
				    	break;
				}
			    //
			    // cout << string((const char*)M.binary()) << endl; 
			    string q((const char*)M.binary());
			    // cout << q << endl << "^" << endl;
			    //
			    auto replyStr = processRequest(q, dbPath);
			    // cout << "!" << endl;
			    //
			    // string s = "Message received @ server!";
			    // for (int j = 0; j <= s.size(); ++j)
			    // 	buffer[j] = s[j];
			    //
			    // cout << "...replying..." << endl;
			    Message reply;
				reply.attachBlob((const unsigned char*)replyStr.c_str(), replyStr.size()+1);
				while (true)
				{
					unsigned char buf[1024];
					auto bytesSent = reply.send(1024, buf);
					// cout << "( " << bytesSent << ")" << endl;
				    send(new_socket, buf, bytesSent, 0);
				    // cout << "sending " << bytesSent << " bytes" << endl;
					if (reply.transferCompleted())
						break;
				}
			    // send(new_socket, hello, 18, 0); 
			    // printf("Hello message sent\n"); 
			    close(new_socket);
			}
		}

		inline int ntwPORT() const { return 8888; }

		// Misc

		void logMessage(const string& msg) const
		{
			consoleLogMessage(msg);
		}

		void logError(const string& msg) const
		{
			logMessage("Error: " + msg);
		}

		void logWarning(const string& msg) const
		{
			logMessage("Warning: " + msg);
		}

		void consoleLogMessage(const string& msg) const
		{
			cout << msg << endl;
		}

	public:
		void ConnectDatabase(const string& dbPath)
		{
			init(dbPath);
		}

		void Start() const
		{
			mainLoop();
		}

		string status() const
		{
			return "Ok";
		}
	};

} // R2

/*Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
//	cout << "multiplying [" << lhs.GetN() << "x" << lhs.GetM() << "] * [" << rhs.GetN() << "x" << rhs.GetM() << "]...";

	if(lhs.GetM() != rhs.GetN()) {
		cout << " impossible to multiply." << endl;
		throw MatrixSizeException("non-consistent dimensions while multiplicating", lhs.GetM(), rhs.GetN());
	}

	int new_n = lhs.GetN();
	int new_m = rhs.GetM();
	int mid_size = lhs.GetM();

	int** newdata = new int*[new_n];
	for(int i = 0; i < new_n; i++) {
		newdata[i] = new int[new_m];

		for(int j = 0; j < new_m; j++) {
			newdata[i][j] = 0;
			for(int k = 0; k < mid_size; k++) {
//				cout << newdata[i][j] << "+=" << lhs.data_[i][k] << '*' << other.data_[k][j] << endl;
				newdata[i][j] += lhs.GetElem(i,k) * rhs.GetElem(k,j);
			}
		}
	}

	cout << "it's [" << new_n << "x" << new_m << "]." << endl;

	return Matrix(new_n, new_m, newdata);
}*/