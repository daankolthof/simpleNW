
#if !defined(_SSLOPTIONS_H)
#define _SSLOPTIONS_H

#include <boost/asio/ssl.hpp>

#include <functional>

class SSLOptions {

public:

	/* Set the file containing the certificate chain in PEM format.
	*/
	void setCertificateChainFile(std::string);

	/* Set the file containing the private key in PEM format.
	*/
	void setPrivateKeyFile(std::string);

private:

	std::string certificate_chain_file_ = "";
	std::string private_key_file_ = "";

};

#endif // _SSLOPTIONS_H