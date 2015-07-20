
#if !defined(_HANDLER_H)
#define _HANDLER_H

#include <memory>

class Connection;

/** Handler is a class that can be registered to a NetworkService and will have its
 * functions called when things happen on the underlying connection.
 * This class should be inherited and its member functions should be defined in child classes.
 */
class Handler {
public:

	/** Will be called whenever a connection opens.
	 * Will be called whenever a connection is opened in the NetworkService the handler is registered to.
	 */
	virtual void OnConnectionOpen(std::shared_ptr<Connection> connection) = 0;

	/** Will be called whenever a connection closes.
	 * Will be called whenever a connection is closed in the NetworkService the handler is registered to.
	 */
	virtual void OnConnectionClose(std::shared_ptr<Connection> connection) = 0;

	/** Will be called whenever data has been received on the connection.
	 * Will be called whenever data has been received on a connection in the NetworkService the handler is registered to.
	 */
	virtual void OnReceive(std::shared_ptr<Connection> connection, char data[], size_t bytes_received) = 0;

	/** Will be called whenever data has been succesfully send through the connection.
	 * Will be called whenver data has been succesfully send through the connection in the NetworkService the handler is registered to.
	 */
	virtual void OnSend(std::shared_ptr<Connection> connection, char data[], size_t data_size, size_t bytes_sent) = 0;
};

#endif  //_HANDLER_H
