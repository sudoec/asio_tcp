#include <iostream>
#include "../asio/asio.hpp"

using asio::ip::tcp;

int main(int argc, char* argv[])
{
	try
	{
		//if (argc != 2)
		//{
		//	std::cerr << "Usage: client <host>" << std::endl;
		//	return 1;
		//}

		asio::io_context io_context;

		tcp::resolver resolver(io_context);
		tcp::resolver::results_type endpoints =
			resolver.resolve("127.0.0.1", "2014");

		tcp::socket socket(io_context);
		asio::connect(socket, endpoints);

		asio::error_code ignored_error;
		std::string sendStr = "    get";
		*(int*)&sendStr[0] = sendStr.size()-4;
		asio::write(socket, asio::buffer(sendStr), ignored_error);

		std::array<char, 128> buf;
		asio::error_code error;
		buf[asio::read(socket, asio::buffer(buf), error)] = '\0';
		//if (error == asio::error::eof)
		//	break; // Connection closed cleanly by peer.
		//else if (error)
		//	throw asio::system_error(error); // Some other error.
		std::string str(buf.data());
		std::cout << str << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
