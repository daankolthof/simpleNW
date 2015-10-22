
#include <TCPServerSSL.hpp>

#include <SSLOptions.hpp>

TCPServerSSL::TCPServerSSL(int threads, uint16_t server_port, NetworkService* callback_service, TransportProtocol tp, SSLOptions ssl_options)
: TCPServer(threads, server_port, callback_service, tp), ssl_context_(this->boost_io_service_, boost::asio::ssl::context::sslv23_server) {

	this->ssl_context_.set_options(boost::asio::ssl::context::default_workarounds
		| boost::asio::ssl::context::no_sslv2
		| boost::asio::ssl::context::single_dh_use);

	if (!ssl_options.certificate_chain_file_.empty) this->ssl_context_.use_certificate_chain_file(ssl_options.certificate_chain_file_);
	if (!ssl_options.private_key_file_.empty) this->ssl_context_.use_private_key_file(ssl_options.private_key_file_, boost::asio::ssl::context::file_format::pem);


}
