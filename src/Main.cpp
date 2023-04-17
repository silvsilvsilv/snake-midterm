#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <vector>
#include <algorithm>
#include <deque>
#include <iostream>
#include <sstream>

#define WIDTH 900
#define HEIGHT 700
#define null ' '

class Color
{
    public:
        //values for r, g and b
        int r,g,b;
        
        /* function to changecolor, by default gives a random value
        [1] red, 
        [2] orange,
        [3] yellow, 
        [4] lime, 
        [5] green, 
        [6] blue-green, 
        [7] cyan, 
        [8] cerulean, 
        [9] blue, 
        [10] purple,
        [11] magenta,
        [12] pink */
        void changeColor(int color = rand()%12+1)
            {
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

class Snakes
{
    public:
        const char* snake;

        void changeSnake(int x = rand()%9)
        {
            switch(x)
            {
                case 0: snake = "res/snakes/1.jpg"; break;
                case 1: snake = "res/snakes/2.jpg"; break;
                case 2: snake = "res/snakes/3.jpg"; break;
                case 3: snake = "res/snakes/4.jpg"; break;
                case 4: snake = "res/snakes/5.jpg"; break;
                case 5: snake = "res/snakes/6.jpg"; break;
                case 6: snake = "res/snakes/7.jpg"; break;
                case 7: snake = "res/snakes/8.jpg"; break;
                case 8: snake = "res/snakes/9.jpg"; break;
                default: break;
            }
        }
};

Color colors; Snakes snakePic;
int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    auto window = SDL_CreateWindow("Snake by John Silva", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    auto renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface* icon = IMG_Load("res/anaconda.png");
    SDL_SetWindowIcon(window, icon);

    SDL_Surface* imageSurface = NULL;
    SDL_Surface* windowSurface = NULL;
    windowSurface = SDL_GetWindowSurface(window);

    

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
    bool isGameOver = false;
    int dir = 0;

    SDL_Rect head {(WIDTH/2),(HEIGHT/2),10,10}; //initializing of the "snake" in the middle of the window
    
    std::deque<SDL_Rect> snakeBody;
    int size = 0;

    std::vector<SDL_Rect> apples;
    int numOfApple = 0;

    std::vector<SDL_Rect> borderBlock;//adds the border
    for(int i = 0 ;i <= WIDTH-10; i++)//for x
    {
        borderBlock.emplace_back(SDL_Rect{i,0,10,10}); //upper part
        borderBlock.emplace_back(SDL_Rect{i,690,10,10}); //bottom part
    }
    for(int i = 0; i <= HEIGHT - 10; i++)// for y
    {
        borderBlock.emplace_back(SDL_Rect{0,i,10,10}); //left part
        borderBlock.emplace_back(SDL_Rect{890,i,10,10}); //right part
    }

    //main game loop
    while(running)
    {
        while(SDL_PollEvent(&e)) //handle input
        {
            if(e.type == SDL_QUIT) 
            { 
                running = false; 
            }

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

                    case SDLK_RETURN: 
                        head.x=450; 
                        head.y=350; 
                        dir = DOWN; 
                        isGameOver = false;
                        size = 0; 
                    break;
                    case SDLK_ESCAPE: running = false; break;
                }
            }
        }

        snakePic.changeSnake();//changes snake picture for when you die
        imageSurface = IMG_Load(snakePic.snake); // loads snake picture

    if(isGameOver == false)
    {
        if( numOfApple == 0 ) //checks if there are no apples in the screen and puts one in a random spot
        {
            int x = (rand()%88*10) + 10 ;
            int y = (rand()%68*10) + 10;
            int w = 10;
            int h = 10;
            apples.emplace_back(SDL_Rect{x, y,w,h});

            //for debugging
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
        std::for_each(snakeBody.begin(), snakeBody.end(), [&](auto& snake_segment){

            //for debugging
            /* std::cout << "Head x: " << head.x << "\nHead y: " << head.y;
            std::cout << "\nSegment x: " << snake_segment.x << "\nSegment y: " << snake_segment.y << "\n\n"; */
            
            if( (head.x == snake_segment.x) && (head.y == snake_segment.y) )
            {
                dir = STOP;
                size = 0;
            }
        });

        // this handles if the snake has hit the borders
        if( head.x >= 880 )
        {
            dir = STOP;
            isGameOver = true;
            head.x = 880;
        }
        else if( head.x <= 10 )
        {
            dir = STOP;
            isGameOver = true;
            head.x = 10;
        }
        else if ( head.y >= 680 )
        {
            dir = STOP;
            isGameOver = true;
            head.y = 680;
        }
        else if ( head.y <= 10 )
        {
            dir = STOP;
            isGameOver = true;
            head.y = 10;
        }

        //make snake go big
        snakeBody.push_front(head);
        
        //prevents snake from being longer than the size variable
        while(snakeBody.size() > size) 
        {
            snakeBody.pop_back();
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
        colors.changeColor(7);
        SDL_SetRenderDrawColor(renderer, colors.r, colors.g, colors.b, 255);
        std::for_each(snakeBody.begin(), snakeBody.end(), [&](auto& snake_segment)
        {
            SDL_RenderFillRect(renderer,&snake_segment);
        });
        SDL_RenderFillRect(renderer, &head);

        // apple color
        SDL_SetRenderDrawColor(renderer, 255,0,0,255);
        std::for_each( apples.begin(), apples.end(), [&](auto& apple){
            SDL_RenderFillRect(renderer, &apple);
        });

        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        std::for_each(borderBlock.begin(), borderBlock.end(), [&](auto& borderBlock){
            SDL_RenderFillRect(renderer, &borderBlock);
        });
        
        SDL_RenderPresent(renderer);//refreshes the window
        SDL_Delay(100 - size*5 );//sets how fast the speed of the game is
    
    }
    else
    {
        SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
        SDL_UpdateWindowSurface(window);
        SDL_FreeSurface(imageSurface);
        SDL_FreeSurface(windowSurface);
        imageSurface = NULL;
        windowSurface = NULL;
    }
    }
    // SDL_Rect dstrect {WIDTH/2,HEIGHT/2,0,0};

    
       
    SDL_DestroyWindow(window);

    return 0;
}