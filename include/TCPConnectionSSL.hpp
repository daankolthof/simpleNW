
#if !defined(_TCPCONNECTIONSSL_H)
#define _TCPCONNECTIONSSL_H

#include <TCPConnectionBase.hpp>

#include <boost/asio/ssl.hpp>

class TCPConnectionSSL : public TCPConnectionBase {

	friend class TCPServerSSL;

public:

	void start_handshake();
	void handle_handshake(const boost::system::error_code& error);

	bool is_open() override;

	void send_nonblocking(char data[], size_t bytes_to_send);
	void send_nonblocking_buffer(char data[], size_t bytes_to_send);

	void start_read();
	void handle_read(std::shared_ptr<Connection> connection, const boost::system::error_code& error, size_t bytes_transferred);

	void start_write();
	void handle_write(std::shared_ptr<Connection> connection, DynamicArray<char>, const boost::system::error_code& error, size_t bytes_transferred);

protected:

	TCPConnectionSSL(boost::asio::io_service& io_service, boost::asio::ssl::context& ssl_context, std::shared_ptr<Server> server_ptr);

	bool endpoint_less_than(Connection*) const;
	bool endpoint_equals(Connection*) const;

	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket_;

};

#endif // _TCPCONNECTIONSSL_H
