#include <ctime>
#include <iostream>
#include <string>
#include "../asio/asio.hpp"

using asio::ip::tcp;

std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return ctime(&now);
}

int main()
{
	try
	{
		asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 2012));

		for (;;)
		{
			tcp::socket socket(io_context);
			acceptor.accept(socket);

			std::array<char, 128> buf;
			asio::error_code error;
			buf[socket.read_some(asio::buffer(buf), error)] = '\0';
			if (error)
			{
				asio::error_code ignored_error;
				socket.write_some(asio::buffer("error"), ignored_error);
				continue;
			}

			std::string str(buf.data());
			if (str == "get")
			{
				std::string message = make_daytime_string();
				asio::error_code ignored_error;
				socket.write_some(asio::buffer(message), ignored_error);
			}
			else
			{
				asio::error_code ignored_error;
				socket.write_some(asio::buffer("error"), ignored_error);
			}

			std::cout << "Server recieved." << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
