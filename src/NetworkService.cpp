
#include "NetworkService.hpp"

//#include "Server.hpp"
#include "ServiceOptions.hpp"
#include "TransportProtocol.hpp"
#include "Handler.hpp"

#include "TCPServer.hpp"
#include "UDPServer.hpp"

#include <array>

NetworkService::NetworkService(ServiceOptions options) {

	/* If port specified in options is not within minimal and maximal port range, throw exception.
	*/
	if (!(ServiceOptions::PORT_MIN <= options.server_port_ && options.server_port_ <= ServiceOptions::PORT_MAX))
		throw new std::invalid_argument("Invalid port number in ServiceOptions during construction of NetworkService.");

	uint16_t server_port = (uint16_t)options.server_port_;

	switch (options.transport_protocol_.protocol_) {

	case TransportProtocol::default:

	case TransportProtocol::ipv4_default:

	case TransportProtocol::ipv4_tcp:

	case TransportProtocol::ipv6_default:

	case TransportProtocol::ipv6_tcp:
		{
			std::shared_ptr<TCPServer> new_server(new TCPServer(options.threads_, server_port, this, options.transport_protocol_));
			new_server->this_shared_ptr_ = new_server;
			this->underlying_server_ = new_server;
		}
		
		break;
	case TransportProtocol::ipv4_udp:

	case TransportProtocol::ipv6_udp:
		{
			std::shared_ptr<UDPServer> new_server(new UDPServer(options.threads_, server_port, this, options.transport_protocol_));
			new_server->this_shared_ptr_ = new_server;
			this->underlying_server_ = new_server;
		}
		break;
	default:
		throw new std::invalid_argument("Invalid TransportProtocol during construction of NetworkService.");
		break;

	}

}

NetworkService::~NetworkService() {
	// Will stop all threads, the IO service and the server.
	this->stop();

	std::cout << "NetworkService destructor called" << std::endl;
}

int NetworkService::addHandler(Handler* handler) {

	if (this->started_server_) {
		return -1;
	}

	{
		std::unique_lock<std::mutex> lock(handler_mtx_);

		handlers_.push_back(handler);
	}
	
	return 0;		
}

std::vector<Handler*>& NetworkService::getHandlers() {
	return this->handlers_;
}

void NetworkService::start() {

	std::cout << "NetworkService.start called" << std::endl;

	this->underlying_server_->run();
	this->started_server_ = true;
}

void NetworkService::stop() {

	this->underlying_server_->stop();
	this->started_server_ = false;
}

void NetworkService::OnConnectionOpen(std::shared_ptr<Connection> connection) const {
	std::cout << "Connection opened" << std::endl;

	for (Handler* h : handlers_) {
		h->OnConnectionOpen(connection);
	}

}

void NetworkService::OnConnectionClose(std::shared_ptr<Connection> connection) const {
	std::cout << "Connection closed" << std::endl;

	for (Handler* h : handlers_) {
		h->OnConnectionClose(connection);
	}

}

void NetworkService::OnReceive(std::shared_ptr<Connection> connection, const char data[], size_t bytes_received) const {
	std::cout << "Reveived: " << std::endl;
	
	for (size_t i1 = 0; i1 < bytes_received; i1++) {
		std::cout << data[i1];
	}

	std::cout << std::endl;

	for (Handler* h : handlers_) {
		h->OnReceive(connection, data, bytes_received);
	}

}

void NetworkService::OnSend(std::shared_ptr<Connection> connection, const char data[], size_t data_size, size_t bytes_sent) const {

	for (Handler* h : handlers_) {
		h->OnSend(connection, data, data_size, bytes_sent);
	}

}