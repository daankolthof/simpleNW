
#include "NetworkService.hpp"

NetworkService::NetworkService(ServiceOptions options) {

	switch (options.transport_protocol_.protocol_) {

	case TransportProtocol::ipv4_default:

		break;
	case TransportProtocol::ipv4_tcp:

		break;
	case TransportProtocol::ipv4_udp:

		break;
	case TransportProtocol::ipv6_default:

		break;
	case TransportProtocol::ipv6_tcp:

		break;
	case TransportProtocol::ipv6_udp:

		break;
	default:

		break;

	}



	if (options.starts_immediately_) {
		this->start();
	}

}

void NetworkService::addHandler(Handler* handler) {

	{
		std::unique_lock<std::mutex> lock(handler_mtx_);

		handlers_.push_back(handler);
	}
		
}

void NetworkService::start() {

	this->started_server_ = true;
}

void NetworkService::stop() {

	this->started_server_ = false;
}
