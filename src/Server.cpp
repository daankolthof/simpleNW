
#include "Server.hpp"

#include "TransportProtocol.hpp"


Server::Server(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp)
: server_port_(server_port), callback_service_(callback_service), tp_(tp)

{
	this->callback_service_ = callback_service;
	this->threads_vec_.resize(threads);

	this->is_initialized_ = true;

}

Server::~Server() {
	/* The destruction will be called when all shared pointers are destroyed.
	* Which  means this object will not be used again.
	*/
	this->boost_io_service_.stop();

}

int Server::run() {

	{
		std::unique_lock<std::mutex> lock(this->server_mtx_);

		if (!this->is_initialized_) {
			return -1;
		}

		this->OnStart();

		for (size_t i1 = 0; i1 < threads_vec_.size(); i1++) {
			threads_vec_[i1] = std::move(std::thread(&onThreadRun, &this->boost_io_service_));
		}

		return threads_vec_.size();

	}
}

void Server::stop() {

	{
		std::unique_lock<std::mutex> lock(this->server_mtx_);

		if (this->stopped)
			return;

		this->stop_server();

	}

	// Underlying IO service is stopped, first unlock so threads can finish themselves.
	// Now join all threads.
	this->join_threads();
}

void Server::stop_server() {
	this->OnStop();

	this->boost_io_service_.stop();

	this->this_shared_ptr_.reset();

	
}

void Server::join_threads() {
	for (size_t i1 = 0; i1 < threads_vec_.size(); i1++) {
		if (threads_vec_[i1].joinable()) {
			threads_vec_[i1].join();
		}
	}
}

void onThreadRun(boost::asio::io_service* boost_io_service) {
	boost_io_service->run();

}
