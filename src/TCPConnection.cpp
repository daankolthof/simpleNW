
#include "TCPConnection.hpp"


TCPConnection::TCPConnection(boost::asio::io_service& io_service, Server* server_ptr)
: Connection(server_ptr), socket_(io_service)
{
}

TCPConnection::~TCPConnection() {
	this->close();
}

void TCPConnection::send_nonblocking(char data[], size_t bytes_to_send) {
	char* bufcopy = new char[bytes_to_send];
	memcpy(bufcopy, data, bytes_to_send);

	std::pair<char*, size_t> buffer_size_tuple;
	std::get<0>(buffer_size_tuple) = bufcopy;
	std::get<1>(buffer_size_tuple) = bytes_to_send;

	this->sendbuffers_vec_.push_back(buffer_size_tuple);
	size_t tuple_index = this->sendbuffers_vec_.size() - 1;


	this->socket_.async_write_some(boost::asio::buffer(bufcopy, bytes_to_send), std::bind(&TCPConnection::handle_write, this, tuple_index, std::placeholders::_1, std::placeholders::_2));
}

void TCPConnection::close() {
	this->socket_.close();
}

void TCPConnection::start_read() {

	this->socket_.async_read_some(boost::asio::buffer(this->data_, this->max_buf_length), std::bind(&TCPConnection::handle_read, this, std::placeholders::_1, std::placeholders::_2));

}

void TCPConnection::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {

	if (!this->socket_.is_open()) {
		return;
	}

	if (error == boost::asio::error::misc_errors::eof) { // Connection closed.
		this->OnConnectionClose();
		return;
	}
	else if (error != boost::system::errc::success) { // If not succesful.
		// Something else went wrong, close connection by deleting object.
		delete this;
		return;
	}

	// Call all handers before reading more data.
	this->OnReceive(bytes_transferred);

	this->start_read();

}

void TCPConnection::start_write() {

}

void TCPConnection::handle_write(size_t buffervec_index, const boost::system::error_code& error, size_t bytes_transferred) {

	// Get the buffer used to store the data to send.
	char* sendbuf_loc = std::get<0>(this->sendbuffers_vec_[buffervec_index]);
	size_t sendbuf_size = std::get<1>(this->sendbuffers_vec_[buffervec_index]);

	// Call any handlers.
	this->OnSend(sendbuf_loc, sendbuf_size);

	// Delete the data afer all handlers are called and data is no longer needed.
	delete[] sendbuf_loc;

	// Remove pointer to the deleted buffer from the vector.
	this->sendbuffers_vec_.erase(this->sendbuffers_vec_.begin() + buffervec_index);

}
