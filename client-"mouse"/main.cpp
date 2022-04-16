#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include "Snake.h"
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


int main(int argc, char **argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDLNet_Init();

	srand(time(NULL));

	

	Snake snake(MAP_W/2,MAP_H/2, {0,255,0,255});
	Snake mouse(MAP_W/2 + 5, MAP_H/2 + 5, {255,0,0,255});
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

	std::cout << "Hello\n";

    
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
		SDLNet_TCP_Recv(client,&bytes, sizeof(int));
		snake.body.resize(bytes);

		for(auto& i: snake.body){
			SDLNet_TCP_Recv(client,&i,sizeof(Segment));
		}

		bytes = mouse.body.size();

		SDLNet_TCP_Send(client, &bytes, sizeof(int));

		

		for(const auto& i: mouse.body){
			SDLNet_TCP_Send(client, &i, sizeof(Segment));
		}
		


		//snake.update();

		mouse.update();
			
	    visual.update_buffer(snake, mouse);

		if(snake.end || mouse.end){
			visual.font_render();
			SDL_Delay(1500);
			break;
		}

		SDL_Delay(time_left(next_time));
		next_time += INTERVAL;

		
	}

	SDL_Quit();
	TTF_Quit();
	SDLNet_Quit();


	return 0;
}
