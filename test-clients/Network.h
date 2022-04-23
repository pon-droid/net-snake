#pragma once

#include "Visual.h"

struct Network {
    TCPsocket begin_host(const char *addr, const Uint16 port);

    

    void connect_clients(const int CLIENT_COUNT, std::vector<Snake>& snakes);
    void setup_players(std::vector<Snake>& snakes);
    void receive_snakes(std::vector<Snake>& snakes);
    void send_snake(const Snake& s);
    void send_snakes(const std::vector<Snake>& snakes);

    void receive_snake(Snake& s, TCPsocket client);
    //void sync_map()

    TCPsocket server;
    std::vector<TCPsocket> clients;

    ~Network();


};