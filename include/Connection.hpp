
#if !defined(_CONNECTION_H)
#define _CONNECTION_H

#include <thread>
#include <mutex>
#include <vector>
#include <tuple>

#include <boost/asio.hpp>

class Server;
class NetworkService;

/** Represents a connection to a peer.
 * Can be used to read from and write to the peer.
 *
 */
class Connection {

public:

	virtual ~Connection();

	/* Return wether the connection is open and valid.
	A valid and open connection will be able to send and receive data. */
	virtual bool is_open() = 0;

	/* Will close or otherwise make connection invalid.
	An invalid and or closed connection will not be able to send and receive data. */
	virtual void close() = 0;

	/* Make an internal copy of the buffer and send it, data can be cleared after calling this. */
	virtual void send_nonblocking(char data[], size_t bytes_to_send) = 0;

	/* Caller of this function is responsible for keeping the data buffer valid until
	send has completed and last Handler has been called*/
	virtual void send_nonblocking_buffer(char data[], size_t bytes_to_send) = 0;

protected:

	Connection(std::shared_ptr<Server> server_ptr);

	std::recursive_mutex connection_mtx_;

	/* Used to keep the object alive, passed to the async functions.
	Delete this and close underlying connection to kill object. */
	std::shared_ptr<Connection> this_shared_ptr_;

	const static int max_buf_length = 1024;

	std::vector<std::pair<char*, size_t>> sendbuffers_vec_;

	const std::shared_ptr<Server> server_ptr_;
	char data_[max_buf_length];

	virtual void start_read() = 0;
	virtual void handle_read(std::shared_ptr<Connection> connection, const boost::system::error_code& error, size_t bytes_transferred) = 0;

	virtual void start_write() = 0;

	void OnConnectionOpen();
	void OnConnectionClose();
	void OnReceive(size_t bytes_received);
	void OnSend(char data[], size_t data_size, size_t bytes_sent);

private:

};

#endif //_CONNECTION_H
