
#include "Game_Engine.hpp"
//fstream PlayerSave ("Data_Saves/PlayerSave.txt");
int main(int argc, char* args[]) {
    /*SDL_Init(SDL_INIT_EVERYTHING);
    */
    SDL_ShowCursor(SDL_DISABLE);
    GameLoop();
    CloseSDL();
    return 0;
}

