#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <time.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGTH 768

#define WORLD_SIZE 70
#define BLOCK_SIZE 10

char world[WORLD_SIZE][WORLD_SIZE];

struct ball
{
   double x;
   double y;
   double size;
   double speed;
   double angle;
};

struct raquette
{
    double angle;
    double distance;
    double size;
    Sint16 x1[4];
    Sint16 y1[4];
    Sint16 x2[4];
    Sint16 y2[4];
};

typedef struct raquette raquette;

raquette raq = { .angle = 0, .distance = 300, .size = 90 };

typedef struct ball ball;

ball world_ball = { .x = 50, .y = 50, .size = 5, .speed = 2.2, .angle = M_PI_4 };

void init_world(void)
{
    memset(world, 0, sizeof(world));
    for(int cmpt = 0; cmpt < WORLD_SIZE; cmpt++)
    {
        world[0][cmpt] = 2;
        world[WORLD_SIZE - 1][cmpt] = 2;
        world[cmpt][0] = 2;
        world[cmpt][WORLD_SIZE - 1] = 2;
    }
    for(int cmpt = 0; cmpt < 50; cmpt++)
    {
        int x = (rand()%30) + 20;
        int y = (rand()%30) + 20;
        world[x][y] = 1;
    }
}

void compute_raquettes_coords(void)
{
    raq.x1[0] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance - 5) * cos(raq.angle) + raq.size / 2 * sin(raq.angle);
    raq.y1[0] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance - 5) * sin(raq.angle) - raq.size / 2 * cos(raq.angle);
    raq.x1[1] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance) * cos(raq.angle) + raq.size / 2 * sin(raq.angle);
    raq.y1[1] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance) * sin(raq.angle) - raq.size / 2 * cos(raq.angle);
    raq.x1[2] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance) * cos(raq.angle) - raq.size / 2 * sin(raq.angle);
    raq.y1[2] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance) * sin(raq.angle) + raq.size / 2 * cos(raq.angle);
    raq.x1[3] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance - 5) * cos(raq.angle) - raq.size / 2 * sin(raq.angle);
    raq.y1[3] = WORLD_SIZE * BLOCK_SIZE / 2 - (raq.distance - 5) * sin(raq.angle) + raq.size / 2 * cos(raq.angle);

//    printf("angle = %f, x1, y1 = %d %d\n", raq.angle, raq.x1[0], raq.y1[0]);

    raq.x2[0] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance + 5) * cos(raq.angle) - raq.size / 2 * sin(raq.angle);
    raq.y2[0] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance + 5) * sin(raq.angle) + raq.size / 2 * cos(raq.angle);
    raq.x2[1] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance) * cos(raq.angle) - raq.size / 2 * sin(raq.angle);
    raq.y2[1] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance) * sin(raq.angle) + raq.size / 2 * cos(raq.angle);
    raq.x2[2] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance) * cos(raq.angle) + raq.size / 2 * sin(raq.angle);
    raq.y2[2] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance) * sin(raq.angle) - raq.size / 2 * cos(raq.angle);
    raq.x2[3] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance + 5) * cos(raq.angle) + raq.size / 2 * sin(raq.angle);
    raq.y2[3] = WORLD_SIZE * BLOCK_SIZE / 2 + (raq.distance + 5) * sin(raq.angle) - raq.size / 2 * cos(raq.angle);
 
//    printf("angle = %f, x1, y1 = %d %d\n", raq.angle, raq.x2[0], raq.y2[0]);
}

void draw_raquette(SDL_Renderer *renderer)
{
    filledPolygonRGBA(renderer, raq.x1, raq.y1, 4, 0, 120, 10, 255);
    filledPolygonRGBA(renderer, raq.x2, raq.y2, 4, 0, 120, 10, 255);
}

void draw_world_case(SDL_Renderer *renderer, int x, int y, int r, int v, int b, int a)
{
    SDL_SetRenderDrawColor(renderer, r, v, b, a);
    SDL_Rect pos = {x * BLOCK_SIZE,  y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_RenderFillRect(renderer, &pos);
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
                case 2 :
                       draw_world_case(renderer, cmpt, cmpt2, 125, 0, 255, 255);
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
//Algo provenant de http://www.faqs.org/faqs/graphics/algorithms-faq/ 2.03
int pnpoly(int npol, Sint16 *xp, Sint16 *yp, Sint16 x, Sint16 y)
{
  int i, j, c = 0;
  for (i = 0, j = npol-1; i < npol; j = i++) {
    if ((((yp[i]<=y) && (y<yp[j])) ||
         ((yp[j]<=y) && (y<yp[i]))) &&
        (x < (xp[j] - xp[i]) * (y - yp[i]) / (yp[j] - yp[i]) + xp[i]))

      c = !c;
  }
  return c;
}

int check_ball_collision_with_raquettes(double x, double y)
{
    int ret;
    ret = pnpoly(4, raq.x1, raq.y1, (Sint16)x, (Sint16)y);
    if(ret)
        printf("collision with raquette 1\n");
    else
    {
        ret = pnpoly(4, raq.x2, raq.y2, (Sint16)x, (Sint16)y);
        if(ret)
            printf("collision with raquette 2\n");
    }
    return ret;
}

void move_ball(void)
{
    int collision_h = 0, collision_v = 0, collision_d = 0;
    int case_x, case_y;
    double x_speed = world_ball.speed * cos(world_ball.angle);
    double y_speed = world_ball.speed * sin(world_ball.angle);
    double new_x = world_ball.x + x_speed, new_y = world_ball.y + y_speed;
    if(x_speed > 0)
        case_x = (new_x + world_ball.size) / BLOCK_SIZE;
    else
        case_x = (new_x - world_ball.size) / BLOCK_SIZE;
    case_y = new_y / BLOCK_SIZE;
    if(world[case_x][case_y] != 0)//Collision horizontale
        collision_h = 1;

    if(y_speed > 0)
        case_y = (new_y + world_ball.size) / BLOCK_SIZE;
    else
        case_y = (new_y - world_ball.size) / BLOCK_SIZE;
    if(x_speed > 0)
        case_x = new_x / BLOCK_SIZE;
    else
        case_x = new_x / BLOCK_SIZE;
    if(world[case_x][case_y] != 0)//Collision verticale
        collision_v = 1;

    if(!collision_h && !collision_v)//ATTENTION, ne peux y avoir a la fois collision horizontale ou verticale, et collision diagonale. Si tel pouvait etre le cas, attention à ne pas faire double rebond au niveau des vitesses.
    {
        if(x_speed > 0)
            case_x = (new_x + world_ball.size) / BLOCK_SIZE;
        else
            case_x = (new_x - world_ball.size) / BLOCK_SIZE;
        if(world[case_x][case_y] != 0)//Collision horizontale + verticale
            collision_d = 1;
    }
    if(collision_h)
    {
        if(x_speed > 0)
            case_x = (new_x + world_ball.size) / BLOCK_SIZE;
        else
            case_x = (new_x - world_ball.size) / BLOCK_SIZE;
        case_y = new_y / BLOCK_SIZE;
        if(world[case_x][case_y] == 1)
            world[case_x][case_y] = 0;
        world_ball.angle = M_PI - world_ball.angle;
        while(world_ball.angle > 2 * M_PI)
           world_ball.angle -= 2 * M_PI;
        while(world_ball.angle < -2 * M_PI)
           world_ball.angle += 2 * M_PI;
        x_speed = world_ball.speed * cos(world_ball.angle);
        y_speed = world_ball.speed * sin(world_ball.angle);
        printf("Horizontal collision new angle %f (%f degres) speed (%f, %f)\n", world_ball.angle, world_ball.angle * 180 / M_PI, x_speed, y_speed);
    }
    if(collision_v)
    {
        if(y_speed > 0)
            case_y = (new_y + world_ball.size) / BLOCK_SIZE;
        else
            case_y = (new_y - world_ball.size) / BLOCK_SIZE;
        case_x = new_x / BLOCK_SIZE;
        if(world[case_x][case_y] == 1)
            world[case_x][case_y] = 0;
        world_ball.angle *= -1;
        while(world_ball.angle > 2 * M_PI)
           world_ball.angle -= 2 * M_PI;
        while(world_ball.angle < -2 * M_PI)
           world_ball.angle += 2 * M_PI;
        x_speed = world_ball.speed * cos(world_ball.angle);
        y_speed = world_ball.speed * sin(world_ball.angle);
        printf("Vertical collision new angle %f (%f degres) speed (%f, %f)\n", world_ball.angle, world_ball.angle * 180 / M_PI, x_speed, y_speed);
    }
    if(collision_d)
    {
        if(x_speed > 0)
            case_x = (new_x + world_ball.size) / BLOCK_SIZE;
        else
            case_x = (new_x - world_ball.size) / BLOCK_SIZE;
        if(y_speed > 0)
            case_y = (new_y + world_ball.size) / BLOCK_SIZE;
        else
            case_y = (new_y - world_ball.size) / BLOCK_SIZE;
        if(world[case_x][case_y] == 1)
            world[case_x][case_y] = 0;
        world_ball.angle = 3 * M_PI_4 - world_ball.angle;
        while(world_ball.angle > 2 * M_PI)
           world_ball.angle -= 2 * M_PI;
        while(world_ball.angle < -2 * M_PI)
           world_ball.angle += 2 * M_PI;
        x_speed = world_ball.speed * cos(world_ball.angle);
        y_speed = world_ball.speed * sin(world_ball.angle);
        printf("Diagonal collision new angle %f (%f degres)\n", world_ball.angle, world_ball.angle * 180 / M_PI);
    }
    if(!collision_h && !collision_v && !collision_d)
    {
        if(check_ball_collision_with_raquettes(world_ball.x + x_speed, world_ball.y + y_speed) == 1)
        {
            double old_angle = world_ball.angle;
            //world_ball.angle = M_PI - world_ball.angle - 2 * raq.angle;
            world_ball.angle = M_PI - world_ball.angle + 2 * raq.angle;
            while(world_ball.angle > 2 * M_PI)
                world_ball.angle -= 2 * M_PI;
            while(world_ball.angle < -2 * M_PI)
                world_ball.angle += 2 * M_PI;
            printf("Raquette (%f %f degres) collision %f (%f degres) => new angle %f (%f degres)\n", raq.angle, raq.angle * 180 / M_PI, old_angle, old_angle * 180 / M_PI, world_ball.angle, world_ball.angle * 180 / M_PI);
            x_speed = world_ball.speed * cos(world_ball.angle);
            y_speed = world_ball.speed * sin(world_ball.angle);
        }
    }
    world_ball.x += x_speed;
    world_ball.y += y_speed;
//    printf("ball move too %d %d, at %f %f speed\n", world_ball.x, world_ball.y, x_speed, y_speed);
}

int main( int argc, char *argv[ ] )
{
    SDL_Window *window;
    if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
        printf( "Can't init SDL:  %s\n", SDL_GetError( ) );
        return EXIT_FAILURE;
    }

    srand(time(0));
    atexit( SDL_Quit );
    SDL_Renderer *renderer;
    window = SDL_CreateWindow("Ma fenêtre de jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGTH, 0 );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    init_world();
    // Main loop
    SDL_Event event;
    while(1)
    {
        Uint8 r = 0,g = 0,b = 0,a = 255;
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderClear(renderer);
        draw_world(renderer);
        compute_raquettes_coords();
        move_ball();
        draw_ball(renderer);
        draw_raquette(renderer);

        //raq.angle = 45 * M_PI / 180;//+= .80 * M_PI / 180.0;
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
                            case SDLK_UP:
                            case SDLK_RIGHT:
                                raq.angle += 2 * M_PI / 180;
                                if(raq.angle > 2 * M_PI)
                                    raq.angle -= 2 * M_PI;
                                break;
                            case SDLK_LEFT:
                            case SDLK_DOWN:
                                raq.angle -= 2 * M_PI / 180;
                                if(raq.angle < -2 * M_PI)
                                    raq.angle += 2 * M_PI;
                                break;
                            case SDLK_ESCAPE:
                                SDL_Quit();
                                return EXIT_SUCCESS;// Quit the program
                                break;
			}
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
