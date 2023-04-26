#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <vector>
#include <algorithm>
#include <deque>
#include <iostream>
#include <string>
#include <cstring>

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

        void changeSnake(int x = rand()%8)
        {   
            x++;
            switch(x)
            {
                // case 0: snake = "res/snakes/1.jpg"; break;
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

class Title
{
    public:
        const char* title;

        void setTitle(int size1, bool isGameOver, int size2, int cases)
        {
            std::string gameRunningTitle = "Snake Game by John Silva | Player 1 Score " + std::to_string(size1*10) + " | Player 2 Score: " + std::to_string(size2*10); 

            if(isGameOver)
            {
                switch(cases)
                {
                case 1 : title = "Game Over! Player Blue Won!";
                break;
                case 2: title = "Game Over! Player Green Won!";
                break;
                case 3: title = "Game Over! Tie!";
                break;
                }
            }
            else
            {
                title = gameRunningTitle.c_str();
            }
            
        }
};

Color colors; Snakes snakePic; Title gameTitle;

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

    bool gameIsRunning = true;
    bool isGameOver = false;
    int dir1 = 0;
    int dir2 = 0;

    SDL_Rect head {(WIDTH/2),(HEIGHT/2),10,10}; //initializing of the "snake" in the middle of the window
    SDL_Rect head2 {((WIDTH/2)-20),HEIGHT/2,10,10};
    
    std::deque<SDL_Rect> snakeBody;
    std::deque<SDL_Rect> snakeBody2;
    int size1 = 1;
    int size2 = 1;

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

    int whoWon = 0;

    //main game loop
    while(gameIsRunning)
    {
        while(SDL_PollEvent(&e)) //handle input
        {
            if(e.type == SDL_QUIT) 
            { 
                gameIsRunning = false; 
            }

            if( (e.type == SDL_KEYDOWN) && (isGameOver == false) )//handles movement when player hasnt died
            {
                switch(e.key.keysym.sym) //controls 
                {
                    //arrow controls snake 1 controls
                    case SDLK_DOWN: (dir1 != UP)? dir1 = DOWN : null; break;
                    case SDLK_UP: (dir1 != DOWN)? dir1 = UP : null; break;
                    case SDLK_LEFT: (dir1 != RIGHT)? dir1 = LEFT : null; break;
                    case SDLK_RIGHT: (dir1 != LEFT)? dir1= RIGHT : null; break;

                    //wasd controls snake 2 controls
                    case SDLK_s: (dir2 != UP)? dir2 = DOWN : null; break;
                    case SDLK_w: (dir2 != DOWN)? dir2 = UP : null; break;
                    case SDLK_a: (dir2 != RIGHT)? dir2 = LEFT : null; break;
                    case SDLK_d: (dir2 != LEFT)? dir2 = RIGHT : null; break;
                }
            }
            else if( (e.type == SDL_KEYDOWN) && (isGameOver == true) )//when player has died, has choice of playing again or exiting
            {
                switch(e.key.keysym.sym)
                {
                    //non gameplay controls
                    case SDLK_RETURN: 
                    head.x=450; 
                    head.y=350;
                    head2.x=430;
                    head2.y=350; 
                    dir1 = DOWN; 
                    dir2 = DOWN;
                    isGameOver = false;
                    size1 = 0; 
                    size2 = 0;
                    break;
                    
                    case SDLK_ESCAPE: gameIsRunning = false; 
                    break;
                }
            }
        }

    if(isGameOver == false)
    {
        if( numOfApple == 0 ) //checks if there are no apples in the screen and puts one in a random spot
        {
            int x = (rand()%87*10) + 20 ;
            int y = (rand()%67*10) + 20;
            int w = 10;
            int h = 10;
            int x2 = (rand()%87*10) + 20;
            int y2 = (rand()%87*10) + 20;
            apples.emplace_back(SDL_Rect{x,y,w,h});
            apples.emplace_back(SDL_Rect{x2,y2,w,h});

            //for debugging
            // std::cout << "X is: " << x << "\nY is: " << y << "\n\n";
            numOfApple+=2;
        }

        

        //collision detection
        std::for_each( apples.begin(), apples.end(), [&](auto& apple){
            
            if( (head.x == apple.x) && (head.y == apple.y) )
            {
                size1 += 1;              
                apple.x = -10;
                apple.y = -10;
                numOfApple -= 1;  
            }
            else if( (head2.x == apple.x) && (head2.y == apple.y) )
            {
                size2 += 1;
                apple.x = -10;
                apple.y = -10;
                numOfApple -= 1;
            }
        });

        //handles death conditions for snake
        //this handles if snake has collided with its own body
        //snake 1
        std::for_each(snakeBody.begin(), snakeBody.end(), [&](auto& snake_segment){

            //for debugging
            /* std::cout << "Head1 x: " << head.x << "\nHead1 y: " << head.y;
            std::cout << "Head2 x: " << head2.x << "\nHead2.y: " << head2.y;
            std::cout << "\nSegment x: " << snake_segment.x << "\nSegment y: " << snake_segment.y << "\n\n"; */
            
            if( (head.x == snake_segment.x) && (head.y == snake_segment.y) )
            {
                dir1 = STOP;
                size1 = 0;
                whoWon = 2;
                isGameOver = true;
            }
            else if ( (head2.x == snake_segment.x) && (head2.y == snake_segment.y) )
            {
                dir1 = STOP;
                size1 = 0;
                whoWon = 1;
                isGameOver = true;
            }
        });

        //snake 2
         std::for_each(snakeBody2.begin(), snakeBody2.end(), [&](auto& snake_segment){

            //for debugging
            /* std::cout << "Head x: " << head.x << "\nHead y: " << head.y;
            std::cout << "\nSegment x: " << snake_segment.x << "\nSegment y: " << snake_segment.y << "\n\n"; */
            
            if( (head2.x == snake_segment.x) && (head2.y == snake_segment.y) )
            {
                dir2 = STOP;
                size2 = 0;
                whoWon = 1;
                isGameOver = true;
            }
            else if( (head.x == snake_segment.x) && (head.y == snake_segment.y) )
            {
                dir2 = STOP;
                size2 = 0;
                whoWon = 2;
                isGameOver = true;
            }
        });
        // occurs when they have a head on collision
        if( (head.x == head2.x) && (head.y == head2.y) )
        {
            whoWon = 3;
            dir1 = STOP; 
            dir2 = STOP;
            isGameOver = true;
        }

        // this handles if the snake1 has hit the borders
        if( head.x >= 880 )
        {
            dir1 = STOP;
            head.x = 880;
            whoWon = 2;
            isGameOver = true;
        }
        else if( head.x <= 10 )
        {
            dir1 = STOP;
            head.x = 10;
            whoWon = 2;
            isGameOver = true;
        }
        else if ( head.y >= 680 )
        {
            dir1 = STOP;
            head.y = 680;
            whoWon = 2;
            isGameOver = true;
        }
        else if ( head.y <= 10 )
        {
            dir1 = STOP;
            head.y = 10;
            whoWon = 2;
            isGameOver = true;
        }
        
        //snake 2
        if( head2.x >= 880 )
        {
            dir2 = STOP;
            head.x = 880;
            whoWon = 1;
            isGameOver = true;
        }
        else if( head2.x <= 10 )
        {
            dir2 = STOP;
            head.x = 10;
            whoWon = 1;
            isGameOver = true;
        }
        else if ( head2.y >= 680 )
        {
            dir2 = STOP;
            head.y = 680;
            whoWon = 1;
            isGameOver = true;
        }
        else if ( head2.y <= 10 )
        {
            dir2 = STOP;
            head.y = 10;
            whoWon = 1;
            isGameOver = true;
        }

        //make snake go big
        snakeBody.push_front(head);
        snakeBody2.push_front(head2);
        
        //prevents snake from being longer than the size variable
        //snake 1
        while( (snakeBody.size() > size1) ) 
        {
            snakeBody.pop_back();
        }
        //snake 2
        while( (snakeBody2.size() > size2) )
        {
            snakeBody2.pop_back();
        }

        //handles the movement for snake 1
        switch (dir1)
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

        //snake 2
        switch (dir2)
        {
        case DOWN: head2.y += 10;   
            break;
        case UP:  head2.y -= 10;
            break;
        case LEFT:  head2.x -= 10;
            break;
        case RIGHT:  head2.x += 10;
            break;
        }

        //bg color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0,255);
        SDL_RenderClear(renderer);

        //snake 1 color
        colors.changeColor(7);
        SDL_SetRenderDrawColor(renderer, colors.r, colors.g, colors.b, 255);
        std::for_each(snakeBody.begin(), snakeBody.end(), [&](auto& snake_segment)
        {
            SDL_RenderFillRect(renderer,&snake_segment);
        });
        SDL_RenderFillRect(renderer, &head);

        //snake 2 color
        colors.changeColor(5);
        SDL_SetRenderDrawColor(renderer, colors.r, colors.g, colors.b, 255);
        std::for_each(snakeBody2.begin(), snakeBody2.end(), [&](auto& snake_segment)
        {
            SDL_RenderFillRect(renderer,&snake_segment);
        });
        SDL_RenderFillRect(renderer, &head2);

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
        SDL_Delay(100 - (size1>size2? size2: size1)*5);//sets how fast the speed of the game is
    
    }
    else
    {
        snakePic.changeSnake();//changes snake picture for when you die
        imageSurface = IMG_Load(snakePic.snake); // loads snake picture

        //sdl image shenanigans
        SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
        SDL_UpdateWindowSurface(window);
        SDL_FreeSurface(imageSurface);
        SDL_FreeSurface(windowSurface);
        imageSurface = NULL;
        windowSurface = NULL;
    }

        gameTitle.setTitle(size1,isGameOver,size2,whoWon);
        SDL_SetWindowTitle(window, gameTitle.title);
    }
     
    SDL_DestroyWindow(window);

    return 0;
}