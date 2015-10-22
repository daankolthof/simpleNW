
#if !defined(_NETWORKSERVICE_H)
#define _NETWORKSERVICE_H

#include <Connection.hpp>

#include <memory>
#include <mutex>
#include <vector>

class Handler;
class ServiceOptions;
class Server;
class ConnectionInfo;

class NetworkService {

	friend class Connection;
	friend class UDPServer;

public:

	NetworkService(ServiceOptions options);
	NetworkService(NetworkService& other) = delete;
	virtual ~NetworkService();


    int addHandler(Handler* handler);
	std::vector<Handler*>& getHandlers();

    void start();
    void stop();

protected:

	std::vector<Handler*> handlers_;
	std::shared_ptr<Server> underlying_server_;

	std::mutex handler_mtx_;

	bool started_server_ = false;

	void OnConnectionOpen(ConnectionInfo connectioninfo) const;
	void OnConnectionClose(ConnectionInfo connectioninfo) const;
	void OnReceive(ConnectionInfo connectioninfo, char data[], size_t bytes_received) const;
	void OnSend(ConnectionInfo connectioninfo, char data[], size_t data_size, size_t bytes_sent) const;

private:

};

#endif  //_NETWORKSERVICE_H
