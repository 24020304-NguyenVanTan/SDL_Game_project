
#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <bits/stdc++.h>
using std::cin;
using std::cout;
using std::min;
using std::max;
using std::fstream;
//Useful const
//Screen size
const int Max_screen_W=1366, Max_screen_H=768;
//Render
const SDL_Point SPRITE_RECT_CENT = {30, 30};
const SDL_Point Ske_thrust_rect_cent = {25,10};
//Physics
const float GRAVITY=0.5, TERMINAL_VELOCITY=15, JUMP_VELOCITY=-15, MOVE_SPEED=5, ACCELERATION=2;
//Player
struct player {
    float x, y;
    float vx=0.0f, vy=0.0f;
    bool onPlatform;
    SDL_Rect Rect;
    SDL_RendererFlip facing;
    int frame;
    int hp;
};
//Melee
struct melee_attack{
    float x, y;
    SDL_Rect Rect;
    int frame;
};
//Enemies
struct weak_skeleton {
    bool update;
    float x, y;
    float vx=0.0f, vy=0.0f;
    bool onPlatform;
    SDL_RendererFlip facing;
    int frame;
    int attack_state;
    int hp;
};
struct projectile{
    double x, y;
    double vx, vy;
    double angle;
};
//Camera
struct camera {
    float x, y;
};
struct mouse {
    int x, y;
    bool left, right;
    SDL_Rect Rect;
};
//Terrain
struct block {
    float x, y; // Position (pixels)
    int w, h; // Width and height (pixels)
    int material;
};
// Function prototypes
//Data
void SaveData();
void RetrieveData();
void ResetData();
//Center menus
void CenterMenus();
//Update
void UpdateCamera();
void UpdatePlayer();
//Render
void RenderPlatforms();
//Core function
void GameLoop();
void CloseSDL();

#endif
