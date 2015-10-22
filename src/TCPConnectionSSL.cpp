
#include <TCPConnectionSSL.hpp>

TCPConnectionSSL::TCPConnectionSSL(boost::asio::io_service& io_service, boost::asio::ssl::context& ssl_context, std::shared_ptr<Server> server_ptr)
: TCPConnectionBase(io_service, server_ptr), ssl_socket_(io_service, ssl_context) {
}

void TCPConnectionSSL::start_handshake() {
	this->ssl_socket_.async_handshake(boost::asio::ssl::stream_base::server,
		std::bind(&TCPConnectionSSL::handle_handshake, this, std::placeholders::_1));
}

void TCPConnectionSSL::handle_handshake(const boost::system::error_code& error) {
	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		if (!this->is_open()) {
			return;
		}

		if (error != boost::system::errc::success) {
			this->close_socket();
			return;
		}

		/* Handshake has completed without any (security) errors.
		 * Start reading data.
		 */
		this->start_read();
	}
}

bool TCPConnectionSSL::is_open() {
	return this->ssl_socket_.lowest_layer().is_open();
}

void TCPConnectionSSL::send_nonblocking(char data[], size_t bytes_to_send) {

	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		if (!this->is_open()) {
			return;
		}

		DynamicArray<char> arr(bytes_to_send);
		memcpy(arr.data(), data, bytes_to_send);

		this->ssl_socket_.async_write_some(boost::asio::buffer(arr.data(), arr.size()), std::bind(&TCPConnectionBase::handle_write, this, this->this_shared_ptr_, std::move(arr), std::placeholders::_1, std::placeholders::_2));
	}
}

void TCPConnectionSSL::send_nonblocking_buffer(char data[], size_t bytes_to_send) {

	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		if (!this->is_open()) {
			return;
		}

		DynamicArray<char> arr(data, bytes_to_send);

		this->ssl_socket_.async_write_some(boost::asio::buffer(arr.data(), arr.size()), std::bind(&TCPConnectionBase::handle_write, this, this->this_shared_ptr_, arr, std::placeholders::_1, std::placeholders::_2));
	}
}

void TCPConnectionSSL::start_read() {

	/* Start an aysnc read. The data received is stored in the data_ member variable.
	Which is passed to any handlers in the handle_read function and subsequent calls. */
	this->ssl_socket_.async_read_some(boost::asio::buffer(this->data_, this->max_buf_length), std::bind(&TCPConnectionBase::handle_read, this, this->this_shared_ptr_, std::placeholders::_1, std::placeholders::_2));

}

void TCPConnectionSSL::handle_read(std::shared_ptr<Connection> connection, const boost::system::error_code& error, size_t bytes_transferred) {

	{
		std::unique_lock<std::recursive_mutex> lock(this->connection_mtx_);

		if (!this->is_open()) {
			return;
		}

		if (error != boost::system::errc::success) {
			this->close_socket();
			return;
		}

		// Call all handers before reading more data.
		this->OnReceive(this->data_, bytes_transferred);

		this->start_read();

	}

}

void TCPConnectionSSL::start_write() {

}

void TCPConnectionSSL::handle_write(std::shared_ptr<Connection> connection, DynamicArray<char> arr, const boost::system::error_code& error, size_t bytes_transferred) {


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

bool TCPConnectionSSL::endpoint_less_than(Connection* connection) const {
	assert(dynamic_cast<TCPConnectionSSL*>(connection) != nullptr); // Check that comparisons are not made between TCP, TCP with SSL and UDP connections.

	TCPConnectionSSL* tcpcon = static_cast<TCPConnectionSSL*>(connection);
	return this->ssl_socket_.lowest_layer().remote_endpoint() < tcpcon->ssl_socket_.lowest_layer().remote_endpoint();
}

bool TCPConnectionSSL::endpoint_equals(Connection* connection) const {
	assert(dynamic_cast<TCPConnectionSSL*>(connection) != nullptr); // Check that comparisons are not made between TCP, TCP with SSL and UDP connectinons.

	TCPConnectionSSL* tcpcon = static_cast<TCPConnectionSSL*>(connection);
	return this->ssl_socket_.lowest_layer().remote_endpoint() == tcpcon->ssl_socket_.lowest_layer().remote_endpoint();
}
