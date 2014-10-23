
#if !defined(_NETWORKSERVICE_H)
#define _NETWORKSERVICE_H

#include <mutex>
#include <vector>

class Handler;
class Connection;
class ServiceOptions;
class Server;

class NetworkService {

	friend class Connection;

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
	Server* underlying_server_;

	std::mutex handler_mtx_;

	bool started_server_ = false;

	void OnConnectionOpen(Connection* connection);
	void OnConnectionClose(Connection* connection);
	void OnReceive(Connection* connection, const char data[], size_t bytes_received);
	void OnSend(Connection* connection, const char data[], size_t bytes_sent);

private:



};

#endif  //_NETWORKSERVICE_H
