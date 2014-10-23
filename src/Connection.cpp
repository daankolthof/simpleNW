
#include "Connection.hpp"

#include "Server.hpp"
#include "NetworkService.hpp"

Connection::Connection(Server* server_ptr)
: server_ptr_(server_ptr)
{
}

Connection::~Connection() {

}

void Connection::OnConnectionOpen() {
	this->server_ptr_->callback_service_->OnConnectionOpen(this);
}

void Connection::OnConnectionClose() {
	this->server_ptr_->callback_service_->OnConnectionClose(this);
}

void Connection::OnReceive(size_t bytes_received) {
	this->server_ptr_->callback_service_->OnReceive(this, this->data_, bytes_received);
}

void Connection::OnSend(char data[], size_t bytes_sent) {
	this->server_ptr_->callback_service_->OnSend(this, this->data_, bytes_sent);
}