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

ConnectionInfo UDPConnection::constructConnectionInfo() {
	ConnectionInfo result(this->this_shared_ptr_);
	return result;
}

bool UDPConnection::endpoint_less_than(Connection* connection) const {
	assert(dynamic_cast<UDPConnection*>(connection) != nullptr); // Check that comparisons are not made between TCP and UDP connections.

	UDPConnection* udpcon = static_cast<UDPConnection*>(connection);
	return this->ep_ < udpcon->ep_;
}

bool UDPConnection::endpoint_equals(Connection* connection) const {
	assert(dynamic_cast<UDPConnection*>(connection) != nullptr); // Check that comparisons are not made between TCP and UDP connectinons.

	UDPConnection* udpcon = static_cast<UDPConnection*>(connection);
	return this->ep_ == udpcon->ep_;
}
