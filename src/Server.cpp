
#include "Server.hpp"

#include "TransportProtocol.hpp"

Server::Server() { }

Server::Server(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp) {
	this->callback_service_ = callback_service;
	this->threads_vec_.resize(threads);

	this->is_initialized_ = true;

}

Server::~Server() {

	this->boost_io_service_.stop();

	for (size_t i1 = 0; i1 < threads_vec_.size(); i1++) {
		if (threads_vec_[i1].joinable()) {
			threads_vec_[i1].join();
		}
	}
}

int Server::run() {

	std::cout << "Server.run called" << std::endl;

	if (!this->is_initialized_) {
		return -1;
	}

	this->OnStart();

	for (size_t i1 = 0; i1 < threads_vec_.size(); i1++) {
		threads_vec_[i1] = std::move(std::thread(&onThreadRun, &this->boost_io_service_, this->callback_service_, this));
	}

	return threads_vec_.size();
}

void Server::stop() {
	boost_io_service_.stop();
}

void Server::OnStart() {
	std::cout << "Server started" << std::endl;
}

void onThreadRun(boost::asio::io_service* boost_io_service, NetworkService* callback_service, Server* server) {
	boost_io_service->run();
}