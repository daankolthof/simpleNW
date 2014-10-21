
#include "TransportProtocol.hpp"

TransportProtocol::TransportProtocol(transpprtcl protocol) {
	this->protocol_ = protocol;
}

namespace IPV4 {

	TransportProtocol DEFAULT() {
		return TransportProtocol(TransportProtocol::ipv4_default);
	}
	TransportProtocol TCP() {
		return TransportProtocol(TransportProtocol::ipv4_tcp);
	}
	TransportProtocol UDP() {
		return TransportProtocol(TransportProtocol::ipv4_udp);
	}

}

namespace IPV6 {

	TransportProtocol DEFAULT() {
		return TransportProtocol(TransportProtocol::ipv6_default);
	}
	TransportProtocol TCP() {
		return TransportProtocol(TransportProtocol::ipv6_tcp);
	}
	TransportProtocol UDP() {
		return TransportProtocol(TransportProtocol::ipv6_udp);
	}

}
