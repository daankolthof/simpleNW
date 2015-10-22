
#if !defined(_TCPSERVER_H)
#define _TCPSERVER_H

#include <Server.hpp>

#include <boost/asio.hpp>

#include <cstring>
#include <functional>
#include <exception>
#include <stdexcept>
#include <tuple>
#include <memory>

class NetworkService;
class TransportProtocol;
class TCPConnection;

class TCPServer : public Server {

public:
	TCPServer(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp);
	~TCPServer();

protected:

	// Wil be set during construction so this will be changed.
	boost::asio::ip::tcp::acceptor tcp_acceptor_ =
		boost::asio::ip::tcp::acceptor(this->boost_io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0));

	std::vector<std::shared_ptr<TCPConnection>> connections;

	virtual void start_accept();
	virtual void handle_accept(std::shared_ptr<TCPConnection> connection, const boost::system::error_code& error);

	void OnStart();
	void OnStop();	

private:


};


#endif //_TCPSERVER_H
