#include <UDPConnection.hpp>

#include <DynamicArray.hpp>
#include <UDPServer.hpp>
#include <ConnectionInfo.hpp>
#include <UDPConnectionRemoteEndpoint.hpp>

#include <iostream>

UDPConnection::UDPConnection(boost::asio::ip::udp::endpoint ep, UDPServer* udp_server)
	: ep_(ep), udp_server_(udp_server) {}

UDPConnection::~UDPConnection() {}

void UDPConnection::send_nonblocking(char data[], size_t bytes_to_send) {

	DynamicArray<char> arr(bytes_to_send);
	memcpy(arr.data(), data, bytes_to_send);

	this->udp_server_->async_send(this->ep_, arr);
}

void UDPConnection::send_nonblocking_buffer(char data[], size_t bytes_to_send) {
	
	DynamicArray<char> arr(data, bytes_to_send);
	this->udp_server_->async_send(this->ep_, arr);
}

ConnectionInfo UDPConnection::getConnectionInfo() {

	ConnectionInfo result;
	UDPConnectionRemoteEndpoint* udpendp = new UDPConnectionRemoteEndpoint();
	udpendp->ep = this->ep_;
	result.connectionendpoint = static_cast<ConnectionRemoteEndpoint*>(udpendp);

	return result;
}
