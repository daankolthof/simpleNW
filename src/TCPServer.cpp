
#include "TCPServer.hpp"

#include "TCPConnection.hpp"
#include "TransportProtocol.hpp"


TCPServer::TCPServer(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp)
: Server(threads, server_port, callback_service, tp)
{

	boost::asio::ip::tcp tcp = boost::asio::ip::tcp::v4();

	switch (tp.protocol_) {

	case TransportProtocol::default:

	case TransportProtocol::ipv4_default:

	case TransportProtocol::ipv4_tcp:
		tcp = boost::asio::ip::tcp::v4();
		break;
	case TransportProtocol::ipv6_default:
	
	case TransportProtocol::ipv6_tcp:
		tcp = boost::asio::ip::tcp::v6();
		break;
	default:
		// Will never happen because protocol enum is already filtered in NetworkService constructor.
		break;

	}

	this->tcp_acceptor_ = std::move(boost::asio::ip::tcp::acceptor(this->boost_io_service_, boost::asio::ip::tcp::endpoint(tcp, server_port)));

}


TCPServer::~TCPServer()
{
}

void TCPServer::start_accept() {

	std::shared_ptr<TCPConnection> connection(new TCPConnection(this->boost_io_service_, this));

	// Async call to accept new connection.
	this->tcp_acceptor_.async_accept(connection->socket_,
		std::bind(&TCPServer::handle_accept, this, connection,
		std::placeholders::_1)
		);
	
}

void TCPServer::handle_accept(std::shared_ptr<TCPConnection> connection, const boost::system::error_code& error) {
	
	if (error) {
		connection->close(connection);
		return;
	}
	
	this->register_new_connection(connection);
	connection->OnConnectionOpen(connection);
	connection->start_read(connection);

	this->start_accept();
}

void TCPServer::OnStart() {
	std::cout << "TCPServer started" << std::endl;
	this->start_accept();
}

void TCPServer::register_new_connection(std::shared_ptr<TCPConnection> connection) {
	//throw new std::exception("TCPServer::register_new_connection is not implemented.");
	connections.push_back(connection);
}
