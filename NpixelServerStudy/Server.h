#pragma once
#include "stdafx.h"
#include "Session.h"

class Server
{
public:
    Server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint)
        : m_acceptor(io_context, endpoint)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        m_acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::make_shared<Session>(std::move(socket), m_room)->Start();
                }
                
                do_accept();
            });
    }

    tcp::acceptor m_acceptor;
    ChattingRoom m_room;

};

