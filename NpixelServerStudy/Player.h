#pragma once
#include "stdafx.h"

class Player
{
public:
    virtual ~Player() {}
    virtual void Send(const chat_message& msg) = 0;
};

