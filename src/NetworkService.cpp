
#include "NetworkService.hpp"

//#include "Server.hpp"
#include "ServiceOptions.hpp"
#include "TransportProtocol.hpp"
//#include "Handler.hpp"

#include "TCPServer.hpp"
#include "UDPServer.hpp"

#include <array>

NetworkService::NetworkService(ServiceOptions options) {

	switch (options.transport_protocol_.protocol_) {

	case TransportProtocol::default:

	case TransportProtocol::ipv4_default:

	case TransportProtocol::ipv4_tcp:

	case TransportProtocol::ipv6_default:

	case TransportProtocol::ipv6_tcp:
		this->underlying_server_ = new TCPServer(options.threads_, options.server_port_, this, options.transport_protocol_);
		break;
	case TransportProtocol::ipv4_udp:

	case TransportProtocol::ipv6_udp:
		this->underlying_server_ = new UDPServer(options.threads_, options.server_port_, this, options.transport_protocol_);
		break;
	default:
		throw new std::invalid_argument("Invalid TransportProtocol during construction of NetworkService.");
		break;

	}

	if (options.starts_immediately_) {
		this->start();
	}

}

NetworkService::~NetworkService() {
	// Will stop all threads, the IO service and the server.
	delete this->underlying_server_;
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

void NetworkService::OnConnectionOpen(Connection* connection) {
	std::cout << "Connection opened" << std::endl;
}

void NetworkService::OnConnectionClose(Connection* connection) {
	std::cout << "Connection closed" << std::endl;
}

void NetworkService::OnReceive(Connection* connection, const char data[], size_t bytes_received) {
	std::cout << "Reveived: " << std::endl;
	
	for (size_t i1 = 0; i1 < bytes_received; i1++) {
		std::cout << data[i1];
	}

	std::cout << std::endl;

}

void NetworkService::OnSend(Connection* connectionn, const char data[], size_t bytes_sent) {

}