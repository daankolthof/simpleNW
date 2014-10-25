
#include "TCPConnection.hpp"


TCPConnection::TCPConnection(boost::asio::io_service& io_service, Server* server_ptr)
: Connection(server_ptr), socket_(io_service)
{
}

TCPConnection::~TCPConnection() {


	/*
	{
		std::unique_lock<std::mutex> lock(this->connection_mtx_);

		if (!this->socket_.is_open()) {
			// Object is already closed, destruct.
			return;
		}

		// Close underlying socket.
		this->close_when_owning_mutex();
		
	}
	*/

	// Let object be destroyed.
}

bool TCPConnection::is_open() {
	return this->socket_.is_open();
}

void TCPConnection::send_nonblocking(std::shared_ptr<Connection> connection, char data[], size_t bytes_to_send) {

	{
		std::unique_lock<std::mutex> lock(this->connection_mtx_);

		if (!this->is_open()) {
			return;
		}

		char* bufcopy = new char[bytes_to_send];
		memcpy(bufcopy, data, bytes_to_send);

		std::pair<char*, size_t> buffer_size_tuple;
		std::get<0>(buffer_size_tuple) = bufcopy;
		std::get<1>(buffer_size_tuple) = bytes_to_send;

		this->sendbuffers_vec_.push_back(buffer_size_tuple);
		size_t tuple_index = this->sendbuffers_vec_.size() - 1;


		this->socket_.async_write_some(boost::asio::buffer(bufcopy, bytes_to_send), std::bind(&TCPConnection::handle_write, this, connection, tuple_index, std::placeholders::_1, std::placeholders::_2));

	}
}

void TCPConnection::close(std::shared_ptr<Connection> connection) {

	{
		std::unique_lock<std::mutex> lock(this->connection_mtx_);

		this->close_when_owning_mutex(connection);
	}

}

void TCPConnection::close_when_owning_mutex(std::shared_ptr<Connection> connection) {

	{
		std::unique_lock<std::mutex> lock(this->connection_mtx_, std::adopt_lock);

		this->socket_.close();
		this->OnConnectionClose(connection);

		// This will go wrong, mutex will unlock and will unlock again in the function calling this function.
	}

}

void TCPConnection::start_read(std::shared_ptr<Connection> connection) {

	this->socket_.async_read_some(boost::asio::buffer(this->data_, this->max_buf_length), std::bind(&TCPConnection::handle_read, this, connection, std::placeholders::_1, std::placeholders::_2));

}

void TCPConnection::handle_read(std::shared_ptr<Connection> connection, const boost::system::error_code& error, size_t bytes_transferred) {

	{
		std::unique_lock<std::mutex> lock(this->connection_mtx_);

		if (!this->socket_.is_open()) {
			return;
		}

		if (error != boost::system::errc::success) {
			this->close_when_owning_mutex(connection);
			return;
		}

		// Call all handers before reading more data.
		this->OnReceive(connection, bytes_transferred);

		this->start_read(connection);

	}

}

void TCPConnection::start_write(std::shared_ptr<Connection> connection) {

}

void TCPConnection::handle_write(std::shared_ptr<Connection> connection, size_t buffervec_index, const boost::system::error_code& error, size_t bytes_transferred) {

	{
		std::unique_lock<std::mutex> lock(this->connection_mtx_);

		// Get the buffer used to store the data to send.
		char* sendbuf_loc = std::get<0>(this->sendbuffers_vec_[buffervec_index]);
		size_t sendbuf_size = std::get<1>(this->sendbuffers_vec_[buffervec_index]);

		if (error == boost::system::errc::success) {
			/* If sending went without any errors, it means message has actually been sent.
			Call any handlers */
			this->OnSend(connection, sendbuf_loc, bytes_transferred);
		}

		// Delete the data afer all handlers are called and data is no longer needed.
		delete[] sendbuf_loc;

		// Remove pointer to the deleted buffer from the vector.
		this->sendbuffers_vec_.erase(this->sendbuffers_vec_.begin() + buffervec_index);

	}

}
