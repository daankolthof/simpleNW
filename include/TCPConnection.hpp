
#if !defined(_TCPCONNECTION_H)
#define _TCPCONNECTION_H

#include "Connection.hpp"

#include <tuple>
#include <vector>

class TCPConnection : public Connection {

public:

	TCPConnection(boost::asio::io_service& io_service, Server* server_ptr);
	~TCPConnection();

	boost::asio::ip::tcp::socket socket_;
	
	void send_nonblocking(char data[], size_t bytes_to_send);
	void close();

	void start_read();
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

	void start_write();
	void handle_write(size_t buffervec_index, const boost::system::error_code& error, size_t bytes_transferred);

protected:

private:

};

#endif
