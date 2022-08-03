#include <vector>
#include <random>
#include "SDL.h"
#include "SDL_image.h"
#include "Animation.h"
#include "Player.h"
#include "Entity.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#define window_width 1200
#define window_height 800
#define fps 60

#define Max_Rocks 10
// initalize random
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0, 99999999);

SDL_Window* Initialize_Window();

int setIcon(SDL_Window* window);

void Initialize_Animation(Animation& pExplosion, Animation& pExplosion_ship, 
    Animation& pSpaceship, Animation& pBackground, Animation& pRock, 
    Animation& pSmall_Rock, Animation& pBullet, SDL_Renderer* renderer);

int Initialize_Audio(Mix_Chunk* audio[3]);

void Initialize_Entity(std::vector<Entity*>& entities, Animation& pSpaceship, Animation& pRock);

int Menu(TTF_Font* font, SDL_Renderer* renderer, Animation& pBackground, Mix_Chunk* Background_Music);

int game_loop(std::vector<Entity*>& entities, Animation& pBullet, Animation& pExplosion,
    Animation& pExplosion_ship, Animation& pSmall_Rock, Animation& pBackground,
    Animation& pRock, SDL_Renderer* renderer, Mix_Chunk* audio[]);

void cap_framerate(Uint32 starting_tick);

void update(std::vector<Entity*>& entities);

int collision(std::vector<Entity*>& entities, SDL_Renderer* renderer,
    Animation& pExplosion, Animation& pExplosion_ship, Animation& pSmall_Rock, Mix_Chunk* explosion);

void gameover(TTF_Font* font, SDL_Renderer* renderer, Entity* Explosion_Ship, Animation& pBackground);

void Destroy(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, Mix_Chunk* audio[]);


// main program
int main(int argc, char* argv[]) 
{
    // initialize SDL2 and create window
    SDL_Window* window = Initialize_Window();
    // set icon for program
    if (!setIcon(window))
        std::cout << "Load icon fail";

    // initialize font and get font from source
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("font/ChrustyRock-ORLA.ttf", 100);

    // initialize renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // initialize animation
    Animation pExplosion, pExplosion_ship, pSpaceship,
        pBackground, pRock, pSmall_Rock, pBullet;
    Initialize_Animation(pExplosion, pExplosion_ship, pSpaceship, 
        pBackground, pRock, pSmall_Rock, pBullet, renderer);

    // initialize audio
    Mix_Chunk* audio[3];
    if (!Initialize_Audio(audio))
        std::cout << "Load audio fail";
    
    // show menu, navigate program
    while (1)
    {
        if (!Menu(font, renderer, pBackground, audio[0]))
        {
            // Close and destroy 
            Destroy(renderer, window, font, audio);
            return 0;
        }
        else
        {
            // init entity
            std::vector<Entity*> entities;
            Initialize_Entity(entities, pSpaceship, pRock);
            // game loop
            if (game_loop(entities, pBullet, pExplosion, pExplosion_ship,
                pSmall_Rock, pBackground, pRock, renderer, audio))
                continue;

            // explosion_ship
            Entity* Explosion_Ship = new Entity(pExplosion_ship, "Explosion_Ship", entities[0]->Get_x() - 60,
                entities[0]->Get_y() - 60, 0);
            entities.clear();
            //gameover loop
            gameover(font, renderer, Explosion_Ship, pBackground);
        }

    }

    return 0;
}



SDL_Window* Initialize_Window()
{

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS); // Initialize SDL2

    return SDL_CreateWindow("Asteroids",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height, SDL_WINDOW_SHOWN);

    
}


int setIcon(SDL_Window* window)
{
    SDL_Surface* icon = IMG_Load("images/icon.png");
    if (icon == NULL)
        return 0;
    SDL_SetWindowIcon(window, icon);

    SDL_FreeSurface(icon);
    return 1;
}

void Initialize_Animation(Animation& pExplosion, Animation& pExplosion_ship, 
    Animation& pSpaceship, Animation& pBackground, Animation& pRock, 
    Animation& pSmall_Rock, Animation& pBullet, SDL_Renderer* renderer)
{
    pExplosion.setting("images/explosions/type_C.png", 0, 0, 256, 256, renderer, 48, 0.5f);
    pExplosion_ship.setting("images/explosions/type_B.png", 0, 0, 192, 192, renderer, 64, 0.5f);
    pSpaceship.setting("images/spaceship.png", 40, 0, 40, 40, renderer, 1, 0);
    pBackground.setting("images/background.jpg", 0, 0, 1920, 1200, renderer, 1, 0);
    pRock.setting("images/rock.png", 0, 0, 64, 64, renderer, 16, 0.2f);
    pBullet.setting("images/fire_red.png", 0, 0, 32, 64, renderer, 16, 0.8f);
    pSmall_Rock.setting("images/rock_small.png", 0, 0, 64, 64, renderer, 16, 0.2f);
}

int Initialize_Audio(Mix_Chunk* audio[3])
{
    Mix_Init(MIX_INIT_MP3);
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
        return 0;

    audio[0] = Mix_LoadWAV("music/background_music.mp3");
    audio[1] = Mix_LoadWAV("music/player_fire.wav");
    audio[2] = Mix_LoadWAV("music/explosion.wav");
    Mix_Volume(0, 40); //background music channel 0
    Mix_Volume(1, 128); // player shoot channel 1
    Mix_Volume(2, 128); // explosion

    return 1;
}

void Initialize_Entity(std::vector<Entity*>& entities, Animation& pSpaceship, Animation& pRock)
{
    
    Player* player = new Player(pSpaceship, "Player", window_width / 2 - 20, window_height / 2 - 20, 0);
    entities.clear();
    entities.push_back(player);
    for (int i = 0; i < 1; i++)
    {
        Asteroid* asteroid = new Asteroid(pRock, "Rock", distribution(generator) % window_width + window_width,
            distribution(generator) % window_height + window_height, 0);
        entities.push_back(asteroid);
    }
}

int Menu(TTF_Font* font, SDL_Renderer* renderer, Animation& pBackground, Mix_Chunk* Background_Music)
{
    Mix_PlayChannel(0, Background_Music, -1);

    SDL_Color white = { 255,255,255 }; // non selected
    SDL_Color yellow = { 255,255,0 };  // selected

    SDL_Surface* smenu[2];

    SDL_Texture* tmenu[2];

    const char* labels[2] = { "Continue", "Exit" };

    bool selected[2] = { 0 , 0};
    
    smenu[0] = TTF_RenderText_Solid(font, labels[0], white);
    smenu[1] = TTF_RenderText_Solid(font, labels[1], white);

    tmenu[0] = SDL_CreateTextureFromSurface(renderer, smenu[0]);
    tmenu[1] = SDL_CreateTextureFromSurface(renderer, smenu[1]);

    SDL_Rect position[2];

    SDL_QueryTexture(tmenu[0], NULL, NULL, &position[0].w, &position[0].h);
    SDL_QueryTexture(tmenu[1], NULL, NULL, &position[1].w, &position[1].h);

    position[0].x = window_width / 2 - position[0].w / 2;
    position[0].y = window_height / 2 - position[0].h / 2 - position[1].h / 2;
    position[1].x = window_width / 2 - position[1].w / 2;
    position[1].y = window_height / 2 + position[1].h / 2 + position[0].h / 2;

    SDL_Rect position_mouse;

    bool running = true;
    Uint32 starting_tick;
    SDL_Event Event;
    SDL_FlushEvent(SDL_KEYDOWN);
    while (running)
    {
        starting_tick = SDL_GetTicks();

        //draw on screen
        SDL_RenderClear(renderer);
        pBackground.draw(renderer, 0, 0, window_width, window_height);
        SDL_RenderCopy(renderer, tmenu[0], NULL, &position[0]);
        SDL_RenderCopy(renderer, tmenu[1], NULL, &position[1]);
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&Event))
        {
            switch (Event.type)
            {
            case SDL_WINDOWEVENT:
                if (Event.window.event == SDL_WINDOWEVENT_CLOSE)
                    return 0;
                break;
            case SDL_KEYDOWN:
                if (Event.key.keysym.sym == SDLK_ESCAPE)
                    return 0;
                break;

            case SDL_MOUSEMOTION:
                position_mouse = { Event.motion.x, Event.motion.y, 1, 1 };
                for (int i = 0; i < 2; i++)
                {
                    if (SDL_HasIntersection(&position[i], &position_mouse))
                    {
                        if (!selected[i])
                        {
                            selected[i] = 1;
                            SDL_FreeSurface(smenu[i]);
                            SDL_DestroyTexture(tmenu[i]);

                            smenu[i] = TTF_RenderText_Solid(font, labels[i], yellow);
                            tmenu[i] = SDL_CreateTextureFromSurface(renderer, smenu[i]);
                        }
                    }
                    else
                    {
                        if (selected[i])
                        {
                            selected[i] = 0;
                            SDL_FreeSurface(smenu[i]);
                            SDL_DestroyTexture(tmenu[i]);

                            smenu[i] = TTF_RenderText_Solid(font, labels[i], white);
                            tmenu[i] = SDL_CreateTextureFromSurface(renderer, smenu[i]);
                        }
                    }

                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                position_mouse = { Event.motion.x, Event.motion.y, 1, 1 };
                // mouse clicked continue
                if (SDL_HasIntersection(&position[0], &position_mouse))
                {
                    SDL_FreeSurface(smenu[0]);
                    SDL_FreeSurface(smenu[1]);
                    SDL_DestroyTexture(tmenu[0]);
                    SDL_DestroyTexture(tmenu[1]);
                    return 1;
                }
                //mouse clicked exit
                else if (SDL_HasIntersection(&position[1], &position_mouse))
                {
                    SDL_FreeSurface(smenu[0]);
                    SDL_FreeSurface(smenu[1]);
                    SDL_DestroyTexture(tmenu[0]);
                    SDL_DestroyTexture(tmenu[1]);
                    return 0;
                }
                break;
            }

        }

        

        //cap_framerate(starting_tick);
    }

    return 1;
}

void cap_framerate(Uint32 starting_tick)
{
    if ((1000 / fps) > (SDL_GetTicks() - starting_tick))
        SDL_Delay(1000 / fps - (SDL_GetTicks() - starting_tick));
}

void update(std::vector<Entity*>& entities)
{
    int n = entities.size();
    for (int i = 0; i < n; i++)
    {
        entities[i]->update();
        if (!entities[i]->isLife())
        {
            entities.erase(entities.begin() + i);
            i--;
            n--;
        }

    }
}

int game_loop(std::vector<Entity*>& entities, Animation& pBullet, Animation& pExplosion,
    Animation& pExplosion_ship, Animation& pSmall_Rock, Animation& pBackground, 
    Animation& pRock, SDL_Renderer* renderer, Mix_Chunk* audio[])
{
    Uint32 starting_tick; // use to limit fps
    Uint32 timeSpawn = 0; // use to spawn asteroids

    SDL_Event Event; // declare event
    SDL_FlushEvent(SDL_KEYDOWN);
    while (1)
    {
        //set gameover
        // use Tick to limit fps
        starting_tick = SDL_GetTicks();

        // multiple key
        const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);

        //handle event (example: key, mouse, ...)
        while (SDL_PollEvent(&Event))
        {

            if (keyboard_state_array[SDL_SCANCODE_ESCAPE] || Event.window.event == SDL_WINDOWEVENT_CLOSE) // nhan nut esc la thoat.
            {
                return 1;
                break;
            }


            if (Event.type == SDL_KEYUP && Event.key.keysym.sym == SDLK_SPACE) // player fire
            {
                Bullet* bullet = new Bullet(pBullet, "Bullet", entities[0]->Get_x(),
                    entities[0]->Get_y(), entities[0]->Get_angle() + 180);
                entities.push_back(bullet);
                Mix_PlayChannel(1, audio[1], 0);
            }

        }


        // collision
        if (collision(entities, renderer, pExplosion, pExplosion_ship, pSmall_Rock, audio[2]))
            return 0;
        // update
        entities[0]->update(keyboard_state_array); // player move
        update(entities);
        keyboard_state_array = NULL;


        // draw on the screen
        //SDL_RenderClear(renderer);
        pBackground.draw(renderer, 0, 0, window_width, window_height);   // background


        for (int i = 0; i < entities.size(); i++)
            entities[i]->draw(renderer);
        SDL_RenderPresent(renderer);


        // 3 seconds spawn rock
        int count_rock = 0;
        for (auto i : entities)
            if (i->get_name() == "Rock")
                count_rock++;
        if (SDL_GetTicks() - timeSpawn > 3000 && count_rock < Max_Rocks)
        {
            timeSpawn = SDL_GetTicks();
            entities.push_back(new Asteroid(pRock, "Rock", distribution(generator) % window_width + window_width,
                distribution(generator) % window_height + window_height, 0));
        }




        cap_framerate(starting_tick); // limt framerate (about 60)
    }
    return 1;
}

int collision(std::vector<Entity*>& entities, SDL_Renderer* renderer,
    Animation& pExplosion, Animation& pExplosion_ship, Animation& pSmall_Rock, Mix_Chunk* explosion)
{
    int n = entities.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (entities[j]->get_name() == "Bullet" && entities[i]->get_name() == "Rock"
                && *entities[i] == *entities[j])
            {
                Mix_PlayChannel(2, explosion, 0);
                Entity* entity = new Entity(pExplosion, "Explosion", entities[i]->Get_x() - 64, entities[i]->Get_y() - 64, 0);
                entities.push_back(entity);

                entities.push_back(new Asteroid(pSmall_Rock, "Small_Rock", entities[i]->Get_x() - 30,
                    entities[i]->Get_y() - 30, distribution(generator) % 360));
                entities.push_back(new Asteroid(pSmall_Rock, "Small_Rock", entities[i]->Get_x() + 30,
                    entities[i]->Get_y() + 30, distribution(generator) % 360));

                entities.erase(entities.begin() + j);
                entities.erase(entities.begin() + i);
                i--;
                j--;
                n++;
            }
            if (entities[j]->get_name() == "Bullet" && entities[i]->get_name() == "Small_Rock"
                && *entities[i] == *entities[j])
            {
                Mix_PlayChannel(2, explosion, 0);
                Entity* entity = new Entity(pExplosion, "Explosion", entities[i]->Get_x() - 64, entities[i]->Get_y() - 64, 0);
                entities.push_back(entity);
                entities.erase(entities.begin() + j);
                entities.erase(entities.begin() + i);
                i--;
                j--;
                n--;
            }
            if ((entities[j]->get_name() == "Rock" || entities[j]->get_name() == "Small_Rock")
                && *entities[0] == *entities[j])
            {
                Mix_PlayChannel(2, explosion, 0);
                return 1;
                break;
            }
        }
    }
    return 0;
}

void gameover(TTF_Font* font, SDL_Renderer* renderer, Entity* Explosion_Ship, Animation& pBackground)
{
    SDL_Color white = { 255, 255, 255 };

    SDL_Surface* sMessage = TTF_RenderText_Solid(font, "GAME OVER", white);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, sMessage);

    int textW = 0;
    int textH = 0;
    SDL_QueryTexture(message, NULL, NULL, &textW, &textH);

    SDL_Rect Message_rect{ window_width / 2 - textW / 2, window_height / 2 - textH, textW, textH };

    bool running = true;
    Uint32 starting_tick; // use to limit fps
    SDL_Event Event; // declare event
    SDL_FlushEvent(SDL_KEYDOWN);
    while (running)
    {
        starting_tick = SDL_GetTicks();
        while (SDL_PollEvent(&Event))
        {
            if (Event.window.event == SDL_WINDOWEVENT_CLOSE || Event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
                break;
            }

        }
        //SDL_RenderClear(renderer);
        Explosion_Ship->update();
        pBackground.draw(renderer, 0, 0, window_width, window_height);   // background


        Explosion_Ship->draw(renderer);
        SDL_RenderCopy(renderer, message, NULL, &Message_rect);
        SDL_RenderPresent(renderer);
        if (Explosion_Ship->isLife() == false)
            break;
        cap_framerate(starting_tick); // limt framerate (about 60)
    }
    SDL_FreeSurface(sMessage);
    SDL_DestroyTexture(message);
}

void Destroy(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, Mix_Chunk* audio[])
{
    for(int i = 0; i < 2; i++)
        Mix_FreeChunk(audio[i]);
    Mix_Quit();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}