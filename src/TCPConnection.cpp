
#include <TCPConnection.hpp>

#include <DynamicArray.hpp>

#include <iostream>

TCPConnection::TCPConnection(boost::asio::io_service& io_service, std::shared_ptr<Server> server_ptr)
: Connection(server_ptr), socket_(io_service)
{
}

TCPConnection::~TCPConnection() {
	/* The destruction will be called when all shared pointers are destroyed.
	 * Which  means this object will not be used again.
	 */
}

bool TCPConnection::is_open() {
	return this->socket_.is_open();
}

void TCPConnection::send_nonblocking(char data[], size_t bytes_to_send) {

	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		if (!this->is_open()) {
			return;
		}

		DynamicArray<char> arr(bytes_to_send);
		memcpy(arr.data(), data, bytes_to_send);

		std::cout << "Should call move constructor now." << std::endl;
		this->socket_.async_write_some(boost::asio::buffer(arr.data(), arr.size()), std::bind(&TCPConnection::handle_write, this, this->this_shared_ptr_, std::move(arr), std::placeholders::_1, std::placeholders::_2));
		std::cout << "Move constructor called." << std::endl;
	}
}

void TCPConnection::send_nonblocking_buffer(char data[], size_t bytes_to_send) {
	/*
	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		if (!this->is_open()) {
			return;
		}

		DynamicArray<char> arr;

		char** ptr = arr.dataref();
		ptr = &data;
		arr.sizeref() = bytes_to_send;
		arr.should_delete_ref() = false;

		this->socket_.async_write_some(boost::asio::buffer(arr.data(), arr.size()), std::bind(&TCPConnection::handle_write, this, this->this_shared_ptr_, arr, std::placeholders::_1, std::placeholders::_2));
	}
	*/

}

void TCPConnection::close() {

	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		this->close_socket();
	}

}

void TCPConnection::close_socket() {
	this->OnConnectionClose();
	this->this_shared_ptr_.reset();
}

void TCPConnection::start_read() {

	/* Start an aysnc read. The data received is stored in the data_ member variable.
	Which is passed to any handlers in the handle_read function and subsequent calls. */
	this->socket_.async_read_some(boost::asio::buffer(this->data_, this->max_buf_length), std::bind(&TCPConnection::handle_read, this, this->this_shared_ptr_, std::placeholders::_1, std::placeholders::_2));

}

void TCPConnection::handle_read(std::shared_ptr<Connection> connection, const boost::system::error_code& error, size_t bytes_transferred) {

	std::cout << "Reveived data" << std::endl;

	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		if (!this->socket_.is_open()) {
			return;
		}

		if (error != boost::system::errc::success) {
			this->close_socket();
			return;
		}

		// Call all handers before reading more data.
		this->OnReceive(bytes_transferred);

		this->start_read();

	}

}

void TCPConnection::start_write() {

}

void TCPConnection::handle_write(std::shared_ptr<Connection> connection, DynamicArray<char> arr, const boost::system::error_code& error, size_t bytes_transferred) {

	
	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		if (error == boost::system::errc::success) {
			/* If sending went without any errors, it means message has actually been sent.
			Call any handlers */
			this->OnSend(arr.data(), arr.size(), bytes_transferred);
		}
	}

	// The DynamicArray used to hold the reference to the written data is deleted here.
}
