
#include <Connection.hpp>

#include <Server.hpp>
#include <NetworkService.hpp>
#include <ConnectionInfo.hpp>

Connection::Connection(std::shared_ptr<Server> server_ptr)
: server_ptr_(server_ptr) {}

Connection::~Connection() {

}

void Connection::OnConnectionOpen() {
	this->server_ptr_->callback_service_->OnConnectionOpen(this->constructConnectionInfo());
}

void Connection::OnConnectionClose() {
	this->server_ptr_->callback_service_->OnConnectionClose(this->constructConnectionInfo());
}

void Connection::OnReceive(char data[], size_t bytes_received) {
	this->server_ptr_->callback_service_->OnReceive(this->constructConnectionInfo(), data, bytes_received);
}

void Connection::OnSend(char data[], size_t data_size, size_t bytes_sent) {
	this->server_ptr_->callback_service_->OnSend(this->constructConnectionInfo(), data, data_size, bytes_sent);
}
