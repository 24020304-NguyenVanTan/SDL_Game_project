
#include "Game_Engine.hpp"

fstream PlayerSave;
fstream EnemySave;

//Window and renderer
SDL_Window* window=SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
SDL_Renderer* renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
int Screen_W, Screen_H;

//Control
bool running = true, InMenu=true, IsPaused = false;
int GameOverDelay = 0;
const Uint8* KeyCheck = SDL_GetKeyboardState(nullptr);
int attack_mode=1;//1 is melee and default, 2 is ranged.
SDL_Event e;
//Menu
SDL_Texture* MainMenuText=IMG_LoadTexture(renderer, "Assets/Menus/Main_Menu.png");
SDL_Rect MainMenuRect={0, 0, 530, 670};

SDL_Texture* NewGameButtonText[]={
    IMG_LoadTexture(renderer, "Assets/Menus/NewGame_button.png"),
    IMG_LoadTexture(renderer, "Assets/Menus/NewGame_button_1.png")
};
SDL_Rect NewGameButtonRect={0, 0, 400, 140};

SDL_Texture* ExitButtonText[]={
    IMG_LoadTexture(renderer, "Assets/Menus/Exit_button.png"),
    IMG_LoadTexture(renderer, "Assets/Menus/Exit_button_1.png")
};
SDL_Rect ExitButtonRect={0, 0, 400, 140};

//Pause Menu
SDL_Texture* PauseMenuText = IMG_LoadTexture(renderer, "Assets/Menus/Pause_Menu.png");
SDL_Rect PauseMenuRect={0, 0, 1000, 430};

SDL_Texture* ContunueButtonText[]={
    IMG_LoadTexture(renderer, "Assets/Menus/Continue_button.png"),
    IMG_LoadTexture(renderer, "Assets/Menus/Continue_button_1.png")
};
SDL_Rect ContinueButtonRect={0, 0, 400, 140};
SDL_Rect Menu_ContinueButtonRect={0, 0, 400, 140};

SDL_Texture* MenuButtonText[]={
    IMG_LoadTexture(renderer, "Assets/Menus/Menu_button.png"),
    IMG_LoadTexture(renderer, "Assets/Menus/Menu_button_1.png")
};
SDL_Rect MenuButtonRect={0, 0, 400, 140};
//Game over menu
SDL_Texture* GameOverMenuText= IMG_LoadTexture(renderer, "Assets/Menus/Game_over_Menu.png");
//This use the same rect as the pause menu cuz I'm lazy
//GUI
SDL_Texture* HPbarFrame=IMG_LoadTexture(renderer, "Assets/Menus/HP_bar.png");
SDL_Texture* CrosshairText=IMG_LoadTexture(renderer, "Assets/Sprites/Attacks/Crosshair.png");
SDL_Texture* PointerText=IMG_LoadTexture(renderer, "Assets/Menus/Mouse.png");
//Tiles
SDL_Texture* Tile_Texture[]={
    IMG_LoadTexture(renderer, "Assets/Tiles/Wall.png"),//0
};
//Player
SDL_Texture* Player_Texture[]={
    IMG_LoadTexture(renderer, "Assets/Sprites/Character/Knight_0.png"),
    IMG_LoadTexture(renderer, "Assets/Sprites/Character/Knight_1.png"),
    IMG_LoadTexture(renderer, "Assets/Sprites/Character/Knight_2.png"),
    IMG_LoadTexture(renderer, "Assets/Sprites/Character/Knight_3.png"),
    IMG_LoadTexture(renderer, "Assets/Sprites/Character/Knight_4.png"),
    IMG_LoadTexture(renderer, "Assets/Sprites/Character/Knight_5.png"),
    IMG_LoadTexture(renderer, "Assets/Sprites/Character/Knight_6.png"),
    IMG_LoadTexture(renderer, "Assets/Sprites/Character/Knight_7.png"),
    IMG_LoadTexture(renderer, "Assets/Sprites/Character/Knight_dead.png"),
};
//Weapon
//Arrow
SDL_Texture* ArrowText=IMG_LoadTexture(renderer, "Assets/Sprites/Weapons/Arrow.png");
SDL_Rect ArrowRect={0, 0, 48, 10};
SDL_Point ArrowRectCent={24, 5};
double TtA;//Time to arrival for enemy's aimbot
double angle_rad;//Used for collision checking
//Enemies
//Weak skeleton
SDL_Texture* Skeleton_Texture[3]={
    IMG_LoadTexture(renderer, "Assets/Sprites/Skeleton/Skeleton_0.png"),
    IMG_LoadTexture(renderer, "Assets/Sprites/Skeleton/Skeleton_1.png"),
    IMG_LoadTexture(renderer, "Assets/Sprites/Skeleton/Skeleton_dead.png"),
};
//Defining
//HP bar;
SDL_Rect HPbarFrameRect = {10, 10, 210, 50};
SDL_Rect HPbarRect = {15, 15, 0, 20};
//Crosshair & mouse
SDL_Rect CrosshairRect={0, 0, 30, 30};
//Camera
camera Camera={0, 0};
//Mouse
mouse Mouse={0, 0, false, false, {0, 0, 30, 30}};
double destx, desty;
//Platform
std::vector<tile> Walls={
    {-80, -80},
    {-40, -80},
    {0, -80},
    {40, -80},
    {80, -80},
    {-80, -40},
    {80, -40},
    {-80, 0},
    {80, 0},
    {-80, 40},
    {80, 40},
    {-80, 80},
    {-40, 80},
    {40, 80},
    {80, 80},
};
float platTop, platBottom, platLeft, platRight, overlapTop, overlapBottom, overlapRight, overlapLeft, minOverlap;
SDL_Rect TileRect={0, 0, 40, 40};
SDL_Rect WallRect={0, 0, 40, 60};
//Player
player Player = {0, 0, 0.0, 0.0, {0, 0, 60, 60}, SDL_FLIP_NONE, 0, 100};
//Enemies
std::vector<enemy> Enemy={
    {0, 0, 160, 0, 0, SDL_FLIP_NONE, 0, 0, 30},
    {0, 320, 160, 0, 0, SDL_FLIP_NONE, 0, 0, 30},
    {1, 160, 40, 0, 0, SDL_FLIP_NONE, 0, 0, 15},
};
double dx, dy, distance;
SDL_Rect EnemyRect={0, 0, 60, 60};

//Arrow
std::vector<projectile> Arrow;
std::vector<projectile> EnemyArrow;
projectile TempArrow={0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
//Weapons
melee_weapon Melee[]={
    {"Sword of King Hero", IMG_LoadTexture(renderer, "Assets/Sprites/Weapons/Sword of King Hero.png"), {0, 0, 72, 20}, {10, 10}},
    {"Tomb guard's Cleaver", IMG_LoadTexture(renderer, "Assets/Sprites/Weapons/Tomb guard's Cleaver.png"), {0, 0, 40, 12}, {6, 7}},
};
int MeleeStage=0;
crossbow Crossbow[]{
    {"Crossbow Plus",{IMG_LoadTexture(renderer, "Assets/Sprites/Weapons/Crossbow Plus/Crossbow_0.png"), IMG_LoadTexture(renderer, "Assets/Sprites/Weapons/Crossbow Plus/Crossbow_1.png"), IMG_LoadTexture(renderer, "Assets/Sprites/Weapons/Crossbow Plus/Crossbow_2.png")}, {0, 0, 50, 34}, {12, 17}, 32},
    {"Crossbow",{IMG_LoadTexture(renderer, "Assets/Sprites/Weapons/Crossbow/Crossbow_0.png"), IMG_LoadTexture(renderer, "Assets/Sprites/Weapons/Crossbow/Crossbow_1.png"), IMG_LoadTexture(renderer, "Assets/Sprites/Weapons/Crossbow/Crossbow_2.png")}, {0, 0, 36, 24}, {8, 14}, 48},
};
int CrossbowCharge=100;
double WeaponAngle=0.0;

//Functions
//Line of sight check
const double step_size = 24.0; // Global constant
bool LineofSight(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1, dy = y2 - y1;
    double distance = sqrt(dx * dx + dy * dy);
    if (distance < 0.5) return true;
    // Step along the line from (x1, y1) to (x2, y2)
    int steps = max(2, int(distance/step_size) + 1);
    double x = x1, y = y1;
    dx/=steps;
    dy/=steps;
    for (int i = 0; i <= steps; i++) {
        x+=dx;
        y+=dy;
        // Check for intersection with platforms
        for (int j = 0; j < Walls.size(); j++) {
            platLeft = Walls[j].x;
            platRight = Walls[j].x + 40;
            platTop = Walls[j].y;
            platBottom = Walls[j].y + 40;
            if (x >= platLeft-5 && x <= platRight+5 && y >= platTop-5 && y <= platBottom+5) {
                return false;
            }
        }
    }
    return true; // No platforms block the line of sight
}
//Data
void ResetData(){
    Player.x=0;
    Player.y=0;
    Player.hp=100;
    GameOverDelay=0;
    cout<<"Data resetted \n";
}
void SaveData() {
    //Player
    PlayerSave.open("Data_Saves/PlayerSave.txt", std::ios::out | std::ios::trunc);
    PlayerSave << Player.x << " " << Player.y << " " << Player.hp;
    PlayerSave.close();
    //Enemy
    EnemySave.open("Data_saves/EnemySave.txt", std::ios::out | std::ios::trunc);
    for(int i = 0; i < Enemy.size(); i++){
        EnemySave<<Enemy[i].type<<" "<<Enemy[i].x<<" "<<Enemy[i].y<<" "<<Enemy[i].hp<<"\n";
    }
    EnemySave.close();
    cout << "Game saved\n";
}
void RetrieveData(){
    //Player
    PlayerSave.open("Data_Saves/PlayerSave.txt", std::ios::in);
    if (PlayerSave.is_open()){
        PlayerSave >> Player.x >> Player.y >> Player.hp;
        cout<<"Retrieve x: "<<Player.x<<", y: "<<Player.y<<", hp: "<<Player.hp<< "\n";
        PlayerSave.close();

        EnemySave.open("Data_Saves/PlayerSave.txt", std::ios::in);
        Enemy.clear();
        enemy TempEnemy={0, 0, 0, 0, 0, SDL_FLIP_NONE, 0, 30};
        while(EnemySave>>TempEnemy.type){
            EnemySave>>TempEnemy.x>>TempEnemy.y>>TempEnemy.hp;
            Enemy.push_back(TempEnemy);
        }
        EnemySave.close();

    }
    else{
        cout<<"No save retrievable, revert back to default \n";
        ResetData();
    }
}
//Menu
void CenterMenus(){
    SDL_GetWindowSize(window, &Screen_W, &Screen_H);
    //Main menu
    MainMenuRect.x=Screen_W/2-265;
    MainMenuRect.y=Screen_H/2-335;
    //Exit button
    ExitButtonRect.x=MainMenuRect.x+65;
    ExitButtonRect.y=MainMenuRect.y+500;
    //Continue button
    Menu_ContinueButtonRect.x=MainMenuRect.x+65;
    Menu_ContinueButtonRect.y=MainMenuRect.y+350;
    //New game button
    NewGameButtonRect.x=MainMenuRect.x+65;
    NewGameButtonRect.y=MainMenuRect.y+200;
    //Pause menu
    PauseMenuRect.x=Screen_W/2-500;
    PauseMenuRect.y=Screen_H/2-215;
    //Continue button
    ContinueButtonRect.x=PauseMenuRect.x+80;
    ContinueButtonRect.y=PauseMenuRect.y+240;
    //Menu button
    MenuButtonRect.x=PauseMenuRect.x+534;
    MenuButtonRect.y=PauseMenuRect.y+240;
}
//Main menu
void MainMenu(){
    SDL_RenderCopy(renderer, MainMenuText, nullptr, &MainMenuRect);

    //New game button
    if(Mouse.x>NewGameButtonRect.x && Mouse.x<NewGameButtonRect.x+400 && Mouse.y>NewGameButtonRect.y && Mouse.y<NewGameButtonRect.y+140){
        SDL_RenderCopy(renderer, NewGameButtonText[1], nullptr, &NewGameButtonRect);
        if(Mouse.left){
            Mouse.left=false;
            InMenu=false;
            ResetData();
        }
    }
    else SDL_RenderCopy(renderer, NewGameButtonText[0], nullptr, &NewGameButtonRect);

    //Continue button
    if(Mouse.x>Menu_ContinueButtonRect.x && Mouse.x<Menu_ContinueButtonRect.x+400 && Mouse.y>Menu_ContinueButtonRect.y && Mouse.y<Menu_ContinueButtonRect.y+140){
        SDL_RenderCopy(renderer, ContunueButtonText[1], nullptr, &Menu_ContinueButtonRect);
        if(Mouse.left){
            Mouse.left=false;
            InMenu=false;
            RetrieveData();
        }
    }
    else SDL_RenderCopy(renderer, ContunueButtonText[0], nullptr, &Menu_ContinueButtonRect);

    //Exit button
    if(Mouse.x>ExitButtonRect.x && Mouse.x<ExitButtonRect.x+400 && Mouse.y>ExitButtonRect.y && Mouse.y<ExitButtonRect.y+140){
        SDL_RenderCopy(renderer, ExitButtonText[1], nullptr, &ExitButtonRect);
        if(Mouse.left){
            Mouse.left=false;
            running = false;
        }
    }
    else SDL_RenderCopy(renderer, ExitButtonText[0], nullptr, &ExitButtonRect);
}
//Pause menu
void PauseMenu(){
    SDL_RenderCopy(renderer, PauseMenuText, nullptr, &PauseMenuRect);

    //Continue button
    if(Mouse.x>ContinueButtonRect.x && Mouse.x<ContinueButtonRect.x+400 && Mouse.y>ContinueButtonRect.y && Mouse.y<ContinueButtonRect.y+140){
        SDL_RenderCopy(renderer, ContunueButtonText[1], nullptr, &ContinueButtonRect);
        if(Mouse.left){
            Mouse.left=false;
            IsPaused=false;
        }
    }
    else SDL_RenderCopy(renderer, ContunueButtonText[0], nullptr, &ContinueButtonRect);

    //Menu button
    if(Mouse.x>MenuButtonRect.x && Mouse.x<MenuButtonRect.x+400 && Mouse.y>MenuButtonRect.y && Mouse.y<MenuButtonRect.y+140){
        SDL_RenderCopy(renderer, MenuButtonText[1], nullptr, &MenuButtonRect);
        if(Mouse.left){
            Mouse.left=false;
            IsPaused=false;
            InMenu=true;
            SaveData();
        }
    }
    else SDL_RenderCopy(renderer, MenuButtonText[0], nullptr, &MenuButtonRect);
}
//Game over menu
void GameOverMenu(){
    SDL_RenderCopy(renderer, GameOverMenuText, nullptr, &PauseMenuRect);
    //New game button
    if(Mouse.x>ContinueButtonRect.x && Mouse.x<ContinueButtonRect.x+400 && Mouse.y>ContinueButtonRect.y && Mouse.y<ContinueButtonRect.y+140){
        SDL_RenderCopy(renderer, NewGameButtonText[1], nullptr, &ContinueButtonRect);
        if(Mouse.left){
            Mouse.left=false;
            ResetData();
        }
    }
    else SDL_RenderCopy(renderer, NewGameButtonText[0], nullptr, &ContinueButtonRect);
    //Menu button
    if(Mouse.x>MenuButtonRect.x && Mouse.x<MenuButtonRect.x+400 && Mouse.y>MenuButtonRect.y && Mouse.y<MenuButtonRect.y+140){
        SDL_RenderCopy(renderer, MenuButtonText[1], nullptr, &MenuButtonRect);
        if(Mouse.left){
            Mouse.left=false;
            InMenu=true;
            ResetData();
        }
    }
    else SDL_RenderCopy(renderer, MenuButtonText[0], nullptr, &MenuButtonRect);

}
//Update
//Camera
void UpdateCamera(){
    Camera.x = Player.x - (Screen_W / 2) + 20;
    Camera.y = Player.y - (Screen_H / 2) + 20;
}
//Enemies
void UpdateEnemy() {
    // Update position (before collision to detect overlaps)
    for (int i = 0; i < Enemy.size(); i++) {
        Enemy[i].y += Enemy[i].vy;
        Enemy[i].x += Enemy[i].vx;
        Enemy[i].vx = 0.0;
        Enemy[i].vy = 0.0;
        if (Enemy[i].hp > 0) {
            for (int j = 0; j < Walls.size(); j++) {
                platTop = Walls[j].y;
                platBottom = Walls[j].y + 40;
                platLeft = Walls[j].x;
                platRight = Walls[j].x + 40;

                // Check for overlap (AABB collision)
                if (Enemy[i].x + 40 > platLeft && Enemy[i].x < platRight && Enemy[i].y + 40 > platTop && Enemy[i].y < platBottom) {
                    // Calculate overlap amounts
                    overlapTop = (Enemy[i].y + 40) - platTop;    // How much enemy overlaps top
                    overlapBottom = platBottom - Enemy[i].y;     // How much enemy overlaps bottom
                    overlapLeft = (Enemy[i].x + 40) - platLeft;  // How much enemy overlaps left
                    overlapRight = platRight - Enemy[i].x;       // How much enemy overlaps right

                    minOverlap = min({overlapTop, overlapBottom, overlapLeft, overlapRight});

                    // Handle collision based on minimum overlap
                    if (minOverlap == overlapTop) { // Top collision
                        Enemy[i].y = platTop - 40;
                        Enemy[i].vy = 0.0;
                    }
                    else if (minOverlap == overlapBottom) { // Bottom collision
                        Enemy[i].y = platBottom;
                        Enemy[i].vy = 0.0;
                    }
                    else if (minOverlap == overlapLeft) { // Left side collision
                        Enemy[i].x = platLeft - 40;
                        Enemy[i].vx = 0.0;
                    }
                    else if (minOverlap == overlapRight) { // Right side collision
                        Enemy[i].x = platRight;
                        Enemy[i].vx = 0.0;
                    }
                }
            }
            // Enemy-Enemy collision
            for (int j = i + 1; j < Enemy.size(); j++) {
                if (Enemy[j].hp > 0){
                    if (Enemy[i].x + 40 > Enemy[j].x && Enemy[i].x < Enemy[j].x + 40 && Enemy[i].y + 40 > Enemy[j].y && Enemy[i].y < Enemy[j].y + 40) {
                        overlapLeft = Enemy[i].x + 40 - Enemy[j].x;
                        overlapRight = Enemy[j].x + 40 - Enemy[i].x;
                        overlapTop = Enemy[i].y + 40 - Enemy[j].y;
                        overlapBottom = Enemy[j].y + 40 - Enemy[i].y;
                        minOverlap = min({overlapTop, overlapBottom, overlapLeft, overlapRight});
                        // Add a small buffer (1 pixel) to prevent sticking
                        if (minOverlap == overlapLeft) {
                            Enemy[i].vx -= (overlapLeft + 1) / 2;
                            Enemy[j].vx += (overlapLeft + 1) / 2;
                        }
                        else if (minOverlap == overlapRight) {
                            Enemy[i].vx += (overlapRight + 1) / 2;
                            Enemy[j].vx -= (overlapRight + 1) / 2;
                        }
                        else if (minOverlap == overlapTop) {
                            Enemy[i].vy -= (overlapTop + 1) / 2;
                            Enemy[j].vy += (overlapTop + 1) / 2;
                        }
                        else if (minOverlap == overlapBottom) {
                            Enemy[i].vy += (overlapBottom + 1) / 2;
                            Enemy[j].vy -= (overlapBottom + 1) / 2;
                        }
                    }
                }
            }
            // Enemy-Player collision
             if (Enemy[i].x + 40 > Player.x && Enemy[i].x < Player.x + 40 && Enemy[i].y + 40 > Player.y && Enemy[i].y < Player.y + 40) {
            overlapLeft = Enemy[i].x + 40 - Player.x;
            overlapRight = Player.x + 40 - Enemy[i].x;
            overlapTop = Enemy[i].y + 40 - Player.y;
            overlapBottom = Player.y + 40 - Enemy[i].y;
            minOverlap = min({overlapTop, overlapBottom, overlapLeft, overlapRight});
            // Add a small buffer (1 pixel) to prevent sticking
            if (minOverlap == overlapLeft) {
                Enemy[i].x += -(overlapLeft + 1)*2 / 3;
                Player.x += (overlapLeft + 1) / 3;
            }
            else if (minOverlap == overlapRight) {
                Enemy[i].x += (overlapRight + 1)*2 / 3;
                Player.x += -(overlapRight + 1) / 3;
            }
            else if (minOverlap == overlapTop) {
                Enemy[i].y += -(overlapTop + 1)*2 / 3;
                Player.y += (overlapTop + 1) / 3;
            }
            else if (minOverlap == overlapBottom) {
                Enemy[i].y += (overlapBottom + 1)*2 / 3;
                Player.y += -(overlapBottom + 1) / 3;
            }
        }
            // Movement and attack logic
            dx = Player.x - Enemy[i].x;
            dy = Player.y - Enemy[i].y;
            distance = sqrt(dx * dx + dy * dy);
            if(Enemy[i].type){
                Enemy[i].attack_state=min(Crossbow[1].max_charge, Enemy[i].attack_state+1);
                if(distance<800){
                    if(distance<300){
                        Enemy[i].vx=-3.0*dx/distance;
                        Enemy[i].vy=-3.0*dy/distance;
                    }
                    else if(distance>650){
                        Enemy[i].vx=3.0*dx/distance;
                        Enemy[i].vy =3.0*dy/distance;
                    }
                    if (dx>0) Enemy[i].facing = SDL_FLIP_NONE;
                    else if (dx<0) Enemy[i].facing = SDL_FLIP_HORIZONTAL;
                    if(Enemy[i].attack_state==Crossbow[1].max_charge){
                        if(LineofSight(Enemy[i].x+20, Enemy[i].y+20, Player.x+20, Player.y+20)){
                            //Leading the shot, cuz aimbot fun
                            TtA=distance/ARROW_VELOCITY;
                            dx = Player.x + TtA*Player.vx - Enemy[i].x;
                            dy = Player.y + TtA*Player.vy - Enemy[i].y;
                            distance = sqrt(dx * dx + dy * dy);
                            //The actual shooting
                            TempArrow.x=Enemy[i].x+20;
                            TempArrow.y=Enemy[i].y+20;
                            TempArrow.vx=ARROW_VELOCITY*dx/distance;
                            TempArrow.vy=ARROW_VELOCITY*dy/distance;
                            TempArrow.angle=atan2(TempArrow.vy,TempArrow.vx)*57.2957795131;
                            TempArrow.distance=0.0;
                            EnemyArrow.push_back(TempArrow);
                            Enemy[i].attack_state=0;
                        }
                        else{
                            Enemy[i].vx=-3.0*dy/distance;
                            Enemy[i].vy=3.0*dx/distance;
                        }
                    }
                }
            }

            else{
                if(distance<800){
                    if (distance>0) {
                        Enemy[i].vx=3.0*dx/distance;
                        Enemy[i].vy =3.0*dy/distance;
                        if (dx>0) Enemy[i].facing = SDL_FLIP_NONE;
                        else if (dx<0) Enemy[i].facing = SDL_FLIP_HORIZONTAL;
                    }
                    if(distance<45 && !Enemy[i].attack_state) Enemy[i].attack_state=1;
                    if(Enemy[i].attack_state){
                        if(Enemy[i].attack_state==45) Enemy[i].attack_state=-1;
                        else if(Enemy[i].attack_state>15 && Enemy[i].attack_state<21){
                            if(distance<50){
                                Player.hp=max(0, Player.hp-2);
                                Player.vx+=5.0*dx/distance;
                                Player.vy+=5.0*dy/distance;
                            }
                        }
                        Enemy[i].attack_state++;
                    }
                }
            }
        }
    }
}
//Player

void UpdateWeapons(){
    if(KeyCheck[SDL_SCANCODE_1]) attack_mode=1;
    else if(KeyCheck[SDL_SCANCODE_2]) attack_mode=2;
    if(attack_mode==1){
        if(Mouse.left && !MeleeStage) MeleeStage=1;
        if(MeleeStage){
            if(MeleeStage==25) MeleeStage=-1;
            else{
                if(MeleeStage>5 && MeleeStage<11){
                    for(int i = 0; i < Enemy.size(); i++){
                        if(Enemy[i].hp>0){
                            dx = -Player.x + Enemy[i].x;
                            dy = -Player.y + Enemy[i].y;
                            distance = sqrt(dx * dx + dy * dy);
                            if(distance<80 && atan2(dy, dx)<atan2(Mouse.y-Melee[0].rect.y-Melee[0].pivot.y,Mouse.x-Melee[0].rect.x-Melee[0].pivot.x)+pi/3 && atan2(dy, dx)>atan2(Mouse.y-Melee[0].rect.y-Melee[0].pivot.y,Mouse.x-Melee[0].rect.x-Melee[0].pivot.x)-pi/3){
                                Enemy[i].hp=max(0, Enemy[i].hp-3);
                                Enemy[i].vx+=5.0*dx/distance;
                                Enemy[i].vy+=5.0*dy/distance;
                            }
                        }
                    }
                }
                // Arrow interception
                if (MeleeStage < 17) {
                    for (int i = EnemyArrow.size() - 1; i >= 0; i--) {
                        angle_rad = EnemyArrow[i].angle * pi / 180.0; // Convert angle to radians
                        // Check tip
                        dx = (EnemyArrow[i].x + 24 * cos(angle_rad)) - Player.x;
                        dy = (EnemyArrow[i].y + 24 * sin(angle_rad)) - Player.y;
                        distance = sqrt(dx * dx + dy * dy);
                        if (distance < 90 && atan2(dy, dx) < atan2(Mouse.y - Melee[0].rect.y - Melee[0].pivot.y, Mouse.x - Melee[0].rect.x - Melee[0].pivot.x) + pi / 3 && atan2(dy, dx) > atan2(Mouse.y - Melee[0].rect.y - Melee[0].pivot.y, Mouse.x - Melee[0].rect.x - Melee[0].pivot.x) - pi / 3) {
                            EnemyArrow.erase(EnemyArrow.begin() + i); // Remove the arrow
                            continue;
                        }
                        // Check midpoint
                        dx = EnemyArrow[i].x - Player.x;
                        dy = EnemyArrow[i].y - Player.y;
                        distance = sqrt(dx * dx + dy * dy);
                        if (distance < 90 && atan2(dy, dx) < atan2(Mouse.y - Melee[0].rect.y - Melee[0].pivot.y, Mouse.x - Melee[0].rect.x - Melee[0].pivot.x) + pi / 3 && atan2(dy, dx) > atan2(Mouse.y - Melee[0].rect.y - Melee[0].pivot.y, Mouse.x - Melee[0].rect.x - Melee[0].pivot.x) - pi / 3) {
                            EnemyArrow.erase(EnemyArrow.begin() + i);
                            continue;
                        }
                        // Check tail
                        dx = (EnemyArrow[i].x - 24 * cos(angle_rad)) - Player.x;
                        dy = (EnemyArrow[i].y - 24 * sin(angle_rad)) - Player.y;
                        distance = sqrt(dx * dx + dy * dy);
                        if (distance < 90 && atan2(dy, dx) < atan2(Mouse.y - Melee[0].rect.y - Melee[0].pivot.y, Mouse.x - Melee[0].rect.x - Melee[0].pivot.x) + pi / 3 && atan2(dy, dx) > atan2(Mouse.y - Melee[0].rect.y - Melee[0].pivot.y, Mouse.x - Melee[0].rect.x - Melee[0].pivot.x) - pi / 3) {
                            EnemyArrow.erase(EnemyArrow.begin() + i);
                            continue;
                        }
                    }
                }
            }
            MeleeStage++;
        }
    }
    else if(attack_mode==2){
        CrossbowCharge=min(Crossbow[0].max_charge, CrossbowCharge+1);
        if(Mouse.left && Crossbow[0].max_charge==CrossbowCharge){
            TempArrow.x=Player.x+20;
            TempArrow.y=Player.y+20;
            TempArrow.vx=ARROW_VELOCITY*(destx-TempArrow.x)/sqrt(pow((destx-TempArrow.x), 2)+pow((desty-TempArrow.y), 2));
            TempArrow.vy=ARROW_VELOCITY*(desty-TempArrow.y)/sqrt(pow((destx-TempArrow.x), 2)+pow((desty-TempArrow.y), 2));
            TempArrow.angle=atan2(desty-TempArrow.y,destx-TempArrow.x)*57.2957795131;
            TempArrow.distance=0.0;
            Arrow.push_back(TempArrow);
            CrossbowCharge=0;
            Mouse.left=false;
        }
    }
}
void UpdatePlayer() {
    // Update position (before collision to detect overlaps)
    Player.y += Player.vy;
    Player.x += Player.vx;
    Player.vx=0.0;
    Player.vy=0.0;
    if(GameOverDelay<64){
        for (int i = 0; i < Walls.size(); i++) {
            platTop = Walls[i].y;
            platBottom = Walls[i].y + 40;
            platLeft = Walls[i].x;
            platRight = Walls[i].x + 40;

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
                    Player.vy = 0.0;       // Stop falling
                }
                else if (minOverlap == overlapBottom) { // Jumping up through bottom (allow jump-through)
                    Player.y = platBottom; // Push player below block
                    Player.vy = 0.0;
                }
                else if (minOverlap == overlapLeft) { // Left side collision
                    Player.x = platLeft - 40; // Push player left of block
                    Player.vx = 0.0;        // Stop horizontal velocity
                }
                else if (minOverlap == overlapRight) { // Right side collision
                    Player.x = platRight; // Push player right of block
                    Player.vx = 0.0;    // Stop horizontal velocity
                }
            }
        }
    }
    if(Player.hp){
        // Movement
        if (KeyCheck[SDL_SCANCODE_W]){
            Player.vy += -MOVE_SPEED;
        }
        if (KeyCheck[SDL_SCANCODE_S]){
            Player.vy += MOVE_SPEED;
        }
        if (KeyCheck[SDL_SCANCODE_A]){
            Player.vx += -MOVE_SPEED;
        }
        if (KeyCheck[SDL_SCANCODE_D]){
            Player.vx += MOVE_SPEED;
        }
        if(Player.vx && Player.vy){
            Player.vx/=sqrt(2);
            Player.vy/=sqrt(2);
        }
        if(Mouse.x>Player.Rect.x) Player.facing=SDL_FLIP_NONE;
        else  Player.facing=SDL_FLIP_HORIZONTAL;
        //Attack
        UpdateWeapons();
    }
    else GameOverDelay=min(64, GameOverDelay+1);
}
//Arrow
void UpdateArrow(){
    for (int i = Arrow.size() - 1; i >= 0; i--) {
        Arrow[i].x += Arrow[i].vx;
        Arrow[i].y += Arrow[i].vy;
        Arrow[i].distance += ARROW_VELOCITY;
        double angle_rad = Arrow[i].angle * pi / 180.0; // Convert angle to radians
        // Check platform collisions
        for (int j = 0; j < Walls.size(); j++) {
            platTop = Walls[j].y;
            platBottom = Walls[j].y + 40;
            platLeft = Walls[j].x;
            platRight = Walls[j].x + 40;
            // Check tip
            dx = Arrow[i].x + 24 * cos(angle_rad);
            dy = Arrow[i].y + 24 * sin(angle_rad);
            if (dx > platLeft && dx < platRight && dy > platTop && dy < platBottom) {
                Arrow[i].distance = ARROW_MAX_RANGE + 1.0;
                break;
            }
            // Check midpoint
            dx = Arrow[i].x;
            dy = Arrow[i].y;
            if (dx > platLeft && dx < platRight && dy > platTop && dy < platBottom) {
                Arrow[i].distance = ARROW_MAX_RANGE + 1.0;
                break;
            }
            // Check tail
            dx = Arrow[i].x - 24 * cos(angle_rad);
            dy = Arrow[i].y - 24 * sin(angle_rad);
            if (dx > platLeft && dx < platRight && dy > platTop && dy < platBottom) {
                Arrow[i].distance = ARROW_MAX_RANGE + 1.0;
                break;
            }
        }
        if (Arrow[i].distance > ARROW_MAX_RANGE) {
            Arrow.erase(Arrow.begin() + i);
            continue;
        }
        // Check enemy collisions
        for (int j = 0; j < Enemy.size(); j++) {
            if (Enemy[j].hp > 0) {
                // Check tip
                dx = Arrow[i].x + 24 * cos(angle_rad);
                dy = Arrow[i].y + 24 * sin(angle_rad);
                if (dx > Enemy[j].x && dx < Enemy[j].x + 40 && dy > Enemy[j].y && dy < Enemy[j].y + 40) {
                    Enemy[j].vx += Arrow[i].vx / 2;
                    Enemy[j].vy += Arrow[i].vy / 2;
                    Enemy[j].hp = max(Enemy[j].hp - 10, 0);
                    Arrow.erase(Arrow.begin() + i);
                    break;
                }
                // Check midpoint
                dx = Arrow[i].x;
                dy = Arrow[i].y;
                if (dx > Enemy[j].x && dx < Enemy[j].x + 40 && dy > Enemy[j].y && dy < Enemy[j].y + 40) {
                    Enemy[j].vx += Arrow[i].vx / 2;
                    Enemy[j].vy += Arrow[i].vy / 2;
                    Enemy[j].hp = max(Enemy[j].hp - 10, 0);
                    Arrow.erase(Arrow.begin() + i);
                    break;
                }
                // Check tail
                dx = Arrow[i].x - 24 * cos(angle_rad);
                dy = Arrow[i].y - 24 * sin(angle_rad);
                if (dx > Enemy[j].x && dx < Enemy[j].x + 40 && dy > Enemy[j].y && dy < Enemy[j].y + 40) {
                    Enemy[j].vx += Arrow[i].vx / 2;
                    Enemy[j].vy += Arrow[i].vy / 2;
                    Enemy[j].hp = max(Enemy[j].hp - 10, 0);
                    Arrow.erase(Arrow.begin() + i);
                    break;
                }
            }
        }
    }
    for (int i = EnemyArrow.size() - 1; i >= 0; i--) {
        EnemyArrow[i].x += EnemyArrow[i].vx;
        EnemyArrow[i].y += EnemyArrow[i].vy;
        EnemyArrow[i].distance += ARROW_VELOCITY;
        double angle_rad = EnemyArrow[i].angle * pi / 180.0; // Convert angle to radians
        // Check platform collisions
        for (int j = 0; j < Walls.size(); j++) {
            platTop = Walls[j].y;
            platBottom = Walls[j].y + 40;
            platLeft = Walls[j].x;
            platRight = Walls[j].x + 40;
            // Check tip
            dx = EnemyArrow[i].x + 24 * cos(angle_rad);
            dy = EnemyArrow[i].y + 24 * sin(angle_rad);
            if (dx > platLeft && dx < platRight && dy > platTop && dy < platBottom) {
                EnemyArrow[i].distance = ARROW_MAX_RANGE + 1.0;
                break;
            }
            // Check midpoint
            dx = EnemyArrow[i].x;
            dy = EnemyArrow[i].y;
            if (dx > platLeft && dx < platRight && dy > platTop && dy < platBottom) {
                EnemyArrow[i].distance = ARROW_MAX_RANGE + 1.0;
                break;
            }
            // Check tail
            dx = EnemyArrow[i].x - 24 * cos(angle_rad);
            dy = EnemyArrow[i].y - 24 * sin(angle_rad);
            if (dx > platLeft && dx < platRight && dy > platTop && dy < platBottom) {
                EnemyArrow[i].distance = ARROW_MAX_RANGE + 1.0;
                break;
            }
        }
        if (EnemyArrow[i].distance > ARROW_MAX_RANGE) {
            EnemyArrow.erase(EnemyArrow.begin() + i);
            continue;
        }
        // Check player collision
        // Check tip
        dx = EnemyArrow[i].x + 24 * cos(angle_rad);
        dy = EnemyArrow[i].y + 24 * sin(angle_rad);
        if (dx > Player.x && dx < Player.x + 40 && dy > Player.y && dy < Player.y + 40) {
            Player.vx += EnemyArrow[i].vx / 10;
            Player.vy += EnemyArrow[i].vy / 10;
            Player.hp = max(Player.hp - 10, 0);
            EnemyArrow.erase(EnemyArrow.begin() + i);
            continue; // Skip to next arrow
        }
        // Check midpoint
        dx = EnemyArrow[i].x;
        dy = EnemyArrow[i].y;
        if (dx > Player.x && dx < Player.x + 40 && dy > Player.y && dy < Player.y + 40) {
            Player.vx += EnemyArrow[i].vx / 10;
            Player.vy += EnemyArrow[i].vy / 10;
            Player.hp = max(Player.hp - 10, 0);
            EnemyArrow.erase(EnemyArrow.begin() + i);
            continue; // Skip to next arrow
        }
        // Check tail
        dx = EnemyArrow[i].x - 24 * cos(angle_rad);
        dy = EnemyArrow[i].y - 24 * sin(angle_rad);
        if (dx > Player.x && dx < Player.x + 40 && dy > Player.y && dy < Player.y + 40) {
            Player.vx += EnemyArrow[i].vx / 10;
            Player.vy += EnemyArrow[i].vy / 10;
            Player.hp = max(Player.hp - 10, 0);
            EnemyArrow.erase(EnemyArrow.begin() + i);
            continue; // Skip to next arrow
        }
    }
}
//Render
//Arrow
void RenderArrow(){
    for(int i = 0; i < Arrow.size(); i++){
        ArrowRect.x=int(Arrow[i].x - Camera.x)-24;
        ArrowRect.y=int(Arrow[i].y - Camera.y)-5;
        SDL_RenderCopyEx(renderer, ArrowText, NULL, &ArrowRect, Arrow[i].angle, &ArrowRectCent, SDL_FLIP_NONE);
    }
    for(int i = 0; i < EnemyArrow.size(); i++){
        ArrowRect.x=int(EnemyArrow[i].x - Camera.x)-24;
        ArrowRect.y=int(EnemyArrow[i].y - Camera.y)-5;
        SDL_RenderCopyEx(renderer, ArrowText, NULL, &ArrowRect, EnemyArrow[i].angle, &ArrowRectCent, SDL_FLIP_NONE);
    }
}
//Weapons
void RenderWeapons(){
    if(attack_mode==1){
        Melee[0].rect.x=Player.Rect.x-Melee[0].pivot.x+20;
        Melee[0].rect.y=Player.Rect.y-Melee[0].pivot.y+50;

        SDL_RenderCopyEx(renderer, Melee[0].texture, NULL, &Melee[0].rect, atan2(Mouse.y-Melee[0].rect.y-Melee[0].pivot.y,Mouse.x-Melee[0].rect.x-Melee[0].pivot.x)*57.2957795131+((int)Player.facing*120-60)*sin(2*pi*min(16,MeleeStage)/16), &Melee[0].pivot, (Player.facing)? SDL_FLIP_VERTICAL:SDL_FLIP_NONE);
    }
    else{
        Crossbow[0].rect.x=Player.Rect.x-Crossbow[0].pivot.x+30;
        Crossbow[0].rect.y=Player.Rect.y-Crossbow[0].pivot.y+40;
        SDL_RenderCopyEx(renderer, Crossbow[0].texture[int(CrossbowCharge * 2 / Crossbow[0].max_charge)], NULL, &Crossbow[0].rect, atan2(Mouse.y-Crossbow[0].rect.y-Crossbow[0].pivot.y,Mouse.x-Crossbow[0].rect.x-Crossbow[0].pivot.x)*57.2957795131, &Crossbow[0].pivot, (Player.facing)? SDL_FLIP_VERTICAL:SDL_FLIP_NONE);
    }
}
//Player
void RenderPlayer(){
    Player.Rect.x = int(Player.x - Camera.x)-10;
    Player.Rect.y = int(Player.y - Camera.y)-20;
    Player.frame=(Player.frame+1)%32;
    if(Player.hp>0){
        if(Player.facing) RenderWeapons();
        SDL_RenderCopyEx(renderer, Player_Texture[int(Player.frame/4)%8], NULL, &Player.Rect, 0.0f, &SPRITE_RECT_CENT, Player.facing);
        if(!Player.facing) RenderWeapons();
    }
    else{
        SDL_RenderCopyEx(renderer, Player_Texture[8], NULL, &Player.Rect, 0.0f, &SPRITE_RECT_CENT, Player.facing);
    }
}
void RenderEnemy(){
    for(int i = 0; i < Enemy.size(); i++){
        if(Enemy[i].x<Camera.x+Screen_W && Enemy[i].x+60>Camera.x && Enemy[i].y<Camera.y+Screen_H && Enemy[i].y+60>Camera.y){
            EnemyRect.x = int(Enemy[i].x - Camera.x)-10;
            EnemyRect.y = int(Enemy[i].y - Camera.y)-20;
            if(Enemy[i].hp>0){
                Enemy[i].frame=(Enemy[i].frame+1)%16;
                if(Enemy[i].facing){
                    if(Enemy[i].type){
                        Crossbow[1].rect.x=EnemyRect.x-Crossbow[1].pivot.x+30;
                        Crossbow[1].rect.y=EnemyRect.y-Crossbow[1].pivot.y+40;
                        SDL_RenderCopyEx(renderer, Crossbow[1].texture[int(Enemy[i].attack_state * 2 / Crossbow[1].max_charge)], NULL, &Crossbow[1].rect, atan2(Player.y-Enemy[i].y, Player.x-Enemy[i].x)*57.2957795131, &Crossbow[1].pivot, SDL_FLIP_VERTICAL);
                    }
                    else{
                        Melee[1].rect.x=EnemyRect.x-Melee[1].pivot.x+30;
                        Melee[1].rect.y=EnemyRect.y-Melee[1].pivot.y+50;
                        SDL_RenderCopyEx(renderer, Melee[1].texture, NULL, &Melee[1].rect, atan2(Enemy[i].vy, Enemy[i].vx)*57.2957795131+60*sin(max(0, min(16, Enemy[i].attack_state-10))*2*pi/16), &Melee[1].pivot, SDL_FLIP_VERTICAL);
                    }
                }
                SDL_RenderCopyEx(renderer, Skeleton_Texture[(Enemy[i].frame/8)%2], NULL, &EnemyRect, 0.0f, &SPRITE_RECT_CENT, Enemy[i].facing);
                if(!Enemy[i].facing){
                    if(Enemy[i].type){
                        Crossbow[1].rect.x=EnemyRect.x-Crossbow[1].pivot.x+30;
                        Crossbow[1].rect.y=EnemyRect.y-Crossbow[1].pivot.y+40;
                        SDL_RenderCopyEx(renderer, Crossbow[1].texture[int(Enemy[i].attack_state * 2 / Crossbow[1].max_charge)], NULL, &Crossbow[1].rect, atan2(Player.y-Enemy[i].y, Player.x-Enemy[i].x)*57.2957795131, &Crossbow[1].pivot, SDL_FLIP_NONE);
                    }
                    else{
                        Melee[1].rect.x=EnemyRect.x-Melee[1].pivot.x+30;
                        Melee[1].rect.y=EnemyRect.y-Melee[1].pivot.y+50;
                        SDL_RenderCopyEx(renderer, Melee[1].texture, NULL, &Melee[1].rect, atan2(Enemy[i].vy, Enemy[i].vx)*57.2957795131-60*sin(max(0, min(16, Enemy[i].attack_state-10))*2*pi/16), &Melee[1].pivot, SDL_FLIP_NONE);
                    }
                }
            }
            else SDL_RenderCopyEx(renderer, Skeleton_Texture[2], NULL, &EnemyRect, 0.0f, &SPRITE_RECT_CENT, Enemy[i].facing);
        }
    }
}
//Platform
void RenderMap(){
    for (int i = 0; i < Walls.size(); i++) {
        WallRect.x=Walls[i].x-Camera.x;
        WallRect.y=Walls[i].y-Camera.y-20;
        if(WallRect.x>-40 && WallRect.x<Screen_W && WallRect.y>-60 && WallRect.y<Screen_H) SDL_RenderCopy(renderer, Tile_Texture[0], nullptr, &WallRect);
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
        destx=Mouse.x+Camera.x;
        desty=Mouse.y+Camera.y;
        SDL_RenderCopy(renderer, CrosshairText, nullptr, &Mouse.Rect);
    }
}
//Game loop
void GameLoop() {
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if(e.type==SDL_KEYDOWN){
                switch( e.key.keysym.sym ){
                    case SDLK_ESCAPE:
                        if(!InMenu && !GameOverDelay) IsPaused=!IsPaused;
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
        SDL_GetMouseState(&Mouse.x, &Mouse.y);
        //Render background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        if(InMenu){
            MainMenu();
        }
        else{
            //Render gameplay
            RenderArrow();
            RenderMap();
            RenderEnemy();
            RenderPlayer();
            RenderHP();
            if(IsPaused){
                PauseMenu();
            }
            else if(GameOverDelay==64){
                GameOverMenu();
            }
            else{
                UpdateCamera();
                UpdateEnemy();
                UpdatePlayer();
                UpdateArrow();
            }
        }
        Mouse.left=false;
        Mouse.right=false;
        RenderMouse();
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}
//Close game and clean up
void CloseGame() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    PlayerSave.close();
    cout<<"Game closed";
}

