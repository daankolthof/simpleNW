
#include <Connection.hpp>

#include <Server.hpp>
#include <NetworkService.hpp>

#include <iostream>


Connection::Connection(std::shared_ptr<Server> server_ptr)
: server_ptr_(server_ptr)
{
	std::cout << "Connection created" << std::endl;
	std::cout << this->server_ptr_.use_count() << std::endl;
}

Connection::~Connection() {

}

void Connection::OnConnectionOpen() {
	this->server_ptr_->callback_service_->OnConnectionOpen(this->this_shared_ptr_);
}

void Connection::OnConnectionClose() {
	this->server_ptr_->callback_service_->OnConnectionClose(this->this_shared_ptr_);
}

void Connection::OnReceive(char data[], size_t bytes_received) {
	this->server_ptr_->callback_service_->OnReceive(this->this_shared_ptr_, data, bytes_received);
}

void Connection::OnSend(char data[], size_t data_size, size_t bytes_sent) {
	this->server_ptr_->callback_service_->OnSend(this->this_shared_ptr_, data, data_size, bytes_sent);
}
