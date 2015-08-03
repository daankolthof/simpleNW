#if !defined(_TCPCONNECTIONREMOTEENDPOINT_H)
#define _TCPCONNECTIONREMOTEENDPOINT_H

#include <ConnectionRemoteEndpoint.hpp>

class TCPConnectionRemoteEndpoint : public ConnectionRemoteEndpoint {

public:

	bool operator<(ConnectionRemoteEndpoint* connection_remote_endpoint) override {

		/* Make sure the operator < is called with an object of the same subclass.
		* This can go wrong if ConnectionInfo objects returned by different NetworkServices are compared,
		* do not do this */
		assert(dynamic_cast<TCPConnectionRemoteEndpoint*>(connection_remote_endpoint) != nullptr);

		TCPConnectionRemoteEndpoint* tcpendp = static_cast<TCPConnectionRemoteEndpoint*>(connection_remote_endpoint);
		return this->ep < tcpendp->ep;
	}

	bool operator==(ConnectionRemoteEndpoint* connection_remote_endpoint) override {

		/* Make sure the operator == is called with an object of the same subclass.
		* This can go wrong if ConnectionInfo objects returned by different NetworkServices are compared,
		* do not do this */
		assert(dynamic_cast<TCPConnectionRemoteEndpoint*>(connection_remote_endpoint) != nullptr);

		TCPConnectionRemoteEndpoint* tcpend = static_cast<TCPConnectionRemoteEndpoint*>(connection_remote_endpoint);
		return this->ep == tcpend->ep;
	}

	boost::asio::ip::tcp::endpoint ep;
};

#endif
