#include "stdafx.h"

#include "ChattingRoom.h"
#include "Player.h"
#include "Server.h"
#include "Session.h"



int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_context io_context;

        std::list<Server> servers;
        for (int i = 1; i < argc; ++i)
        {
            tcp::endpoint endPoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_context, endPoint);
        }

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

}