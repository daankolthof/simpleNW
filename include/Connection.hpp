
#if !defined(_CONNECTION_H)
#define _CONNECTION_H

#include <vector>
#include <tuple>

#include <boost/asio.hpp>

class Server;
class NetworkService;


class Connection {

public:

	Connection();
	Connection(Server* server_ptr);
	virtual ~Connection();

	const static int max_buf_length = 1024;

	std::vector<std::pair<char*, size_t>> sendbuffers_vec_;

	Server* server_ptr_;
	char data_[max_buf_length];

	virtual void send_nonblocking(char data[], size_t bytes_to_send) = 0;

	virtual void start_read() = 0;
	virtual void handle_read(const boost::system::error_code& error, size_t bytes_transferred) = 0;

	virtual void start_write() = 0;
	virtual void handle_write(size_t buffervec_index, const boost::system::error_code& error, size_t bytes_transferred) = 0;

	void OnConnectionOpen();
	void OnConnectionClose();
	void OnReceive(size_t bytes_received);
	void OnSend(char data[], size_t bytes_sent);


protected:

private:

};

#endif
