#include<SDL2/SDL.h>
#include<SDL2/SDL_net.h>


#define SCR_H (500)
#define SCR_W (500)
#define PL_S (.000001)
#define SEG (1)

typedef struct Fellow {
	float x, y;
} Fellow;



int run(void){
	SDL_Event event;
	
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT){
			return 0;
		}
	}

	return 1;
}

void draw_fellow(Fellow friend, SDL_Renderer *rend){
	SDL_SetRenderDrawColor(rend,0,255,0,255);
	SDL_Rect rect;

	rect.x = friend.x;
	rect.y = friend.y;

	rect.w = 5;
	rect.h = 5;

	SDL_RenderFillRect(rend,&rect);
}

void move_fellow(Fellow *friend, double delta){
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if(keys[SDL_SCANCODE_UP]){
		friend->y -= PL_S * delta;
	}
	if(keys[SDL_SCANCODE_DOWN]){
		friend->y += PL_S * delta;
	}
	if(keys[SDL_SCANCODE_RIGHT]){
		friend->x += PL_S * delta;
	}
	if(keys[SDL_SCANCODE_LEFT]){
		friend->x -= PL_S * delta;
	}
	
}

void draw_brother(Fellow friend, SDL_Renderer *rend){
	SDL_SetRenderDrawColor(rend,0,0,255,255);
	SDL_Rect rect;

	rect.x = friend.x;
	rect.y = friend.y;

	rect.w = 5;
	rect.h = 5;

	SDL_RenderFillRect(rend,&rect);
}

TCPsocket begin_host(char *addr, Uint16 port){
	IPaddress ip;
	SDLNet_ResolveHost(&ip,addr,port);

	TCPsocket client = SDLNet_TCP_Open(&ip);

	return client;
}

int main(void){
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	SDL_Window *win;
	win = SDL_CreateWindow("the other",0,0, SCR_W, SCR_H, SDL_WINDOW_SHOWN);

	SDL_Renderer *rend;
	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	Fellow friend;
	friend.x = SCR_W/2;
	friend.y = SCR_H/2;

	Fellow brother = friend;

	TCPsocket client = begin_host("127.0.0.1",1234);
	
	
    double t = 0.0;
    const double dt = 0.01;

    double curr_time = SDL_GetTicks();
    double accum = 0.0;
    printf("Work\n");
    
	while(run()){
        SDLNet_TCP_Send(client, &brother, sizeof(Fellow));
        if(SDLNet_TCP_Recv(client, &friend, sizeof(Fellow))){
        	 SDL_SetRenderDrawColor(rend,0,0,0,255);
	         SDL_RenderClear(rend);
	         draw_fellow(friend,rend);
	         draw_brother(brother,rend);
		     SDL_RenderPresent(rend);
        }


        
        double nt = SDL_GetTicks();
        double ft = nt - curr_time;
        curr_time = nt;

        accum += ft;

        while(accum >= dt){
          move_fellow(&brother,t);
          accum -= dt;
          t += dt;        	
        }

        
	
	}

	SDL_Quit();

	return 0;
}
