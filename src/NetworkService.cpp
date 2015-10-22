
#include <NetworkService.hpp>

#include <ServiceOptions.hpp>
#include <TransportProtocol.hpp>
#include <Handler.hpp>

#include <ConnectionInfo.hpp>
#include <Connection.hpp>

#include <TCPServer.hpp>
#include <UDPServer.hpp>

#include <array>
#include <iostream>

NetworkService::NetworkService(ServiceOptions options) {

	/* If port specified in options is not within minimal and maximal port range, throw exception.
	*/
	if (!(ServiceOptions::PORT_MIN <= options.server_port_ && options.server_port_ <= ServiceOptions::PORT_MAX))
		throw new std::invalid_argument("Invalid port number in ServiceOptions during construction of NetworkService.");

	uint16_t server_port = (uint16_t)options.server_port_;

	switch (options.transport_protocol_.protocol_) {

	case TransportProtocol::ip_default:

	case TransportProtocol::ipv4_default:

	case TransportProtocol::ipv4_tcp:

	case TransportProtocol::ipv6_default:

	case TransportProtocol::ipv6_tcp:
		{
			
			// Check whether SSL should be used.
			if (options.ssl_options.options_set) {
				std::shared_ptr<Server> new_server(TCPServerSSL(options.threads_, server_port, this, options.transport_protocol_, options.ssl_options));
				new_server->this_shared_ptr_ = new_server;
				this->underlying_server_ = new_server;
			}
			else {
				std::shared_ptr<TCPServer> new_server(new TCPServer(options.threads_, server_port, this, options.transport_protocol_));
				new_server->this_shared_ptr_ = new_server;
				this->underlying_server_ = new_server;
			}
		}
		
		break;
	case TransportProtocol::ipv4_udp:

	case TransportProtocol::ipv6_udp:
		{
			// SSL over UDP is not supported.
			if (options.ssl_options.options_set()) {
				throw new std::invalid_argument("Argument mismatch, SSL options are given, but TransportProtocol specifies UDP. SSL over UDP is not supported.");
			}

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
	/* Will stop all threads, the IO service and the server.
	*/
	if(this->started_server_) this->stop();

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

	this->underlying_server_->run();
	this->started_server_ = true;
}

void NetworkService::stop() {

	this->underlying_server_->stop();
	this->started_server_ = false;
}

void NetworkService::OnConnectionOpen(ConnectionInfo connectioninfo) const {

	for (Handler* h : handlers_) {
		h->OnConnectionOpen(connectioninfo);
	}

}

void NetworkService::OnConnectionClose(ConnectionInfo connectioninfo) const {

	for (Handler* h : handlers_) {
		h->OnConnectionClose(connectioninfo);
	}

}

void NetworkService::OnReceive(ConnectionInfo connectioninfo, char data[], size_t bytes_received) const {

	for (Handler* h : handlers_) {
		h->OnReceive(connectioninfo, data, bytes_received);
	}

}

void NetworkService::OnSend(ConnectionInfo connectioninfo, char data[], size_t data_size, size_t bytes_sent) const {

	for (Handler* h : handlers_) {
		h->OnSend(connectioninfo, data, data_size, bytes_sent);
	}

}
