#include "Visual.h"
#include <time.h>

#define INTERVAL (60)

enum States {
	END
};

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

int main(int argc, char **argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDLNet_Init();

	srand(time(NULL));

	
	std::vector<Snake> snakes;

	Snake snake(MAP_W/2,MAP_H/2, {0,255,0,255}, "htirpone");
	Snake mouse(MAP_W/2 + 5, MAP_H/2 + 5, {255,0,0,255}, "dashiella");

	snakes.push_back(snake);
	snakes.push_back(mouse);
	Visual visual;



	

	
	TCPsocket client = begin_host("101.190.18.1",1234);


	
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

		snakes.back().update();
		

		

		SDLNet_TCP_Recv(client, &bytes, sizeof(int));

		snakes.front().body.resize(bytes);

		for(auto &i: snakes.front().body){
			SDLNet_TCP_Recv(client,&i,sizeof(Segment));
		}

		bytes = snakes.back().body.size();

		SDLNet_TCP_Send(client, &bytes, sizeof(int));

		

		for(const auto& i: snakes.back().body){
			SDLNet_TCP_Send(client, &i, sizeof(Segment));
		}

		/*
		for(int i = 0; i < snakes.size(); i++){
			snakes[i].update();
			snakes[i].check_col(snakes,i);
			visual.update_state(snakes[i]);
		}
		*/

		//visual.update_state(snakes.back());

		int random = 0;

		visual.MAP.fill(0);

		SDLNet_TCP_Recv(client, &random, sizeof(int));


		visual.MAP[random] = 1;



		

		
			
	    visual.update_buffer(snakes);
		

		//check_snakes(snakes);

		

		SDLNet_TCP_Recv(client, &bytes, sizeof(int));
		



		if(bytes == 1){
			SDLNet_TCP_Recv(client, &bytes, sizeof(int));

			visual.font_render({255,255,255,255},snakes[bytes].name + " has perished");
			SDL_Delay(10000);
			
		}



		SDL_Delay(time_left(next_time));
		next_time += INTERVAL;

		
	}





	return 0;
}
