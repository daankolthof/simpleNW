
#include "Connection.hpp"

#include "Server.hpp"
#include "NetworkService.hpp"

Connection::Connection() {
	//this->this_shared_ptr = std::make_shared<Connection>(this);
}

Connection::Connection(std::shared_ptr<Server> server_ptr)
: server_ptr_(server_ptr)
{
}

Connection::~Connection() {

}

void Connection::OnConnectionOpen() {
	this->server_ptr_->callback_service_->OnConnectionOpen(this->this_shared_ptr_);
}

void Connection::OnConnectionClose() {
	this->server_ptr_->callback_service_->OnConnectionClose(this->this_shared_ptr_);
}

void Connection::OnReceive(size_t bytes_received) {
	this->server_ptr_->callback_service_->OnReceive(this->this_shared_ptr_, this->data_, bytes_received);
}

void Connection::OnSend(char data[], size_t bytes_sent) {
	this->server_ptr_->callback_service_->OnSend(this->this_shared_ptr_, data, bytes_sent);
}
