#if !defined(_CONNECTIONINFO_H)
#define _CONNECTIONINFO_H

#include <boost/asio.hpp>
#include <ConnectionRemoteEndpoint.hpp>
#include <Connection.hpp>

class ConnectionInfo {

	friend class TCPConnectionBase;
	friend class UDPConnection;

	friend bool operator<(const ConnectionInfo&, const ConnectionInfo&);
	friend bool operator==(const ConnectionInfo&, const ConnectionInfo&);

public:

	ConnectionInfo(const ConnectionInfo& connectioninfo) {
		this->connection_ = connectioninfo.connection_;
	}

	/* Visual Studio 2013 doesn't support default move constructors... */
	ConnectionInfo(ConnectionInfo&& connectioninfo) {
		this->connection_ = connectioninfo.connection_;
	}

	~ConnectionInfo() = default;

	Connection* getConnection() {
		return this->connection_.get();
	}

private:

	ConnectionInfo(std::shared_ptr<Connection> connection) {
		this->connection_ = connection;
	}

	std::shared_ptr<Connection> connection_;
	
};

inline bool operator<(const ConnectionInfo& ci1, const ConnectionInfo& ci2) {
	return ci1.connection_->endpoint_less_than(ci2.connection_.get());
}

inline bool operator==(const ConnectionInfo& ci1, const ConnectionInfo& ci2) {
	return ci1.connection_->endpoint_equals(ci2.connection_.get());
}

#endif
