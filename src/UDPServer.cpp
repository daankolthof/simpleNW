
#include <UDPServer.hpp>

#include <DynamicArray.hpp>
#include <TransportProtocol.hpp>
#include <NetworkService.hpp>
#include <Handler.hpp>
#include <UDPConnection.hpp>

#include <iostream>

UDPServer::UDPServer(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp)
: Server(threads, server_port, callback_service, tp), udp_serversocket_(this->boost_io_service_,
	boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), -1))
{
	switch (tp.protocol_) {

	case TransportProtocol::ip_default:

	case TransportProtocol::ipv4_default:

	case TransportProtocol::ipv4_udp:
		this->udp_serversocket_ = boost::asio::ip::udp::socket(
			this->boost_io_service_,
			boost::asio::ip::udp::endpoint(
				boost::asio::ip::udp::v4(), server_port));
		break;
	case TransportProtocol::ipv6_default:

	case TransportProtocol::ipv6_udp:
		this->udp_serversocket_ = boost::asio::ip::udp::socket(
			this->boost_io_service_,
			boost::asio::ip::udp::endpoint(
				boost::asio::ip::udp::v6(), server_port));
		break;
	default:
		// Will never happen because protocol enum is already filtered in NetworkService constructor.
		assert(false);
		break;

	}
}

UDPServer::~UDPServer() {

}

void UDPServer::async_send(boost::asio::ip::udp::endpoint& ep, DynamicArray<char>& arr) {
	this->getUDPSocket().async_send_to(boost::asio::buffer(arr.data(), arr.size()), ep,
		std::bind(&UDPServer::handle_write, this, ep, arr, std::placeholders::_1, std::placeholders::_2));
}

void UDPServer::handle_write(boost::asio::ip::udp::endpoint ep, DynamicArray<char> arr, const boost::system::error_code& error, size_t bytes_transferred) {
	
	if (error == boost::system::errc::success) {
		this->OnSend(ep, arr.data(), bytes_transferred);
	}

}

void UDPServer::start_read() {

	DynamicArray<char> arr(1024);
	boost::asio::ip::udp::endpoint* ep = new boost::asio::ip::udp::endpoint();
	
	this->udp_serversocket_.async_receive_from(
		boost::asio::buffer(arr.data(), arr.size()),
		*ep,
		std::bind(&UDPServer::handle_read, this, ep, arr, std::placeholders::_1, std::placeholders::_2));
}

void UDPServer::handle_read(boost::asio::ip::udp::endpoint* ep, DynamicArray<char> arr, const boost::system::error_code& error, size_t bytes_transferred) {
	this->start_read();

	if (error == boost::system::errc::success) {
		/* A message has been succesfully received, call any handlers. */
		this->OnReceive(*ep, arr.data(), bytes_transferred);
	}

}

void UDPServer::OnStart() {

	this->start_read();
}

void UDPServer::OnStop() {

	this->udp_serversocket_.close();
}

void UDPServer::OnReceive(boost::asio::ip::udp::endpoint ep, char data[], size_t bytes_transferred) {
	callback_service_->OnReceive(std::shared_ptr<UDPConnection>(new UDPConnection(ep, this)), data, bytes_transferred);
}

void UDPServer::OnSend(boost::asio::ip::udp::endpoint ep, char data[], size_t bytes_transferred) {
	callback_service_->OnReceive(std::shared_ptr<UDPConnection>(new UDPConnection(ep, this)), data, bytes_transferred);
}
