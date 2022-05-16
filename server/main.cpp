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


std::vector<Snake> lobby_mode(Visual& visual, Server& server){

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
    
    snakes.push_back({0,0, {255,0,0,255}, "big pon"});

    auto next_time = SDL_GetTicks() + INTERVAL;

    int state = LOBBY;

    while(run()){
        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if(keys[SDL_SCANCODE_9]){
        	state = GAME;

        }
    	server.catch_clients(snakes);
    	server.send_list(snakes);

    	visual.draw_lobby(snakes);

    	server.state_signal(state);

    	if(state == GAME){
    		        	while(run()){
        		
        	}

        	break;
    	}

    	SDL_Delay(time_left(next_time));
		next_time += INTERVAL;

		
    }

	return snakes;
}

int main(int argc, char **argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDLNet_Init();

	srand(time(NULL));


	std::cout << "Work\n";

	Visual visual;
	Server server;

	lobby_mode(visual,server);
	return 0;



	
	std::vector<Snake> snakes;

	Snake snake(MAP_W/2,MAP_H/2, {0,255,0,255}, "htirpone");
	Snake mouse(MAP_W/2 + 5, MAP_H/2 + 5, {255,0,0,255}, "dashiella");

	snakes.push_back(snake);
	snakes.push_back(mouse);
	Visual visual;



	

	TCPsocket serv = begin_host(NULL,1234);
	TCPsocket client;

	

	while(1){
		client = SDLNet_TCP_Accept(serv);
		if(client){
			break;
		}
	}
	
	/*
    double t = 0.0;
    const double dt = 0.01;

    double curr_time = SDL_GetTicks();
    double accum = 0.0;
	*/

    auto next_time = SDL_GetTicks() + INTERVAL;
	int bytes = 0;



    
	while(run()){
/*
        //SDLNet_TCP_Send(client, &friend, sizeof(Fellow));
        //SDLNet_TCP_Recv(client, &brother, sizeof(Fellow));
        t = 0.0;
        
        double nt = SDL_GetTicks();
        double ft = nt - curr_time;
        curr_time = nt;

        accum += ft;

        while(accum >= dt){
          accum -= dt;
          t += dt;        	
        }
*/      /*
		mouse.body.clear();
		bytes = snake.body.size();
		SDLNet_TCP_Send(client,&bytes, sizeof(int));
		for(const auto& i: snake.body){
			SDLNet_TCP_Send(client,&i,sizeof(Segment));
		}

		SDLNet_TCP_Recv(client, &bytes, sizeof(int));

		
		mouse.body.resize(bytes);

		

		for(auto& i: mouse.body){
			SDLNet_TCP_Recv(client, &i, sizeof(Segment));
		}
		

*/


		//snakes.front().update();

		int random = rand() % (MAP_W * MAP_H);

		snakes.front().update();


		bytes = snakes.front().body.size();

		SDLNet_TCP_Send(client, &bytes, sizeof(int));
		SDLNet_TCP_Send(client, &snakes.back().hit, sizeof(bool));


		for(const auto &i: snakes.front().body){
			SDLNet_TCP_Send(client,&i,sizeof(Segment));
		}

		SDLNet_TCP_Recv(client, &bytes, sizeof(int));

		snakes.back().body.resize(bytes);

		for(auto& i: snakes.back().body){
			SDLNet_TCP_Recv(client, &i, sizeof(Segment));
		}

		/*
		for(int i = 0; i < snakes.size(); i++){
			snakes[i].update();
			snakes[i].check_col(snakes,i);
			visual.update_state(snakes[i]);
		}
		*/
		if(visual.apple){
			random = visual.last_pos;
		}

		SDLNet_TCP_Send(client, &random, sizeof(int));


		visual.update_state(snakes.back(),random);
		visual.update_state(snakes.front(),random);


			
	    visual.update_buffer(snakes);

		snakes.back().check_col(snakes,1);
		snakes.front().check_col(snakes,0);
		

		//check_snakes(snakes);

		

		int a = 0;
		bool end = false;
		for(const auto &i: snakes){
			if(i.end){
				end = true;
				break;
			}
			a++;
		}

		if(end){
			bytes = 1;
			SDLNet_TCP_Send(client, &bytes, sizeof(int));
			SDLNet_TCP_Send(client, &a, sizeof(int));
			visual.font_render({255,255,255,255},snakes[a].name + " has perished");
			SDL_Delay(1500);

		

			snakes[0] = snake;
			snakes[1] = mouse;

			//return 0;
		} else {
			bytes = 0;
			SDLNet_TCP_Send(client, &bytes, sizeof(int));
		}

		SDL_Delay(time_left(next_time));
		next_time += INTERVAL;

		
	}





	return 0;
}
