#pragma once
#include "Snake.h"

struct Client {
	Client(const std::string new_ip);
	std::string ip;
	TCPsocket server;
    int snake_num;
	TCPsocket bind_to_TCP(std::string addr, const Uint16 port);
	int join_server(const Snake& s);
	void sync_lobby(std::vector<Snake>& snakes);
	bool in_server;
	void rec_list(std::vector<Snake>& snakes);
	
	bool state_signal();

	void receive_snakes(std::vector<Snake>& snakes);
	void send_dir(const int& dx, const int& dy);
	void receive_apple(std::array<int, (MAP_W*MAP_H)>& MAP);
};
