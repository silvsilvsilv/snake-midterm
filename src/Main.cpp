#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <vector>
#include <algorithm>
#include <deque>

using namespace std;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    auto window = SDL_CreateWindow("Snake by John Silva", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 700, 0);
    auto renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface* icon = IMG_Load("res/anaconda.png");
    SDL_SetWindowIcon(window, icon);
    
    SDL_Event e;
    enum Direction
    {
        DOWN,
        LEFT,
        RIGHT,
        UP,
        STOP
    };

    bool running = true;
    int dir = 0;

    SDL_Rect head; //initializing of the "snake"
    head.x=450;
    head.y=350;
    head.w=10;
    head.h=10;

    //main game loop
    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT) { running = false; }  
            if(e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym) //controls 
                {
                    //arrow controls
                    case SDLK_DOWN: dir = DOWN; break;
                    case SDLK_UP: dir = UP; break;
                    case SDLK_LEFT: dir = LEFT; break;
                    case SDLK_RIGHT: dir = RIGHT; break;

                    //wasd controls like real gamer 8)
                    case SDLK_s: dir = DOWN; break;
                    case SDLK_w: dir = UP; break;
                    case SDLK_a: dir = LEFT; break;
                    case SDLK_d: dir = RIGHT; break;

                    case SDLK_RETURN: head.x=450; head.y=350; break;
                }
            }
        }


        //TEMP: handles the death of the snake when hitting the corners
        // will add the death condition for when it hits its own body part
        if( head.x >= 890 )
        {
            dir = STOP;
            head.x = 890;
        }
        else if( head.x <= 0 )
        {
            dir = STOP;
            head.x = 0;
        }
        else if ( head.y >= 690 )
        {
            dir = STOP;
            head.y = 690;
        }
        else if ( head.y <= 0 )
        {
            dir = STOP;
            head.y = 0;
        }

        //handles the movement
        switch (dir)
        {
        case DOWN: head.y +=10;   
            break;
        case UP: head.y -= 10;
            break;
        case LEFT: head.x -= 10;
            break;
        case RIGHT: head.x += 10;
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0,255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderFillRect(renderer, &head);

        SDL_RenderPresent(renderer);
        SDL_Delay(40);

    }

    return 0;
}