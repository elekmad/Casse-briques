#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGTH 600

#define WORLD_SIZE 60
#define BLOCK_SIZE 5

char world[WORLD_SIZE][WORLD_SIZE];

struct ball
{
   int x;
   int y;
   int size;
   int x_speed;
   int y_speed;
};

struct raquette
{
    double angle;
    int distance;
    int size;
};

typedef struct raquette raquette;

raquette raq = { .angle = 0, .distance = 40, .size = 30 };

typedef struct ball ball;

ball world_ball = { .x = 50, .y = 50, .size = 5, .x_speed = 2, .y_speed = 1 };

void init_world(void)
{
    memset(world, 0, sizeof(world));
    for(int cmpt = 0; cmpt < WORLD_SIZE; cmpt++)
    {
        world[0][cmpt] = 1;
        world[WORLD_SIZE - 1][cmpt] = 1;
        world[cmpt][0] = 1;
        world[cmpt][WORLD_SIZE - 1] = 1;
    }
}

void draw_world_case(SDL_Renderer *renderer, int x, int y, int r, int v, int b, int a)
{
    SDL_SetRenderDrawColor(renderer, r, v, b, a);
    SDL_Rect pos = {x * BLOCK_SIZE,  y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_RenderFillRect(renderer, &pos);
}

void draw_raquette(SDL_Renderer *renderer)
{
    Sint16 x[4], y[4];
    x[0] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance - 5) * cos(raq.angle) + raq.size / 2 * sin(raq.angle);
    y[0] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance - 5) * sin(raq.angle) - raq.size / 2 * cos(raq.angle);
    x[1] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance) * cos(raq.angle) + raq.size / 2 * sin(raq.angle);
    y[1] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance) * sin(raq.angle) - raq.size / 2 * cos(raq.angle);
    x[2] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance) * cos(raq.angle) - raq.size / 2 * sin(raq.angle);
    y[2] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance) * sin(raq.angle) + raq.size / 2 * cos(raq.angle);
    x[3] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance - 5) * cos(raq.angle) - raq.size / 2 * sin(raq.angle);
    y[3] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance - 5) * sin(raq.angle) + raq.size / 2 * cos(raq.angle);
    printf("angle = %f, x1, y1 = %d %d\n", raq.angle, x[0], y[0]);
    filledPolygonRGBA(renderer, x, y, 4, 0, 120, 10, 255);

    x[0] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance + 5) * cos(raq.angle) - raq.size / 2 * sin(raq.angle);
    y[0] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance + 5) * sin(raq.angle) + raq.size / 2 * cos(raq.angle);
    x[1] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance) * cos(raq.angle) - raq.size / 2 * sin(raq.angle);
    y[1] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance) * sin(raq.angle) + raq.size / 2 * cos(raq.angle);
    x[2] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance) * cos(raq.angle) + raq.size / 2 * sin(raq.angle);
    y[2] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance) * sin(raq.angle) - raq.size / 2 * cos(raq.angle);
    x[3] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance + 5) * cos(raq.angle) + raq.size / 2 * sin(raq.angle);
    y[3] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance + 5) * sin(raq.angle) - raq.size / 2 * cos(raq.angle);
    printf("angle = %f, x1, y1 = %d %d\n", raq.angle, x[0], y[0]);
    filledPolygonRGBA(renderer, x, y, 4, 0, 120, 10, 255);
}

void draw_world(SDL_Renderer *renderer)
{
    for(int cmpt = 0; cmpt < WORLD_SIZE; cmpt++)
    {
        for(int cmpt2 = 0; cmpt2 < WORLD_SIZE; cmpt2++)
        {
            switch(world[cmpt][cmpt2])
            {
                case 0 :
                       break;
                case 1 :
                       draw_world_case(renderer, cmpt, cmpt2, 0, 0, 255, 255);
                       break;
            }
        }
    }
}

void draw_ball(SDL_Renderer *renderer)
{
    filledCircleRGBA(renderer, world_ball.x, world_ball.y, world_ball.size, 200, 0, 50, 255);
}

void undraw_ball(SDL_Renderer *renderer)
{
    filledCircleRGBA(renderer, world_ball.x, world_ball.y, world_ball.size, 0, 0, 0, 255);
}

void move_ball(void)
{
    int collision_h = 0, collision_v = 0, collision_d = 0;
    int case_x, case_y;
    int new_x = world_ball.x + world_ball.x_speed, new_y = world_ball.y + world_ball.y_speed;
    if(world_ball.x_speed > 0)
        case_x = (new_x + world_ball.size) / BLOCK_SIZE;
    else
        case_x = (new_x - world_ball.size) / BLOCK_SIZE;
    case_y = new_y / BLOCK_SIZE;
    if(world[case_x][case_y] != 0)//Collision horizontale
        collision_h = 1;

    if(world_ball.y_speed > 0)
        case_y = (new_y + world_ball.size) / BLOCK_SIZE;
    else
        case_y = (new_y - world_ball.size) / BLOCK_SIZE;
    if(world_ball.x_speed > 0)
        case_x = new_x / BLOCK_SIZE;
    else
        case_x = new_x / BLOCK_SIZE;
    if(world[case_x][case_y] != 0)//Collision verticale
        collision_v = 1;

    if(!collision_h && !collision_v)//ATTENTION, ne peux y avoir a la fois collision horizontale ou verticale, et collision diagonale. Si tel pouvait etre le cas, attention à ne pas faire double rebond au niveau des vitesses.
    {
        if(world_ball.x_speed > 0)
            case_x = (new_x + world_ball.size) / BLOCK_SIZE;
        else
            case_x = (new_x - world_ball.size) / BLOCK_SIZE;
        if(world[case_x][case_y] != 0)//Collision horizontale + verticale
            collision_d = 1;
    }
    if(collision_h)
    {
        if(world_ball.x_speed > 0)
            case_x = (new_x + world_ball.size) / BLOCK_SIZE;
        else
            case_x = (new_x - world_ball.size) / BLOCK_SIZE;
        case_y = new_y / BLOCK_SIZE;
        world[case_x][case_y] = 0;
        world_ball.x_speed *= -1;
    }
    if(collision_v)
    {
        if(world_ball.y_speed > 0)
            case_y = (new_y + world_ball.size) / BLOCK_SIZE;
        else
            case_y = (new_y - world_ball.size) / BLOCK_SIZE;
        case_x = new_x / BLOCK_SIZE;
        world[case_x][case_y] = 0;
        world_ball.y_speed *= -1;
    }
    if(collision_d)
    {
        if(world_ball.x_speed > 0)
            case_x = (new_x + world_ball.size) / BLOCK_SIZE;
        else
            case_x = (new_x - world_ball.size) / BLOCK_SIZE;
        if(world_ball.y_speed > 0)
            case_y = (new_y + world_ball.size) / BLOCK_SIZE;
        else
            case_y = (new_y - world_ball.size) / BLOCK_SIZE;
        world[case_x][case_y] = 0;
        world_ball.y_speed *= -1;
        world_ball.x_speed *= -1;
    }
    world_ball.x += world_ball.x_speed;
    world_ball.y += world_ball.y_speed;
}

int main( int argc, char *argv[ ] )
{
    SDL_Window *window;
    if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
        printf( "Can't init SDL:  %s\n", SDL_GetError( ) );
        return EXIT_FAILURE;
    }

    atexit( SDL_Quit );
    SDL_Renderer *renderer;
    window = SDL_CreateWindow("Ma fenêtre de jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0 );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int x=0, xspeed=0, y=0, yspeed=0;
    init_world();
    // Main loop
    SDL_Event event;
    while(1)
    {
        Uint8 r = 0,g = 0,b = 0,a = 255;
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderClear(renderer);
        draw_world(renderer);
        move_ball();
        draw_ball(renderer);
        draw_raquette(renderer);
	if(x + xspeed < 640 && x + xspeed >= 0)
	    x += xspeed;
	if(y + yspeed < 480 && y + yspeed >= 0)
	    y += yspeed;
        raq.angle += 1.0 * M_PI / 180.0;
        //if(raq.angle > M_PI_2)
        //    raq.angle = 0;

       // Check for messages
        if (SDL_PollEvent(&event))
        {
            // Check for the quit message
            switch (event.type)
            {
                case SDL_QUIT:
                    SDL_Quit();
		    return EXIT_SUCCESS;// Quit the program
                    break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			printf("keydown %d\n", event.key.keysym.sym);
			switch(event.key.keysym.sym)
			{
                            //case SDLK_DOWN:
                            //    yspeed = (event.type == SDL_KEYDOWN ? SPEED_RATE : 0);
                            //    break;
                            //case SDLK_UP:
                            //    yspeed = (event.type == SDL_KEYDOWN ? -SPEED_RATE : 0);
                            //    break;
                            //case SDLK_RIGHT:
                            //    xspeed = (event.type == SDL_KEYDOWN ? SPEED_RATE : 0);
                            //    break;
                            //case SDLK_LEFT:
                            //    xspeed = (event.type == SDL_KEYDOWN ? -SPEED_RATE : 0);
                            //    break;
                            case SDLK_ESCAPE:
                                SDL_Quit();
                                return EXIT_SUCCESS;// Quit the program
                                break;
			}
		        printf("xspeed %d yspeed %d \n", xspeed, yspeed);
		        break;
            }
        }
       
        //Update the display
        SDL_RenderPresent(renderer);	
        
    }

    // Tell the SDL to clean up and shut down
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
