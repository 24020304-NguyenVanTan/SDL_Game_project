
#include "Game_Engine.hpp"
fstream PlayerSave("Data_Saves/PlayerSave.txt");
fstream WeakSkeSave("Data_Saves/WeakSkeSave.txt");
//Window and renderer
SDL_Window* window=SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Max_screen_W, Max_screen_H, SDL_WINDOW_SHOWN);
SDL_Renderer* renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//Control
bool IsPaused = false;
bool running = true, InMenu=true;
const Uint8* KeyCheck = SDL_GetKeyboardState(nullptr);
SDL_Event e;
//Texture
//Menu
SDL_Texture* MainMenuText=IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Menus/Main_Menu.png");
SDL_Rect MainMenuRect={Max_screen_W/2-265, Max_screen_H/2-335, 530, 670};

SDL_Texture* NewGameButtonText[]={
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Menus/NewGame_button.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Menus/NewGame_button_1.png")
};
SDL_Rect NewGameButtonRect={MainMenuRect.x+65, 400, 400, 140};

SDL_Texture* ExitButtonText[]={
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Menus/Exit_button.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Menus/Exit_button_1.png")
};
SDL_Rect ExitButtonRect={MainMenuRect.x+65, 550, 400, 140};

//Pause Menu
SDL_Texture* PauseMenuText = IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Menus/Pause_Menu.png");
SDL_Rect PauseMenuRect={183, 169, 1000, 430};
SDL_Texture* ContunueButtonText[]={
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Menus/Continue_button.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Menus/Continue_button_1.png")
};
SDL_Rect ContinueButtonRect={263, 409, 400, 140};
SDL_Texture* MenuButtonText[]={
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Menus/Menu_button.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Menus/Menu_button_1.png")
};
SDL_Rect MenuButtonRect={703, 409, 400, 140};

//GUI
SDL_Texture* HPbarFrame=IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Menus/HP_bar.png");
SDL_Texture* CrosshairText=IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Attacks/Crosshair.png");
SDL_Texture* PointerText=IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Menus/Mouse.png");
//Platform
SDL_Texture* Block_Texture[]={
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Blocks/Stone.png"),//0
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Blocks/Stone Bricks.png"),//1
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Blocks/Mossy Stone Bricks.png"),//2
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Blocks/Cracked Stone Bricks.png"),//3
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Blocks/Moss Block.png"),//4
};
//Player
SDL_Texture* Player_Texture[]={
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Character/Character_L_0.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Character/Character_L_1.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Character/Character_L_2.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Character/Character_L_3.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Character/Character_L_4.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Character/Character_L_5.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Character/Character_L_6.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Character/Character_L_7.png")
};
//Melee attack
SDL_Texture* Slash_Attack_Texture[]={
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Attacks/Slash/Slash_0.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Attacks/Slash/Slash_1.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Attacks/Slash/Slash_2.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Attacks/Slash/Slash_3.png"),
};
SDL_Point SlashRectCent={50, 50};
//Weak skeleton thrust
SDL_Texture* Ske_thrust=IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Attacks/Ske_thrust.png");
SDL_Rect Ske_thrust_rect={0, 0, 50, 20};
//Enemies
//Weak skeleton
SDL_Texture* Weak_Skeleton_Texture[3]={
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Weak skeleton/Weak_Ske_0.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Weak skeleton/Weak_Ske_1.png"),
    IMG_LoadTexture(renderer, "D:/SDL_Game_Project/Assets/Sprites/Weak skeleton/Dead_Weak_Ske.png"),
};
//Defining
melee_attack Slash={0, 0, {0, 0, 100, 100}, 0};
//HP bar;
SDL_Rect HPbarFrameRect = {10, 10, 210, 50};
SDL_Rect HPbarRect = {15, 15, 0, 20};
//Crosshair & mouse
SDL_Rect CrosshairRect={0, 0, 30, 30};
//Camera
camera Camera={0, 0};
//Mouse
mouse Mouse={0, 0, false, false, {0, 0, 30, 30}};
//Platform
std::vector<block> Platforms = {
    {-40, 0, 40, 600, 1},//Map left bound
    {0, 600, 1000, 600, 1},//First platform
    {1000, 640, 40, 600, 1},
    {1040, 680, 40, 600, 1},
    {1080, 720, 40, 600, 1},
    {1120, 760, 40, 600, 1},
};
float platTop, platBottom, platLeft, platRight, overlapTop, overlapBottom, overlapRight, overlapLeft, minOverlap;
SDL_Rect platRect;
//Player
player Player = {680, 560, 0.0f, 0.0f, true, {0, 0, 60, 60}, SDL_FLIP_NONE, 0, 100};
//Enemies
std::vector<weak_skeleton> WeakSkeletons={
    {true, 40, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 0, 0, 30},
    {true, 80, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 1, 0, 30},
    {true, 120, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 0, 0, 30},
    {true, 160, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 1, 0, 30},
    {true, 200, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 0, 0, 30},
    {true, 240, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 1, 0, 30},
    {true, 280, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 0, 0, 30},
    {true, 320, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 1, 0, 30},
    {true, 360, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 0, 0, 30},
    {true, 400, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 1, 0, 30},
    {true, 440, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 0, 0, 30},
    {true, 480, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 1, 0, 30},
    {true, 520, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 0, 0, 30},
    {true, 560, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 1, 0, 30},
    {true, 600, 560, 0.0f, 0.0f, true, SDL_FLIP_NONE, 0, 0, 30},
};
SDL_Rect WeakSkeletonRect={0, 0, 70, 70};
//Functions
void SaveData(){
    //Player
    PlayerSave<<Player.x<<" "<<Player.y+20<<" "<<Player.hp;

}
void RetrieveData(){
    //Player
    PlayerSave>>Player.x>>Player.y>>Player.hp;
}
//Menu
//Main menu
void MainMenu(){
    SDL_RenderCopy(renderer, MainMenuText, nullptr, &MainMenuRect);
    //New game button
    if(Mouse.x>NewGameButtonRect.x && Mouse.x<NewGameButtonRect.x+400 && Mouse.y>NewGameButtonRect.y && Mouse.y<NewGameButtonRect.y+140){
        SDL_RenderCopy(renderer, NewGameButtonText[1], nullptr, &NewGameButtonRect);
        if(Mouse.left){
            InMenu=false;
        }
    }
    else SDL_RenderCopy(renderer, NewGameButtonText[0], nullptr, &NewGameButtonRect);
    //Exit button
    if(Mouse.x>ExitButtonRect.x && Mouse.x<ExitButtonRect.x+400 && Mouse.y>ExitButtonRect.y && Mouse.y<ExitButtonRect.y+140){
        SDL_RenderCopy(renderer, ExitButtonText[1], nullptr, &ExitButtonRect);
        if(Mouse.left){
            running = false;
        }
    }
    else SDL_RenderCopy(renderer, ExitButtonText[0], nullptr, &ExitButtonRect);
}
//Pause menu
void PauseMenu(){
    SDL_RenderCopy(renderer, PauseMenuText, nullptr, &PauseMenuRect);
    //Continue button
    if(Mouse.x>233 && Mouse.x<633 && Mouse.y>409 && Mouse.y<549){
        SDL_RenderCopy(renderer, ContunueButtonText[1], nullptr, &ContinueButtonRect);
        if(Mouse.left){
            IsPaused=false;
        }
    }
    else SDL_RenderCopy(renderer, ContunueButtonText[0], nullptr, &ContinueButtonRect);
    //Menu button
    if(Mouse.x>703 && Mouse.x<1103 && Mouse.y>409 && Mouse.y<549){
        SDL_RenderCopy(renderer, MenuButtonText[1], nullptr, &MenuButtonRect);
        if(Mouse.left){
            IsPaused=false;
            InMenu=true;
        }
    }
    else SDL_RenderCopy(renderer, MenuButtonText[0], nullptr, &MenuButtonRect);
}
//Update
//Camera
void UpdateCamera(){
    Camera.x = max(0.0f ,Player.x - (Max_screen_W / 2) + 20);
    Camera.y = Player.y - (Max_screen_H / 2) + 20;
}
//Enemies
//Weak skeletons
void UpdateWeakSkeletons(){
    for(int i = 0; i < WeakSkeletons.size(); i++){
        if(WeakSkeletons[i].update){
            //Gravity
            WeakSkeletons[i].vy=min(WeakSkeletons[i].vy + GRAVITY, TERMINAL_VELOCITY);
            //Update pos
            WeakSkeletons[i].x+=WeakSkeletons[i].vx;
            WeakSkeletons[i].y+=WeakSkeletons[i].vy;

            WeakSkeletons[i].onPlatform = false;
            for (int j = 0; j < Platforms.size(); j++) {
                platTop = Platforms[j].y;
                platBottom = Platforms[j].y + Platforms[j].h;
                platLeft = Platforms[j].x;
                platRight = Platforms[j].x + Platforms[j].w;

                // Check for overlap (AABB collision)
                if (WeakSkeletons[i].x + 40 > platLeft && WeakSkeletons[i].x < platRight && WeakSkeletons[i].y + 40 > platTop && WeakSkeletons[i].y < platBottom) {
                    // Calculate overlap amounts
                    overlapTop = (WeakSkeletons[i].y + 40) - platTop;    // How much player overlaps top
                    overlapBottom = platBottom - WeakSkeletons[i].y;     // How much player overlaps bottom
                    overlapLeft = (WeakSkeletons[i].x + 40) - platLeft;  // How much player overlaps left
                    overlapRight = platRight - WeakSkeletons[i].x;       // How much player overlaps right

                    minOverlap = min({overlapTop, overlapBottom, overlapLeft, overlapRight});

                    // Handle collision based on minimum overlap and velocity
                    if (minOverlap == overlapTop) { // Falling onto block (top collision)
                        WeakSkeletons[i].y = platTop - 40; // Position player on top
                        WeakSkeletons[i].vy = 0.0f;       // Stop falling
                        WeakSkeletons[i].onPlatform = true;
                    }
                    else if (minOverlap == overlapBottom) { // Jumping up through bottom (allow jump-through)
                        WeakSkeletons[i].y = platBottom; // Push player below block
                        WeakSkeletons[i].vy = 0.0f;
                    }
                    else if (minOverlap == overlapLeft) { // Left side collision
                        WeakSkeletons[i].x = platLeft - 40; // Push player left of block
                        WeakSkeletons[i].vx*=-0.5;        // Stop horizontal velocity
                    }
                    else if (minOverlap == overlapRight) { // Right side collision
                        WeakSkeletons[i].x = platRight; // Push player right of block
                        WeakSkeletons[i].vx*=-0.5;    // Stop horizontal velocity
                    }
                }
            }
        }
        if(WeakSkeletons[i].hp<=0 && WeakSkeletons[i].onPlatform){
            WeakSkeletons[i].vx=0.0f;
            WeakSkeletons[i].update=false;
        }
        if(WeakSkeletons[i].hp>0){
            if(sqrt(pow((WeakSkeletons[i].x-Player.x),2)+pow((WeakSkeletons[i].y-Player.y), 2))<600){ //Near player
                // Horizontal movement (A/D) only on Platforms/ground
                if (WeakSkeletons[i].onPlatform) {
                    if (WeakSkeletons[i].x>Player.x){
                        WeakSkeletons[i].vx = -MOVE_SPEED*0.5;
                        WeakSkeletons[i].facing=SDL_FLIP_NONE;
                    }
                    else if (WeakSkeletons[i].x<Player.x){
                        WeakSkeletons[i].vx = MOVE_SPEED*0.5;
                        WeakSkeletons[i].facing=SDL_FLIP_HORIZONTAL;
                    }
                    else WeakSkeletons[i].vx = 0.0f;
                }
            }
        }
        //Handle attacking
        if(WeakSkeletons[i].onPlatform && abs(Player.x-WeakSkeletons[i].x)<30 && Player.y==WeakSkeletons[i].y && WeakSkeletons[i].attack_state==0 && WeakSkeletons[i].hp>0) WeakSkeletons[i].attack_state++;
        if(!WeakSkeletons[i].onPlatform) WeakSkeletons[i].attack_state=0;
        if(WeakSkeletons[i].attack_state){
            WeakSkeletons[i].vx=0.0f;
            if(WeakSkeletons[i].attack_state==25){
                if(abs(Player.x-WeakSkeletons[i].x)<50 && Player.y==WeakSkeletons[i].y){
                    Player.vy-=3;
                    Player.vx-=5-int(WeakSkeletons[i].facing)*10;
                    Player.hp=max(Player.hp-10,0);
                }
                WeakSkeletons[i].update=true;
                WeakSkeletons[i].attack_state=-1;
            }
            WeakSkeletons[i].attack_state++;
        }
    }
}
//Player
void UpdatePlayer() {
    // Apply gravity
    Player.vy = min(Player.vy + GRAVITY, TERMINAL_VELOCITY);
    // Update position (before collision to detect overlaps)
    Player.y += Player.vy;
    Player.x += Player.vx;

    Player.onPlatform = false;
    for (int i = 0; i < Platforms.size(); i++) {
        platTop = Platforms[i].y;
        platBottom = Platforms[i].y + Platforms[i].h;
        platLeft = Platforms[i].x;
        platRight = Platforms[i].x + Platforms[i].w;

        // Check for overlap (AABB collision)
        if (Player.x + 40 > platLeft && Player.x < platRight && Player.y + 40 > platTop && Player.y < platBottom) {

            // Calculate overlap amounts
            overlapTop = (Player.y + 40) - platTop;    // How much player overlaps top
            overlapBottom = platBottom - Player.y;     // How much player overlaps bottom
            overlapLeft = (Player.x + 40) - platLeft;  // How much player overlaps left
            overlapRight = platRight - Player.x;       // How much player overlaps right

            minOverlap = min({overlapTop, overlapBottom, overlapLeft, overlapRight});

            // Handle collision based on minimum overlap and velocity
            if (minOverlap == overlapTop) { // Falling onto block (top collision)
                Player.y = platTop - 40; // Position player on top
                Player.vy = 0.0f;       // Stop falling
                Player.onPlatform = true;
            }
            else if (minOverlap == overlapBottom) { // Jumping up through bottom (allow jump-through)
                Player.y = platBottom; // Push player below block
                Player.vy = 0.0f;
            }
            else if (minOverlap == overlapLeft) { // Left side collision
                Player.x = platLeft - 40; // Push player left of block
                Player.vx*=-0.5;        // Stop horizontal velocity
            }
            else if (minOverlap == overlapRight) { // Right side collision
                Player.x = platRight; // Push player right of block
                Player.vx*=-0.5;    // Stop horizontal velocity
            }
        }
    }
    // Horizontal movement (A/D) only on Platforms/ground
    if (KeyCheck[SDL_SCANCODE_W] && Player.onPlatform){
        Player.vy = JUMP_VELOCITY;
    }
    if (KeyCheck[SDL_SCANCODE_A]){
        Player.vx = max(-MOVE_SPEED, Player.vx-ACCELERATION);
        Player.facing=SDL_FLIP_NONE;
    }
    else if (KeyCheck[SDL_SCANCODE_D]){
        Player.vx = min(MOVE_SPEED, Player.vx+ACCELERATION);
        Player.facing=SDL_FLIP_HORIZONTAL;
    }
    else if(Player.onPlatform) Player.vx = 0.0f;

    if(Mouse.left && Slash.frame==0) Slash.frame=1;
}
//Render
//Player
void RenderPlayer(){
    Player.frame=(Player.frame+1)%32;
    Player.Rect.x = int(Player.x - Camera.x)-10;
    Player.Rect.y = int(Player.y - Camera.y)-20;
    SDL_RenderCopyEx(renderer, Player_Texture[int(Player.frame/4)%8], NULL, &Player.Rect, 0.0f, &SPRITE_RECT_CENT, Player.facing);
}
void RenderWeakSkeletons(){
    for(int i = 0; i < WeakSkeletons.size(); i++){
        if(WeakSkeletons[i].x<Camera.x+Max_screen_W && WeakSkeletons[i].x+70>Camera.x && WeakSkeletons[i].y<Camera.y+Max_screen_H && WeakSkeletons[i].y+70>Camera.y){
            WeakSkeletonRect.x = int(WeakSkeletons[i].x - Camera.x)-15;
            WeakSkeletonRect.y = int(WeakSkeletons[i].y - Camera.y)-30;
            if(WeakSkeletons[i].hp>0){
                WeakSkeletons[i].frame=(WeakSkeletons[i].frame+1)%16;
                SDL_RenderCopyEx(renderer, Weak_Skeleton_Texture[(WeakSkeletons[i].frame/8)%2], NULL, &WeakSkeletonRect, 0.0f, &SPRITE_RECT_CENT, WeakSkeletons[i].facing);
            }
            else SDL_RenderCopyEx(renderer, Weak_Skeleton_Texture[2], NULL, &WeakSkeletonRect, 0.0f, &SPRITE_RECT_CENT, WeakSkeletons[i].facing);
        }
    }
}

void RenderWeakSkeletonsAttacks(){
    for(int i = 0; i < WeakSkeletons.size(); i++){
        if(WeakSkeletons[i].attack_state>21){
            Ske_thrust_rect.x=WeakSkeletons[i].x-Camera.x+int(WeakSkeletons[i].facing)*30-15;
            Ske_thrust_rect.y=WeakSkeletons[i].y-Camera.y+15;
            SDL_RenderCopyEx(renderer, Ske_thrust, NULL, &Ske_thrust_rect, 0.0f, &Ske_thrust_rect_cent, WeakSkeletons[i].facing);
        }
    }
}
//Platform
void RenderMap(){
    for (int i = 0; i < Platforms.size(); i++) {
        for(int j = 0; j < Platforms[i].w; j+=40){
            for(int k = 0; k < Platforms[i].h; k+= 40){
                platRect = {int(Platforms[i].x - Camera.x+j), int(Platforms[i].y - Camera.y+k), 40, 40};
                SDL_RenderCopy(renderer, Block_Texture[Platforms[i].material], nullptr, &platRect);
            }
        }
    }
}
//Health bar
void RenderHP(){
    SDL_RenderCopy(renderer, HPbarFrame, nullptr, &HPbarFrameRect);
    SDL_SetRenderDrawColor(renderer, 255-max(0, int(5.1*(Player.hp-50))), min(int(Player.hp*5.1), 255), 0, 255);
    HPbarRect.w=Player.hp*2;
    SDL_RenderFillRect(renderer, &HPbarRect);
}
//Crosshair
void RenderMouse(){
    Mouse.Rect.x=Mouse.x;
    Mouse.Rect.y=Mouse.y;
    if(IsPaused || InMenu) SDL_RenderCopy(renderer, PointerText, nullptr, &Mouse.Rect);
    else{
        Mouse.Rect.x-=15;
        Mouse.Rect.y-=15;
        SDL_RenderCopy(renderer, CrosshairText, nullptr, &Mouse.Rect);
    }
}
//Attacks
void RenderAttack(){
    Slash.Rect.x=Player.x-Camera.x-50+int(Player.facing)*50;
    Slash.Rect.y=Player.y-Camera.y-40;
    if(Slash.frame<3)SDL_RenderCopyEx(renderer, Slash_Attack_Texture[0], NULL, &Slash.Rect, 0.0f, &SlashRectCent, Player.facing);
    else if(Slash.frame<5) SDL_RenderCopyEx(renderer, Slash_Attack_Texture[1], NULL, &Slash.Rect, 0.0f, &SlashRectCent, Player.facing);
    else if(Slash.frame<7) SDL_RenderCopyEx(renderer, Slash_Attack_Texture[2], NULL, &Slash.Rect, 0.0f, &SlashRectCent, Player.facing);
    else if(Slash.frame<9) SDL_RenderCopyEx(renderer, Slash_Attack_Texture[3], NULL, &Slash.Rect, 0.0f, &SlashRectCent, Player.facing);
    if(Slash.frame==2){
        for(int i = 0; i < WeakSkeletons.size(); i++){
            if ((WeakSkeletons[i].x + 40 > Slash.Rect.x+Camera.x && WeakSkeletons[i].x < Slash.Rect.x+Camera.x+100) && (WeakSkeletons[i].y + 40 > Slash.Rect.y+Camera.y && WeakSkeletons[i].y < Slash.Rect.y+Camera.y+100 && WeakSkeletons[i].hp)){
                WeakSkeletons[i].vy-=5-Player.vy*0.1;
                WeakSkeletons[i].vx-=6-int(Player.facing)*12-Player.vx*0.1;
                WeakSkeletons[i].hp-=10;
            }
        }
    }
    if(Slash.frame==15) Slash.frame=-1;
    Slash.frame++;
}
//Game loop
void GameLoop() {
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if(e.type==SDL_KEYDOWN){
                switch( e.key.keysym.sym ){
                    case SDLK_ESCAPE:
                        if(!InMenu) IsPaused=!IsPaused;
                        break;
                    default:
                        break;
                }
            }
            if(e.type==SDL_MOUSEBUTTONDOWN){
                if(e.button.button==SDL_BUTTON_LEFT) Mouse.left=true;
                if(e.button.button==SDL_BUTTON_RIGHT) Mouse.right=true;
            }
        }
        SDL_GetGlobalMouseState(&Mouse.x, &Mouse.y);
        //Render background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        if(InMenu){
            MainMenu();
        }
        else{
            //Render gameplay;
            RenderMap();
            RenderWeakSkeletons();
            RenderPlayer();
            RenderHP();
            if(IsPaused){
                PauseMenu();
            }
            else{
                UpdateCamera();
                RenderWeakSkeletonsAttacks();
                if(Slash.frame) RenderAttack();
                UpdateWeakSkeletons();
                UpdatePlayer();;
            }
        }
        Mouse.left=false;
        Mouse.right=false;
        RenderMouse();
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void CloseSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
}
