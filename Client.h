#pragma once
#include "Snake.h"

struct Client {
	Client(const std::string new_ip);
	std::string ip;
	TCPsocket server;
	TCPsocket bind_to_TCP(std::string addr, const Uint16 port);
	int join_server(const Snake& s);
	void sync_lobby(std::vector<Snake>& snakes);
	bool in_server;
};
