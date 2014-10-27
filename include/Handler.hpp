
#if !defined(_HANDLER_H)
#define _HANDLER_H

#include <memory>

class Connection;

class Handler {
public:
    virtual void OnConnectionOpen(std::shared_ptr<Connection> connection) = 0;
    virtual void OnConnectionClose(std::shared_ptr<Connection> connection) = 0;
	virtual void OnReceive(std::shared_ptr<Connection> connection, const char data[], size_t bytes_received) = 0;
	virtual void OnSend(std::shared_ptr<Connection> connection, const char data[], size_t data_size, size_t bytes_sent) = 0;
};

#endif  //_HANDLER_H
