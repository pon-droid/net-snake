#include "Client.h"

Client::Client(const std::string new_ip){
	ip = new_ip;
	in_server = false;
}

TCPsocket Client::bind_to_TCP(std::string addr, const Uint16 port){
    
	IPaddress ip;
	
	if(addr == "host"){
	    SDLNet_ResolveHost(&ip, NULL, port);
	} else {
		SDLNet_ResolveHost(&ip,addr.c_str(),port);
	}
	

	TCPsocket client = SDLNet_TCP_Open(&ip);

	return client;

}

int Client::join_server(const Snake& s){
    if(in_server){
    	return 2;
    }
    
	server = bind_to_TCP(ip,1234);

	if(server){
		int size = strlen(s.name.c_str()) + 1;

		const char* name = s.name.c_str();

		SDLNet_TCP_Send(server, &size, sizeof(int));
		SDLNet_TCP_Send(server, name, size);
		SDLNet_TCP_Send(server, &s.colour, sizeof(SDL_Colour));

        in_server = true;
		return 1;
	}

	return 0;
}

void Client::sync_lobby(std::vector<Snake>& snakes){
    int size = -10;
	SDLNet_TCP_Recv(server,&size, sizeof(int));

	if(size > snakes.size()){
		SDLNet_TCP_Recv(server, &size, sizeof(int));
		char player_name[size];
		SDLNet_TCP_Recv(server, player_name, size);
		Snake snake(size); // ID of snakes is not needed for clients, no worries
		snake.name = player_name;
		SDLNet_TCP_Recv(server, &snake.colour, sizeof(SDL_Colour));
	} 
}
