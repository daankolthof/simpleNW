
#if !defined(_UDPSERVER_H)
#define _UDPSERVER_H

#include "Server.hpp"
#include "NetworkService.hpp"

class UDPServer : public Server {

public:

	UDPServer(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp);
	~UDPServer();

protected:

private:


};



#endif