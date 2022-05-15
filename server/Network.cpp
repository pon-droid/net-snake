#include "Network.h"

TCPsocket Network::begin_host(const char *addr, const Uint16 port){
    
	IPaddress ip;
	SDLNet_ResolveHost(&ip,addr,port);

	TCPsocket client = SDLNet_TCP_Open(&ip);

	return client;

}

Network::Network(){
	server = begin_host(NULL, 1234);

}

void Network::connect_clients(std::vector<Snake>& snakes){
/*

	server = begin_host(NULL,1234);

    


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

	Snake tmp;
	for(int i = 0; i < CLIENT_COUNT + 1; i++){
		snakes.push_back(tmp);
	}
*/


	int time = SDL_GetTicks();

	TCPsocket client;
	while(true){
		client = SDLNet_TCP_Accept(server);
		if(client){
			clients.push_back(client);

			int strlength;

			SDLNet_TCP_Recv(clients.back(), &strlength, sizeof(int));

			char player_name[strlength];
			SDLNet_TCP_Recv(clients.back(), player_name, strlength);

			Snake snake;
			snake.name = player_name;

			snake.index = clients.size() - 1;

			SDLNet_TCP_Recv(clients.back(), &snake.colour, sizeof(SDL_Colour));
			SDLNet_TCP_Send(clients.back(), &snake.index, sizeof(int));

			return;
		} /*else if((SDL_GetTicks() - time) > 500){
			return;
		}*/
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
		SDLNet_TCP_Send(clients[cli_num], &i, sizeof(int));
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

int Network::init_send_snake(const Snake& s){
	int size = strlen(s.name.c_str()) + 1;

	const char *string = s.name.c_str();

	Segment dir;
	dir.x = s.dx;
	dir.y = s.dy;

	SDLNet_TCP_Send(server, &size, sizeof(int));
	SDLNet_TCP_Send(server, string, size);
	SDLNet_TCP_Send(server, &s.colour, sizeof(SDL_Colour));
	int element;
	SDLNet_TCP_Recv(server, &element, sizeof(int));

	return element;
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