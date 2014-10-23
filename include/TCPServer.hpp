
#if !defined(_TCPSERVER_H)
#define _TCPSERVER_H

#include "Server.hpp"

#include <boost/asio.hpp>

#include <cstring>
#include <functional>
#include <exception>
#include <stdexcept>
#include <tuple>

class NetworkService;
class TransportProtocol;
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


#endif
