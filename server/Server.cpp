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

		//int size = snakes.size() - 1;

		//SDLNet_TCP_Send(client, &size, sizeof(int)); // So that clients can identify their snake

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

/*	for(const auto &i: snakes){
		for(const auto &j: i.body){
			SDLNet_TCP_Send(clients[i.id], &j, sizeof(Segment));
		}
	} */

	auto compact = [&](const Snake snake){
    	std::string buffer;
	    buffer += "V";


		for(const auto &i: snake.body){
			buffer += '0' + i.x;
			buffer += '0' + i.y;
		}

		return buffer;
	};


	

	for(const auto &i: clients){
		for(const auto &j: snakes){
				//SDLNet_TCP_Send(i, &j.id, sizeof(int));
		
				std::string data = compact(j);
		
				const char* positions = data.c_str();
		
				int size = strlen(positions) + 1;
				SDLNet_TCP_Send(i, &size, sizeof(int));
				SDLNet_TCP_Send(i, positions, size);
				//SDLNet_TCP_Send(i, &i.hit, sizeof(bool));
			}
	}
	
}

void Server::send_map(const int& apple){
	for(const auto &i: clients){
		SDLNet_TCP_Send(i, &apple, sizeof(int));
	}
}

void Server::receive_inputs(std::vector<Snake>& snakes){
/*
	for(const auto &i: clients){
		int dx = 0;
		int dy = 0;

		SDLNet_TCP_Recv(i, &dx, sizeof(int));
		SDLNet_TCP_Recv(i, &dy, sizeof(int));

		
	}
*/
	for(int i = 0; i < clients.size(); i++){
		int dx = 0;
		int dy = 0;

		SDLNet_TCP_Recv(clients[i], &dx, sizeof(int));
		SDLNet_TCP_Recv(clients[i], &dy, sizeof(int));

		snakes[i+1].dx = dx;
		snakes[i+1].dy = dy;
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
	auto collate = [&](const TCPsocket client, const Snake s){
		//int size = snakes.size();

		const char* name = s.name.c_str();
       
		//SDLNet_TCP_Send(client, &size, sizeof(int));
		int nsize = strlen(s.name.c_str()) + 1;
		SDLNet_TCP_Send(client, &nsize, sizeof(int));
		SDLNet_TCP_Send(client, name, nsize);
		

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

