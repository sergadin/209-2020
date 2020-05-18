#pragma once

#include <string>

namespace R2
{
	using namespace std;
	class Server
	{
	protected:
		// Server attributes

		void init(const string& dbPath);

		// Database

		string processRequest(const string& query) const;
		void readDatabase() const;
		void saveDatabase() const;

		// Networking

		void mainLoop() const;
		inline int ntwPORT() const { return 8091; }

		// Misc

		void logMessage(const string& msg) const;
		void logError(const string& msg) const;
		void logWarning(const string& msg) const;
		void consoleLogMessage(const string& msg) const;
	public:
		void Start() const;
		string status() const;
	};
}