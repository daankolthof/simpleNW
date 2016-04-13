
#if !defined(_UDPCONNECTION_H)
#define _UDPCONNECTION_H

#include <Connection.hpp>

class UDPServer;

class UDPConnection : public Connection {

	friend class UDPServer;

public:

	~UDPConnection();

	/* Return whether the connection is open and valid.
	Because this is UDP, the connection does not close and data can always be sent. */
	bool is_open() {
		return true;
	}

	/* Do nothing because there is no connection to close. */
	void close() {}

	void send_nonblocking(char[], size_t);
	void send_nonblocking_buffer(char[], size_t);

	ConnectionInfo constructConnectionInfo() override;

	bool endpoint_less_than(Connection*) const override;
	bool endpoint_equals(Connection*) const override;

	size_t hash() const override {
		std::ostringstream stream;
		stream << this->ep_;
		return std::hash<std::string>()(stream.str());
	}

protected:

	UDPConnection(boost::asio::ip::udp::endpoint, UDPServer*);

	boost::asio::ip::udp::endpoint ep_;
	UDPServer* udp_server_;

private:

};

#endif // _UDPCONNECTION_H
