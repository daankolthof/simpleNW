
#if !defined(_SERVER_H)
#define _SERVER_H

#include <algorithm>
#include <atomic>
#include <mutex>
#include <thread>
#include <boost/asio.hpp>

class NetworkService;
class TransportProtocol;

class Server {

	// For callbacks.
	friend class Connection;

	// For constructing and starting the server.
	friend class NetworkService;

public:

	virtual ~Server();	

protected:

	Server();
	Server(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp);
	Server(Server& other) = delete;

	boost::asio::io_service boost_io_service_;

	NetworkService* callback_service_;
	
	std::mutex server_mtx_;

	/* Used to keep the object alive, passed to the async functions.
	Delete this and stop underlying server to kill object. */
	std::shared_ptr<Server> this_shared_ptr_;

	bool is_initialized_ = false;
	bool stopped = false;

	std::vector<std::thread> threads_vec_;

	virtual void OnStart() = 0;
	virtual void OnStop() = 0;

	int run();
	void stop();

	void stop_server();

	

private:

	void join_threads();

};

void onThreadRun(boost::asio::io_service* boost_io_service);

#endif  //_SERVER_H
