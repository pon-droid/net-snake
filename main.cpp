#include<SDL2/SDL_net.h>
#include"Snake.h"
#include"Visual.h"
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


int main(int argc, char **argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	srand(time(NULL));

	

	Snake snake(MAP_W/2,MAP_H/2);
	Visual visual;

	visual.MAP[MAP_H/2 * MAP_W + MAP_W/2 + 1] = 1;

	

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
*/      
		bytes = snake.body.size();
		SDLNet_TCP_Send(client,&bytes, sizeof(int));
		for(const auto& i: snake.body){
			SDLNet_TCP_Send(client,&i,sizeof(Segment));
		}
		


		snake.update();

		visual.update_state(snake);


	
	    visual.update_buffer(snake);

		SDL_Delay(time_left(next_time));
		next_time += INTERVAL;

		
	}

	SDL_Quit();
	SDLNet_Quit();


	return 0;
}
