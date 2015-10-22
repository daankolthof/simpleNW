
#include <TCPServerSSL.hpp>

#include <SSLOptions.hpp>
#include <TCPConnectionSSL.hpp>

TCPServerSSL::TCPServerSSL(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp, SSLOptions ssl_options)
: TCPServer(threads, server_port, callback_service, tp), ssl_context_(this->boost_io_service_, boost::asio::ssl::context::sslv23_server) {

	this->ssl_context_.set_options(boost::asio::ssl::context::default_workarounds
		| boost::asio::ssl::context::no_sslv2
		| boost::asio::ssl::context::single_dh_use);

	if (!ssl_options.certificate_chain_file_.empty) this->ssl_context_.use_certificate_chain_file(ssl_options.certificate_chain_file_);
	if (!ssl_options.private_key_file_.empty) this->ssl_context_.use_private_key_file(ssl_options.private_key_file_, boost::asio::ssl::context::file_format::pem);

}

void TCPServerSSL::start_accept() {

	// Fill in the connections smart pointer to itself.
	std::shared_ptr<TCPConnectionSSL> connection(new TCPConnectionSSL(this->boost_io_service_, this->ssl_context_, this->this_shared_ptr_));
	connection->this_shared_ptr_ = connection;

	// Async call to accept new connection.
	this->tcp_acceptor_.async_accept(connection->ssl_socket_.lowest_layer(),
		std::bind(&TCPServerSSL::handle_accept, this, connection,
		std::placeholders::_1)
		);

}

void TCPServerSSL::handle_accept(std::shared_ptr<TCPConnectionSSL> connection, const boost::system::error_code& error) {

	{
		std::unique_lock<std::mutex> lock(this->server_mtx_);

		if (this->boost_io_service_.stopped()) {
			this->stop_server();
		}

		if (error == boost::asio::error::operation_aborted) {
			/* Operation is aborted due to server stopping or other reasons,
			 * just return doing nothing. */
			return;
		} else if (error) {
			/* Another error, mark the connection as closed. */
			connection->close();
		}
		else
		{
			connection->start_handshake();
		}

		this->start_accept();

	}
}
