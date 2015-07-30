
#if !defined(_UDPSERVER_H)
#define _UDPSERVER_H

#include <Server.hpp>

class NetworkService;
class TransportProtocol;

template<typename T>
class DynamicArray;

class UDPServer : public Server {

	friend class UDPConnection;

public:

	UDPServer(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp);
	~UDPServer();

protected:
	
	/* Is set in the initializer list of the constructor and later set to the
	 * right value during parsing of the arguments passed to the constructor. */
	boost::asio::ip::udp::socket udp_serversocket_;

	boost::asio::ip::udp::socket& getUDPSocket() {
		return this->udp_serversocket_;
	}

	void async_send(boost::asio::ip::udp::endpoint&, DynamicArray<char>&);
	void handle_write(boost::asio::ip::udp::endpoint, DynamicArray<char>, const boost::system::error_code& error, size_t bytes_transferred);

	void start_read();
	void handle_read(boost::asio::ip::udp::endpoint*, DynamicArray<char>, const boost::system::error_code& error, size_t bytes_transferred);

	void OnStart();
	void OnStop();

	void OnReceive(boost::asio::ip::udp::endpoint, char[], size_t);
	void OnSend(boost::asio::ip::udp::endpoint, char[], size_t);

private:

};



#endif //_UDPSERVER_H
