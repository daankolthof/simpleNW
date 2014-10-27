
#include "UDPServer.hpp"

#include "TransportProtocol.hpp"
#include "NetworkService.hpp"

UDPServer::UDPServer(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp)
: Server(threads, server_port, callback_service, tp)
{

}

UDPServer::~UDPServer() {

}

void UDPServer::OnStart() {

}

void UDPServer::OnStop() {

}
