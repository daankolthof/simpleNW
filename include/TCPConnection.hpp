
#if !defined(_TCPCONNECTION_H)
#define _TCPCONNECTION_H

#include "Connection.hpp"
#include "Server.hpp"

#include <tuple>
#include <vector>


class TCPConnection : public Connection {

public:

	TCPConnection(boost::asio::io_service& io_service, Server* server_ptr);
	~TCPConnection();

	const static int max_buf_length = 1024;

	std::vector<std::pair<char*, size_t>> sendbuffers_vec_;

	boost::asio::ip::tcp::socket socket_;
	char data_[max_buf_length];

	Server* server_ptr_;

	void send_nonblocking(char data[], size_t bytes_to_send);
	void close();

	void start_read();
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

	void start_write();
	void handle_write(size_t buffervec_index);

	/* TODO: Move these to the parent class.
	*/
	void OnConnectionOpen();
	void OnConnectionClose();
	void OnReceive(size_t bytes_received);
	void OnSend(char data[], size_t bytes_sent);

protected:

private:

};

#endif
