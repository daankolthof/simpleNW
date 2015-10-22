
#if !defined(_TCPSERVERSSL_H)
#define _TCPSERVERSSL_H

#include <TCPServer.hpp>

#include <boost/asio/ssl.hpp>

class SSLOptions;

class TCPServerSSL : public TCPServer {

public:

	TCPServerSSL(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp, SSLOptions ssl_options);

private:

	boost::asio::ssl::context ssl_context_;

};



#endif // _TCPSERVERSSL_H
