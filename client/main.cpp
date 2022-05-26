#include "Visual.h"
#include "Client.h"
#include <time.h>

#define INTERVAL (60)



Uint32 time_left(Uint32 time){
	auto now = SDL_GetTicks();

	if(time <= now){
		return Uint32(0);
	} else {
		return time - now;
	}
}


bool run(void){
	SDL_Event event;
	
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT){
			return false;
		}
	}

	return true;
}





TCPsocket begin_host(char *addr, Uint16 port){
	IPaddress ip;
	SDLNet_ResolveHost(&ip,addr,port);

	TCPsocket client = SDLNet_TCP_Open(&ip);

	return client;
}
/*
void check_snakes(const std::vector<Snake>& snakes){
    //Check only the heads, to see if they collide
	// If head in (insert big list of segments) then fail
	// Maybe shared_ptr?
/*
	for(int i = 0; i < snakes.size() - 1; i++){
		for(const auto &j: snakes[i+1].body){
			if(j == snakes[i].body.front()){
				std::cout << snakes[i].name << "\n";
			}
		}
	}
/*

	std::unordered_map<Segment,int,Body_hash>doubles;

	for(const auto &i: snakes){
		for(const auto &j: i.body){
			doubles[j]++;
		}
	}

	for(const auto &i: doubles){
		if(i.second > 1){
			std::cout << i.first.owner << "\n";
		}
	}

	
}
*/

//enum {LOBBY, GAME};


std::vector<Snake> lobby_mode(Visual& visual, Client& client){

/*
	Network network;
		std::cout << "work\n";

	Visual visual;
			std::cout << "work\n";




	std::vector<Snake> snakes;

	Snake me(0,0,{255,0,0,255}, "htirpone");
	//Snake she(0,0,{255,0,0,255}, "dashi");
	

	snakes.push_back(me);
	//snakes.push_back(she);
	std::cout << "set\n";



	///network.setup_players(snakes);


	
	int state = LOBBY;

	int CLIENT_COUNT = 1;

	
	std::cout << "work\n";
	while(run()){
		network.connect_clients(snakes);
		visual.draw_lobby(snakes);
		std::cout << snakes.size() << std::endl;
	}
	*/



	std::vector<Snake>snakes;
	//Snake me(0,0, {255,0,0,255}, "big pon");
	
	Snake me(10,20, {0,0,255,255}, "darsh");
	snakes.push_back(me);

    auto next_time = SDL_GetTicks() + INTERVAL;



	while(run()){
		client.join_server(snakes.front());


		//std::cout << "After join\n";
		client.rec_list(snakes);
        //std::cout << "After sync\n";
		visual.draw_lobby(snakes);

		if(client.state_signal()){
			return snakes;
		}

		SDL_Delay(time_left(next_time));
		next_time += INTERVAL;

	}


	return snakes;
}


void game_mode(Visual& visual, Client& client, std::vector<Snake> snakes){
    auto next_time = SDL_GetTicks() + INTERVAL;

    auto set_direction = [&](int& dx, int& dy){
    	const Uint8 *keys = SDL_GetKeyboardState(NULL);


    	if(keys[SDL_SCANCODE_RIGHT] && !(dx == -1) && !(dy == 0)){
        	dx = 1;
        	dy = 0;
        	return;
    	}
    	if(keys[SDL_SCANCODE_LEFT] && !(dx == 1) && !(dy == 0)){
        	dx = -1;
        	dy = 0;
        	return;
    	}
    	if(keys[SDL_SCANCODE_UP] && !(dx == 0) && !(dy == 1)){
        	dx = 0;
        	dy = -1;
        	return;
    	}
    	if(keys[SDL_SCANCODE_DOWN] && !(dx == 0) && !(dy == -1)){
        	dx = 0;
        	dy = 1;
        	return;
    	}
    };
    int dx = 1;
    int dy = 1;
    while(run()){

    	set_direction(dx,dy);
    	client.send_dir(dx,dy);

    	client.receive_snakes(snakes);
    	client.receive_apple(visual.MAP);
    	visual.update_buffer(snakes);
    	

		SDL_Delay(time_left(next_time));
		next_time += INTERVAL;
    }
}

int main(int argc, char **argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDLNet_Init();

	srand(time(NULL));


	std::cout << "Work\n";

	Visual visual;
	Client client("124.183.33.112");

	auto snakes = lobby_mode(visual, client);



	game_mode(visual, client, snakes);
	


	return 0;
}
