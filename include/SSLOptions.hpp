
#if !defined(_SSLOPTIONS_H)
#define _SSLOPTIONS_H

#include <boost/asio/ssl.hpp>

#include <functional>

class SSLOptions {

	friend class TCPServerSSL;

public:

	/* Set the file containing the certificate chain in PEM format.
	*/
	void setCertificateChainFile(std::string& filename) {
		this->options_set_ = true;
		this->certificate_chain_file_ = filename;
	}

	/* Set the file containing the private key in PEM format.
	*/
	void setPrivateKeyFile(std::string& filename) {
		this->options_set_ = true;
		this->private_key_file_ = filename;
	}

	/* Returns true if any SSL options are set, meaning SSL will be used.
	*/
	bool options_set() {
		return this->options_set_;
	}


private:

	bool options_set_ = false;

	std::string certificate_chain_file_ = "";
	std::string private_key_file_ = "";

};

#endif // _SSLOPTIONS_H
