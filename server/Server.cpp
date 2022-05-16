#include "Server.h"

TCPsocket Server::bind_to_TCP(const std::string addr, const Uint16 port){
    
	IPaddress ip;
	
	if(addr == "host"){
	    SDLNet_ResolveHost(&ip, NULL, port);
	} else {
		SDLNet_ResolveHost(&ip,addr.c_str(),port);
	}
	

	TCPsocket client = SDLNet_TCP_Open(&ip);

	return client;

}

Server::Server(){
	server = bind_to_TCP("host", 1234);
}

bool Server::catch_clients(std::vector<Snake>& snakes){
	TCPsocket client = SDLNet_TCP_Accept(server);

	if(client){
	//	int id;
	//	SDLNet_TCP_Recv(client, &id, sizeof(int));
		clients.push_back(client);
		
		Snake snake(clients.size() - 1);



		int strlength;
		
		SDLNet_TCP_Recv(client, &strlength, sizeof(int));
		
		char player_name[strlength];
		SDLNet_TCP_Recv(client, player_name, strlength);
		

		snake.name = player_name;
		
		SDLNet_TCP_Recv(client, &snake.colour, sizeof(SDL_Colour));

		snakes.push_back(snake);

		return true;
	}

	return false;
}

void Server::send_player_info(const std::vector<Snake>& snakes){
	for(const auto &i: clients){
		for(const auto &j: snakes){
			
            int size = strlen(j.name.c_str()) + 1;

		    const char* name = j.name.c_str();

		    SDLNet_TCP_Send(i, &size, sizeof(int));
		    SDLNet_TCP_Send(i, name, size);
		    SDLNet_TCP_Send(i, &j.colour, sizeof(SDL_Colour));
		}
	}
}

void Server::send_snakes(const std::vector<Snake>& snakes){

	for(const auto &i: snakes){
		for(const auto &j: i.body){
			SDLNet_TCP_Send(clients[i.id], &j, sizeof(Segment));
		}
	}
	
}

void Server::sync_lobby(const std::vector<Snake>& snakes){

  for(const auto &i: clients){    
		for(const auto &j: snakes){
            int size = snakes.size();
		    SDLNet_TCP_Send(i, &size, sizeof(int));
		    
		    const char* name = j.name.c_str();
		    size = strlen(name) + 1;

		    std::cout << size << "\n";
		    
			SDLNet_TCP_Send(i, &size, sizeof(int));
			SDLNet_TCP_Send(i, name, size);
			SDLNet_TCP_Send(i, &j.colour, sizeof(SDL_Colour));
		}
}


    
		    

}

void Server::send_list(const std::vector<Snake>& snakes){
    if( !(clients.size() > 0)){
    	return;
    }
	auto collate = [&](TCPsocket client, Snake s){
		//int size = snakes.size();

		const char* name = s.name.c_str();
       
		//SDLNet_TCP_Send(client, &size, sizeof(int));
		int size = strlen(s.name.c_str()) + 1;
		SDLNet_TCP_Send(client, &size, sizeof(int));
		SDLNet_TCP_Send(client, name, size);
		

		SDLNet_TCP_Send(client, &s.colour, sizeof(SDL_Colour));
	};

	for(const auto &i: clients){
	    int size = snakes.size();
	    SDLNet_TCP_Send(i, &size, sizeof(int));
		for(const auto &j: snakes){
			collate(i,j);
		}
	}

	
}

bool Server::state_signal(const int& STATE){
	for(const auto &i: clients){
		SDLNet_TCP_Send(i, &STATE, sizeof(int));
	}
	
	if(STATE != LOBBY){
		return true;
	} else {
		return false;
	}
}
