
#if !defined(_NETWORKSERVICE_H)
#define _NETWORKSERVICE_H

#include "Server.hpp"
#include "ServiceOptions.hpp"
#include "TransportProtocol.hpp"
#include "Handler.hpp"

#include "TCPServer.hpp"
#include "UDPServer.hpp"

#include <mutex>
#include <vector>


class NetworkService {

	friend class Connection;
	friend class TCPConnection;

public:

	NetworkService(ServiceOptions options);
	virtual ~NetworkService();

    int addHandler(Handler* handler);
	std::vector<Handler*>& getHandlers();

    void start();
    void stop();

protected:

	std::vector<Handler*> handlers_;
	Server* underlying_server_;

	std::mutex handler_mtx_;

	bool started_server_ = false;

	void OnConnectionOpen(Connection* connection);
	void OnConnectionClose(Connection* connection);
	void OnReceive(Connection* connection, char data[], size_t bytes_received);
	void OnSend(Connection* connection, char data[], size_t bytes_sent);

private:



};

#endif  //_NETWORKSERVICE_H
