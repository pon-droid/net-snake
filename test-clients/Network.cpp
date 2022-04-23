#include "Network.h"

TCPsocket Network::begin_host(const char *addr, const Uint16 port){
    
	IPaddress ip;
	SDLNet_ResolveHost(&ip,addr,port);

	TCPsocket client = SDLNet_TCP_Open(&ip);

	return client;

}

void Network::connect_clients(const int CLIENT_COUNT, std::vector<Snake>& snakes){
		std::cout << "Not\n";

	server = begin_host(NULL,1234);

    
	std::cout << "Not\n";


	for(int i = 0; i < CLIENT_COUNT; i++){
		TCPsocket client;
		while(true){
			client = SDLNet_TCP_Accept(server);
			if(client){
				clients.push_back(client);
				break;
			}
		}
	}
	std::cout << "Not\n";

	Snake tmp;
	for(int i = 0; i < CLIENT_COUNT + 1; i++){
		snakes.push_back(tmp);
	}


}

void Network::setup_players(std::vector<Snake>& snakes){
	
	for(int i = 1; i < snakes.size(); i++){
		int cli_num = i - 1;
		int a;

		SDLNet_TCP_Recv(clients[cli_num], &a, sizeof(int));
		char tmp[a];

		SDLNet_TCP_Recv(clients[cli_num],tmp, a);

		snakes[i].name = tmp;

		SDLNet_TCP_Recv(clients[cli_num], &snakes[i].colour, sizeof(SDL_Colour));
	}

}

void Network::send_snakes(const std::vector<Snake>& snakes){
	for(const auto &i: snakes){
		
		int size = i.body.size();

		SDLNet_TCP_Send(server, &size, sizeof(int));

		for(const auto &j: i.body){
			SDLNet_TCP_Send(server, &j, sizeof(Segment));
		}

	}
}

void Network::send_snake(const Snake& s){
	int size = s.body.size();
	SDLNet_TCP_Send(server, &size, sizeof(int));

	Segment seg = {s.dx, s.dy};

	SDLNet_TCP_Send(server, &seg, sizeof(Segment));

	for(const auto &i: s.body){
		SDLNet_TCP_Send(server, &i, sizeof(Segment));
	}
}

void Network::receive_snake(Snake& s, TCPsocket client){
	int size;
	SDLNet_TCP_Recv(client, &size, sizeof(int));

	s.body.resize(size);

	Segment seg;
	SDLNet_TCP_Recv(client, &seg, sizeof(Segment));

	s.dx = seg.x;
	s.dy = seg.y;

	for(auto &i: s.body){
		SDLNet_TCP_Recv(client, &i, sizeof(Segment));
	}
}


void Network::receive_snakes(std::vector<Snake>& snakes){
	for(int i = 1; i < snakes.size(); i++){
		int cli_num = i - 1;
		int body_length;
		SDLNet_TCP_Recv(clients[cli_num], &body_length, sizeof(int));

		snakes[i].body.resize(body_length);

		for(int j = 0; j < snakes[i].body.size(); j++){
			SDLNet_TCP_Recv(clients[cli_num], &snakes[i].body[j], sizeof(Segment));
		}
	}
}


Network::~Network(){
	for(auto &i: clients){
		SDLNet_TCP_Close(i);
	}

	SDLNet_TCP_Close(server);
}