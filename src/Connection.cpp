
#include "Connection.hpp"

#include "Server.hpp"
#include "NetworkService.hpp"

Connection::Connection(Server* server_ptr)
: server_ptr_(server_ptr)
{
}

Connection::~Connection() {

}

void Connection::OnConnectionOpen(std::shared_ptr<Connection> connection) {
	this->server_ptr_->callback_service_->OnConnectionOpen(connection);
}

void Connection::OnConnectionClose(std::shared_ptr<Connection> connection) {
	this->server_ptr_->callback_service_->OnConnectionClose(connection);
}

void Connection::OnReceive(std::shared_ptr<Connection> connection, size_t bytes_received) {
	this->server_ptr_->callback_service_->OnReceive(connection, this->data_, bytes_received);
}

void Connection::OnSend(std::shared_ptr<Connection> connection, char data[], size_t bytes_sent) {
	this->server_ptr_->callback_service_->OnSend(connection, this->data_, bytes_sent);
}
