#include "Client.h"

Client::Client(const std::string new_ip){
	ip = new_ip;
	in_server = false;
	snake_num = 0;
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
		
		//SDLNet_TCP_Recv(server, &snake_num, sizeof(int));

		

        in_server = true;
		return 1;
	}

	return 0;
}

void Client::sync_lobby(std::vector<Snake>& snakes){
    if(!in_server){
    	return;
    }
    //int size = -10;
	//SDLNet_TCP_Recv(server,&size, sizeof(int));

	//if(size > snakes.size()){
	    int size = 0;
		if( SDLNet_TCP_Recv(server, &size, sizeof(int)) <= 0){
			return;
		}
		char player_name[size];
		SDLNet_TCP_Recv(server, player_name, size);
		Snake snake(size); // ID of snakes is not needed for clients, no worries
		snake.name = player_name;
		SDLNet_TCP_Recv(server, &snake.colour, sizeof(SDL_Colour));

		snakes.push_back(snake);
	//} 
}

void Client::rec_list(std::vector<Snake>& snakes){
	if(!in_server){
		return;
	}

	int size;
	SDLNet_TCP_Recv(server, &size, sizeof(int));

	snakes.resize(size);

	//std::cout << "still on " << snakes.size() << "\n";

	for(int i = 0; i < snakes.size(); i++){
	    
		SDLNet_TCP_Recv(server, &size, sizeof(int));
		//std::cout << "new size is: " << size << "\n";
		char name[size];
		SDLNet_TCP_Recv(server, name, size);
		Snake snake(i);
		SDLNet_TCP_Recv(server, &snake.colour, sizeof(SDL_Colour));

		snake.name = name;

		snakes[i] = snake;
		
		
		
	}
}

bool Client::state_signal(){
    if(!in_server){
    	return false;
    }
	int state = LOBBY;

	SDLNet_TCP_Recv(server, &state, sizeof(int));

	if(state == GAME){
		return true;
	} else {
		return false;
	}
}

void Client::receive_snakes(std::vector<Snake>& snakes){
   auto decompact = [&](const std::string buffer){
        std::deque<Segment>body;
        
		if(buffer[0] != 'V'){
			return body;
		}

		//int num = buffer[1] - '0';
    	
		for(int i = 1; i < buffer.size(); i+=2){
			Segment seg;
			seg.x = buffer[i] - '0';
			seg.y = buffer[i+1] - '0';
			body.push_back(seg);
		}


		return body;

  };

    for(auto &i: snakes){
        int size = 0;
    	SDLNet_TCP_Recv(server, &size, sizeof(int));
    	char buffer[size];
    	SDLNet_TCP_Recv(server, buffer, size);
    	//SDLNet_TCP_Recv(server, &i.end, sizeof(bool));


        i.body = decompact(buffer);
    	
    }
	
}

void Client::send_dir(const int& dx, const int& dy){
	SDLNet_TCP_Send(server, &dx, sizeof(int));
	SDLNet_TCP_Send(server, &dy, sizeof(int));
}

void Client::receive_apple(std::array<int, (MAP_W*MAP_H)>& MAP){
	int apple = 0;
	SDLNet_TCP_Recv(server, &apple, sizeof(int));
	MAP.fill(0);
	MAP[apple] = 1;
}
