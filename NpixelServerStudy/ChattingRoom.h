#pragma once
#include "stdafx.h"

#include "Player.h"
typedef std::shared_ptr<Player> ChatPlayerPtr;


class ChattingRoom
{
public:
	void Enter(ChatPlayerPtr player);
	void Leave(ChatPlayerPtr player);
	void Send(const chat_message& msg);

private:
	std::set<ChatPlayerPtr> m_Player;

	chat_message_queue m_recentMsg;
};

