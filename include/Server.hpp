
#if !defined(_SERVER_H)
#define _SERVER_H

#include <algorithm>
#include <atomic>
#include <thread>
#include <boost/asio.hpp>

class NetworkService;
class TransportProtocol;

class Server {

public:

	Server();
	Server(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp);

	virtual ~Server();

	boost::asio::io_service boost_io_service_;

	NetworkService* callback_service_;

	int run();
	void stop();

	

protected:

	virtual void OnStart();

	bool is_initialized_ = false;

	std::vector<std::thread> threads_vec_;

	

private:


};

void onThreadRun(boost::asio::io_service* boost_io_service, NetworkService* callback_service, Server* server);

#endif  //_SERVER_H
