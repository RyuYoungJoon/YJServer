#pragma once
#include "stdafx.h"


class Session : public Player
    , public std::enable_shared_from_this<Session>
{
    tcp::socket m_socket;
    ChattingRoom& m_room;
    chat_message readMsg;
    chat_message_queue writeMsg;

public:
    Session(tcp::socket _socket, ChattingRoom& _room)
        : m_socket(std::move(_socket)), m_room(_room)
    {}

    void Start()
    {
        m_room.Enter(shared_from_this());
        do_read_header();
    }

    void Send(const chat_message& msg)
    {
        bool writeProgress = !writeMsg.empty();
        writeMsg.push_back(msg);
        if (!writeProgress)
        {
            do_write();
        }
    }


private:
    void do_read_header()
    {
        auto self(shared_from_this());
        boost::asio::async_read(m_socket, boost::asio::buffer(readMsg.data(), chat_message::header_length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec && readMsg.decode_header())
                {
                    do_read_body();
                }
                else
                {
                    m_room.Leave(shared_from_this());
                }
            });
    }

    void do_read_body()
    {
        auto self(shared_from_this());
        boost::asio::async_read(m_socket, boost::asio::buffer(readMsg.body(), readMsg.body_length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    m_room.Send(readMsg);
                    do_read_header();
                }
                else
                {
                    m_room.Leave(shared_from_this());
                }
            });
    }

    void do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(m_socket, boost::asio::buffer(writeMsg.front().data(), writeMsg.front().length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    writeMsg.pop_front();
                    if (!writeMsg.empty())
                    {
                        do_write();
                    }
                }
                else
                {
                    m_room.Leave(shared_from_this());
                }
            });
    }

};