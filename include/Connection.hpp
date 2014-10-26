
#if !defined(_CONNECTION_H)
#define _CONNECTION_H

#include <thread>
#include <mutex>
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

	std::mutex connection_mtx_;

	/* To keep the object alive, passed to the async functions.
	Delete this to kill object. */
	std::shared_ptr<Connection> this_shared_ptr_;

	const static int max_buf_length = 1024;

	std::vector<std::pair<char*, size_t>> sendbuffers_vec_;

	Server* server_ptr_;
	char data_[max_buf_length];

	virtual bool is_open() = 0;

	virtual void send_nonblocking(char data[], size_t bytes_to_send) = 0;

	virtual void start_read() = 0;
	virtual void handle_read(std::shared_ptr<Connection> connection, const boost::system::error_code& error, size_t bytes_transferred) = 0;

	virtual void start_write() = 0;
	virtual void handle_write(std::shared_ptr<Connection> connection, size_t buffervec_index, const boost::system::error_code& error, size_t bytes_transferred) = 0;

	void OnConnectionOpen();
	void OnConnectionClose();
	void OnReceive(size_t bytes_received);
	void OnSend(char data[], size_t bytes_sent);

protected:

private:

};

#endif
