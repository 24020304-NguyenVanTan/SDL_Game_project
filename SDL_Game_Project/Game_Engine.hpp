
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
using std::string;
using std::fstream;
//Useful const
//Render
const SDL_Point SPRITE_RECT_CENT = {30, 30};
//Physics
const double MOVE_SPEED=5.0;
const double pi=3.14159265359;
//Player
struct player {
    double x, y;
    double vx, vy;
    SDL_Rect Rect;
    SDL_RendererFlip facing;
    int frame;
    int hp;
};
//Melee
struct melee_weapon{
    string name;
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Point pivot;
};
struct crossbow{
    string name;
    SDL_Texture* texture[3];
    SDL_Rect rect;
    SDL_Point pivot;
    int max_charge;
};
//Enemies
struct enemy {
    int type;//0 is melee, 1 is ranged,
    double x, y;
    double vx, vy;
    SDL_RendererFlip facing;
    int frame;
    int attack_state;
    int hp;
};
struct projectile{
    double x, y;
    double vx, vy;
    double angle;
    double distance;
};
const double ARROW_MAX_RANGE=700.0;
const double ARROW_VELOCITY=39.9;
//Camera
struct camera {
    double x, y;
};
struct mouse {
    int x, y;
    bool left, right;
    SDL_Rect Rect;
};
//Terrain
struct tile {
    int x, y; // Position (pixels)
};
// Function prototypes
//Data
void SaveData();
void RetrieveData();
void ResetData();
//Update
void UpdateCamera();
void UpdatePlayer();
//Render
void RenderPlatforms();
//Core function
void CenterMenus();
void GameLoop();
void CloseGame();

#endif

