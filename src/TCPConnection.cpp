
#include <TCPConnection.hpp>

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

void TCPConnection::send_nonblocking(const char data[], size_t bytes_to_send) {

	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		if (!this->is_open()) {
			return;
		}

		char* buf = new char[bytes_to_send];
		memcpy(buf, data, bytes_to_send);

		std::tuple<char*, size_t> tup;
		std::get<0>(tup) = buf;
		std::get<1>(tup) = bytes_to_send;

		/*
		char* bufcopy = new char[bytes_to_send];
		memcpy(bufcopy, data, bytes_to_send);

		std::pair<char*, size_t> buffer_size_tuple;
		std::get<0>(buffer_size_tuple) = bufcopy;
		std::get<1>(buffer_size_tuple) = bytes_to_send;

		this->sendbuffers_vec_.push_back(buffer_size_tuple);
		size_t tuple_index = this->sendbuffers_vec_.size() - 1;

		*/

		this->socket_.async_write_some(boost::asio::buffer(buf, bytes_to_send), std::bind(&TCPConnection::handle_write, this, this->this_shared_ptr_, tup, std::placeholders::_1, std::placeholders::_2));

	}
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

	this->socket_.async_read_some(boost::asio::buffer(this->data_, this->max_buf_length), std::bind(&TCPConnection::handle_read, this, this->this_shared_ptr_, std::placeholders::_1, std::placeholders::_2));

}

void TCPConnection::handle_read(std::shared_ptr<Connection> connection, const boost::system::error_code& error, size_t bytes_transferred) {

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

/*
void TCPConnection::handle_write(std::shared_ptr<Connection> connection, size_t buffervec_index, const boost::system::error_code& error, size_t bytes_transferred) {

	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		// Get the buffer used to store the data to send.
		char* sendbuf_loc = std::get<0>(this->sendbuffers_vec_[buffervec_index]);
		size_t sendbuf_size = std::get<1>(this->sendbuffers_vec_[buffervec_index]);

		if (error == boost::system::errc::success) {
			/* If sending went without any errors, it means message has actually been sent.
			Call any handlers */ /*
			this->OnSend(sendbuf_loc, sendbuf_size, bytes_transferred);
		}

		// Delete the data afer all handlers are called and data is no longer needed.
		delete[] sendbuf_loc;

		// Remove pointer to the deleted buffer from the vector.
		this->sendbuffers_vec_.erase(this->sendbuffers_vec_.begin() + buffervec_index);

	}

}
*/

void TCPConnection::handle_write(std::shared_ptr<Connection> connection, std::tuple<char*, size_t> buf, const boost::system::error_code& error, size_t bytes_transferred) {


	
	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		if (error == boost::system::errc::success) {
			/* If sending went without any errors, it means message has actually been sent.
			Call any handlers */
			this->OnSend(std::get<0>(buf), std::get<1>(buf), bytes_transferred);
		}

		// Delete the allocated memory.
		delete std::get<0>(buf);
	}

}
