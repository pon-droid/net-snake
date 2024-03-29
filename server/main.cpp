#include "Server.h"
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


std::vector<Snake> lobby_mode(Visual& visual, Server& server, int& STATE){

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
	
/*
	Visual visual;
	Server server;

	std::vector<Snake>snakes;
	//Snake me(0,0, {255,0,0,255}, "big pon");
	snakes.push_back({0,0, {255,0,0,255}, "big pon"});

	while(run()){
		if(server.catch_clients(snakes)){
			server.sync_lobby(snakes);			
		}

		visual.draw_lobby(snakes);
	}
*/

    //Visual visual;
    //Server server;

    std::vector<Snake> snakes;
    
    snakes.push_back({10,10, {255,0,0,255}, "big pon"});

    auto next_time = SDL_GetTicks() + INTERVAL;

    //int state = LOBBY;

    while(run()){
        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if(keys[SDL_SCANCODE_9]){ STATE = GAME; }
        
    	server.catch_clients(snakes);
    	server.send_list(snakes);

    	visual.draw_lobby(snakes);

    	if(server.state_signal(STATE)){
    		break;
    	}
        

    	SDL_Delay(time_left(next_time));
		next_time += INTERVAL;

		
    }

	return snakes;
}

void game_mode(Visual& visual, Server& server, std::vector<Snake>& snakes){
	auto next_time = SDL_GetTicks() + INTERVAL;

	while(run()){
		server.receive_inputs(snakes);
		snakes.front().control();
		

		for(auto &i: snakes) { if(i.end){continue;} i.update(); visual.update_state(i); }

		visual.update_buffer(snakes);

		server.send_snakes(snakes);
		server.send_map(visual.last_pos);

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
	Server server;
    int STATE = LOBBY;
	auto snakes = lobby_mode(visual,server, STATE);

	game_mode(visual, server, snakes);

	
	
	return 0;
}
