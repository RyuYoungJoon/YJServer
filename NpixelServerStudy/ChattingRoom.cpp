#include "stdafx.h"
#include "ChattingRoom.h"

void ChattingRoom::Enter(ChatPlayerPtr player)
{
	m_Player.insert(player);
	for (auto& msg : m_recentMsg)
		player->Send(msg);
}

void ChattingRoom::Leave(ChatPlayerPtr player)
{
	m_Player.erase(player);
}

void ChattingRoom::Send(const chat_message& msg)
{
	m_recentMsg.push_back(msg);
	while (m_recentMsg.size() > MaxBuffer)
		m_recentMsg.pop_front();

	for (auto& player : m_Player)
		player->Send(msg);
}
