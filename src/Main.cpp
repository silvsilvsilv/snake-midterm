#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <vector>
#include <algorithm>
#include <deque>
#include <iostream>

#define WINDOW_WIDTH 900
#define WINDOW_LENGTH 700
#define null ' '

class Color
{
    public:
        int r,g,b;

        void changeColor()
            {
                int color = rand()%12+1;

                switch( color )
                {
                    case 1: //red
                        r = 255; g = 0; b = 0;
                        break;
                    case 2: //orange
                        r = 255; g = 128; b = 0;
                        break;
                    case 3: //yellow
                        r = 255; g = 255; b = 0;
                        break;
                    case 4: //lime
                        r = 128; g = 255; b = 0;
                        break;
                    case 5: //green
                        r = 0; g = 255; b = 0;
                        break;
                    case 6: //blue-green
                        r = 0; g = 255; b = 128;
                        break;
                    case 7: //cyan
                        r = 0; g = 255; b = 255;
                        break;
                    case 8: //cerulean
                        r = 0; g = 128; b = 255;
                        break;
                    case 9: //blue
                        r = 0; g = 0; b = 255;
                        break;
                    case 10: //purple
                        r = 127; g = 0; b = 255;
                        break;
                    case 11: //magenta
                        r = 255; g = 0; b = 255;
                        break;
                    case 12: //pink
                        r = 255; g = 0; b = 127;
                        break;    
                }
            }
};

Color color;
int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    auto window = SDL_CreateWindow("Snake by John Silva", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_LENGTH, 0);
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
    

    SDL_Rect head {(WINDOW_WIDTH/2),(WINDOW_LENGTH/2),10,10}; //initializing of the "snake" in the middle of the window
    
    std::deque<SDL_Rect> rq;
    int size = 0;

    std::vector<SDL_Rect> apples;
    int numOfApple = 0;

    //main game loop
    while(running)
    {
        while(SDL_PollEvent(&e)) //handle input
        {
            if(e.type == SDL_QUIT) { running = false; }  
            if(e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym) //controls 
                {
                    //arrow controls
                    case SDLK_DOWN: (dir != UP)? dir = DOWN : null; break;
                    case SDLK_UP: (dir != DOWN)? dir = UP : null; break;
                    case SDLK_LEFT: (dir != RIGHT)? dir = LEFT : null; break;
                    case SDLK_RIGHT: (dir != LEFT)? dir = RIGHT : null; break;

                    //wasd controls
                    case SDLK_s: (dir != UP)? dir = DOWN : null; break;
                    case SDLK_w: (dir != DOWN)? dir = UP : null; break;
                    case SDLK_a: (dir != RIGHT)? dir = LEFT : null; break;
                    case SDLK_d: (dir != LEFT)? dir = RIGHT : null; break;

                    case SDLK_RETURN: head.x=450; head.y=350; dir = DOWN; break;
                }
            }
        }

        if( numOfApple == 0 ) //checks if there are no apples in the screen and puts one in a random spot
        {
            int x = (rand()%88*10) + 10 ;
            int y = (rand()%68*10) + 10;
            int w = 10;
            int h = 10;
            apples.emplace_back(SDL_Rect{x, y,w,h});

            // std::cout << "X is: " << x << "\nY is: " << y << "\n\n";
            numOfApple++;
        }
        

        //collision detection
        std::for_each( apples.begin(), apples.end(), [&](auto& apple){
            if( (head.x == apple.x) && (head.y == apple.y) )
            {
                size += 1;              
                apple.x = -10;
                apple.y = -10;
                numOfApple = 0;  
            }
        });

        //handles death conditions for snake
        //this handles if snake has collided with its own body
        std::for_each(rq.begin(), rq.end(), [&](auto& snake_segment){
            if( (head.x == snake_segment.x) && (head.y == snake_segment.y) )
            {
                size = 0;
                dir = STOP;
            }
        });

        // this handles if the snake has hit the borders
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

        //make snake go big
        rq.push_front(head);
        
        //prevents snake from being longer than the size variable
        while(rq.size() > size) 
        {
            rq.pop_back();
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

        //bg color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0,255);
        SDL_RenderClear(renderer);

        //snake color
        color.changeColor();
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        std::for_each(rq.begin(), rq.end(), [&](auto& snake_segment)
        {
            SDL_RenderFillRect(renderer,&snake_segment);
        });
        SDL_RenderFillRect(renderer, &head);

        // apple color
        SDL_SetRenderDrawColor(renderer, 255,0,0,255);
        std::for_each( apples.begin(), apples.end(), [&](auto& apple){
            SDL_RenderFillRect(renderer, &apple);
        });

        SDL_RenderPresent(renderer);//refreshes the window
        SDL_Delay(100 - size*5 );//sets how fast the speed of the game is

    }

    return 0;
}