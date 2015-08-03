#if !defined(_BUFFERINGHANDLER_H)
#define _BUFFERINGHANDLER_H

#include <Handler.hpp>

#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <vector>

class ConnectionInfo;

class BufferingHandler : public Handler {

public:

	BufferingHandler(std::function<void(std::shared_ptr<Connection>, std::vector<char>)>);

	char getDelimiter();
	void setDelimiter(char);

protected:

private:

	void OnReceive(std::shared_ptr<Connection>, char[], size_t) override;

	std::atomic<char> delimiter_ = '\n';

	std::mutex buffers_mtx;
	std::map<ConnectionInfo, std::pair<std::unique_ptr<std::vector<char>>, std::unique_ptr<std::mutex>>> buffers_;
	
	std::function<void(std::shared_ptr<Connection>, std::vector<char>)> tocall_;

};

#endif
