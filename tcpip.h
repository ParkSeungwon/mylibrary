//tcpip.h class definition
#include <string>
#include <thread>
#include <arpa/inet.h>

class Tcpip 
{
public:
	Tcpip(int port);
	virtual ~Tcpip();
	void send(std::string s);
	std::string recv();

protected:
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;

private:
	char buffer[1024];
};

class Client : public Tcpip
{
public:
	Client(std::string ip = "127.0.0.1", int port = 2001); 
};

class Server : public Tcpip
{
public:
	Server(int port = 2001, unsigned int time_out = 600, int queue = 10, std::string e = "end");
	void start(std::function<std::string(std::string)> functor);

protected:
	static void timed_out(int sig);
	std::string end_string;
	unsigned int time_out;
};
