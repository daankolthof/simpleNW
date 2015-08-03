#if !defined(_UDPCONNECTIONREMOTEENDPOINT_H)
#define _UDPCONNECTIONREMOTEENDPOINT_H

#include <ConnectionRemoteEndpoint.hpp>

class UDPConnectionRemoteEndpoint : public ConnectionRemoteEndpoint {

public:

	bool operator<(ConnectionRemoteEndpoint* connection_remote_endpoint) override {
		
		/* Make sure the operator < is called with an object of the same subclass.
		 * This can go wrong if ConnectionInfo objects returned by different NetworkServices are compared,
		 * do not do this */
		assert(dynamic_cast<UDPConnectionRemoteEndpoint*>(connection_remote_endpoint) != nullptr);
		
		UDPConnectionRemoteEndpoint* udpendp = static_cast<UDPConnectionRemoteEndpoint*>(connection_remote_endpoint);
		return this->ep < udpendp->ep;
	}

	bool operator==(ConnectionRemoteEndpoint* connection_remote_endpoint) override {

		/* Make sure the operator == is called with an object of the same subclass.
		* This can go wrong if ConnectionInfo objects returned by different NetworkServices are compared,
		* do not do this */
		assert(dynamic_cast<UDPConnectionRemoteEndpoint*>(connection_remote_endpoint) != nullptr);

		UDPConnectionRemoteEndpoint* udpend = static_cast<UDPConnectionRemoteEndpoint*>(connection_remote_endpoint);
		return this->ep == udpend->ep;
	}

	boost::asio::ip::udp::endpoint ep;
};

#endif
