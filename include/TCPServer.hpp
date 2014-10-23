
#if !defined(_TCPSERVER_H)
#define _TCPSERVER_H

#include "Connection.hpp"
#include "Handler.hpp"
#include "Server.hpp"
#include "TransportProtocol.hpp"
#include "NetworkService.hpp"

#include <boost/asio.hpp>

#include <cstring>
#include <functional>
#include <exception>
#include <stdexcept>
#include <tuple>

class TCPConnection;

class TCPServer : public Server {

public:
	TCPServer(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp);
	~TCPServer();

	// Wil be set during construction so this will be changed.
	boost::asio::ip::tcp::acceptor tcp_acceptor_ =
		boost::asio::ip::tcp::acceptor(this->boost_io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0));

	

protected:

	void start_accept();
	void handle_accept(TCPConnection* connection, const boost::system::error_code& error);

	void OnStart();

	void register_new_connection(TCPConnection* connection);
	

private:


};

class TCPConnection : public Connection {

public:

	TCPConnection(boost::asio::io_service& io_service, Server* server_ptr);
	~TCPConnection();

	const static int max_buf_length = 1024;

	std::vector<std::pair<char*, size_t>> sendbuffers_vec_;

	boost::asio::ip::tcp::socket socket_;
	char data_[max_buf_length];

	Server* server_ptr_;

	void send_nonblocking(char data[], size_t bytes_to_send);
	void close();

	void start_read();
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

	void start_write();
	void handle_write(size_t buffervec_index);

	/* TODO: Move these to the parent class.
	*/
	void OnConnectionOpen();
	void OnConnectionClose();
	void OnReceive(size_t bytes_received);
	void OnSend(char data[], size_t bytes_sent);

protected:

private:

};

#endif
