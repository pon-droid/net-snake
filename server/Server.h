#pragma once

#include "Visual.h"




struct Server {
    TCPsocket bind_to_TCP(const std::string addr, const Uint16 port);
	Server();
	//std::vector<TCPsocket>clients;
	std::vector<TCPsocket>clients;
	TCPsocket server;	

	
	bool catch_clients(std::vector<Snake>& snakes); // Used
	
	void send_player_info(const std::vector<Snake>& snakes);
	void send_snakes(const std::vector<Snake>& snakes);
	void sync_lobby(const std::vector<Snake>& snakes);
	
	void send_list(const std::vector<Snake>& snakes); // Used
	void receive_inputs(std::vector<Snake>& snakes); // Used
	void send_map(const int& apple);
	
	void send_min(const std::vector<Snake>& snakes);
	bool state_signal(const int& STATE);
};
