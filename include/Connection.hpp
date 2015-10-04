
#if !defined(_CONNECTION_H)
#define _CONNECTION_H

#include <thread>
#include <mutex>
#include <vector>
#include <tuple>

#include <boost/asio.hpp>

class Server;
class NetworkService;
class ConnectionInfo;

/** Represents a connection to a peer.
 * Can be used to read from and write to the peer.
 *
 */
class Connection {

	friend class ConnectionInfo;

	friend bool operator<(const ConnectionInfo&, const ConnectionInfo&);
	friend bool operator==(const ConnectionInfo&, const ConnectionInfo&);

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

	Connection() {}
	Connection(std::shared_ptr<Server> server_ptr);

	/* Used to keep the object alive, passed to the async functions.
	Delete this and close underlying connection to kill object. */
	std::shared_ptr<Connection> this_shared_ptr_;

	const std::shared_ptr<Server> server_ptr_;

	void OnConnectionOpen();
	void OnConnectionClose();
	void OnReceive(char data[], size_t bytes_received);
	void OnSend(char data[], size_t data_size, size_t bytes_sent);

	virtual ConnectionInfo constructConnectionInfo() = 0;
	virtual bool endpoint_less_than(Connection*) const = 0;
	virtual bool endpoint_equals(Connection*) const = 0;

private:

};

#endif //_CONNECTION_H
