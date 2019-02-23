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

std::string make_string(asio::streambuf& streambuf)
{
	return { asio::buffers_begin(streambuf.data()),
			asio::buffers_end(streambuf.data()) };
}

int main()
{
	try
	{
		asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 2014));

		for (;;)
		{
			tcp::socket socket(io_context);
			acceptor.accept(socket);

			asio::streambuf buf;
			asio::error_code error;
			//std::array<char, 128> buf;
			//buf[asio::read_some(socket, buf, error)] = '\0';
			//asio::read_until(socket, buf, '\0', error);
			asio::read(socket, buf, asio::transfer_exactly(4), error);
			int tranSize = *asio::buffer_cast<const int*>(buf.data());
			buf.consume(4);
			asio::read(socket, buf, asio::transfer_exactly(tranSize), error);
			
			if (error)
			{
				asio::error_code ignored_error;
				asio::write(socket, asio::buffer("error"), ignored_error);
				continue;
			}

			std::string str(make_string(buf));
			if (str == "get")
			{
				std::string message = make_daytime_string();
				asio::error_code ignored_error;
				asio::write(socket, asio::buffer(message), asio::transfer_exactly(message.size()), ignored_error);
			}
			else
			{
				asio::error_code ignored_error;
				asio::write(socket, asio::buffer("error"), ignored_error);
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
