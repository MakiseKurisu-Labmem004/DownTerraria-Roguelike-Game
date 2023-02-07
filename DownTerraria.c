#include "DownTerraria.h"

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048);
    TTF_Init();
    SDL_StopTextInput();
    srand((unsigned)time(NULL));
    LoadResource();
    Window = SDL_CreateWindow("DownTerraria", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 720, SDL_WINDOW_SHOWN);
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    Mix_PlayMusic(BGM[1], -1);
    while (1)
    {
        if (!GameOver)
            SDL_RenderClear(Renderer);
        clock_t StartTime = clock();
        while (SDL_PollEvent(&MainEvent))
        {
            if (!GameOver)
            {
                switch (MainEvent.type)
                {
                case SDL_KEYDOWN:
                    switch (MainEvent.key.keysym.sym)
                    {
                    case SDLK_d:
                    case SDLK_RIGHT: // 按下右方向
                        if ((!Judge_Start) || (!Judge_Preparing))
                        {
                            Judge_Right = true;
                            Judge_Left = false;
                            Look_To_R = 'R';
                        }
                        break;
                    case SDLK_a:
                    case SDLK_LEFT: // 按下左方向
                        if ((!Judge_Start) || (!Judge_Preparing))
                        {
                            Judge_Left = true;
                            Judge_Right = false;
                            Look_To_R = 'L';
                        }
                        break;
                    case SDLK_SPACE: // 按下空格
                        if ((!Judge_Start) || (!Judge_Preparing))
                        {
                            if (Must_Hit_Space && On_Land)
                            {
                                if (!Judge_Jump)
                                {
                                    Mix_PlayChannel(-1, music1, 0);
                                }
                                Judge_Jump = true;
                                Must_Hit_Space = false;
                            }
                            if ((Have_Hit_Once || (!On_Land)) && Jump_Speed <= 30)
                            {
                                if (Bullet_Consume != Bullet_Num)
                                    Attacking = true;
                                else
                                {
                                    if (EmptyAppear)
                                    {
                                        Mix_PlayChannel(-1, music5, 0);
                                        EmptyAppear = false;
                                    }
                                    Have_Hit_Once = false;
                                }
                            }
                        }
                        break;
                    case SDLK_e:
                        if (HaveRob && ClickOnce)
                            HaveChat = true;
                        if (CanInteract && (!Interacting) && ClickOnce)
                        {
                            Interacting = true;
                            ClickOnce = false;
                        }
                        else if (Interacting && ClickOnce)
                        {
                            Interacting = false;
                            ClickOnce = false;
                        }
                        break;
                    default:
                        break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (MainEvent.key.keysym.sym)
                    {
                    case SDLK_d:
                    case SDLK_RIGHT: // 松开右方向
                        if ((!Judge_Start) || (!Judge_Preparing))
                            Judge_Right = false;
                        break;
                    case SDLK_a:
                    case SDLK_LEFT: // 松开左方向
                        if ((!Judge_Start) || (!Judge_Preparing))
                            Judge_Left = false;
                        break;
                    case SDLK_SPACE:
                        if ((!Judge_Start) || (!Judge_Preparing))
                        {
                            if (Judge_Jump && (!Have_Hit_Once))
                                Have_Hit_Once = true;
                            Must_Hit_Space = true;
                            if (Attacking)
                                Attacking = false;
                            EmptyAppear = true;
                        }
                        break;
                    case SDLK_e:
                        ClickOnce = true;
                        break;
                    default:
                        break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (Buying || Choosing)
                    {
                        if (MainEvent.button.y > 100 && MainEvent.button.y < 235)
                        {
                            if (MainEvent.button.x < 270 && MainEvent.button.x > 170)
                                ItemIndex = 1;
                            else if (MainEvent.button.x > 300 && MainEvent.button.x < 400)
                                ItemIndex = 2;
                            else if (MainEvent.button.x > 430 && MainEvent.button.x < 530)
                                ItemIndex = 3;
                        }
                    }
                    break;
                case SDL_QUIT: // 相当于按窗口的X键
                    QUIT();
                    return 0;
                default:
                    break;
                }
            }
            else
            {
                switch (MainEvent.type)
                {
                case SDL_MOUSEBUTTONDOWN:
                    if (420 < MainEvent.button.x && 520 > MainEvent.button.x && MainEvent.button.y > 405 && MainEvent.button.y < 450)
                        ReTry();
                    if (420 < MainEvent.button.x && 520 > MainEvent.button.x && MainEvent.button.y > 465 && MainEvent.button.y < 510)
                    {
                        QUIT();
                        return 0;
                    }
                    break;
                case SDL_QUIT: // 相当于按窗口的X键
                    QUIT();
                    return 0;
                }
            }
        }
        if (!Judge_Start) // 如果没开始
        {
            FIRST_LOAD();
        }
        if (Judge_Start)
        {
            if (Judge_Preparing)
            {
                LOAD_PREPARE();
                if (!Level_Finish)
                    LOAD_LEVEL();
            }
            else
            {
                if (Start_BOSS)
                    LOAD_BOSS();
                else
                    LOAD_GAME();
            }
        }
        ControlFPS(StartTime);
        if (!Judge_Start)
            Judge_BulletOnLand(&Me_Bullet, &Stone);
        if (Judge_Start)
        {
            LOAD_ACCOUNT();
            LOAD_MONEY1();
            LOAD_MONEY2();
            LOAD_MONEY3();
            LOAD_MONEY4();
        }
        if (!GameOver)
        {
            COPY_OBJECT();
            MOVE_OBJECT();
        }
        LOAD_HP_CLIP();
        SDL_RenderPresent(Renderer);
    }
}

void FIRST_LOAD() // 加载主界面的行为
{
    Judge_Enter();
    Judge_BackChange();
    LOAD_FIRSTBACK();
    if ((Judge_Right || Judge_Left) && (!Judge_Jump))
        LOAD_RUN();
    else if (Judge_Jump)
        LOAD_ONSKY();
    else
        LOAD_STAND();
    if (Title_Appear)
    {
        if (!RedioOnce)
        {
            Mix_PlayChannel(-1, music6, 0);
            RedioOnce = true;
        }
        LOAD_TITLE();
    }
}

void LOAD_FIRSTBACK() // 加载初始界面
{
    SDL_Surface *BackSurface = IMG_Load("resource/BACK_FIRST.png");
    SDL_Texture *BackTexture = SDL_CreateTextureFromSurface(Renderer, BackSurface);
    if (Judge_BackMove() && (!Back_Changing))
    {
        if (Judge_Right) // 看人物面向哪个方向
        {
            Back_X -= Me_Speed;
            MoveNodePlus(&Me_Bullet, true, -Me_Speed);
            MoveNodePlus(&Me_BulletFire, true, -Me_Speed);
            if (Title_Appear)
                Title_X -= Me_Speed;
        }
        if (Judge_Left)
        {
            Back_X += Me_Speed;
            MoveNodePlus(&Me_Bullet, true, Me_Speed);
            MoveNodePlus(&Me_BulletFire, true, Me_Speed);
            if (Title_Appear)
                Title_X += Me_Speed;
        }
    }
    if (Back_Changing)
    {
        Back_Change(); // 只需改变Me_X和Back_X,注意改变Judge_BackLeft和Back_Changing
    }
    BackRect.x = Back_X;
    BackRect.y = Back_Y;
    BackRect.w = BackSurface->w;
    BackRect.h = BackSurface->h;
    SDL_RenderCopy(Renderer, BackTexture, NULL, &BackRect);
    SDL_FreeSurface(BackSurface);
    SDL_DestroyTexture(BackTexture);
}

void LOAD_GAME()
{
    if (!GameOver)
    {
        Init_X = Me_X;
        Init_Y = Me_Y;
        LOAD_BACK1();
        LOAD_BACK2();
        LOAD_BACK3();
        PrintList(&Finalline, FinallineSurface);
        PrintList(&Finalline2, Finalline2Surface);
        if (Buff[8])
        {
            Judge_BulletOnEnemy(&Star, &Enemy);
            Judge_BulletOnEnemy(&Star, &Enemy2);
            Judge_BulletOnEnemy(&Star, &Enemy3);
            Judge_BulletOnEnemy(&Star, &Enemy4);
        }
        Judge_BulletOnEnemy(&Me_Bullet, &Enemy);
        Judge_BulletOnEnemy(&Me_Bullet, &Enemy2);
        Judge_BulletOnEnemy(&Me_Bullet, &Enemy3);
        Judge_BulletOnEnemy(&Me_Bullet, &Enemy4);
        Judge_BulletOnLand(&Me_Bullet, &Stone);
        MoveNode(&RandomBuff);
        Move_Land_Node(&Land, DISTANCE);
        Move_Land_Node(&Stone, DISTANCE);
        Move_Fire_Node(&Me_BulletFire, DISTANCE);
        Move_Fire_Node(&Die, DISTANCE);
        Move_Enemy_Node(&Enemy, DISTANCE);
        Move_Enemy_Node(&Enemy2, DISTANCE);
        Move_Enemy_Node(&Enemy3, DISTANCE);
        Move_Enemy_Node(&Enemy4, DISTANCE);
        Move_Enemy_Bullet(&EnemyBullet, DISTANCE);
        Move_Fire_Node(&Finalline, DISTANCE);
        Move_Fire_Node(&Finalline2, DISTANCE);
        Move_Enemy_Node(&NPC, DISTANCE);
        Move_Enemy_Node(&Money, DISTANCE);
        Collect_Money(&Money);
        Judge_MeOnEnemy(&Enemy);
        Judge_MeOnEnemy(&Enemy2);
        Judge_MeOnEnemy(&Enemy3);
        Judge_MeOnEnemy(&Enemy4);
        Judge_MeOnEnemy(&EnemyBullet);
        DISTANCE = 0;
        LOAD_ME();
        if (modify)
        {
            LOAD_BACK1();
            LOAD_BACK2();
            LOAD_BACK3();
            MoveNode(&RandomBuff);
            Move_Land_Node(&Land, DISTANCE);
            Move_Land_Node(&Stone, DISTANCE);
            Move_Fire_Node(&Me_BulletFire, DISTANCE);
            Move_Fire_Node(&Die, DISTANCE);
            Move_Fire_Node(&Finalline, DISTANCE);
            Move_Fire_Node(&Finalline2, DISTANCE);
            Move_Enemy_Node(&Enemy, DISTANCE);
            Move_Enemy_Node(&Enemy2, DISTANCE);
            Move_Enemy_Node(&Enemy3, DISTANCE);
            Move_Enemy_Bullet(&EnemyBullet, DISTANCE);
            Move_Enemy_Node(&Enemy4, DISTANCE);
            Move_Enemy_Node(&NPC, DISTANCE);
            Move_Enemy_Node(&Money, DISTANCE);
            modify = false;
            DISTANCE = 0;
            LOAD_ME();
        }
        WhichNPC = Judge_CanInteract(&NPC);
        if (!CanInteract)
        {
            ClickOnce = true;
            Interacting = false;
            WhichNPC = 0;
        }
        if (Interacting)
        {
            LOAD_INTERACTION();
            Judge_Buy();
        }
        Judge_EnterNextLevel();
        if (!biu)
            LOAD_BIU();
        if (InvicibleTime == 0 && BeingInvicible)
            BeingInvicible = false;
        if (InvicibleTime > 0)
            BeingInvicible = true;
        if (InvicibleTime > 0 && BeingInvicible)
            InvicibleTime -= 1;
    }
    JudgeGameOver();
}

void LOAD_BOSS()
{
    if (!GameOver)
    {
        Init_X = Me_X;
        Init_Y = Me_Y;
        LOAD_BACK3();
        if (Buff[8])
        {
            Judge_BulletOnEnemy(&Star, &Boss);
            Judge_BulletOnEnemy(&Star, &Enemy2);
            Judge_BulletOnEnemy(&Star, &Enemy3);
        }
        Move_Land_Node(&Stone, DISTANCE);
        Move_Enemy_Bullet(&EnemyBullet, DISTANCE); // 激光
        Move_Enemy_Bullet(&Enemy3, DISTANCE);      // 血蛭
        Move_Enemy_Node(&Money, DISTANCE);
        Move_Enemy_Node(&Enemy2, DISTANCE); // 饿鬼
        Move_BossItem(&Enemy, DISTANCE);
        Move_Fire_Node(&Me_BulletFire, DISTANCE);
        Move_BossItem(&BossEye, DISTANCE);
        Move_Boss(&Boss, DISTANCE);
        Collect_Money(&Money);
        Judge_BulletOnLand(&Me_Bullet, &Stone);
        Judge_MeOnEnemy(&Enemy);
        Judge_MeOnEnemy(&Enemy3);
        Judge_MeOnEnemy(&EnemyBullet);
        Judge_MeOnEnemy(&Enemy2);
        Judge_MeOnEnemy(&Boss);
        Judge_BulletOnEnemy(&Me_Bullet, &Boss);
        Judge_BulletOnEnemy(&Me_Bullet, &Enemy2);
        Judge_BulletOnEnemy(&Me_Bullet, &Enemy3);
        DISTANCE = 0;
        LOAD_ME();
        if (modify)
        {
            LOAD_BACK3();
            Move_Enemy_Bullet(&EnemyBullet, DISTANCE);
            Move_Enemy_Node(&Money, DISTANCE);
            Move_Fire_Node(&Me_BulletFire, DISTANCE);
            modify = false;
            DISTANCE = 0;
            LOAD_ME();
        }
        if (InvicibleTime == 0 && BeingInvicible)
            BeingInvicible = false;
        if (InvicibleTime > 0)
            BeingInvicible = true;
        if (InvicibleTime > 0 && BeingInvicible)
            InvicibleTime -= 1;
    }
    JudgeGameOver();
}

void JudgeGameOver()
{
    if (Me_HP <= 0 || BOSS_BLOOD <= 0)
    {
        if (!GameOver && BOSS_BLOOD <= 0)
            Mix_PlayChannel(-1, music28, 0);
        if (!GameOver)
            OverRand = rand() % 7;
        GameOver = true;
        LOAD_SCORE();
    }
}

void ReTry()
{
    RemoveNode(&Me_Bullet);
    RemoveNode(&Star);
    RemoveNode(&Me_BulletFire);
    RemoveNode(&Die);
    RemoveNode(&Empty);
    RemoveNode(&Finalline);
    RemoveNode(&Finalline2);
    RemoveNode(&RandomBuff);
    Remove_Land_Node(&Stone);
    Remove_Land_Node(&Land);
    Remove_Enemy_Node(&Enemy);
    Remove_Enemy_Node(&Enemy2);
    Remove_Enemy_Node(&Enemy3);
    Remove_Enemy_Node(&Enemy4);
    Remove_Enemy_Node(&Money);
    Remove_Enemy_Node(&NPC);
    Remove_Enemy_Node(&EnemyBullet);
    Remove_Enemy_Node(&Boss);
    Remove_Enemy_Node(&BossEye);
    Mix_HaltMusic();
    Mix_PlayMusic(BGM[1], -1);
    CurrentLevel = 1;
    SDL_FreeSurface(PlatSurface[1]);
    sprintf_s(FileName, 30, "resource/PLATFORM%d.png", 1);
    PlatSurface[1] = IMG_Load(FileName);
    for (int i = 1; i < 3; i += 1)
    {
        SDL_FreeSurface(LandSurface[i]);
        SDL_FreeSurface(Enemy1LSurface[i]);
        SDL_FreeSurface(Enemy1RSurface[i]);
        SDL_FreeSurface(Enemy2LSurface[i]);
        SDL_FreeSurface(Enemy2RSurface[i]);
        SDL_FreeSurface(Enemy3Surface[i]);
        sprintf_s(FileName, 30, "resource/STONE%d.png", i);
        LandSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_1_%d_L.png", CurrentLevel, i);
        Enemy1LSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_1_%d_R.png", CurrentLevel, i);
        Enemy1RSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_2_%d_L.png", CurrentLevel, i);
        Enemy2LSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_2_%d_R.png", CurrentLevel, i);
        Enemy2RSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_3_%d.png", CurrentLevel, i);
        Enemy3Surface[i] = IMG_Load(FileName);
        SDL_FreeSurface(Enemy1LSurface[3]);
        SDL_FreeSurface(Enemy1RSurface[3]);
        SDL_FreeSurface(Enemy3Surface[3]);
        sprintf_s(FileName, 30, "resource/Enemy%d_1_%d_FL.png", CurrentLevel, 1);
        Enemy1LSurface[3] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_1_%d_FR.png", CurrentLevel, 1);
        Enemy1RSurface[3] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_3_%d.png", CurrentLevel, 1);
        Enemy3Surface[3] = IMG_Load(FileName);
    }
    GameOver = false;
    Judge_Start = true;
    Judge_Preparing = true;
    Level_Finish = false;
    BeingInvicible = false;
    MaxInvicibleTime = 20;
    InvicibleTime = 0;
    DISTANCE = 0;
    Me_HP = 4;
    InitLand_Y = 720;
    NPCLastRand = -1;
    WhichNPC = 0;
    ClickOnce = true;
    HaveChat = false;
    ItemIndex = 0;
    HaveRob = false;
    Buying = false;
    Choosing = false;
    HaveItems = false;
    CanInteract = false;
    Interacting = false;
    Judge_Right = false;
    Judge_Left = false;
    Hit_Twice = false;
    Entering_NextLevel = false;
    Entering_NextLevel2 = false;
    RedioOnce = false;
    EmptyAppear = true;
    biu = false;
    NextJudge = false;
    modify = false;
    StartMove = false;
    StoreHeart = false;
    BOSS_BLOOD = 4000;
    Start_BOSS = false;
    Drop_Distance = 0;
    Body_Distance = 200;
    Eye_Distance1 = 0;
    Eye_Distance2 = 300;
    Last_Random_Buff = 0;
    biu_Which = 10;
    Stand_Which = 1;
    StandDelay = 0;
    Me_X = 300;
    Back_X = 0;
    Back_Y = -900;
    Back_1_Y = -400;
    Back_3_Y = 1000;
    Jump_Speed = 30;
    Attacking = false;
    Have_Hit_Once = false;
    Must_Hit_Space = true;
    Back_Pre1_Y = 0;
    On_Land = false;
    Bullet_Consume = 0;
    PreRound = 5;
    Level_Which = 1;
    Level_Add = 1;
    BulletDistance = 250;
    BulletDamage = 10;
    Cu = 0;
    Ag = 0;
    Au = 0;
    Pt = 0;
    for (int i = 0; i < 13; i++)
        Buff[i] = false;
}

void LOAD_SCORE()
{
    SDL_Surface *GradeSurface = IMG_Load("resource/SCORE.png");
    SDL_Texture *GradeTexture = SDL_CreateTextureFromSurface(Renderer, GradeSurface);
    GradeRect.x = 100;
    GradeRect.y = 100;
    GradeRect.w = GradeSurface->w;
    GradeRect.h = GradeSurface->h;
    SDL_RenderCopy(Renderer, GradeTexture, NULL, &GradeRect);
    SDL_FreeSurface(GradeSurface);
    SDL_DestroyTexture(GradeTexture);
    SDL_Texture *ScoreTexture = NULL;
    SDL_Surface *ScoreSurface = TTF_RenderUTF8_Blended(ScoreFont, "SCORE>>>", MoneyColor);
    ScoreTexture = SDL_CreateTextureFromSurface(Renderer, ScoreSurface);
    ScoreRect.x = 120;
    ScoreRect.y = 120;
    ScoreRect.w = ScoreSurface->w;
    ScoreRect.h = ScoreSurface->h;
    SDL_RenderCopy(Renderer, ScoreTexture, NULL, &ScoreRect);
    SDL_DestroyTexture(ScoreTexture);
    SDL_FreeSurface(ScoreSurface);
    sprintf(FileName, "%d", 10000 * Pt + 100 * Au + Ag + (CurrentLevel - 1) * 1145 + (BOSS_LIFE - BOSS_BLOOD));
    SDL_Surface *ScoreNumSurface = TTF_RenderUTF8_Blended(ScoreFont, FileName, MoneyColor);
    SDL_Texture *ScoreNumTexture = SDL_CreateTextureFromSurface(Renderer, ScoreNumSurface);
    ScoreNumRect.x = 330;
    ScoreNumRect.y = 120;
    ScoreNumRect.w = ScoreNumSurface->w;
    ScoreNumRect.h = ScoreNumSurface->h;
    SDL_RenderCopy(Renderer, ScoreNumTexture, NULL, &ScoreNumRect);
    SDL_FreeSurface(ScoreNumSurface);
    SDL_DestroyTexture(ScoreNumTexture);
    SDL_Texture *RetryTexture = NULL;
    SDL_Surface *RetrySurface = TTF_RenderUTF8_Blended(ScoreFont, "< Retry >", MoneyColor);
    RetryTexture = SDL_CreateTextureFromSurface(Renderer, RetrySurface);
    RetryRect.x = 400;
    RetryRect.y = 400;
    RetryRect.w = RetrySurface->w;
    RetryRect.h = RetrySurface->h;
    SDL_RenderCopy(Renderer, RetryTexture, NULL, &RetryRect);
    SDL_DestroyTexture(RetryTexture);
    SDL_FreeSurface(RetrySurface);
    SDL_Texture *ExitTexture = NULL;
    SDL_Surface *ExitSurface = TTF_RenderUTF8_Blended(ScoreFont, "< Exit >", MoneyColor);
    ExitTexture = SDL_CreateTextureFromSurface(Renderer, ExitSurface);
    ExitRect.x = 400;
    ExitRect.y = 460;
    ExitRect.w = ExitSurface->w;
    ExitRect.h = ExitSurface->h;
    SDL_RenderCopy(Renderer, ExitTexture, NULL, &ExitRect);
    SDL_DestroyTexture(ExitTexture);
    SDL_FreeSurface(ExitSurface);
    SDL_Texture *OverTexture = NULL;
    SDL_Surface *OverSurface = NULL;
    if (BOSS_BLOOD > 0)
        switch (OverRand)
        {
        case 0:
            OverSurface = IMG_Load("resource/OVER1.png");
            break;
        case 1:
            OverSurface = IMG_Load("resource/OVER2.png");
            break;
        case 2:
            OverSurface = IMG_Load("resource/OVER3.png");
            break;
        case 3:
            OverSurface = IMG_Load("resource/OVER4.png");
            break;
        case 4:
            OverSurface = IMG_Load("resource/OVER5.png");
            break;
        case 5:
            OverSurface = IMG_Load("resource/OVER6.png");
            break;
        case 6:
            OverSurface = IMG_Load("resource/OVER7.png");
            break;
        default:
            break;
        }
    else
        OverSurface = IMG_Load("resource/WIN.png");
    OverTexture = SDL_CreateTextureFromSurface(Renderer, OverSurface);
    OverRect.x = 120;
    OverRect.y = 660;
    OverRect.w = OverSurface->w;
    OverRect.h = OverSurface->h;
    SDL_RenderCopy(Renderer, OverTexture, NULL, &OverRect);
    SDL_DestroyTexture(OverTexture);
    SDL_FreeSurface(OverSurface);
}

void LOAD_ME()
{
    if ((Judge_Right || Judge_Left) && (!Judge_Jump))
        LOAD_RUN(); // 注意改
    else if (Judge_Jump)
        LOAD_ONSKY(); // 注意改
    else
        LOAD_STAND();
}

void Judge_EnterNextLevel()
{ // X
    if (((!Entering_NextLevel) && Finalline == NULL) || (StartMove && (!Entering_NextLevel2) && Finalline2 == NULL))
    {
        if (CurrentLevel < 3)
        {
            RemoveTime = true;
            if (!Entering_NextLevel)
                Entering_NextLevel = true;
            else
                Entering_NextLevel2 = true;
            Choosing = true;
            CurrentLevel += 1;
            RemoveNode(&Me_Bullet);
            RemoveNode(&Star);
            RemoveNode(&Me_BulletFire);
            RemoveNode(&Die);
            RemoveNode(&Empty);
            RemoveNode(&Finalline);
            Remove_Land_Node(&Stone);
            Remove_Land_Node(&Land);
            Remove_Enemy_Node(&Enemy);
            Remove_Enemy_Node(&Enemy2);
            Remove_Enemy_Node(&Enemy3);
            Remove_Enemy_Node(&Money);
            Remove_Enemy_Node(&NPC);
            RemoveTime = false;
            HaveItems = false;
            ItemsOnSale = NULL;
            InitLand_Y = 6000;
            if (CurrentLevel == 2)
                LoadResource2();
            else
                LoadResource3();
            Mix_HaltMusic();
            if (CurrentLevel == 2)
                Mix_PlayMusic(BGM[2], -1);
            if (CurrentLevel == 3)
                Mix_PlayMusic(BGM[3], -1);
        }
    }
    else if ((Entering_NextLevel || Entering_NextLevel2) && Choosing)
    {
        LOAD_RANDOM_ITEM();
        Judge_Buy();
    }
}
void LOAD_BACK1()
{
    if (Finalline != NULL && CurrentLevel == 1)
    {
        if (Finalline->ObjectRect.y > 0)
        {
            SDL_Surface *BackSurface = IMG_Load("resource/BACK_1.png");
            Back_1_Y += DISTANCE;
            SDL_Texture *BackTexture = SDL_CreateTextureFromSurface(Renderer, BackSurface);
            BackRect.x = 0;
            BackRect.y = Back_1_Y;
            BackRect.w = BackSurface->w;
            BackRect.h = BackSurface->h;
            SDL_RenderCopy(Renderer, BackTexture, NULL, &BackRect);
            SDL_DestroyTexture(BackTexture);
            SDL_FreeSurface(BackSurface);
            if (Back_1_Y <= -1840)
                Back_1_Y = -400;
        }
    }
}
void LOAD_BACK2()
{
    if (CurrentLevel <= 2 && Back_3_Y > 720)
    {
        Back_2_Y += DISTANCE;
        if (Back_2_Y <= 720)
        {
            SDL_Surface *BackSurface = IMG_Load("resource/BACK_2.png");
            SDL_Texture *BackTexture = SDL_CreateTextureFromSurface(Renderer, BackSurface);
            Back2Rect.x = 0;
            Back2Rect.y = Back_2_Y;
            Back2Rect.w = BackSurface->w;
            Back2Rect.h = BackSurface->h;
            SDL_RenderCopy(Renderer, BackTexture, NULL, &Back2Rect);
            SDL_DestroyTexture(BackTexture);
            SDL_FreeSurface(BackSurface);
            if (Back_2_Y <= -1840)
                Back_2_Y = -400;
        }
        if (!Level_Finish)
            LOAD_LEVEL();
    }
}
void LOAD_BACK3()
{
    if (CurrentLevel >= 2 && StartMove)
    {
        Back_3_Y += DISTANCE;
        if (Back_3_Y <= 720)
        {
            SDL_Surface *BackSurface = IMG_Load("resource/BACK_3.png");
            SDL_Texture *BackTexture = SDL_CreateTextureFromSurface(Renderer, BackSurface);
            Back3Rect.x = 0;
            Back3Rect.y = Back_3_Y;
            Back3Rect.w = BackSurface->w;
            Back3Rect.h = BackSurface->h;
            SDL_RenderCopy(Renderer, BackTexture, NULL, &Back3Rect);
            SDL_DestroyTexture(BackTexture);
            SDL_FreeSurface(BackSurface);
            if (Back_3_Y <= -1840)
                Back_3_Y = -400;
        }
        if (!Level_Finish)
            LOAD_LEVEL();
    }
}

bool Judge_Drop(LAND **List)
{
    LAND *Now = *List;
    while (Now != NULL)
    {
        if (Now->LandRect.y < 400)
            if (SDL_PointInRect(&Point_1, &(Now->LandRect)) || SDL_PointInRect(&Point_2, &(Now->LandRect)))
            {
                if (Judge_Jump)
                    Now->OnIt = true; // 调整过后重设为false
                return false;
            }
        Now = Now->Next;
    }
    return true;
}
bool Return_Init_X(LAND **List)
{
    LAND *Now = *List;
    while (Now != NULL)
    {
        if (Now->LandRect.y < 400)
            if (SDL_PointInRect(&Point_L, &(Now->LandRect)) || SDL_PointInRect(&Point_R, &(Now->LandRect)) || SDL_PointInRect(&Point_LL, &(Now->LandRect)) || SDL_PointInRect(&Point_RR, &(Now->LandRect)))
                return true;
        Now = Now->Next;
    }
    return false;
}

bool Modify_DISTANCE()
{
    for (LAND *i = Land; i != NULL; i = i->Next)
    {
        if (i->OnIt)
        {
            DISTANCE = (300 - i->LandRect.y);
            i->OnIt = false;
            return true;
        }
    }
    for (LAND *i = Stone; i != NULL; i = i->Next)
    {
        if (i->OnIt)
        {
            DISTANCE = (300 - i->LandRect.y);
            i->OnIt = false;
            return true;
        }
    }
    return false;
}

void LOAD_BIU()
{
    SDL_Texture *BiuTexture = NULL;
    sprintf_s(FileName, 30, "resource/BIU%d.png", biu_Which);
    SDL_Surface *BiuSurface = IMG_Load(FileName);
    BiuTexture = SDL_CreateTextureFromSurface(Renderer, BiuSurface);
    BiuRect.x = Me_X - 5;
    BiuRect.y = 0;
    BiuRect.w = BiuSurface->w;
    BiuRect.h = BiuSurface->h;
    SDL_RenderCopy(Renderer, BiuTexture, NULL, &BiuRect);
    SDL_DestroyTexture(BiuTexture);
    SDL_FreeSurface(BiuSurface);
    if (biu_Which != 1)
        biu_Which -= 1;
    else
        biu = true;
}

void UpdatePoint() // RUN的判定
{
    Point_1.x = Me_X + 5;
    Point_1.y = Me_Y + 45;
    Point_2.x = Me_X + 25;
    Point_2.y = Me_Y + 45;
    Point_L.x = Me_X;
    Point_L.y = Me_Y + 35;
    Point_R.x = Me_X + 25;
    Point_R.y = Me_Y + 35;
    Point_LL.x = Me_X;
    Point_LL.y = Me_Y + 5;
    Point_RR.x = Me_X + 35;
    Point_RR.y = Me_Y + 5;
    Point_D.x = Me_X + 10;
    Point_D.y = Me_Y + 45;
}
void UpdatePointJ() // JUMP的判定,上升过程中不进行该判定
{
    Point_1.x = Me_X + 5;
    Point_1.y = Me_Y + 45;
    Point_2.x = Me_X + 25;
    Point_2.y = Me_Y + 45;
    Point_L.x = Me_X - 5;
    Point_L.y = Me_Y + 35;
    Point_R.x = Me_X + 45;
    Point_R.y = Me_Y + 35;
    Point_LL.x = Me_X - 5;
    Point_LL.y = Me_Y + 5;
    Point_RR.x = Me_X + 45;
    Point_RR.y = Me_Y + 5;
    Point_D.x = Me_X + 10;
    Point_D.y = Me_Y + 45;
    Point_DD.x = Me_X;
    Point_DD.y = Me_Y + 35;
    Point_DDD.x = Me_X + 40;
    Point_DDD.y = Me_Y + 45;
}

void LOAD_RUN() // 加载跑姿态
{
    SDL_Texture *MeTexture = NULL;
    if (BeingInvicible)
        sprintf_s(FileName, 30, "resource/RUN_%c_%dV.png", Look_To_R, Run_Which);
    else
        sprintf_s(FileName, 30, "resource/RUN_%c_%d.png", Look_To_R, Run_Which);
    SDL_Surface *MeSurface = IMG_Load(FileName);
    if (((!Judge_BackMove()) && (!Back_Changing)) || (Judge_Start))
    {
        if (Look_To_R == 'R') // 看人物面向哪个方向
            Me_X += Me_Speed;
        else
            Me_X -= Me_Speed;
        if (Judge_Start)
        {
            if (Me_X < 100)
                Me_X = 100;
            if (Me_X > 560)
                Me_X = 560;
            UpdatePoint();
        }
    }
    if (Me_X < 0)
        Me_X = 0;
    if ((!Judge_Start) && (!Judge_BackLeft) && Me_X > 100 && (!Back_Changing))
    {
        Judge_Jump = true;
        On_Land = false;
        Jump_Which = 3;
        Jump_Speed = 0;
    }
    if (Judge_Start && (Return_Init_X(&Land) || Return_Init_X(&Stone)))
        Me_X = Init_X;
    if (Judge_Start && Judge_Drop(&Land) && Judge_Drop(&Stone))
    {
        Judge_Jump = true;
        On_Land = false;
        Jump_Which = 3;
        Jump_Speed = 0;
    }
    MeTexture = SDL_CreateTextureFromSurface(Renderer, MeSurface);
    if (Run_Which == 2 || Run_Which == 4 || Run_Which == 7)
        MeRect.y = Me_Y + 2;
    else
        MeRect.y = Me_Y + 5;
    MeRect.x = Me_X;
    MeRect.w = MeSurface->w;
    MeRect.h = MeSurface->h;
    SDL_RenderCopy(Renderer, MeTexture, NULL, &MeRect);
    SDL_DestroyTexture(MeTexture);
    SDL_FreeSurface(MeSurface);
    if (Run_Delay == 2)
    {
        if (Run_Which == 10)
            Run_Which = 1;
        else
            Run_Which += 1;
        Run_Delay = 0;
    }
    else
        Run_Delay += 1;
    if (Run_M_Delay == 6)
    {
        Mix_PlayChannel(-1, music7, 0);
        Run_M_Delay = 0;
    }
    else
        Run_M_Delay += 1;
}

void LOAD_STAND() // 加载站立姿态
{
    SDL_Texture *MeTexture = NULL;
    if (BeingInvicible)
        sprintf_s(FileName, 30, "resource/STAND%d_%cV.png", Stand_Which, Look_To_R);
    else
        sprintf_s(FileName, 30, "resource/STAND%d_%c.png", Stand_Which, Look_To_R);
    SDL_Surface *MeSurface = IMG_Load(FileName);
    MeTexture = SDL_CreateTextureFromSurface(Renderer, MeSurface);
    MeRect.x = Me_X;
    if (Stand_Which == 2 || Stand_Which == 3 || Stand_Which == 4)
        MeRect.y = Me_Y;
    else
        MeRect.y = Me_Y + 5;
    MeRect.w = MeSurface->w;
    MeRect.h = MeSurface->h;
    SDL_RenderCopy(Renderer, MeTexture, NULL, &MeRect);
    SDL_DestroyTexture(MeTexture);
    SDL_FreeSurface(MeSurface);
    if (StandDelay == 4)
    {
        if (Stand_Which != 6)
            Stand_Which += 1;
        else
            Stand_Which = 1;
        StandDelay = 0;
    }
    else
        StandDelay += 1;
}

void LOAD_ONSKY() // 加载跳跃姿态
{
    SDL_Texture *MeTexture = NULL;
    SDL_Surface *MeSurface = NULL;
    if (!Attacking)
    {
        if (BeingInvicible)
            sprintf_s(FileName, 30, "resource/JUMP%d_%cV.png", Jump_Which, Look_To_R);
        else
            sprintf_s(FileName, 30, "resource/JUMP%d_%c.png", Jump_Which, Look_To_R);
        MeSurface = IMG_Load(FileName);
        MeTexture = SDL_CreateTextureFromSurface(Renderer, MeSurface);
        if (!Judge_Start)
            Me_Y -= Jump_Speed;
        else
            DISTANCE += Jump_Speed;
        if (Jump_Speed > -30)
            Jump_Speed -= Gravity;
        if (Jump_Delay == Jump_Delay_Limit[Jump_Which - 1])
        {
            if (Jump_Which != 4)
                Jump_Which += 1;
            Jump_Delay = 0;
        }
        else
            Jump_Delay += 1;
    }
    else
    {
        if (BeingInvicible)
            sprintf_s(FileName, 30, "resource/ATTACK%d_%cV.png", Attack_Which, Look_To_R);
        else
            sprintf_s(FileName, 30, "resource/ATTACK%d_%c.png", Attack_Which, Look_To_R);
        MeSurface = IMG_Load(FileName);
        MeTexture = SDL_CreateTextureFromSurface(Renderer, MeSurface);
        if (!Judge_Start)
            Me_Y -= Attack_Speed;
        else
            DISTANCE += Attack_Speed;
        Jump_Which = 4;
        Jump_Speed = Attack_Speed;
        if (Attack_Which == 1)
            LOAD_ATTACK();
        if (Attack_Delay == 4 && Attack_Which == 2)
        {
            Attack_Which = 1;
            Attack_Delay = 0;
        }
        else if (Attack_Delay == 1 && Attack_Which == 1)
            Attack_Which = 2;
        else
            Attack_Delay += 1;
        if (Bullet_Consume < Bullet_Num && Bullet_Delay == 4)
        {
            Mix_PlayChannel(-1, music3, 0);
            if (!Buff[11])
                Bullet_Consume += 1;
            else
            {
                int r = rand() % 3;
                if (r == 0)
                    Bullet_Consume += 1;
            }
            Bullet_Delay = 1;
            CreateBullet(Me_X + 7);
            if (Buff[10])
            {
                CreateBullet(Me_X - 23);
                CreateBullet(Me_X + 37);
            }
        }
        else if (Bullet_Consume < Bullet_Num && Bullet_Delay < 4)
            Bullet_Delay += 1;
        else if (Bullet_Consume == Bullet_Num)
        {
            Have_Hit_Once = false;
            Attacking = false;
            CreateEmpty();
        }
    }
    if (!Judge_Start)
    {
        if (!Judge_BackMove() && (!Back_Changing))
        {
            if (Judge_Right)
                Me_X += Me_Speed;
            if (Judge_Left)
                Me_X -= Me_Speed;
        }
        if (Me_X < 0)
            Me_X = 0;
        if (Me_Y > 390)
        {
            if (Me_X <= 100)
                Me_X = 100;
            if (Me_X > 560)
                Me_X = 560;
        }
        if ((Me_X < 100 && Me_Y >= 360 && (!Judge_BackLeft) && (!Back_Changing)) || (Back_Changing && Me_Y >= 360) || (Judge_BackLeft && Me_Y >= 360)) // 暂时的落地判定
        {
            if (Bullet_Consume != 0)
                Mix_PlayChannel(-1, music4, 0);
            else
                Mix_PlayChannel(-1, music2, 0);
            Me_Y = 360;
            Judge_Jump = false;
            Jump_Which = 1;
            Jump_Speed = 30;
            Bullet_Delay = 4;
            Jump_Delay = 0;
            Attack_Which = 1;
            Have_Hit_Once = false;
            Attack_Delay = 0;
            Attacking = false;
            On_Land = true;
            Bullet_Consume = 0;
        }
        if (Me_X >= 100 && Me_Y >= 750 && (!Judge_BackLeft)) // 暂时的落地判定
        {
            Me_Y = 750;
            Judge_Start = true; // 游戏开始
            Judge_Jump = false;
            Jump_Which = 1;
            Attack_Delay = 0;
            Attack_Which = 1;
            Judge_Left = false;
            Judge_Right = false;
            Jump_Speed = 30;
            Jump_Delay = 0;
            Judge_Preparing = true;
            Have_Hit_Once = false;
            Attacking = false;
        }
    }
    else
    {
        if (Judge_Right)
            Me_X += Me_Speed;
        if (Judge_Left)
            Me_X -= Me_Speed;
        if (Me_X <= 100)
            Me_X = 100;
        if (Me_X > 560)
            Me_X = 560;
        UpdatePoint();
        if (Return_Init_X(&Land) || Return_Init_X(&Stone))
            Me_X = Init_X;
        if (NextJudge)
        {
            UpdatePointJ();
            if (Jump_Speed <= 0 && (!Judge_Drop(&Land) || !Judge_Drop(&Stone)))
            {
                if (Bullet_Consume != 0)
                    Mix_PlayChannel(-1, music4, 0);
                else
                    Mix_PlayChannel(-1, music2, 0);
                DISTANCE = 0;
                Judge_Jump = false;
                Jump_Which = 1;
                Jump_Speed = 30;
                Jump_Delay = 0;
                Bullet_Delay = 4;
                Attack_Which = 1;
                NextJudge = false;
                Have_Hit_Once = false;
                Attack_Delay = 0;
                Attacking = false;
                On_Land = true;
                Bullet_Consume = 0;
                modify = Modify_DISTANCE();
            }
        }
        else
            NextJudge = true;
    }
    MeRect.x = Me_X;
    MeRect.y = Me_Y;
    MeRect.w = MeSurface->w;
    MeRect.h = MeSurface->h;
    SDL_RenderCopy(Renderer, MeTexture, NULL, &MeRect);
    SDL_DestroyTexture(MeTexture);
    SDL_FreeSurface(MeSurface);
}

void LOAD_ATTACK()
{
    SDL_Surface *AttackSurface = IMG_Load("resource/ATTACK.png");
    SDL_Texture *AttackTexture = NULL;
    AttackTexture = SDL_CreateTextureFromSurface(Renderer, AttackSurface);
    AttackRect.x = Me_X;
    AttackRect.y = Me_Y + 35;
    AttackRect.w = AttackSurface->w;
    AttackRect.h = AttackSurface->h;
    SDL_RenderCopy(Renderer, AttackTexture, NULL, &AttackRect);
    SDL_FreeSurface(AttackSurface);
    SDL_DestroyTexture(AttackTexture);
}

void LOAD_TITLE()
{

    SDL_Texture *TitleTexture = NULL;
    sprintf_s(FileName, 30, "resource/TITLE%d.png", Title_Which);
    SDL_Surface *TitleSurface = IMG_Load(FileName);
    TitleTexture = SDL_CreateTextureFromSurface(Renderer, TitleSurface);
    TitleRect.x = Title_X;
    TitleRect.y = Title_Y;
    TitleRect.w = TitleSurface->w;
    TitleRect.h = TitleSurface->h;
    SDL_RenderCopy(Renderer, TitleTexture, NULL, &TitleRect);
    SDL_FreeSurface(TitleSurface);
    SDL_DestroyTexture(TitleTexture);
    if (Title_Which != 9)
    {
        if (Title_Delay == 0)
        {
            if (Title_Delay != 9)
                Title_Which += 1;
            Title_Delay = 0;
        }
        else
            Title_Delay += 1;
    }
}

void LOAD_HP_CLIP()
{
    SDL_Texture *ClipTexture = NULL;
    sprintf_s(FileName, 30, "resource/CLIP%d.png", (Bullet_Num - Bullet_Consume));
    SDL_Surface *ClipSurface = IMG_Load(FileName);
    ClipTexture = SDL_CreateTextureFromSurface(Renderer, ClipSurface);
    ClipRect.x = 640;
    ClipRect.y = 100;
    ClipRect.w = 24;
    ClipRect.h = 288;
    SDL_RenderCopy(Renderer, ClipTexture, NULL, &ClipRect);
    SDL_DestroyTexture(ClipTexture);
    SDL_FreeSurface(ClipSurface);
    SDL_Texture *HpTexture = NULL;
    sprintf_s(FileName, 30, "resource/HP%d_%d.png", MAXHP, Me_HP);
    SDL_Surface *HpSurface = IMG_Load(FileName);
    HpTexture = SDL_CreateTextureFromSurface(Renderer, HpSurface);
    HpRect.x = 20;
    HpRect.y = 50;
    HpRect.w = HpSurface->w;
    HpRect.h = HpSurface->h;
    SDL_RenderCopy(Renderer, HpTexture, NULL, &HpRect);
    SDL_DestroyTexture(HpTexture);
    SDL_FreeSurface(HpSurface);
}
void LOAD_ACCOUNT()
{
    SDL_Texture *AccountTexture = NULL;
    SDL_Surface *AccountSurface = IMG_Load("resource/ACCOUNT.png");
    AccountTexture = SDL_CreateTextureFromSurface(Renderer, AccountSurface);
    AccountRect.x = 20;
    AccountRect.y = 150;
    AccountRect.w = AccountSurface->w;
    AccountRect.h = AccountSurface->h;
    SDL_RenderCopy(Renderer, AccountTexture, NULL, &AccountRect);
    SDL_FreeSurface(AccountSurface);
    SDL_DestroyTexture(AccountTexture);
}
void LOAD_MONEY1()
{
    sprintf(FileName, "%d", Cu);
    SDL_Surface *Num1Surface = TTF_RenderUTF8_Blended(MoneyFont, FileName, MoneyColor);
    SDL_Texture *MoneyTexture = SDL_CreateTextureFromSurface(Renderer, Num1Surface);
    Num1Rect.x = 50;
    Num1Rect.y = 250;
    Num1Rect.w = Num1Surface->w;
    Num1Rect.h = Num1Surface->h;
    SDL_RenderCopy(Renderer, MoneyTexture, NULL, &Num1Rect);
    SDL_FreeSurface(Num1Surface);
    SDL_DestroyTexture(MoneyTexture);
}
void LOAD_MONEY2()
{
    sprintf(FileName, "%d", Ag);
    SDL_Surface *Num2Surface = TTF_RenderUTF8_Blended(MoneyFont, FileName, MoneyColor);
    SDL_Texture *MoneyTexture = SDL_CreateTextureFromSurface(Renderer, Num2Surface);
    Num2Rect.x = 50;
    Num2Rect.y = 220;
    Num2Rect.w = Num2Surface->w;
    Num2Rect.h = Num2Surface->h;
    SDL_RenderCopy(Renderer, MoneyTexture, NULL, &Num2Rect);
    SDL_FreeSurface(Num2Surface);
    SDL_DestroyTexture(MoneyTexture);
}
void LOAD_MONEY3()
{
    sprintf(FileName, "%d", Au);
    SDL_Surface *Num3Surface = TTF_RenderUTF8_Blended(MoneyFont, FileName, MoneyColor);
    SDL_Texture *MoneyTexture = SDL_CreateTextureFromSurface(Renderer, Num3Surface);
    Num3Rect.x = 50;
    Num3Rect.y = 187;
    Num3Rect.w = Num3Surface->w;
    Num3Rect.h = Num3Surface->h;
    SDL_RenderCopy(Renderer, MoneyTexture, NULL, &Num3Rect);
    SDL_FreeSurface(Num3Surface);
    SDL_DestroyTexture(MoneyTexture);
}
void LOAD_MONEY4()
{
    sprintf(FileName, "%d", Pt);
    SDL_Surface *Num4Surface = TTF_RenderUTF8_Blended(MoneyFont, FileName, MoneyColor);
    SDL_Texture *MoneyTexture = SDL_CreateTextureFromSurface(Renderer, Num4Surface);
    Num4Rect.x = 50;
    Num4Rect.y = 152;
    Num4Rect.w = Num4Surface->w;
    Num4Rect.h = Num4Surface->h;
    SDL_RenderCopy(Renderer, MoneyTexture, NULL, &Num4Rect);
    SDL_FreeSurface(Num4Surface);
    SDL_DestroyTexture(MoneyTexture);
}

bool Judge_BackMove() // 判断主界面什么时候移动
{
    if (Judge_BackLeft)
    {
        if (Judge_Left && Back_X == 0)
            return false;
        else if (Judge_Right || Judge_Left)
            if (Me_X < 300)
                return false;
            else
                return true;
        else
            return false;
    }
    else
        return false;
}

void Judge_BackChange() // 判断主界面什么时候切换
{
    if (Judge_BackLeft)
    {
        if (Back_X <= -300 && Judge_Entering)
        {
            Judge_Entering = false;
            Back_Changing = true;
        }
    }
    else
    {
        if (Me_X <= 0 && Judge_Entering)
        {
            Back_Changing = true;
            Judge_Entering = false;
        }
    }
}

void Back_Change() // 主界面切换
{
    if (Judge_BackLeft)
    {
        if (Back_X > -600)
        {
            Back_X -= 30;
            MoveNodePlus(&Me_Bullet, true, -30);
            MoveNodePlus(&Me_BulletFire, true, -30);
            if (Title_Appear)
                Title_X -= 30;
        }
        if (Back_X <= -600)
        {
            MoveNodePlus(&Me_Bullet, true, -(Back_X + 600));
            MoveNodePlus(&Me_Bullet, true, -(Back_X + 600));
            Title_X = Title_X - (Back_X + 600);
            Back_X = -600;
        }
        if (!Judge_Right)
        {
            if (Me_X > 0)
                Me_X -= 30;
            if (Me_X <= 0)
                Me_X = 0;
        }
        if (Judge_Right)
        {
            Me_X -= 20;
        }
        if (Back_X == -600)
        {
            if (!Title_Appear)
                Title_Appear = true;
            Judge_BackLeft = false;
            Back_Changing = false;
            Judge_Entering = false;
        }
    }
    else
    {
        if (!Judge_Left)
        {
            Back_X += 30;
            MoveNodePlus(&Me_Bullet, true, 30);
            MoveNodePlus(&Me_BulletFire, true, 30);
            if (Title_Appear)
                Title_X += 30;
        }
        if (Judge_Left)
        {
            Back_X += 40;
            MoveNodePlus(&Me_Bullet, true, 40);
            MoveNodePlus(&Me_BulletFire, true, 40);
            if (Title_Appear)
                Title_X += 40;
        }
        if (Me_X < 300)
            Me_X += 30; // 想办法改成30
        if (Me_X >= 300)
            Me_X = 300;
        if (Me_X == 300)
        {
            Judge_BackLeft = true;
            Back_Changing = false;
            Judge_Entering = false;
        }
    }
}

void Judge_Enter() // 判定线
{
    if (Judge_BackLeft)
        if (Back_X == -300 && Judge_Right)
            Judge_Entering = true;
    if (!Judge_BackLeft)
        if (Me_X == 0 && Judge_Left)
            Judge_Entering = true;
}

void LOAD_PREPARE()
{
    if (Back_Y > -900)
    {
        Back_Y -= 30;
        Title_Y -= 30;
        LOAD_FIRSTBACK();
        LOAD_TITLE();
    }
    else
    {
        SDL_Surface *BackSurface = IMG_Load("resource/BACK_PRE1.png");
        SDL_Texture *BackTexture = SDL_CreateTextureFromSurface(Renderer, BackSurface);
        BackRect.x = 0;
        BackRect.y = Back_Pre1_Y;
        BackRect.w = BackSurface->w;
        BackRect.h = BackSurface->h;
        SDL_RenderCopy(Renderer, BackTexture, NULL, &BackRect);
        SDL_FreeSurface(BackSurface);
        SDL_DestroyTexture(BackTexture);
    }
    if (PreRound > 0)
    {
        Back_Pre1_Y -= 30;
        if (Back_Pre1_Y <= -1440)
        {
            PreRound -= 1;
            Back_Pre1_Y = 0;
        }
    }
    Move_Land_Node(&Land, -30);
    Move_Land_Node(&Stone, -30);
    if (PreRound == 1 && Back_Pre1_Y == -900)
    {
        if (rand() % 2 == 1)
            CreatePrepare1(840);
        else
            CreatePrepare2(840);
    }
    else if (PreRound == 0)
    {
        Judge_Jump = false;
        Jump_Which = 1;
        Jump_Speed = 30;
        Jump_Delay = 0;
        Attack_Which = 1;
        Have_Hit_Once = false;
        Attack_Delay = 0;
        Attacking = false;
        Bullet_Consume = 0;
        Judge_Preparing = false;
        Level_Finish = true;
        On_Land = true;
        Level_Add = 1;
        Level_Which = 1;
        Me_X = 320;
        Me_Y = 260;
        LOAD_RANDOM_BARRIAR();
        LOAD_RANDOM_ENEMY(&Stone, 15);
        LOAD_RANDOM_ENEMY(&Land, 10);
    }
}
void LOAD_LEVEL()
{
    SDL_Texture *LevelTexture = NULL;
    sprintf_s(FileName, 30, "resource/LEVEL%d_%d.png", CurrentLevel, Level_Which);
    SDL_Surface *LevelSurface = IMG_Load(FileName);
    LevelTexture = SDL_CreateTextureFromSurface(Renderer, LevelSurface);
    LevelRect.x = 180;
    LevelRect.y = 300;
    LevelRect.w = LevelSurface->w;
    LevelRect.h = LevelSurface->h;
    SDL_RenderCopy(Renderer, LevelTexture, NULL, &LevelRect);
    SDL_DestroyTexture(LevelTexture);
    SDL_FreeSurface(LevelSurface);
    if (Level_Which != 10)
    {
        if (Level_Delay >= 3)
        {
            Level_Which += Level_Add;
            Level_Delay = 0;
        }
        else
            Level_Delay += 1;
    }
    else
    {
        if (Level_Delay >= 90)
        {
            Level_Which += Level_Add;
            Level_Delay = 0;
        }
        else
            Level_Delay += 1;
    }
    if (Level_Which == 10)
        Level_Add = -1;
    if (Level_Which == 0)
        Level_Finish = true;
}
void LOAD_TIP(int x, int y)
{
    SDL_Texture *TipTexture = NULL;
    TipTexture = SDL_CreateTextureFromSurface(Renderer, TipSurface[1]);
    TipRect.x = x;
    TipRect.y = y;
    TipRect.w = TipSurface[1]->w;
    TipRect.h = TipSurface[1]->h;
    SDL_RenderCopy(Renderer, TipTexture, NULL, &TipRect);
    SDL_DestroyTexture(TipTexture);
}
void LOAD_CHAT1(int x, int y)
{
    SDL_Texture *TipTexture = NULL;
    TipTexture = SDL_CreateTextureFromSurface(Renderer, TipSurface[2]);
    TipRect.x = x;
    TipRect.y = y;
    TipRect.w = TipSurface[2]->w;
    TipRect.h = TipSurface[2]->h;
    SDL_RenderCopy(Renderer, TipTexture, NULL, &TipRect);
    SDL_DestroyTexture(TipTexture);
}
void LOAD_CHAT2(int x, int y)
{
    SDL_Texture *TipTexture = NULL;
    TipTexture = SDL_CreateTextureFromSurface(Renderer, TipSurface[3]);
    TipRect.x = x;
    TipRect.y = y;
    TipRect.w = TipSurface[3]->w;
    TipRect.h = TipSurface[3]->h;
    SDL_RenderCopy(Renderer, TipTexture, NULL, &TipRect);
    SDL_DestroyTexture(TipTexture);
}
void LOAD_CHAT3(int x, int y)
{
    SDL_Texture *TipTexture = NULL;
    TipTexture = SDL_CreateTextureFromSurface(Renderer, TipSurface[4]);
    TipRect.x = x;
    TipRect.y = y;
    TipRect.w = TipSurface[4]->w;
    TipRect.h = TipSurface[4]->h;
    SDL_RenderCopy(Renderer, TipTexture, NULL, &TipRect);
    SDL_DestroyTexture(TipTexture);
}
void LOAD_CHAT4(int x, int y)
{
    SDL_Texture *TipTexture = NULL;
    TipTexture = SDL_CreateTextureFromSurface(Renderer, TipSurface[5]);
    TipRect.x = x;
    TipRect.y = y;
    TipRect.w = TipSurface[5]->w;
    TipRect.h = TipSurface[5]->h;
    SDL_RenderCopy(Renderer, TipTexture, NULL, &TipRect);
    SDL_DestroyTexture(TipTexture);
}

void LOAD_INTERACTION()
{
    if (WhichNPC == 1 && (Pt * 1000000 + Au * 10000 + Ag * 100 + Cu) >= 5000)
    {
        Mix_PlayChannel(-1, music17, 0);
        RANDOM_BUFF();
        Modify_Account(0, 50, 0, false);
        Interacting = false;
    }
    else if (WhichNPC == 2)
    {
        if (!HaveChat)
        {
            LOAD_CHAT1(280, NPC->EnemyRect.y - 40);
            int au = rand() % 4 + 3;
            int ag = rand() % 100;
            int cu = rand() % 100;
            if (!HaveRob)
            {
                Mix_PlayChannel(-1, music23, 0);
                Modify_Account(au, ag, cu, true);
                HaveRob = true;
            }
        }
        else
            LOAD_CHAT2(280, NPC->EnemyRect.y - 40);
    }
    else if (WhichNPC == 3)
    {
        Start_BOSS = true;
        Mix_PlayChannel(-1, music11, 0);
        CreateDie(NPC->EnemyRect.x + 10, NPC->EnemyRect.y);
        Mix_PlayChannel(-1, music18, 0);
        LAND *Now = Stone;
        while (Now != NULL)
        {
            Now->BeDamaged = true;
            Now = Now->Next;
        }
        Mix_HaltMusic();
        Mix_PlayMusic(BGM[4], -1);
        Interacting = false;
        Enemy = NULL;
        Enemy2 = NULL;
        Enemy3 = NULL;
        EnemyBullet = NULL;
        Add_Enemy_Node(&Boss, Create_Enemy_Node(0, BOSS_LIFE, 1, 10, 0, 1, 1, 100, 720, false, false, false, 3, 262, 500));
        Add_Enemy_Node(&BossEye, Create_Enemy_Node(0, 1, 1, 10, 0, 1, 1, 110, 720, false, false, false, 3, 120, 120));
        Add_Enemy_Node(&BossEye, Create_Enemy_Node(0, 2, 1, 10, 0, 1, 1, 472, 720, false, false, false, 3, 120, 120));
    }
    else if (WhichNPC == 4)
    {
        LOAD_RANDOM_ITEM();
        Buying = true;
    }
}

void LOAD_RANDOM_ITEM() // 链表死循环
{
    int r = rand() % 12 + 1;
    int last = 0;
    int laslast = 0;
    int x = 170;
    if (!HaveItems)
    {
        ReLoadList();
        ItemsOnSale = NULL;
        while (Items[r]->BeBought)
        {
            r = rand() % 12 + 1;
        }
        last = r;
        Add_Item_Node(&ItemsOnSale, Items[r]);
        Items[r]->ItemRect.x = x;
        Items[r]->ItemRect.y = 100;
        Items[r]->Index = 1;
        while (r == last || (Items[r]->BeBought))
        {
            r = rand() % 12 + 1;
        }
        laslast = r;
        Items[r]->ItemRect.x = x + 130;
        Items[r]->ItemRect.y = 100;
        Items[r]->Index = 2;
        Add_Item_Node(&ItemsOnSale, Items[r]);
        while (r == last || r == laslast || (Items[r]->BeBought))
        {
            r = rand() % 12 + 1;
        }
        Items[r]->ItemRect.x = x + 260;
        Items[r]->ItemRect.y = 100;
        Items[r]->Index = 3;
        Add_Item_Node(&ItemsOnSale, Items[r]);
    }
    HaveItems = true;
}
void Judge_Buy()
{
    if ((Choosing || Buying) && ItemIndex != 0)
    {
        ITEM *Now = ItemsOnSale;
        for (int i = 1; i < ItemIndex; i++)
        {
            Now = Now->Next;
        }
        if (Buying && !Now->BeBought && (Pt * 1000000 + Au * 10000 + Ag * 100 + Cu) >= (Now->Au * 10000 + Now->Ag * 100 + Now->Cu))
        {
            Now->BeBought = true;
            ItemIndex = 0;
            StartBuff(Now->Which_Item);
            Mix_PlayChannel(-1, music23, 0);
            Modify_Account(Now->Au, Now->Ag, Now->Cu, false);
        }
        if (Choosing && !Now->BeBought)
        {
            Now->BeBought = true;
            StartBuff(Now->Which_Item);
            Choosing = false;
            Level_Finish = false;
            Level_Add = 1;
            Level_Which = 1;
            HaveItems = false;
            ItemIndex = 0;
            ItemsOnSale = NULL;
            Mix_PlayChannel(-1, music23, 0);
            if (rand() % 2 == 1)
                CreatePrepare1(5600);
            else
                CreatePrepare2(5600);
            LOAD_RANDOM_BARRIAR();
            LOAD_RANDOM_ENEMY(&Stone, 15);
            LOAD_RANDOM_ENEMY(&Land, 10);
        }
    }
}
void Modify_Account(int au, int ag, int cu, bool add)
{
    if (add)
    {
        Cu += cu;
        if (Cu >= 100)
        {
            Cu -= 100;
            Ag += 1;
        }
        Ag += ag;
        if (Ag >= 100)
        {
            Ag -= 100;
            Au += 1;
        }
        Au += au;
        if (Au >= 100)
        {
            Au -= 100;
            Pt += 1;
        }
    }
    else
    {
        Cu -= cu;
        if (Cu < 0)
        {
            Ag -= 1;
            Cu += 100;
        }
        Ag -= ag;
        if (Ag < 0)
        {
            Au -= 1;
            Ag += 100;
        }
        Au -= au;
        if (Au < 0)
        {
            Pt -= 1;
            Au += 100;
        }
    }
}
void StartBuff(int which)
{
    Buff[which] = true;
    switch (which)
    {
    case 2:
        BulletDistance *= 1.2;
        break;
    case 3:
        if (Me_HP < MAXHP)
            Me_HP += 1;
        break;
    case 4:
        BulletDamage += 5;
        break;
    case 9:
        MaxInvicibleTime *= 2;
        break;
    case 10:
        BulletDistance *= 0.75;
        break;
    default:
        break;
    }
}
void RANDOM_BUFF()
{
    if (Last_Random_Buff != 0)
    {
        if (Last_Random_Buff == 1)
            BulletDamage -= 5;
        if (Last_Random_Buff == 2)
            BulletDamage -= 3;
        if (Last_Random_Buff == 3)
            BulletDistance /= 1.3;
        if (Last_Random_Buff == 4)
        {
            BulletDamage -= 4;
            BulletDistance /= 1.2;
        }
        if (Last_Random_Buff == 5)
            BulletDistance /= 0.9;
        if (Last_Random_Buff == 6)
            BulletDamage += 2;
        if (Last_Random_Buff == 7)
        {
            BulletDamage += 1;
            BulletDistance /= 0.9;
        }
    }
    int r = rand() % 7 + 1;
    switch (r)
    {
    case 1: // 险恶
        BulletDamage += 5;
        break;
    case 2: // 愤怒
        BulletDamage += 3;
        break;
    case 3: // 巨大
        BulletDistance *= 1.3;
        break;
    case 4: // 虚幻
        BulletDistance *= 1.2;
        BulletDamage += 4;
        break;
    case 5: // 软弱
        BulletDistance *= 0.9;
        break;
    case 6: // 碎裂
        BulletDamage -= 2;
        break;
    case 7: // 可怕
        BulletDamage -= 1;
        BulletDistance *= 0.9;
        break;
    default:
        break;
    }
    Last_Random_Buff = r;
    OB *Now = CreateNode(1, -20, false, 0, -1, r, 230, 200, 30, false);
    Now->Alpha = 255;
    AddNode(&RandomBuff, Now);
}

void LOAD_RANDOM_BARRIAR() // 30个障碍
{
    int y = InitLand_Y;
    for (int i = 1; i <= 32; i++)
    {
        if (i != 11 && i != 23)
        {
            Rand = rand() % 14 + 1;
            if (LastRand == Rand)
                Rand += 1;
            if (Rand == 15)
                Rand = 1;
            LastRand = Rand;
            switch (Rand)
            {
            case 1:
                CreateStone1(y);
                y += 1000;
                break;
            case 2:
                CreateStone2(y);
                y += 900;
                break;
            case 3:
                CreateStone3(y);
                y += 1050;
                break;
            case 4:
                CreateStone4(y);
                y += 850;
                break;
            case 5:
                CreateStone5(y);
                y += 1000;
                break;
            case 6:
                CreateStone6(y);
                y += 1000;
                break;
            case 7:
                CreateStone7(y);
                y += 800;
                break;
            case 8:
                CreateStone8(y);
                y += 800;
                break;
            case 9:
                CreateStone9(y);
                y += 800;
                break;
            case 10:
                CreateStone10(y);
                y += 800;
                break;
            case 11:
                CreateStone11(y);
                y += 750;
                break;
            case 12:
                CreateStone12(y);
                y += 750;
                break;
            case 13:
                CreatePlat1(y);
                y += 900;
                break;
            case 14:
                CreatePlat2(y);
                y += 1000;
                break;
            default:
                break;
            }
        }
        else
        {
            y += 50;
            Add_Land_Node(&Stone, Create_Land_Node(150, y, 2, true, false, false, 2 * BulletDamage));
            Add_Land_Node(&Stone, Create_Land_Node(200, y, 1, false, false, false, 2 * BulletDamage));
            Add_Land_Node(&Stone, Create_Land_Node(250, y, 1, false, false, false, 2 * BulletDamage));
            Add_Land_Node(&Stone, Create_Land_Node(300, y, 1, false, false, false, 2 * BulletDamage));
            Add_Land_Node(&Stone, Create_Land_Node(350, y, 1, false, false, false, 2 * BulletDamage));
            Add_Land_Node(&Stone, Create_Land_Node(400, y, 2, true, false, false, 2 * BulletDamage));
            CreateNPC(250, y);
            y += 300;
        }
    }
    if (CurrentLevel == 1)
    {
        y += 600;
        CreateFinalline(y);
        Back_2_Y = y + 720;
    }
    if (CurrentLevel == 2)
    {
        y += 600;
        CreateFinalline2(y);
        Back_3_Y = y + 720;
        StartMove = true;
    }
    if (CurrentLevel == 3)
    {
        y += 800;
        Add_Land_Node(&Stone, Create_Land_Node(100, y, 2, false, false, false, 2 * BulletDamage));
        Add_Land_Node(&Stone, Create_Land_Node(150, y, 2, false, false, false, 2 * BulletDamage));
        Add_Land_Node(&Stone, Create_Land_Node(200, y, 2, false, false, false, 2 * BulletDamage));
        Add_Land_Node(&Stone, Create_Land_Node(250, y, 2, false, false, false, 2 * BulletDamage));
        Add_Land_Node(&Stone, Create_Land_Node(300, y, 2, false, false, false, 2 * BulletDamage));
        Add_Land_Node(&Stone, Create_Land_Node(350, y, 2, false, false, false, 2 * BulletDamage));
        Add_Land_Node(&Stone, Create_Land_Node(400, y, 2, false, false, false, 2 * BulletDamage));
        Add_Land_Node(&Stone, Create_Land_Node(450, y, 2, false, false, false, 2 * BulletDamage));
        Add_Land_Node(&Stone, Create_Land_Node(500, y, 2, false, false, false, 2 * BulletDamage));
        Add_Land_Node(&Stone, Create_Land_Node(550, y, 2, false, false, false, 2 * BulletDamage));
        CreateFinalNPC(y - 50);
    }
}
void LOAD_RANDOM_ENEMY(LAND **List, int r)
{
    if (*List == NULL)
        return;
    LAND *Now = *List;
    int i = 1;
    while (Now != NULL)
    {

        if (Now->CanExertEnemy)
        {
            i += 1;
            switch (rand() % r)
            {
            case 0:
            case 1:
                if (CurrentLevel == 1)
                    CreateEnemy1(Now->LandRect.x, Now->LandRect.y - ENEMY1_1_Hight);
                else if (CurrentLevel == 2)
                    CreateEnemy5(Now->LandRect.x, Now->LandRect.y - ENEMY2_3_Hight);
                else if (CurrentLevel == 3)
                    CreateEnemy7(Now->LandRect.x, Now->LandRect.y - ENEMY3_3_Hight);
                break;
            case 2:
                if (CurrentLevel == 3)
                    CreateEnemy7(Now->LandRect.x + 10, Now->LandRect.y - ENEMY3_3_Hight);
            case 3:
                if (CurrentLevel == 1)
                    CreateEnemy2(Now->LandRect.x, Now->LandRect.y - ENEMY1_1_Hight);
                else if (CurrentLevel == 2)
                    CreateEnemy4(Now->LandRect.x, Now->LandRect.y - ENEMY2_2_Hight - 7);
                else if (CurrentLevel == 3)
                {
                    CreateEnemy8(Now->LandRect.x, Now->LandRect.y - 50);
                    break;
                }
            case 4:
                if (CurrentLevel == 1)
                    CreateEnemy2(Now->LandRect.x + 20, Now->LandRect.y - ENEMY1_1_Hight - 50);
                else if (CurrentLevel == 2)
                    CreateEnemy4(Now->LandRect.x + 20, Now->LandRect.y - ENEMY2_2_Hight - 57);
                else if (CurrentLevel == 3)
                    CreateEnemy7(Now->LandRect.x, Now->LandRect.y - ENEMY3_3_Hight);
                break;
            case 5:
                if (CurrentLevel == 3)
                {
                    CreateEnemy9(Now->LandRect.x + 10, Now->LandRect.y - 110);
                    break;
                }
            case 6:
                if (CurrentLevel == 1)
                    CreateEnemy3(Now->LandRect.x, Now->LandRect.y - ENEMY1_3_Hight);
                else if (CurrentLevel == 2)
                    CreateEnemy6(Now->LandRect.x, Now->LandRect.y - ENEMY2_4_Hight);
                else if (CurrentLevel == 3)
                    CreateEnemy9(Now->LandRect.x + 10, Now->LandRect.y - 110);
                break;
            default:
                break;
            }
            if (i % 10 == 0 && CurrentLevel == 3)
            {
                switch (rand() % 4)
                {
                case 0:
                    CreateEnemy10(400, Now->LandRect.y + 100, 20);
                    CreateEnemy10(450, Now->LandRect.y + 100, -70);
                    break;
                case 1:
                    CreateEnemy10(300, Now->LandRect.y + 100, -20);
                    CreateEnemy10(250, Now->LandRect.y + 100, 70);
                    break;
                case 2:
                    CreateEnemy10(300, Now->LandRect.y, -30);
                    break;
                case 3:
                    CreateEnemy10(400, Now->LandRect.y, 30);
                    break;
                default:
                    break;
                }
                i = 0;
            }
        }
        Now = Now->Next;
    }
}

void COPY_OBJECT()
{
    if (!Start_BOSS)
    {
        PrintList(&Me_Bullet, BulletSurface);
        PrintList(&Me_BulletFire, FireSurface);
        PrintList(&Empty, EmptySurface);
        Print_Land_List(&Land, PlatSurface); // 需要改Land和Stone
        Print_Land_List(&Stone, LandSurface);
        PrintList(&Star, StarSurface);
        PrintBulletList(&EnemyBullet, EnemyBulletSurface);
        if (Judge_Start && !Judge_Preparing)
        {
            if (CurrentLevel == 1 || CurrentLevel == 3)
                Print_Enemy_List(&Enemy, Enemy1LSurface, Enemy1RSurface);
            else if (CurrentLevel == 2 || CurrentLevel == 3)
                PrintExList(&Enemy4, Enemy4Surface, Enemy4Surface);
            Print_Enemy_List(&Enemy2, Enemy2LSurface, Enemy2RSurface);
            Print_Enemy_List(&Enemy3, Enemy3Surface, Enemy3Surface);
            Print_NPC_List(&NPC, NPCLSurface, NPCRSurface);
            Print_Money_List(&Money, CuSurface, AgSurface, AuSurface, LoveSurface);
            PrintList(&Die, DieSurface);
            PrintWordList(&RandomBuff, RandomBuffSurface);
        }
        if (Interacting || Choosing)
        {
            if (Buying || Choosing)
                Print_Item_List(&ItemsOnSale, ItemsSurface);
        }
    }
    else
    {
        Print_Land_List(&Stone, LandSurface);
        PrintList(&Me_Bullet, BulletSurface);
        PrintList(&Me_BulletFire, FireSurface);
        PrintList(&Empty, EmptySurface);
        PrintList(&Star, StarSurface);
        Print_Enemy_List(&Enemy, BossItemSurface, BossItemSurface);
        PrintExList(&Enemy2, BossBulletSurface, BossBulletSurface);
        PrintBulletList(&Enemy3, BossBulletSurface);
        PrintBulletList(&EnemyBullet, BossBulletSurface);
        Print_Money_List(&Money, CuSurface, AgSurface, AuSurface, LoveSurface);
        PrintBossEye(&BossEye, BossEyeSurface);
        PrintBossBody(&Boss, BossBodySurface);
        PrintBossLife();
    }
}

void MOVE_OBJECT()
{
    MoveNode(&Me_Bullet);
    MoveNode(&Star);
    Move_Fire_Node(&Me_BulletFire, DISTANCE);
    MoveNode(&Empty);
}

void PrintList(OB **List, SDL_Surface *SurList[])
{
    for (OB *i = *List; i != NULL; i = i->Next)
    {
        if (i->ObjectRect.y < 720)
        {
            i->ObjectRect.w = SurList[i->Which]->w;
            i->ObjectRect.h = SurList[i->Which]->h;
            SDL_Texture *TexList = SDL_CreateTextureFromSurface(Renderer, SurList[i->Which]);
            SDL_RenderCopy(Renderer, TexList, NULL, &i->ObjectRect);
            SDL_DestroyTexture(TexList);
        }
    }
}
void PrintWordList(OB **List, SDL_Surface *SurList[])
{
    for (OB *i = *List; i != NULL; i = i->Next)
    {
        if (i->ObjectRect.y < 720)
        {
            i->ObjectRect.w = SurList[i->Which]->w;
            i->ObjectRect.h = SurList[i->Which]->h;
            SDL_Texture *TexList = SDL_CreateTextureFromSurface(Renderer, SurList[i->Which]);
            SDL_SetTextureBlendMode(TexList, SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(TexList, i->Alpha);
            i->Alpha -= 10;
            if (i->Alpha < 0)
                i->Alpha = 0;
            SDL_RenderCopy(Renderer, TexList, NULL, &i->ObjectRect);
            SDL_DestroyTexture(TexList);
        }
    }
}
void Print_Item_List(ITEM **List, SDL_Surface *SurList[])
{
    for (ITEM *i = *List; i != NULL; i = i->Next)
    {
        i->ItemRect.w = SurList[i->Which_Item]->w;
        i->ItemRect.h = SurList[i->Which_Item]->h;
        SDL_Texture *TexList = SDL_CreateTextureFromSurface(Renderer, SurList[i->Which_Item]);
        SDL_RenderCopy(Renderer, TexList, NULL, &i->ItemRect);
        SDL_DestroyTexture(TexList);
    }
}
void Print_Land_List(LAND **List, SDL_Surface *SurList[])
{
    for (LAND *i = *List; i != NULL; i = i->Next)
    {
        if (i->LandRect.y > 800)
            continue;
        i->LandRect.w = SurList[i->Which]->w;
        i->LandRect.h = SurList[i->Which]->h;
        SDL_Texture *TexList = SDL_CreateTextureFromSurface(Renderer, SurList[i->Which]);
        SDL_RenderCopy(Renderer, TexList, NULL, &i->LandRect);
        SDL_DestroyTexture(TexList);
    }
}
void Print_Enemy_List(ENEMY **List, SDL_Surface *SurListL[], SDL_Surface *SurListR[])
{
    for (ENEMY *i = *List; i != NULL; i = i->Next)
    {
        if (i->EnemyRect.y > 1000)
            continue;
        SDL_Texture *TexList = NULL;
        if (!(i->Turn_To_Right))
        {
            i->EnemyRect.w = SurListL[i->Which]->w;
            i->EnemyRect.h = SurListL[i->Which]->h;
            if (!(i->Droping))
                TexList = SDL_CreateTextureFromSurface(Renderer, SurListL[i->Which]);
            else
                TexList = SDL_CreateTextureFromSurface(Renderer, SurListL[i->Type + 1]);
        }
        else
        {
            i->EnemyRect.w = SurListR[i->Which]->w;
            i->EnemyRect.h = SurListR[i->Which]->h;
            if (!(i->Droping))
                TexList = SDL_CreateTextureFromSurface(Renderer, SurListR[i->Which]);
            else
                TexList = SDL_CreateTextureFromSurface(Renderer, SurListR[i->Type + 1]);
        }
        SDL_RenderCopy(Renderer, TexList, NULL, &i->EnemyRect);
        SDL_DestroyTexture(TexList);
    }
}
void Print_NPC_List(ENEMY **List, SDL_Surface *SurListL[], SDL_Surface *SurListR[])
{
    for (ENEMY *i = *List; i != NULL; i = i->Next)
    {
        if (i->EnemyRect.y > 720)
            continue;
        SDL_Texture *TexList = NULL;
        if (!(i->Turn_To_Right))
        {
            i->EnemyRect.w = SurListL[i->Which_Enemy - 20]->w;
            i->EnemyRect.h = SurListL[i->Which_Enemy - 20]->h;
            TexList = SDL_CreateTextureFromSurface(Renderer, SurListL[i->Which_Enemy - 20]);
        }
        else
        {
            i->EnemyRect.w = SurListR[i->Which_Enemy - 20]->w;
            i->EnemyRect.h = SurListR[i->Which_Enemy - 20]->h;
            TexList = SDL_CreateTextureFromSurface(Renderer, SurListR[i->Which_Enemy - 20]);
        }
        SDL_RenderCopy(Renderer, TexList, NULL, &i->EnemyRect);
        SDL_DestroyTexture(TexList);
    }
}
void Print_Money_List(ENEMY **List, SDL_Surface *SurList4[], SDL_Surface *SurList3[], SDL_Surface *SurList2[], SDL_Surface *SurList5[])
{
    for (ENEMY *i = *List; i != NULL; i = i->Next)
    {
        if (i->EnemyRect.y > 720)
            continue;
        SDL_Texture *TexList = NULL;
        switch (i->Which_Enemy)
        {
        case -4:
            i->EnemyRect.w = SurList4[i->Which]->w;
            i->EnemyRect.h = SurList4[i->Which]->h;
            TexList = SDL_CreateTextureFromSurface(Renderer, SurList4[i->Which]);
            break;
        case -3:
            i->EnemyRect.w = SurList3[i->Which]->w;
            i->EnemyRect.h = SurList3[i->Which]->h;
            TexList = SDL_CreateTextureFromSurface(Renderer, SurList3[i->Which]);
            break;
        case -2:
            i->EnemyRect.w = SurList2[i->Which]->w;
            i->EnemyRect.h = SurList2[i->Which]->h;
            TexList = SDL_CreateTextureFromSurface(Renderer, SurList2[i->Which]);
            break;
        case -5:
            i->EnemyRect.w = SurList2[i->Which]->w;
            i->EnemyRect.h = SurList2[i->Which]->h;
            TexList = SDL_CreateTextureFromSurface(Renderer, SurList5[i->Which]);
            break;
        default:
            break;
        }
        SDL_RenderCopy(Renderer, TexList, NULL, &i->EnemyRect);
        SDL_DestroyTexture(TexList);
    }
}
void PrintExList(ENEMY **List, SDL_Surface *SurListL[], SDL_Surface *SurListR[])
{
    for (ENEMY *i = *List; i != NULL; i = i->Next)
    {
        if (i->EnemyRect.y > 1000)
            continue;
        SDL_Texture *TexList = NULL;
        i->EnemyRect.w = SurListL[i->Which]->w;
        i->EnemyRect.h = SurListL[i->Which]->h;
        TexList = SDL_CreateTextureFromSurface(Renderer, SurListL[i->Which]);
        if (i->EnemyRect.y >= Me_Y && i->EnemyRect.x > Me_X)
            SDL_RenderCopyEx(Renderer, TexList, NULL, &i->EnemyRect, atan((i->EnemyRect.y - Me_Y) / (i->EnemyRect.x - Me_X)) * 180 / 3.1416 - 90, NULL, SDL_FLIP_NONE);
        else if (i->EnemyRect.y >= Me_Y && i->EnemyRect.x < Me_X)
            SDL_RenderCopyEx(Renderer, TexList, NULL, &i->EnemyRect, -atan((i->EnemyRect.y - Me_Y) / (Me_X - i->EnemyRect.x)) * 180 / 3.1416 + 90, NULL, SDL_FLIP_NONE);
        else if (i->EnemyRect.y < Me_Y && i->EnemyRect.x >= Me_X)
            SDL_RenderCopyEx(Renderer, TexList, NULL, &i->EnemyRect, -atan((Me_Y - i->EnemyRect.y) / (i->EnemyRect.x - Me_X)) * 180 / 3.1416 - 90, NULL, SDL_FLIP_NONE);
        else if (i->EnemyRect.y < Me_Y && i->EnemyRect.x == Me_X)
            SDL_RenderCopyEx(Renderer, TexList, NULL, &i->EnemyRect, 180, NULL, SDL_FLIP_NONE);
        else if (i->EnemyRect.y < Me_Y && i->EnemyRect.x <= Me_X)
            SDL_RenderCopyEx(Renderer, TexList, NULL, &i->EnemyRect, atan((Me_Y - i->EnemyRect.y) / (Me_X - i->EnemyRect.x)) * 180 / 3.1416 + 90, NULL, SDL_FLIP_NONE);
        else if (i->EnemyRect.y > Me_Y && i->EnemyRect.x == Me_X)
            SDL_RenderCopyEx(Renderer, TexList, NULL, &i->EnemyRect, 0, NULL, SDL_FLIP_NONE);
        SDL_DestroyTexture(TexList);
    }
}
void PrintBulletList(ENEMY **List, SDL_Surface *SurList[])
{
    for (ENEMY *i = *List; i != NULL; i = i->Next)
    {
        if (i->EnemyRect.y > 800)
            continue;
        SDL_Texture *TexList = NULL;
        i->EnemyRect.w = SurList[i->Which]->w;
        i->EnemyRect.h = SurList[i->Which]->h;
        i->ExPoint.x = (i->EnemyRect.w / 2);
        i->ExPoint.y = (i->EnemyRect.h / 10);
        TexList = SDL_CreateTextureFromSurface(Renderer, SurList[i->Which]);
        SDL_RenderCopyEx(Renderer, TexList, NULL, &i->EnemyRect, i->Angle, &i->ExPoint, SDL_FLIP_NONE);
        SDL_DestroyTexture(TexList);
    }
}
void PrintBossBody(ENEMY **List, SDL_Surface *SurList[])
{
    for (ENEMY *i = *List; i != NULL; i = i->Next)
    {
        SDL_Texture *TexList = NULL;
        i->EnemyRect.w = SurList[i->Which]->w;
        i->EnemyRect.h = SurList[i->Which]->h;
        TexList = SDL_CreateTextureFromSurface(Renderer, SurList[i->Which]);
        SDL_RenderCopy(Renderer, TexList, NULL, &i->EnemyRect);
        SDL_DestroyTexture(TexList);
    }
}
void PrintBossEye(ENEMY **List, SDL_Surface *SurList[])
{
    for (ENEMY *i = *List; i != NULL; i = i->Next)
    {
        if (i->EnemyRect.y > 750)
            continue;
        SDL_Texture *TexList = NULL;
        i->EnemyRect.w = SurList[i->Which]->w;
        i->EnemyRect.h = SurList[i->Which]->h;
        TexList = SDL_CreateTextureFromSurface(Renderer, SurList[i->Which]);
        if (i->EnemyRect.y + 60 >= Me_Y - 20 && i->EnemyRect.x + 60 > Me_X + 10)
        {
            double a = atan((i->EnemyRect.y + 60 - Me_Y + 20) / (i->EnemyRect.x + 60 - Me_X - 10)) * 180 / 3.1416 - 90;
            if (a < -30)
                a = -30;
            SDL_RenderCopyEx(Renderer, TexList, NULL, &i->EnemyRect, a, NULL, SDL_FLIP_NONE);
        }
        else if (i->EnemyRect.y + 60 >= Me_Y - 20 && i->EnemyRect.x + 60 < Me_X + 10)
        {
            double a = -atan((i->EnemyRect.y + 60 - Me_Y + 20) / (Me_X + 10 - i->EnemyRect.x - 60)) * 180 / 3.1416 + 90;
            if (a > 30)
                a = 30;
            SDL_RenderCopyEx(Renderer, TexList, NULL, &i->EnemyRect, a, NULL, SDL_FLIP_NONE);
        }
        else if (i->EnemyRect.y + 60 > Me_Y - 20 && i->EnemyRect.x + 60 == Me_X + 10)
            SDL_RenderCopyEx(Renderer, TexList, NULL, &i->EnemyRect, 0, NULL, SDL_FLIP_NONE);
        SDL_DestroyTexture(TexList);
    }
}
void PrintBossLife()
{
    SDL_Surface *LifeSurface = IMG_Load("resource/BOSSLIFE.png");
    SDL_Texture *LifeTexture = SDL_CreateTextureFromSurface(Renderer, LifeSurface);
    LifeRect.x = 100;
    LifeRect.y = 660;
    LifeRect.w = LifeSurface->w;
    LifeRect.h = LifeSurface->h;
    SDL_RenderCopy(Renderer, LifeTexture, NULL, &LifeRect);
    SDL_DestroyTexture(LifeTexture);
    SDL_FreeSurface(LifeSurface);
    SDL_Surface *BloodSurface = IMG_Load("resource/BOSSBLOOD.png");
    SDL_Texture *BloodTexture = SDL_CreateTextureFromSurface(Renderer, BloodSurface);
    BloodRect.x = 131;
    BloodRect.y = 673;
    BloodRect.w = (int)(443 * BOSS_BLOOD / BOSS_LIFE);
    BloodRect.h = BloodSurface->h;
    SDL_RenderCopy(Renderer, BloodTexture, NULL, &BloodRect);
    SDL_DestroyTexture(BloodTexture);
    SDL_FreeSurface(BloodSurface);
}

void LoadResource()
{
    for (int i = 1; i < 2; i += 1)
    {
        sprintf_s(FileName, 30, "resource/BULLET%d.png", i);
        BulletSurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i < 4; i += 1)
    {
        sprintf_s(FileName, 30, "resource/FIRE%d.png", i);
        FireSurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i < 6; i += 1)
    {
        sprintf_s(FileName, 30, "resource/EMPTY%d.png", i);
        EmptySurface[i] = IMG_Load(FileName);
    }
    sprintf_s(FileName, 30, "resource/PLATFORM%d.png", CurrentLevel);
    PlatSurface[1] = IMG_Load(FileName);
    for (int i = 1; i < 3; i += 1)
    {
        sprintf_s(FileName, 30, "resource/STONE%d.png", CurrentLevel * 2 - 2 + i);
        LandSurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i < 3; i += 1)
    {
        sprintf_s(FileName, 30, "resource/Enemy%d_1_%d_L.png", CurrentLevel, i);
        Enemy1LSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_1_%d_R.png", CurrentLevel, i);
        Enemy1RSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_2_%d_L.png", CurrentLevel, i);
        Enemy2LSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_2_%d_R.png", CurrentLevel, i);
        Enemy2RSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_3_%d.png", CurrentLevel, i);
        Enemy3Surface[i] = IMG_Load(FileName);
    }
    sprintf_s(FileName, 30, "resource/Enemy%d_1_%d_FL.png", CurrentLevel, 1);
    Enemy1LSurface[3] = IMG_Load(FileName);
    sprintf_s(FileName, 30, "resource/Enemy%d_1_%d_FR.png", CurrentLevel, 1);
    Enemy1RSurface[3] = IMG_Load(FileName);
    sprintf_s(FileName, 30, "resource/Enemy%d_3_%d.png", CurrentLevel, 1);
    Enemy3Surface[3] = IMG_Load(FileName);
    for (int i = 1; i < 9; i += 1)
    {
        sprintf_s(FileName, 30, "resource/DIE%d.png", i);
        DieSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Cu%d.png", i);
        CuSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Ag%d.png", i);
        AgSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Au%d.png", i);
        AuSurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i < 10; i++)
    {
        sprintf_s(FileName, 30, "resource/BOSSBODY%d.png", i);
        BossBodySurface[i] = IMG_Load(FileName);
    }
    BossEyeSurface[1] = IMG_Load("resource/BOSSEYE.png");
    for (int i = 1; i < 12; i++)
    {
        sprintf_s(FileName, 30, "resource/BOSSITEM%d.png", i);
        BossItemSurface[i] = IMG_Load(FileName);
    }
    LoveSurface[1] = IMG_Load("resource/LOVE.png");
    for (int i = 1; i < 5; i += 1)
    {
        sprintf_s(FileName, 30, "resource/NPC%d_L.png", i);
        NPCLSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/NPC%d_R.png", i);
        NPCRSurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i < 13; i += 1)
    {
        sprintf_s(FileName, 30, "resource/ITEM%d.png", i);
        ItemsSurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i < 8; i += 1)
    {
        sprintf_s(FileName, 30, "resource/RANDOMBUFF%d.png", i);
        RandomBuffSurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i < 5; i += 1)
    {
        sprintf_s(FileName, 30, "resource/BGM%d.mp3", i);
        BGM[i] = Mix_LoadMUS(FileName);
    }
    FinallineSurface[1] = IMG_Load("resource/FINALLINE1.png");
    Finalline2Surface[1] = IMG_Load("resource/FINALLINE2.png");
    EnemyBulletSurface[1] = IMG_Load("resource/ENEMY3_4.png");
    EnemyBulletSurface[2] = IMG_Load("resource/ENEMYBULLET1.png");
    BossBulletSurface[1] = IMG_Load("resource/BOSSENEMY1.png");
    BossBulletSurface[2] = IMG_Load("resource/BOSSENEMY2.png");
    BossBulletSurface[3] = IMG_Load("resource/BOSSBULLET.png");
    TipSurface[1] = IMG_Load("resource/TIP.png");
    TipSurface[2] = IMG_Load("resource/CHAT1.png");
    TipSurface[3] = IMG_Load("resource/CHAT2.png");
    TipSurface[4] = IMG_Load("resource/CHAT3.png");
    TipSurface[5] = IMG_Load("resource/CHAT4.png");
    StarSurface[1] = IMG_Load("resource/STAR.png");
    music1 = Mix_LoadWAV("resource/JUMP.mp3");
    music2 = Mix_LoadWAV("resource/DROP.mp3");
    music3 = Mix_LoadWAV("resource/BOOM.mp3");
    music4 = Mix_LoadWAV("resource/CLIP.mp3");
    music5 = Mix_LoadWAV("resource/EMPTY.mp3");
    music6 = Mix_LoadWAV("resource/TITLE.mp3");
    music7 = Mix_LoadWAV("resource/RUN.mp3");
    music8 = Mix_LoadWAV("resource/STONEEXPLODE.mp3");
    music9 = Mix_LoadWAV("resource/KILL1.mp3");
    music10 = Mix_LoadWAV("resource/KILL2.mp3");
    music11 = Mix_LoadWAV("resource/KILL3.mp3");
    music12 = Mix_LoadWAV("resource/HURT.mp3");
    music13 = Mix_LoadWAV("resource/KILL4.mp3");
    music14 = Mix_LoadWAV("resource/KILL5.mp3");
    music15 = Mix_LoadWAV("resource/CREEP.mp3");
    music16 = Mix_LoadWAV("resource/SHOOT1.mp3");
    music17 = Mix_LoadWAV("resource/IRON.mp3");
    music18 = Mix_LoadWAV("resource/BOSS.mp3");
    music19 = Mix_LoadWAV("resource/DING1.mp3");
    music20 = Mix_LoadWAV("resource/DING2.mp3");
    music21 = Mix_LoadWAV("resource/DING3.mp3");
    music22 = Mix_LoadWAV("resource/DING4.mp3");
    music23 = Mix_LoadWAV("resource/DING.mp3");
    music24 = Mix_LoadWAV("resource/LOVE.mp3");
    music25 = Mix_LoadWAV("resource/KILL6.mp3");
    music26 = Mix_LoadWAV("resource/SKILL1.mp3");
    music27 = Mix_LoadWAV("resource/SKILL3.mp3");
    music28 = Mix_LoadWAV("resource/BOSSDIE.mp3");
    music29 = Mix_LoadWAV("resource/KILL7.mp3");
    MoneyFont = TTF_OpenFont("resource/CHILLER.TTF", 22);
    ScoreFont = TTF_OpenFont("resource/CHILLER.TTF", 50);
    Items[1] = Create_Item_Node(1, 0, 1, 53, 14);
    Items[2] = Create_Item_Node(2, 0, 1, 3, 72);
    Items[3] = Create_Item_Node(3, 0, 5, 2, 1);
    Items[4] = Create_Item_Node(4, 0, 2, 11, 6);
    Items[5] = Create_Item_Node(5, 0, 3, 26, 7);
    Items[6] = Create_Item_Node(6, 0, 4, 0, 0);
    Items[7] = Create_Item_Node(7, 0, 1, 2, 5);
    Items[8] = Create_Item_Node(8, 0, 8, 21, 7);
    Items[9] = Create_Item_Node(9, 0, 2, 86, 23);
    Items[10] = Create_Item_Node(10, 0, 2, 5, 66);
    Items[11] = Create_Item_Node(11, 0, 11, 11, 11);
    Items[12] = Create_Item_Node(12, 0, 8, 3, 4);
}
void LoadResource2()
{
    SDL_FreeSurface(PlatSurface[1]);
    sprintf_s(FileName, 30, "resource/PLATFORM%d.png", CurrentLevel);
    PlatSurface[1] = IMG_Load(FileName);
    for (int i = 1; i < 3; i += 1)
    {
        SDL_FreeSurface(LandSurface[i]);
        sprintf_s(FileName, 30, "resource/STONE%d.png", CurrentLevel * 2 - 2 + i);
        LandSurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i < 3; i += 1)
    {
        SDL_FreeSurface(Enemy2LSurface[i]);
        SDL_FreeSurface(Enemy2RSurface[i]);
        SDL_FreeSurface(Enemy3Surface[i]);
        SDL_FreeSurface(Enemy4Surface[i]);
        sprintf_s(FileName, 30, "resource/Enemy%d_2_%d_L.png", CurrentLevel, i);
        Enemy2LSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_2_%d_R.png", CurrentLevel, i);
        Enemy2RSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_3_%d.png", CurrentLevel, i);
        Enemy3Surface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_4_%d.png", CurrentLevel, i);
        Enemy4Surface[i] = IMG_Load(FileName);
    }
    SDL_FreeSurface(Enemy3Surface[3]);
    Enemy3Surface[3] = IMG_Load("resource/Enemy2_3_1.png");
}
void LoadResource3()
{
    SDL_FreeSurface(PlatSurface[1]);
    sprintf_s(FileName, 30, "resource/PLATFORM%d.png", CurrentLevel);
    PlatSurface[1] = IMG_Load(FileName);
    for (int i = 1; i < 3; i += 1)
    {
        SDL_FreeSurface(LandSurface[i]);
        sprintf_s(FileName, 30, "resource/STONE%d.png", CurrentLevel * 2 - 2 + i);
        LandSurface[i] = IMG_Load(FileName);
    }
    for (int i = 1; i < 3; i += 1)
    {
        SDL_FreeSurface(Enemy1LSurface[i]);
        SDL_FreeSurface(Enemy1RSurface[i]);
        SDL_FreeSurface(Enemy2LSurface[i]);
        SDL_FreeSurface(Enemy2RSurface[i]);
        SDL_FreeSurface(Enemy3Surface[i]);
        sprintf_s(FileName, 30, "resource/Enemy%d_1_%d_L.png", CurrentLevel, i);
        Enemy1LSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_1_%d_R.png", CurrentLevel, i);
        Enemy1RSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_2_%d_L.png", CurrentLevel, i);
        Enemy2LSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_2_%d_R.png", CurrentLevel, i);
        Enemy2RSurface[i] = IMG_Load(FileName);
        sprintf_s(FileName, 30, "resource/Enemy%d_3_%d.png", CurrentLevel, i);
        Enemy3Surface[i] = IMG_Load(FileName);
    }
    SDL_FreeSurface(Enemy3Surface[3]);
    Enemy3Surface[3] = IMG_Load("resource/Enemy3_3_1.png");
}

void AddNode(OB **List, OB *NewNode)
{
    if (*List == NULL)
    {
        *List = NewNode;
        return;
    }
    OB *Now = *List;
    while (Now->Next != NULL)
        Now = Now->Next;
    Now->Next = NewNode;
}
void Add_Item_Node(ITEM **List, ITEM *NewNode)
{
    if (*List == NULL)
    {
        *List = NewNode;
        return;
    }
    ITEM *Now = *List;
    while (Now->Next != NULL)
        Now = Now->Next;
    Now->Next = NewNode;
}
void Add_Land_Node(LAND **List, LAND *NewNode)
{
    if (*List == NULL)
    {
        *List = NewNode;
        return;
    }
    LAND *Now = *List;
    while (Now->Next != NULL)
        Now = Now->Next;
    Now->Next = NewNode;
}
void Add_Enemy_Node(ENEMY **List, ENEMY *NewNode)
{
    if (*List == NULL)
    {
        *List = NewNode;
        return;
    }
    ENEMY *Now = *List;
    while (Now->Next != NULL)
        Now = Now->Next;
    Now->Next = NewNode;
}
// 改
OB *CreateNode(int Hp, int Distance, bool Toward_X, int Damage, int Speed, int Which, int X, int Y, int Type, bool AppearLoop)
{
    OB *New = malloc(sizeof(OB));
    New->ObjectRect.x = X;
    New->ObjectRect.y = Y;
    New->Type = Type;
    New->Distance = Distance;
    New->Which = Which;
    New->Speed = Speed;
    New->Hp = Hp;
    New->AppearLoop = AppearLoop;
    New->Toward_X = Toward_X;
    New->Damage = Damage;
    New->Next = NULL;
    return New;
}
LAND *Create_Land_Node(int Land_X, int Land_Y, int Which, bool BeDamaged, bool PassBullet, bool CanExertEnemy, int Hp)
{
    LAND *New = malloc(sizeof(LAND));
    New->LandRect.x = Land_X;
    New->LandRect.y = Land_Y;
    New->LandRect.w = 50;
    New->LandRect.h = 50;
    New->Hp = Hp;
    New->Which = Which;
    New->BeDamaged = BeDamaged;
    New->PassBullet = PassBullet;
    New->CanExertEnemy = CanExertEnemy;
    New->OnIt = false;
    New->Next = NULL;
    return New;
}
ENEMY *Create_Enemy_Node(int Which_Enemy, int Hp, int Damage, int Speed, int JumpSpeed, int Which, int Type, int X, int Y, bool CanFly, bool CanJump, bool CanShoot, int Music, int H, int W)
{
    ENEMY *New = malloc(sizeof(ENEMY));
    New->Which_Enemy = Which_Enemy;
    New->EnemyRect.x = X;
    New->EnemyRect.y = Y;
    New->Hp = Hp;
    New->Distance = 0;
    New->Delay = 0;
    New->ActionDelay = 0;
    New->W = W;
    New->H = H;
    if (rand() % 2 == 0)
        New->Turn_To_Right = true;
    else
        New->Turn_To_Right = false;
    New->JumpSpeed = 0;
    New->DropRun = false;
    New->CanFly = CanFly;
    New->CanJump = CanJump;
    New->CanShoot = CanShoot;
    New->Which = Which;
    New->Type = Type;
    New->JumpSpeed = JumpSpeed;
    New->Which = Which;
    New->Damage = Damage;
    New->Droping = false;
    New->Speed = Speed;
    New->Music = Music;
    New->Next = NULL;
    return New;
}
ENEMY *Create_DropEnemy_Node(int Which_Enemy, int Hp, int Damage, int Speed, int JumpSpeed, int Which, int Type, int X, int Y, bool CanFly, bool CanJump, bool CanShoot, int Music, int H, int W)
{
    {
        ENEMY *New = malloc(sizeof(ENEMY));
        New->Which_Enemy = Which_Enemy;
        New->EnemyRect.x = X;
        New->EnemyRect.y = Y;
        New->Hp = Hp;
        New->Distance = 0;
        New->Delay = 0;
        New->ActionDelay = 20;
        New->W = W;
        New->H = H;
        if (rand() % 2 == 0)
            New->Turn_To_Right = true;
        else
            New->Turn_To_Right = false;
        New->JumpSpeed = 0;
        New->DropRun = false;
        New->CanFly = CanFly;
        New->CanJump = CanJump;
        New->CanShoot = CanShoot;
        New->Which = Which;
        New->Type = Type;
        New->JumpSpeed = -JumpSpeed;
        New->Which = Which;
        New->Damage = Damage;
        New->Droping = true;
        New->Speed = Speed;
        New->Music = Music;
        New->Next = NULL;
        return New;
    }
}
ITEM *Create_Item_Node(int Which_Item, int Index, int Au, int Ag, int Cu)
{
    ITEM *New = malloc(sizeof(ITEM));
    New->Which_Item = Which_Item;
    New->Au = Au;
    New->Ag = Ag;
    New->Cu = Cu;
    New->BeBought = false;
    New->Index = Index;
    New->Next = NULL;
    return New;
}
// 改
void RemoveNode(OB **List)
{
    if (*List == NULL)
        return;
    OB *Now = *List;
    OB *Pre = NULL;
    while (Now != NULL)
    {
        if (!GameOver && !RemoveTime)
        {
            if (Now->Hp <= 0)
            {
                if (*List == Now)
                {
                    *List = Now->Next;
                    free(Now);
                    Now = *List;
                }
                else
                {
                    Pre->Next = Now->Next;
                    free(Now);
                    Now = Pre;
                }
            }
            else
            {
                Pre = Now;
                Now = Now->Next;
            }
        }
        else
        {
            *List = Now->Next;
            free(Now);
            Now = *List;
        }
    }
}
void Remove_Land_Node(LAND **List)
{
    if (*List == NULL)
        return;
    LAND *Now = *List;
    LAND *Pre = NULL;
    while (Now != NULL)
    {
        if (!GameOver && !RemoveTime)
        {
            if (Now->Hp <= 0)
            {
                if (*List == Now)
                {
                    *List = Now->Next;
                    free(Now);
                    Now = *List;
                }
                else
                {
                    Pre->Next = Now->Next;
                    free(Now);
                    Now = Pre->Next;
                }
            }
            else
            {
                Pre = Now;
                Now = Now->Next;
            }
        }
        else
        {
            *List = Now->Next;
            free(Now);
            Now = *List;
        }
    }
}
void Remove_Enemy_Node(ENEMY **List)
{
    if (*List == NULL)
        return;
    ENEMY *Now = *List;
    ENEMY *Pre = NULL;
    while (Now != NULL)
    {
        if (!GameOver && !RemoveTime)
        {
            if (Now->Hp <= 0)
            {
                if (*List == Now)
                {
                    *List = Now->Next;
                    free(Now);
                    Now = *List;
                }
                else
                {
                    Pre->Next = Now->Next;
                    free(Now);
                    Now = Pre;
                }
            }
            else
            {
                Pre = Now;
                Now = Now->Next;
            }
        }
        else
        {
            *List = Now->Next;
            free(Now);
            Now = *List;
        }
    }
}
// 改
void CreateBullet(int x)
{
    AddNode(&Me_Bullet, CreateNode(BulletHp, BulletDistance, false, BulletDamage, BulletSpeed, 1, x, Me_Y + 50, BulletType, true));
}
void CreateStar()
{
    AddNode(&Star, CreateNode(BulletHp, 800, false, 3 * BulletDamage, BulletSpeed, 1, 100, -41, BulletType, true));
    AddNode(&Star, CreateNode(BulletHp, 800, false, 3 * BulletDamage, BulletSpeed, 1, 150, -50, BulletType, true));
    AddNode(&Star, CreateNode(BulletHp, 800, false, 3 * BulletDamage, BulletSpeed, 1, 200, -60, BulletType, true));
    AddNode(&Star, CreateNode(BulletHp, 800, false, 3 * BulletDamage, BulletSpeed, 1, 250, -55, BulletType, true));
    AddNode(&Star, CreateNode(BulletHp, 800, false, 3 * BulletDamage, BulletSpeed, 1, 300, -41, BulletType, true));
    AddNode(&Star, CreateNode(BulletHp, 800, false, 3 * BulletDamage, BulletSpeed, 1, 350, -80, BulletType, true));
    AddNode(&Star, CreateNode(BulletHp, 800, false, 3 * BulletDamage, BulletSpeed, 1, 400, -60, BulletType, true));
    AddNode(&Star, CreateNode(BulletHp, 800, false, 3 * BulletDamage, BulletSpeed, 1, 450, -50, BulletType, true));
    AddNode(&Star, CreateNode(BulletHp, 800, false, 3 * BulletDamage, BulletSpeed, 1, 500, -70, BulletType, true));
    AddNode(&Star, CreateNode(BulletHp, 800, false, 3 * BulletDamage, BulletSpeed, 1, 550, -50, BulletType, true));
}
void CreateFire(int x, int y)
{
    AddNode(&Me_BulletFire, CreateNode(BulletFireHp, 1, false, 0, 0, 1, x - 5, y - 58, BulletFireType, false));
}
void CreateDie(int x, int y)
{
    AddNode(&Die, CreateNode(BulletFireHp, 1, false, 0, 0, 1, x + 10, y, 8, false));
}
void CreateEmpty()
{
    AddNode(&Empty, CreateNode(1, 1, false, 0, 0, 1, Me_X - 42, Me_Y - 20, EmptyType, false));
}
void CreateFinalline(int y)
{
    AddNode(&Finalline, CreateNode(1, 1, false, 0, 0, 1, 0, y, 1, true));
}
void CreateFinalline2(int y)
{
    AddNode(&Finalline2, CreateNode(1, 1, false, 0, 0, 1, 0, y, 1, true));
}

void CreateEnemy1(int x, int y) // 甲虫
{
    Add_Enemy_Node(&Enemy, Create_Enemy_Node(1, 30, 1, 5, 0, 1, 2, x, y, false, false, false, 1, 31, 40));
}
void CreateEnemy2(int x, int y) // 蝙蝠
{
    Add_Enemy_Node(&Enemy2, Create_Enemy_Node(2, 30, 1, 9, 0, 1, 2, x, y, true, false, false, 2, 28, 41));
}
void CreateEnemy3(int x, int y) // 史莱姆
{
    Add_Enemy_Node(&Enemy3, Create_Enemy_Node(3, 20, 1, 8, 20, 1, 2, x, y, false, true, false, 3, 39, 50));
}
void CreateEnemy4(int x, int y) // 恶翅史莱姆
{
    Add_Enemy_Node(&Enemy2, Create_Enemy_Node(4, 20, 1, 9, 0, 1, 2, x, y, true, false, false, 4, 70, 37));
}
void CreateEnemy5(int x, int y) // 腐化史莱姆
{
    Add_Enemy_Node(&Enemy3, Create_Enemy_Node(5, 30, 1, 8, 20, 1, 2, x, y, false, true, false, 3, 34, 50));
}
void CreateEnemy6(int x, int y) // 腐化者
{
    Add_Enemy_Node(&Enemy4, Create_Enemy_Node(6, 30, 1, 6, 0, 1, 2, x, y, true, false, false, 5, 80, 44));
}
void CreateEnemy7(int x, int y) // 熔岩史莱姆
{
    Add_Enemy_Node(&Enemy3, Create_Enemy_Node(7, 30, 1, 8, 20, 1, 2, x, y, false, true, false, 3, 39, 40));
}
void CreateEnemy8(int x, int y) // 地狱蝙蝠
{
    Add_Enemy_Node(&Enemy2, Create_Enemy_Node(8, 30, 1, 9, 0, 1, 2, x, y, true, false, false, 2, 39, 32));
}
void CreateEnemy9(int x, int y) // 红魔鬼
{
    Add_Enemy_Node(&Enemy, Create_Enemy_Node(9, 20, 1, 6, 0, 1, 2, x, y, true, false, true, 6, 60, 98));
}
void CreateEnemy10(int x, int y, int angle) // 骨蛇
{
    ENEMY *Now = Create_Enemy_Node(10, 100, 1, 30, 0, 1, 2, x, y, false, false, false, 2, 483, 50);
    Now->Angle = angle;
    Add_Enemy_Node(&EnemyBullet, Now);
}
void CreateEnemy11(int x, int y, int angle) // 恶魔三叉戟
{
    ENEMY *Now = Create_Enemy_Node(11, 1, 1, 20, 0, 2, 1, x, y, false, false, false, 2, 60, 15);
    Now->Angle = angle;
    Add_Enemy_Node(&EnemyBullet, Now);
}
void CreateEnemy12(int x, int y) // 饿鬼
{
    Add_Enemy_Node(&Enemy2, Create_Enemy_Node(2, 20, 1, 8, 0, 2, 1, x, y, true, false, false, 7, 42, 29));
}
void CreateEnemy13(int x, int y, int angle) // 激光
{
    ENEMY *Now = Create_Enemy_Node(3, 1, 1, 20, 0, 3, 1, x, y, false, false, false, 2, 66, 3);
    Now->Angle = angle;
    Add_Enemy_Node(&EnemyBullet, Now);
}

void CreateMoney(int x, int y)
{
    int r;
    if (!Buff[6])
        r = rand() % 100;
    else
        r = rand() % 103;
    if (0 <= r && r < 40)
    {
        Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, 1, 10, 1, 8, x, y, false, true, false, 19, 18, 19));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, -4, 6, 1, 8, x, y, false, true, false, 19, 18, 19));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, 1, 8, 8, 8, x, y, false, true, false, 19, 18, 19));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, -1, 16, 4, 8, x, y, false, true, false, 19, 18, 19));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, 4, 8, 1, 8, x, y, false, true, false, 19, 18, 19));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, -2, 6, 1, 8, x, y, false, true, false, 19, 18, 19));
        if (r > 20)
        {
            Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, 6, 10, 1, 8, x, y, false, true, false, 19, 18, 19));
            Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, 1, 16, 1, 8, x, y, false, true, false, 19, 18, 19));
            Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, -2, 8, 1, 8, x, y, false, true, false, 19, 18, 19));
        }
    }
    else if (40 <= r && r < 80)
    {

        Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, -1, 6, 1, 8, x, y, false, true, false, 19, 18, 19));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, 4, 10, 4, 8, x, y, false, true, false, 19, 18, 19));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, -2, 14, 4, 8, x, y, false, true, false, 19, 18, 19));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-3, 1, 0, 1, 16, 1, 8, x, y, false, true, false, 19, 22, 18));
    }
    else if (80 <= r && r < 97)
    {
        Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, 4, 12, 4, 8, x, y, false, true, false, 19, 18, 19));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-4, 1, 0, -4, 8, 4, 8, x, y, false, true, false, 19, 18, 19));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-3, 1, 0, 1, 10, 1, 8, x, y, false, true, false, 19, 22, 18));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-3, 1, 0, 2, 16, 1, 8, x, y, false, true, false, 19, 22, 18));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-3, 1, 0, -1, 8, 4, 8, x, y, false, true, false, 19, 22, 18));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-3, 1, 0, 2, 14, 4, 8, x, y, false, true, false, 19, 22, 18));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-3, 1, 0, -1, 10, 1, 8, x, y, false, true, false, 19, 22, 18));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-3, 1, 0, -2, 16, 1, 8, x, y, false, true, false, 19, 22, 18));
    }
    else if ((97 <= r && r < 99 && !Buff[6]) || (97 <= r && r < 101 && Buff[6]))
    {
        Add_Enemy_Node(&Money, Create_Enemy_Node(-2, 1, 0, 2, 12, 1, 8, x, y, false, true, false, 19, 25, 18));
    }
    else if ((r == 99 && !Buff[6]) || (101 <= r && r < 103 && Buff[6]))
    {
        Add_Enemy_Node(&Money, Create_Enemy_Node(-2, 1, 0, 2, 12, 1, 8, x, y, false, true, false, 19, 25, 18));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-2, 1, 0, -2, 16, 1, 8, x, y, false, true, false, 19, 25, 18));
        Add_Enemy_Node(&Money, Create_Enemy_Node(-2, 1, 0, 1, 10, 1, 8, x, y, false, true, false, 19, 25, 18));
    }
    r = rand() % 20;
    if (r == 0)
        Add_Enemy_Node(&Money, Create_Enemy_Node(-5, 1, 0, 0, 16, 1, 1, x, y, false, true, false, 19, 27, 30));
    if ((r == 3 || r == 2) && CurrentLevel >= 2)
        Add_Enemy_Node(&Money, Create_Enemy_Node(-5, 1, 0, 0, 16, 1, 1, x, y, false, true, false, 19, 27, 30));
    if ((r == 6 || r == 7) && CurrentLevel >= 3)
        Add_Enemy_Node(&Money, Create_Enemy_Node(-5, 1, 0, 0, 16, 1, 1, x, y, false, true, false, 19, 27, 30));
    if ((r == 4 || r == 5) && (Buff[3]))
        Add_Enemy_Node(&Money, Create_Enemy_Node(-5, 1, 0, 0, 16, 1, 1, x, y, false, true, false, 19, 27, 30));
}
void CreateNPC(int x, int y)
{
    int r = rand() % 4;
    if (NPCLastRand == r)
        r = 3;
    else
        NPCLastRand = r;
    switch (r)
    {
    case 0:
        Add_Enemy_Node(&NPC, Create_Enemy_Node(21, 1, 0, 0, 0, 1, 1, x, y - 50, false, false, false, 0, 50, 28)); // NPC1
        break;
    case 1:
        Add_Enemy_Node(&NPC, Create_Enemy_Node(22, 1, 0, 0, 0, 1, 1, x, y - 50, false, false, false, 0, 50, 35)); // NPC3
        break;
    case 2:
        Add_Enemy_Node(&NPC, Create_Enemy_Node(24, 1, 0, 0, 0, 1, 1, x, y - 50, false, false, false, 0, 50, 34)); // NPC4
        break;
    default:
        Add_Enemy_Node(&NPC, Create_Enemy_Node(24, 1, 0, 0, 0, 1, 1, x, y - 50, false, false, false, 0, 50, 34)); // NPC4
        break;
    }
}
void CreateFinalNPC(int y)
{
    Add_Enemy_Node(&NPC, Create_Enemy_Node(23, 1, 0, 0, 0, 1, 1, 300, y, false, false, false, 0, 50, 28)); // NPC3
}

void CreatePlatform(int x, int y)
{
    Add_Land_Node(&Land, Create_Land_Node(x, y, 1, false, true, true, 1));
} // 随关卡自动更换材质
void CreateStone1(int y) // 右侧大型石头1,不可破坏
{
    Add_Land_Node(&Stone, Create_Land_Node(550, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 50, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 50, 2, true, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(400, y + 100, 2, true, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(400, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(400, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(400, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(350, y + 150, 2, true, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(300, y + 150, 2, true, false, true, 2 * BulletDamage));
    Random_Create(1, y);
}
void CreateStone2(int y) // 左侧大型石头1,不可破坏
{
    Add_Land_Node(&Stone, Create_Land_Node(100, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 50, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 50, 2, true, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(250, y + 150, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(250, y + 200, 1, false, false, false, 2 * BulletDamage));
    Random_Create(2, y);
}
void CreateStone3(int y) // 右侧大型的石头2,不可破坏
{
    Add_Land_Node(&Stone, Create_Land_Node(550, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 300, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 350, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 400, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 450, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 50, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 300, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 350, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 400, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 50, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 300, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 350, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(400, y + 300, 1, false, false, true, 2 * BulletDamage));
    Random_Create(3, y);
}
void CreateStone4(int y) // 左侧大型石头2,不可破坏
{
    Add_Land_Node(&Stone, Create_Land_Node(100, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 300, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 350, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 100, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 300, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 200, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(250, y + 200, 1, false, false, true, 2 * BulletDamage));
    Random_Create(4, y);
}
void CreateStone5(int y) // 右侧大型的石头3,不可破坏
{
    Add_Land_Node(&Stone, Create_Land_Node(550, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 300, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 350, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 50, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 300, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 100, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(400, y + 200, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(350, y + 200, 1, false, false, true, 2 * BulletDamage));
    Random_Create(5, y);
}
void CreateStone6(int y) // 左侧大型石头3,不可破坏
{
    Add_Land_Node(&Stone, Create_Land_Node(100, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 300, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 350, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 400, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 50, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 300, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 350, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 50, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 300, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(250, y + 100, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(250, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(250, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(250, y + 250, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(300, y + 100, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(300, y + 150, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(300, y + 200, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(350, y + 100, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(350, y + 150, 1, false, false, false, 2 * BulletDamage));
    Random_Create(6, y);
}
void CreateStone7(int y) // 右侧中型石头1,不可破坏
{
    Add_Land_Node(&Stone, Create_Land_Node(550, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 50, 1, false, false, false, 2 * BulletDamage));
    Random_Create(7, y);
}
void CreateStone8(int y) // 左侧中型石头1,不可破坏
{
    Add_Land_Node(&Stone, Create_Land_Node(100, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y, 2, true, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 50, 2, true, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y, 2, true, false, true, 2 * BulletDamage));
    Random_Create(8, y);
}
void CreateStone9(int y) // 右侧中型石头2,可破坏
{
    Add_Land_Node(&Stone, Create_Land_Node(550, y, 2, true, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 50, 2, true, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 100, 2, true, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y, 2, true, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 50, 2, true, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 100, 2, true, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y + 50, 2, true, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(400, y + 50, 2, true, false, true, 2 * BulletDamage));
    Random_Create(9, y);
}
void CreateStone10(int y) // 左侧中型石头2,可破坏
{
    Add_Land_Node(&Stone, Create_Land_Node(100, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 50, 2, true, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(250, y + 50, 2, true, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(300, y + 50, 2, true, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(350, y + 50, 2, true, false, true, 2 * BulletDamage));
    Random_Create(10, y);
}
void CreateStone11(int y) // 右侧小型石头1,不可破坏
{
    Add_Land_Node(&Stone, Create_Land_Node(550, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 50, 1, false, false, false, 2 * BulletDamage));
    Random_Create(11, y);
}
void CreateStone12(int y) // 左侧小型石头1,不可破坏
{
    Add_Land_Node(&Stone, Create_Land_Node(100, y, 1, false, false, true, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 50, 1, false, false, false, 2 * BulletDamage));
    Random_Create(12, y);
}
void CreatePlat1(int y)
{
    Add_Land_Node(&Land, Create_Land_Node(100, y + 200, 1, false, true, true, 1));
    Add_Land_Node(&Land, Create_Land_Node(350, y + 400, 1, false, true, true, 1));
    Add_Land_Node(&Land, Create_Land_Node(250, y + 650, 1, false, true, true, 1));
}
void CreatePlat2(int y)
{
    Add_Land_Node(&Land, Create_Land_Node(400, y + 200, 1, false, true, true, 1));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 400, 1, false, false, true, 2));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 400, 1, false, false, true, 2));
    Add_Land_Node(&Stone, Create_Land_Node(200, y + 400, 1, false, false, true, 2));
    Add_Land_Node(&Land, Create_Land_Node(200, y + 650, 1, false, true, true, 1));
}
void CreatePrepare1(int y)
{
    Add_Land_Node(&Stone, Create_Land_Node(100, y - 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y - 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(100, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y - 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(150, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(200, y, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(250, y, 2, true, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(300, y, 2, true, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(350, y, 2, true, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(400, y, 2, true, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(500, y - 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y - 50, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y + 100, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(550, y - 100, 1, false, false, false, 2 * BulletDamage));
}
void CreatePrepare2(int y)
{
    Add_Land_Node(&Stone, Create_Land_Node(200, y, 2, true, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(250, y, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(300, y, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(350, y, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(400, y, 1, false, false, false, 2 * BulletDamage));
    Add_Land_Node(&Stone, Create_Land_Node(450, y, 2, true, false, false, 2 * BulletDamage));
}

void Random_Create(int i, int y)
{
    int r = rand() % 3 + 1;
    switch (r)
    {
    case 1:
        CreatePlatform(arr1[i - 1], y + arr2[i - 1]);
        Add_Land_Node(&Stone, Create_Land_Node(arr3[i - 1], y + arr4[i - 1], 2, true, false, true, 2 * BulletDamage));
        Add_Land_Node(&Stone, Create_Land_Node(arr5[i - 1], y + arr6[i - 1], 2, true, false, true, 2 * BulletDamage));
        break;
    case 2:
        CreatePlatform(arr7[i - 1], y + arr8[i - 1]);
        Add_Land_Node(&Stone, Create_Land_Node(arr9[i - 1], y + arr10[i - 1], 2, true, false, true, 2 * BulletDamage));
        CreatePlatform(arr11[i - 1], y + arr12[i - 1]);
        break;
    case 3:
        CreatePlatform(arr13[i - 1], y + arr14[i - 1]);
        Add_Land_Node(&Stone, Create_Land_Node(arr15[i - 1], y + arr16[i - 1], 2, true, false, true, 2 * BulletDamage));
        Add_Land_Node(&Stone, Create_Land_Node(arr17[i - 1], y + arr18[i - 1], 2, true, false, true, 2 * BulletDamage));
        Add_Land_Node(&Stone, Create_Land_Node(arr19[i - 1], y + arr20[i - 1], 2, true, false, false, 2 * BulletDamage));
        break;
    default:
        break;
    }
}
void RandomBarriar(int y)
{
    int r1 = rand() % 11 + 1;
    int r2 = rand() % 11 + 1;
    while (r2 == 9)
        r2 = rand() % 11 + 1;
    switch (rand() % 10)
    {
    case 0:
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, 1, 1, 100 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, r2, 1, 350 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        break;
    case 1:
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, 1, 1, 150 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, r2, 1, 400 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        break;
    case 2:
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, r1, 1, 200 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, 1, 1, 450 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        break;
    case 3:
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, r1, 1, 100 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, r2, 1, 450 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        break;
    case 4:
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, r1, 1, 150 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, 1, 1, 500 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        break;
    case 5:
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, r1, 1, 300 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, r1, 1, 500 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        break;
    case 6:
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, 1, 1, 300 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, r1, 1, 500 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        break;
    case 7:
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, 1, 1, 300 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, r1, 1, 500 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        break;
    case 8:
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, r1, 1, 300 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, 1, 1, 500 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        break;
    case 9:
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, r1, 1, 300 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        Add_Enemy_Node(&Enemy, Create_Enemy_Node(101, 1, 0, 0, 0, 1, 1, 500 + rand() % 50, y + rand() % 100, false, false, false, 4, 1, 1));
        break;
    default:
        break;
    }
}
void BodySkill()
{
    Mix_PlayChannel(-1, music26, 0);
    if (rand() % 2 == 0)
    {
        ENEMY *Now1 = Create_Enemy_Node(1, 1, 1, 20, 0, 1, 2, 350, Boss->EnemyRect.y + 100, false, false, false, 7, 129, 20);
        Now1->Angle = 0;
        Add_Enemy_Node(&Enemy3, Now1);
        ENEMY *Now2 = Create_Enemy_Node(1, 1, 1, 20, 0, 1, 2, 350, Boss->EnemyRect.y + 100, false, false, false, 7, 129, 20);
        Now2->Angle = 40;
        Add_Enemy_Node(&Enemy3, Now2);
        ENEMY *Now3 = Create_Enemy_Node(1, 1, 1, 20, 0, 1, 2, 350, Boss->EnemyRect.y + 100, false, false, false, 7, 129, 20);
        Now3->Angle = -40;
        Add_Enemy_Node(&Enemy3, Now3);
        if (BOSS_BLOOD < 1000)
        {
            ENEMY *Now4 = Create_Enemy_Node(1, 1, 1, 20, 0, 1, 2, 350, Boss->EnemyRect.y + 100, false, false, false, 7, 129, 20);
            Now4->Angle = 20;
            Add_Enemy_Node(&Enemy3, Now4);
            ENEMY *Now5 = Create_Enemy_Node(1, 1, 1, 20, 0, 1, 2, 350, Boss->EnemyRect.y + 100, false, false, false, 7, 129, 20);
            Now5->Angle = -20;
            Add_Enemy_Node(&Enemy3, Now5);
        }
    }
    else
    {
        if (BOSS_BLOOD < 1000)
            for (int i = 1; i <= 5; i++)
                CreateEnemy12(300 + rand() % 100, Boss->EnemyRect.y + rand() % 100);
        else
            for (int i = 1; i <= 3; i++)
                CreateEnemy12(300 + rand() % 100, Boss->EnemyRect.y + rand() % 100);
    }
}

void MoveNode(OB **List)
{
    if (*List == NULL)
        return;
    OB *Now = *List;
    while (Now != NULL)
    {
        if (Now->Toward_X)
            Now->ObjectRect.x += Now->Speed;
        else
        {
            Now->ObjectRect.y += Now->Speed;
            Now->Distance -= Now->Speed;
            if ((Now->Distance < 0 && Now->Speed > 0) || (Now->Distance > 0 && Now->Speed < 0))
            {
                Now->Hp = 0;
            }
        }
        if (Now->Which != Now->Type && (Now->Type < 20))
        {
            Now->Which += 1;
        }
        else if ((Now->Which == Now->Type && (Now->Type < 20)) && (!Now->AppearLoop))
        {
            Now->Hp = 0;
        }
        Now = Now->Next;
    }
    RemoveNode(List);
}
void MoveNodePlus(OB **List, bool Toward_X, int Speed)
{
    if (*List == NULL)
        return;
    OB *Now = *List;
    while (Now != NULL)
    {
        if (Toward_X)
            Now->ObjectRect.x += Speed;
        else
        {
            Now->ObjectRect.y += Speed;
            Now->Distance -= Speed;
        }
        Now = Now->Next;
    }
}
void Move_Fire_Node(OB **List, int Distance)
{
    if (*List == NULL)
        return;
    OB *Now = *List;
    while (Now != NULL)
    {
        Now->ObjectRect.y += Distance;
        if (Now->Which != Now->Type)
        {
            Now->Which += 1;
        }
        else if ((Now->Which == Now->Type && (!Now->AppearLoop)) || Now->ObjectRect.y < -720)
        {
            Now->Hp = 0;
        }
        Now = Now->Next;
    }
    RemoveNode(List);
}
void Move_Land_Node(LAND **List, int Distance)
{
    bool remove = false;
    if (*List == NULL)
        return;
    LAND *Now = *List;
    while (Now != NULL)
    {
        Now->LandRect.y += Distance;
        if (Now->LandRect.y < -400)
        {
            Now->Hp = 0;
            remove = true;
        }
        Now = Now->Next;
    }
    if (remove)
        Remove_Land_Node(List);
}
void Move_Enemy_Node(ENEMY **List, int Distance)
{
    if (*List == NULL)
        return;
    ENEMY *Now = *List;
    while (Now != NULL)
    {
        Now->EnemyRect.y += Distance;
        Now->EnemyRect.y += Now->Distance;
        Now->Distance = 0;
        if (Now->EnemyRect.y < -300 || (Now->EnemyRect.y > 1000 && Now->DropRun) || (Now->EnemyRect.y > 1200 && Now->Droping))
        {
            Now->Hp = 0;
            if (Now->Which_Enemy == 24)
            {
                HaveItems = false;
                ItemsOnSale = NULL;
                Buying = false;
                ItemIndex = 0;
                WhichNPC = 0;
            }
            if (Now->Which_Enemy == 22)
            {
                HaveChat = false;
                HaveRob = false;
                WhichNPC = 0;
            }
        }
        if (!Start_BOSS)
        {
            if (Now->Which != Now->Type && Now->Delay == 2)
            {
                Now->Which += 1;
                Now->Delay = 0;
            }
            else if (Now->Which == Now->Type && Now->Delay == 2)
            {
                Now->Which = 1;
                Now->Delay = 0;
            }
            else
                Now->Delay += 1;
        }
        if (Now->EnemyRect.y < 1000 || Start_BOSS)
        {
            if (!(Now->CanFly) && (!(Now->CanJump)) && (Now->Speed != 0))
            {
                if (Now->EnemyRect.y < 320 && (Now->EnemyRect.x < Me_X + 150 || Now->EnemyRect.x > Me_X - 150))
                    Now->DropRun = true;
                if (Now->DropRun)
                    Enemy_Drop_Run(Now);
                else
                    Enemy_Normal_Run(Now);
            }
            else if (!(Now->CanFly) && (!(Now->CanJump)) && Now->Speed == 0)
            {
                if (Now->EnemyRect.x <= Me_X)
                    Now->Turn_To_Right = true;
                else
                    Now->Turn_To_Right = false;
            }
            else if (Now->CanFly && Now->EnemyRect.y < 720)
            {
                Enemy_Fly(Now);
                if (Now->Which_Enemy == 9)
                {
                    if (Now->ActionDelay == 30)
                    {
                        if (Me_X + 10 < Now->EnemyRect.x && Me_Y < Now->EnemyRect.y - 60)
                            CreateEnemy11(Now->EnemyRect.x, Now->EnemyRect.y - 60, -90 + (atan((Now->EnemyRect.y - 60 - Me_Y) / (Now->EnemyRect.x - Me_X - 10))) / 3.1416 * 180);
                        else if (Me_X + 10 < Now->EnemyRect.x && Me_Y > Now->EnemyRect.y - 60)
                            CreateEnemy11(Now->EnemyRect.x, Now->EnemyRect.y - 60, -90 - (atan((Me_Y - Now->EnemyRect.y + 60) / (Now->EnemyRect.x - Me_X - 10))) / 3.1416 * 180);
                        else if (Me_X + 10 > Now->EnemyRect.x && Me_Y < Now->EnemyRect.y - 60)
                            CreateEnemy11(Now->EnemyRect.x, Now->EnemyRect.y - 60, 90 - (atan((Now->EnemyRect.y - 60 - Me_Y) / (Me_X + 10 - Now->EnemyRect.x))) / 3.1416 * 180);
                        else if (Me_X + 10 > Now->EnemyRect.x && Me_Y > Now->EnemyRect.y - 60)
                            CreateEnemy11(Now->EnemyRect.x, Now->EnemyRect.y - 60, 90 + (atan((Me_Y - Now->EnemyRect.y + 60) / (Me_X + 10 - Now->EnemyRect.x))) / 3.1416 * 180);
                        else if (Me_X == Now->EnemyRect.x && Me_Y > Now->EnemyRect.y - 60)
                            CreateEnemy11(Now->EnemyRect.x, Now->EnemyRect.y - 60, 180);
                        else if (Me_X == Now->EnemyRect.x && Me_Y < Now->EnemyRect.y - 60)
                            CreateEnemy11(Now->EnemyRect.x, Now->EnemyRect.y - 60, 0);
                        Mix_PlayChannel(-1, music16, 0);
                        Now->ActionDelay = 0;
                    }
                    else
                        Now->ActionDelay += 1;
                }
            }
            else if (Now->CanJump)
            {
                if (Now->Which_Enemy > 0 && ((Now->EnemyRect.y < 700 && (!Now->Droping)) || Now->Droping))
                {
                    if ((Now->ActionDelay == 30 && Now->Which_Enemy == 3) || (Now->ActionDelay == 20 && Now->Which_Enemy == 5) || (Now->ActionDelay == 10 && Now->Which_Enemy == 7))
                    {
                        if (Now->EnemyRect.y < 630)
                        {
                            if (Now->EnemyRect.x < Me_X)
                                Now->Speed *= (Now->Speed < 0 ? -1 : 1);
                            else
                                Now->Speed *= (Now->Speed < 0 ? 1 : -1);
                        }
                        else
                        {
                            if (Now->EnemyRect.x > Me_X)
                                Now->Speed *= (Now->Speed < 0 ? -1 : 1);
                            else
                                Now->Speed *= (Now->Speed < 0 ? 1 : -1);
                        }
                        Now->ActionDelay += 1;
                        Enemy_Jump(Now);
                    }
                    else if ((Now->ActionDelay == 31 && Now->Which_Enemy == 3) || (Now->ActionDelay == 21 && Now->Which_Enemy == 5) || (Now->ActionDelay == 11 && Now->Which_Enemy == 7))
                    {
                        if (!Now->Droping)
                        {
                            Now->ActionDelay = 0;
                            Now->JumpSpeed = 20;
                            Now->Speed = 8;
                        }
                        else
                            Enemy_Jump(Now);
                    }
                    else if (Now->ActionDelay < 30 && (!Now->Droping))
                        Now->ActionDelay += 1;
                }
                else if (Now->Which_Enemy < 0)
                {
                    Enemy_Jump(Now);
                    if (!Now->Droping)
                    {
                        Now->JumpSpeed = 0;
                        Now->Speed = 0;
                    }
                }
            }
        }
        Now = Now->Next;
    }
    Remove_Enemy_Node(List);
}
void Enemy_Normal_Run(ENEMY *Now)
{
    Update_E1_Point(Now);
    Judge_Enemy_Turn(&Stone, &Land, Now);
    if (Now->Turn_To_Right)
        Now->EnemyRect.x += Now->Speed;
    else
        Now->EnemyRect.x -= Now->Speed;
}
void Enemy_Drop_Run(ENEMY *Now)
{
    Update_E1_Point(Now);
    if (Judge_Enemy_Drop(&Stone, &Land, Now))
        Now->Distance += 10;
    if (Now->EnemyRect.x < Me_X && Now->EnemyRect.x + Now->Speed < Me_X)
    {
        Now->Turn_To_Right = true;
        Now->EnemyRect.x += Now->Speed;
    }
    else if (Now->EnemyRect.x > Me_X && Now->EnemyRect.x - Now->Speed > Me_X)
    {
        Now->Turn_To_Right = false;
        Now->EnemyRect.x -= Now->Speed;
    }
}
void Enemy_Fly(ENEMY *Now)
{
    int InitX = Now->EnemyRect.x;
    int InitY = Now->EnemyRect.y;
    bool modify = false;
    if ((Me_X + 10) > (Now->EnemyRect.x + 20) && (Me_Y + 10) > Now->EnemyRect.y)
    {
        Now->Turn_To_Right = true;
        Now->EnemyRect.x += (Now->Speed * cos(atan(((Me_Y + 11) - Now->EnemyRect.y) / ((Me_X + 10.0) - (Now->EnemyRect.x + 20)))));
        Now->EnemyRect.y += (Now->Speed * sin(atan(((Me_Y + 11) - Now->EnemyRect.y) / ((Me_X + 10.0) - (Now->EnemyRect.x + 20)))));
    }
    else if ((Me_X + 10) > (Now->EnemyRect.x + 20) && (Me_Y + 10) <= Now->EnemyRect.y)
    {
        Now->Turn_To_Right = true;
        Now->EnemyRect.x += (Now->Speed * cos(atan((Now->EnemyRect.y - (Me_Y + 11)) / ((Me_X + 10.0) - (Now->EnemyRect.x + 20)))));
        Now->EnemyRect.y -= (Now->Speed * sin(atan((Now->EnemyRect.y - (Me_Y + 11)) / ((Me_X + 10.0) - (Now->EnemyRect.x + 20)))));
    }
    else if ((Me_X + 10) < (Now->EnemyRect.x + 20) && (Me_Y + 10) >= Now->EnemyRect.y)
    {
        Now->Turn_To_Right = false;
        Now->EnemyRect.x -= (Now->Speed * cos(atan(((Me_Y + 11) - Now->EnemyRect.y) / ((Now->EnemyRect.x + 20) - (Me_X + 10.0)))));
        Now->EnemyRect.y += (Now->Speed * sin(atan(((Me_Y + 11) - Now->EnemyRect.y) / ((Now->EnemyRect.x + 20) - (Me_X + 10.0)))));
    }
    else if ((Me_X + 10) < (Now->EnemyRect.x + 20) && (Me_Y + 10) < Now->EnemyRect.y)
    {
        Now->Turn_To_Right = false;
        Now->EnemyRect.x -= (Now->Speed * cos(atan((Now->EnemyRect.y - (Me_Y + 11)) / ((Now->EnemyRect.x + 20) - (Me_X + 10.0)))));
        Now->EnemyRect.y -= (Now->Speed * sin(atan((Now->EnemyRect.y - (Me_Y + 11)) / ((Now->EnemyRect.x + 20) - (Me_X + 10.0)))));
    }
    else if ((Me_X + 10) == (Now->EnemyRect.x + 20) && (Me_Y + 10) < Now->EnemyRect.y)
    {
        Now->Turn_To_Right = false;
        Now->EnemyRect.y -= Now->Speed;
    }
    else if ((Me_X + 10) == (Now->EnemyRect.x + 20) && (Me_Y + 10) >= Now->EnemyRect.y)
    {
        Now->Turn_To_Right = false;
        Now->EnemyRect.y += Now->Speed;
    }
    if (Now->Which_Enemy == 6)
    {
        LAND *LandNow = Stone;
        while (LandNow != NULL)
        {
            if (LandNow->LandRect.y > 1500)
                break;
            if (SDL_HasIntersection(&(LandNow->LandRect), &(Now->EnemyRect)))
            {
                Now->EnemyRect.x = InitX;
                Now->EnemyRect.y = InitY;
                break;
            }
            LandNow = LandNow->Next;
        }
    }
    else
    {
        Update_E2_Point(Now);
        switch (Judge_Enemy_Crash(&Stone, Now))
        {
        case 1:
        case 2:
            if (Now->EnemyRect.y == InitY)
                Now->EnemyRect.y -= 3;
            else
                Now->EnemyRect.y = InitY;
            modify = true;
            break;
        case 3:
            if (Now->EnemyRect.x == InitX)
                Now->EnemyRect.x += 3;
            else
                Now->EnemyRect.x = InitX;
            modify = true;
            break;
        case 4:
            if (Now->EnemyRect.x == InitX)
                Now->EnemyRect.x -= 3;
            else
                Now->EnemyRect.x = InitX;
            modify = true;
            break;
        case 5:
        case 6:
            if (Now->EnemyRect.y == InitY)
                Now->EnemyRect.y += 3;
            else
                Now->EnemyRect.y = InitY;
            modify = true;
            break;
        default:
            break;
        }
        if (modify)
        {
            Update_E2_Point(Now);
            switch (Judge_Enemy_Crash(&Stone, Now))
            {
            case 1:
            case 2:
                if (Now->EnemyRect.y == InitY)
                    Now->EnemyRect.y -= 3;
                else
                    Now->EnemyRect.y = InitY;
                break;
            case 3:
                if (Now->EnemyRect.x == InitX)
                    Now->EnemyRect.x += 3;
                else
                    Now->EnemyRect.x = InitX;
                break;
            case 4:
                if (Now->EnemyRect.x == InitX)
                    Now->EnemyRect.x -= 3;
                else
                    Now->EnemyRect.x = InitX;
                break;
            case 5:
            case 6:
                if (Now->EnemyRect.y == InitY)
                    Now->EnemyRect.y += 3;
                else
                    Now->EnemyRect.y = InitY;
                break;
            default:
                break;
            }
        }
    }
}
void Enemy_Jump(ENEMY *Now)
{
    Now->Droping = true;
    Now->EnemyRect.x += Now->Speed;
    Now->EnemyRect.y -= Now->JumpSpeed;
    if (Now->JumpSpeed > -30)
        Now->JumpSpeed -= 3;
    if (Now->JumpSpeed < -30)
        Now->JumpSpeed = -30;
    Judge_Enemy_Drop(&Stone, &Land, Now);
    if (Now->EnemyRect.x < 100)
        Now->EnemyRect.x = 100;
    if (Now->EnemyRect.x > 600 - Now->W)
        Now->EnemyRect.x = 600 - Now->W;
}
void Move_Enemy_Bullet(ENEMY **List, int Distance)
{
    if (*List == NULL)
        return;
    ENEMY *Now = *List;
    while (Now != NULL)
    {
        Now->EnemyRect.y += Distance;
        if (Now->EnemyRect.y < 750 || Start_BOSS)
        {
            if (Now->Which_Enemy == 10 && SDL_HasIntersection(&Back3Rect, &(Now->EnemyRect)))
            {
                if (Now->ActionDelay == 4)
                {
                    Mix_PlayChannel(-1, music15, 0);
                    Now->ActionDelay = 0;
                }
                else
                    Now->ActionDelay += 1;
            }
            if (-90 <= Now->Angle && Now->Angle <= 0)
            {
                Now->EnemyRect.x -= sin(-Now->Angle * 0.01745) * Now->Speed;
                Now->EnemyRect.y -= cos(-Now->Angle * 0.01745) * Now->Speed;
            }
            else if (0 < Now->Angle && Now->Angle <= 90)
            {
                Now->EnemyRect.x += sin(Now->Angle * 0.01745) * Now->Speed;
                Now->EnemyRect.y -= cos(Now->Angle * 0.01745) * Now->Speed;
            }
            else if (-180 < Now->Angle && Now->Angle < -90)
            {
                Now->EnemyRect.x -= cos((-Now->Angle - 90) * 0.01745) * Now->Speed;
                Now->EnemyRect.y += sin((-Now->Angle - 90) * 0.01745) * Now->Speed;
            }
            else if (90 < Now->Angle && Now->Angle <= 180)
            {
                Now->EnemyRect.x += cos((Now->Angle - 90) * 0.01745) * Now->Speed;
                Now->EnemyRect.y += sin((Now->Angle - 90) * 0.01745) * Now->Speed;
            }
        }
        if ((Now->EnemyRect.y < -500 || Now->EnemyRect.x > 1250 || Now->EnemyRect.x < -500) && Now->Which_Enemy == 10)
            Now->Hp = 0;
        if (Now->Which_Enemy == 11 && (Now->EnemyRect.x < 100 || Now->EnemyRect.x > 585 || Now->EnemyRect.y < -70 || Now->EnemyRect.y > 720))
        {
            Now->Hp = 0;
            CreateDie(Now->EnemyRect.x, Now->EnemyRect.y);
        }
        if (Start_BOSS && (Now->EnemyRect.x < -100 || Now->EnemyRect.x > 700 || Now->EnemyRect.y < -100))
            Now->Hp = 0;
        Now = Now->Next;
    }
    Remove_Enemy_Node(List);
}
void Move_Boss(ENEMY **List, int Distance)
{
    if (*List == NULL)
        return;
    ENEMY *Now = *List;
    while (Now != NULL)
    {
        if (BOSS_BLOOD < 4000)
            Now->Speed = 12;
        if (BOSS_BLOOD < 2000)
            Now->Speed = 16;
        Now->EnemyRect.y += Distance;
        if (Now->Which != Now->Type && Now->Delay == 3)
        {
            Now->Which += 1;
            Now->Delay = 0;
        }
        else if (Now->Which == Now->Type && Now->Delay == 3)
        {
            Now->Which = 1;
            Now->Delay = 0;
        }
        else
            Now->Delay += 1;
        Now->EnemyRect.y += Now->Speed;
        Drop_Distance += Now->Speed;
        Body_Distance += Now->Speed;
        if (Drop_Distance > 150)
        {
            RandomBarriar(Now->EnemyRect.y);
            Drop_Distance = 0;
        }
        if (Body_Distance > 700)
        {
            BodySkill();
            Body_Distance = 0;
        }
        Now = Now->Next;
    }
}
void Move_BossItem(ENEMY **List, int Distance)
{
    if (*List == NULL)
        return;
    ENEMY *Now = *List;
    while (Now != NULL)
    {
        Now->EnemyRect.y += Distance;
        Now->EnemyRect.y += Now->Speed;
        if (Now->EnemyRect.y < -50 && Now->Which_Enemy != 0)
            Now->Hp = 0;
        if (Now->Which_Enemy == 0)
        {
            if (BOSS_BLOOD < 4000)
                Now->Speed = 12;
            if (BOSS_BLOOD < 2000)
                Now->Speed = 16;
            if (Now->Hp == 1)
                Eye_Distance1 += Now->Speed;
            if (Now->Hp == 2)
                Eye_Distance2 += Now->Speed;
            if (Eye_Distance1 > 600 || Eye_Distance2 > 600)
            {
                if (BOSS_BLOOD < 3000)
                    Mix_PlayChannel(-1, music27, 1);
                else
                    Mix_PlayChannel(-1, music27, 0);
                if (Now->Hp == 1)
                    Eye_Distance1 = 0;
                if (Now->Hp == 2)
                    Eye_Distance2 = 0;
                if (Me_X + 10 < Now->EnemyRect.x + 60 && Me_Y < Now->EnemyRect.y)
                    CreateEnemy13(Now->EnemyRect.x + 60, Now->EnemyRect.y, -90 + (atan((Now->EnemyRect.y - Me_Y) / (Now->EnemyRect.x + 60 - Me_X - 10))) / 3.1416 * 180);
                else if (Me_X + 10 > Now->EnemyRect.x + 60 && Me_Y < Now->EnemyRect.y)
                    CreateEnemy13(Now->EnemyRect.x + 60, Now->EnemyRect.y, 90 - (atan((Now->EnemyRect.y - Me_Y) / (Me_X + 10 - Now->EnemyRect.x - 60))) / 3.1416 * 180);
                else if (Me_X + 10 == Now->EnemyRect.x + 60 && Me_Y < Now->EnemyRect.y)
                    CreateEnemy13(Now->EnemyRect.x + 60, Now->EnemyRect.y, 0);
                if (BOSS_BLOOD < 3000)
                {
                    if (Me_X + 10 < Now->EnemyRect.x + 60 && Me_Y < Now->EnemyRect.y + 50)
                        CreateEnemy13(Now->EnemyRect.x + 60, Now->EnemyRect.y + 50, -90 + (atan((Now->EnemyRect.y + 50 - Me_Y) / (Now->EnemyRect.x + 60 - Me_X - 10))) / 3.1416 * 180);
                    else if (Me_X + 10 > Now->EnemyRect.x + 60 && Me_Y < Now->EnemyRect.y + 50)
                        CreateEnemy13(Now->EnemyRect.x + 60, Now->EnemyRect.y + 50, 90 - (atan((Now->EnemyRect.y + 50 - Me_Y) / (Me_X + 10 - Now->EnemyRect.x - 60))) / 3.1416 * 180);
                    else if (Me_X + 10 == Now->EnemyRect.x + 60 && Me_Y < Now->EnemyRect.y + 50)
                        CreateEnemy13(Now->EnemyRect.x + 60, Now->EnemyRect.y + 50, 0);
                }
                if (BOSS_BLOOD < 1000)
                {
                    if (Me_X + 10 < Now->EnemyRect.x + 60 && Me_Y < Now->EnemyRect.y + 100)
                        CreateEnemy13(Now->EnemyRect.x + 60, Now->EnemyRect.y + 100, -90 + (atan((Now->EnemyRect.y + 100 - Me_Y) / (Now->EnemyRect.x + 60 - Me_X - 10))) / 3.1416 * 180);
                    else if (Me_X + 10 > Now->EnemyRect.x + 60 && Me_Y < Now->EnemyRect.y + 100)
                        CreateEnemy13(Now->EnemyRect.x + 60, Now->EnemyRect.y + 100, 90 - (atan((Now->EnemyRect.y + 100 - Me_Y) / (Me_X + 10 - Now->EnemyRect.x - 60))) / 3.1416 * 180);
                    else if (Me_X + 10 == Now->EnemyRect.x + 60 && Me_Y < Now->EnemyRect.y + 100)
                        CreateEnemy13(Now->EnemyRect.x + 60, Now->EnemyRect.y + 100, 0);
                }
            }
        }
        Now = Now->Next;
    }
    Remove_Enemy_Node(List);
}

void ReLoadList()
{
    for (int i = 1; i < 13; i++)
    {
        Items[i]->Next = NULL;
        Items[i]->Index = 0;
    }
}

void Judge_Enemy_Turn(LAND **List1, LAND **List2, ENEMY *E)
{
    LAND *Now1 = *List1;
    LAND *Now2 = *List2;
    bool inRect1 = false;
    bool inRect2 = false;
    while (Now1 != NULL)
    {
        if (Now1->LandRect.y < 780)
        {
            if (SDL_PointInRect(&(E->Point3), &(Now1->LandRect)))
            {
                E->Turn_To_Right = true;
                return;
            }
            if (SDL_PointInRect(&(E->Point4), &(Now1->LandRect)))
            {
                E->Turn_To_Right = false;
                return;
            }
            if (SDL_PointInRect(&(E->Point1), &(Now1->LandRect)))
                inRect1 = true;
            if (SDL_PointInRect(&(E->Point2), &(Now1->LandRect)))
                inRect2 = true;
        }
        if (Now1->LandRect.y > 1500)
            break;
        Now1 = Now1->Next;
    }
    while (Now2 != NULL)
    {
        if (Now2->LandRect.y < 780)
        {
            if (SDL_PointInRect(&(E->Point3), &(Now2->LandRect)))
            {
                E->Turn_To_Right = true;
                return;
            }
            if (SDL_PointInRect(&(E->Point4), &(Now2->LandRect)))
            {
                E->Turn_To_Right = false;
                return;
            }
            if (SDL_PointInRect(&(E->Point1), &(Now2->LandRect)))
                inRect1 = true;
            if (SDL_PointInRect(&(E->Point2), &(Now2->LandRect)))
                inRect2 = true;
        }
        if (Now2->LandRect.y > 1500)
            break;
        Now2 = Now2->Next;
    }
    if (inRect1 && inRect2)
        return;
    else
        E->Turn_To_Right = !(E->Turn_To_Right);
}
bool Judge_Enemy_Drop(LAND **List1, LAND **List2, ENEMY *E)
{
    LAND *Now1 = *List1;
    LAND *Now2 = *List2;
    bool inRect1 = false;
    bool inRect2 = false;
    while (Now1 != NULL)
    {
        if (Now1->LandRect.y < 1200)
        {
            if (SDL_PointInRect(&(E->Point3), &(Now1->LandRect)))
                E->EnemyRect.x += (E->Speed > 0 ? E->Speed : (-E->Speed));
            if (SDL_PointInRect(&(E->Point4), &(Now1->LandRect)))
                E->EnemyRect.x -= (E->Speed > 0 ? E->Speed : (-E->Speed));
            Update_E1_Point(E);
            if (E->JumpSpeed <= 0)
            {
                if (SDL_PointInRect(&(E->Point1), &(Now1->LandRect)))
                {
                    E->Distance = (Now1->LandRect.y - E->EnemyRect.y - E->H);
                    inRect1 = true;
                    break;
                }
                if (SDL_PointInRect(&(E->Point2), &(Now1->LandRect)))
                {
                    E->Distance = (Now1->LandRect.y - E->EnemyRect.y - E->H);
                    inRect2 = true;
                    break;
                }
            }
        }
        if (Now1->LandRect.y > 2000)
            break;
        Now1 = Now1->Next;
    }
    while (Now2 != NULL)
    {
        if (Now2->LandRect.y < 1200)
        {
            if (SDL_PointInRect(&(E->Point3), &(Now2->LandRect)))
                E->EnemyRect.x += (E->Speed > 0 ? E->Speed : (-E->Speed));
            if (SDL_PointInRect(&(E->Point4), &(Now2->LandRect)))
                E->EnemyRect.x -= (E->Speed > 0 ? E->Speed : (-E->Speed));
            Update_E1_Point(E);
            if (E->JumpSpeed <= 0)
            {
                if (SDL_PointInRect(&(E->Point1), &(Now2->LandRect)))
                {
                    E->Distance = (Now2->LandRect.y - E->EnemyRect.y - E->H);
                    inRect1 = true;
                    break;
                }
                if (SDL_PointInRect(&(E->Point2), &(Now2->LandRect)))
                {
                    E->Distance = (Now2->LandRect.y - E->EnemyRect.y - E->H);
                    inRect2 = true;
                    break;
                }
            }
        }
        if (Now2->LandRect.y > 2000)
            break;
        Now2 = Now2->Next;
    }
    E->EnemyRect.y += E->Distance;
    E->Distance = 0;
    if (inRect1 || inRect2)
    {
        E->Droping = false;
        return false;
    }
    else
    {
        E->Droping = true;
        return true;
    }
}
int Judge_Enemy_Crash(LAND **List, ENEMY *E)
{
    LAND *Now = *List;
    while (Now != NULL)
    {
        if (Now->LandRect.y < 1000)
        {
            if (SDL_PointInRect(&(E->Point3), &(Now->LandRect)))
                return 3;
            if (SDL_PointInRect(&(E->Point4), &(Now->LandRect)))
                return 4;
            if (SDL_PointInRect(&(E->Point1), &(Now->LandRect)))
                return 1;
            if (SDL_PointInRect(&(E->Point2), &(Now->LandRect)))
                return 2;
            if (SDL_PointInRect(&(E->Point5), &(Now->LandRect)))
                return 5;
            if (SDL_PointInRect(&(E->Point6), &(Now->LandRect)))
                return 6;
        }
        if (Now->LandRect.y > 1300)
            break;
        Now = Now->Next;
    }
    return 0;
}
void Update_E1_Point(ENEMY *Now)
{
    Now->Point1.x = Now->EnemyRect.x + 5;
    Now->Point1.y = Now->EnemyRect.y + Now->H + 5;
    Now->Point2.x = Now->EnemyRect.x + Now->W - 5;
    Now->Point2.y = Now->EnemyRect.y + Now->H + 5;
    Now->Point3.x = Now->EnemyRect.x - 3;
    Now->Point3.y = Now->EnemyRect.y + 10;
    Now->Point4.x = Now->EnemyRect.x + Now->W + 3;
    Now->Point4.y = Now->EnemyRect.y + 10;
}
void Update_E2_Point(ENEMY *Now)
{
    Now->Point1.x = Now->EnemyRect.x + 3;
    Now->Point1.y = Now->EnemyRect.y + Now->H + 2;
    Now->Point2.x = Now->EnemyRect.x + Now->W - 3;
    Now->Point2.y = Now->EnemyRect.y + Now->H + 2;
    Now->Point3.x = Now->EnemyRect.x - 3;
    Now->Point3.y = Now->EnemyRect.y + 10;
    Now->Point4.x = Now->EnemyRect.x + Now->W + 3;
    Now->Point4.y = Now->EnemyRect.y + 10;
    Now->Point5.x = Now->EnemyRect.x + 3;
    Now->Point5.y = Now->EnemyRect.y - 2;
    Now->Point6.x = Now->EnemyRect.x + Now->W - 3;
    Now->Point6.y = Now->EnemyRect.y - 2;
}

void Judge_BulletOnLand(OB **List, LAND **SList)
{
    if (*List == NULL) // 只用检测前三个子弹
        return;
    OB *Now = *List;
    if ((!Judge_Start) && (Judge_BackLeft || (!Judge_BackLeft && Now->ObjectRect.x < 100 && (!Back_Changing)) || Back_Changing))
    {
        for (int i = 1; i <= 3; i++)
        {
            if (Now == NULL)
                break;
            if (Now->ObjectRect.y >= 380)
            {
                Now->Hp = 0;
                CreateFire(Now->ObjectRect.x, 380 + 20);
                Now = Now->Next;
            }
        }
    }
    else if (Judge_Start)
    {
        while (Now != NULL)
        {
            LAND *SNow = *SList;
            while (SNow != NULL)
            {
                if (SNow->LandRect.y > 1500)
                    break;
                else
                {
                    if (SDL_HasIntersection(&(Now->ObjectRect), &(SNow->LandRect)))
                    {
                        Now->Hp = 0;
                        CreateFire(Now->ObjectRect.x, SNow->LandRect.y);
                        if (SNow->BeDamaged)
                            SNow->Hp -= Now->Damage;
                        if (SNow->Hp <= 0)
                            Mix_PlayChannel(-1, music8, 0);
                        break;
                    }
                }
                SNow = SNow->Next;
            }
            Now = Now->Next;
        }
    }
    RemoveNode(List);
    Remove_Land_Node(SList);
}
void Judge_MeOnEnemy(ENEMY **List)
{
    ENEMY *Now = *List;
    UpdatePointJ();
    while (Now != NULL)
    {
        if ((!BeingInvicible) || Start_BOSS)
        {
            if (Now->EnemyRect.y < 500)
            {
                if (SDL_PointInRect(&Point_D, &(Now->EnemyRect)) || SDL_PointInRect(&Point_DD, &(Now->EnemyRect)) || SDL_PointInRect(&Point_DDD, &(Now->EnemyRect)))
                {
                    if ((Now->Which_Enemy != 10 && Now->Which_Enemy != 11) || (Now->Which_Enemy != 3 && Start_BOSS))
                    {
                        if (Bullet_Consume != 0)
                            Mix_PlayChannel(-1, music4, 0);
                        Bullet_Consume = 0;
                        CreateDie(Now->EnemyRect.x, Now->EnemyRect.y);
                        if (Now->Which_Enemy != 4 && !Start_BOSS)
                            CreateMoney(Now->EnemyRect.x + Now->W / 2, Now->EnemyRect.y + Now->H / 2);
                        if (Now->Which_Enemy != 0)
                            Now->Hp = 0;
                        Judge_Jump = true;
                        Jump_Speed = 6;
                    }
                    if (Start_BOSS && Now->Which == 1 && Now->Which_Enemy == 101 && (Me_HP < 4 || (!StoreHeart && Buff[7])))
                        Add_Enemy_Node(&Money, Create_Enemy_Node(-5, 1, 0, 0, 20, 1, 1, Now->EnemyRect.x, Now->EnemyRect.y, false, true, false, 19, 27, 30));
                    if (((((Now->Which_Enemy == 3 || Now->Which_Enemy == 6) && (!Buff[1])) || Now->Which_Enemy == 10 || Now->Which_Enemy == 11) && !Start_BOSS) || Now->Which_Enemy == 0 || (Start_BOSS && (Now->Which_Enemy == 1 || Now->Which_Enemy == 2 || Now->Which_Enemy == 3)))
                    {
                        if (Now->Which_Enemy == 0)
                        {
                            Jump_Speed = 40;
                            Judge_Jump = true;
                        }
                        if (!Buff[5] && !BeingInvicible)
                        {
                            Me_HP -= 1;
                            Mix_PlayChannel(-1, music12, 0);
                            if (StoreHeart)
                            {
                                Me_HP += 1;
                                Mix_PlayChannel(-1, music24, 0);
                                StoreHeart = false;
                            }
                            InvicibleTime = MaxInvicibleTime;
                            BeingInvicible = true;
                            if (Buff[8])
                                CreateStar();
                        }
                        else if (Buff[5] && !BeingInvicible)
                        {
                            int r = rand() % 4;
                            if (0 <= r && r < 3)
                            {
                                Me_HP -= 1;
                                if (StoreHeart)
                                {
                                    Me_HP += 1;
                                    Mix_PlayChannel(-1, music24, 0);
                                    StoreHeart = false;
                                }
                                Mix_PlayChannel(-1, music12, 0);
                                InvicibleTime = MaxInvicibleTime;
                                BeingInvicible = true;
                                if (Buff[8])
                                    CreateStar();
                            }
                        }
                    }
                    if (Now->Which_Enemy == 4 && !Start_BOSS)
                    {
                        Add_Enemy_Node(&Enemy3, Create_DropEnemy_Node(5, 30, 1, 0, 0, 1, 2, Now->EnemyRect.x, Now->EnemyRect.y + 50, false, true, false, 3, 34, 50));
                    }
                    switch (Now->Music)
                    {
                    case 1:
                        Mix_PlayChannel(-1, music9, 0);
                        break;
                    case 2:
                        Mix_PlayChannel(-1, music10, 0);
                        break;
                    case 3:
                        Mix_PlayChannel(-1, music11, 0);
                        break;
                    case 4:
                        Mix_PlayChannel(-1, music13, 0);
                        break;
                    case 5:
                        Mix_PlayChannel(-1, music14, 0);
                        break;
                    case 6:
                        Mix_PlayChannel(-1, music25, 0);
                        break;
                    case 7:
                        Mix_PlayChannel(-1, music29, 0);
                        break;
                    default:
                        break;
                    }
                }
                else if (SDL_HasIntersection(&MeRect, &(Now->EnemyRect)))
                {
                    if (!Start_BOSS)
                    {
                        Now->Hp = 0;
                        if (!Buff[5])
                        {
                            Me_HP -= 1;
                            if (StoreHeart)
                            {
                                Me_HP += 1;
                                Mix_PlayChannel(-1, music24, 0);
                                StoreHeart = false;
                            }
                            Mix_PlayChannel(-1, music12, 0);
                            InvicibleTime = MaxInvicibleTime;
                            BeingInvicible = true;
                            if (Buff[8])
                                CreateStar();
                        }
                        else
                        {
                            int r = rand() % 4;
                            if (0 <= r && r < 3)
                            {
                                Me_HP -= 1;
                                Mix_PlayChannel(-1, music12, 0);
                                InvicibleTime = MaxInvicibleTime;
                                if (Buff[8])
                                    CreateStar();
                            }
                        }
                        Judge_Jump = true;
                        Jump_Speed = 6;
                        switch (Now->Music)
                        {
                        case 1:
                            Mix_PlayChannel(-1, music9, 0);
                            break;
                        case 2:
                            Mix_PlayChannel(-1, music10, 0);
                            break;
                        case 3:
                            Mix_PlayChannel(-1, music11, 0);
                            break;
                        case 4:
                            Mix_PlayChannel(-1, music13, 0);
                            break;
                        case 5:
                            Mix_PlayChannel(-1, music14, 0);
                            break;
                        case 6:
                            Mix_PlayChannel(-1, music25, 0);
                            break;
                        default:
                            break;
                        }
                    }
                    else if ((Now->Which_Enemy == 0 || Now->Which_Enemy == 3 || Now->Which_Enemy == 2 || Now->Which_Enemy == 1) && !BeingInvicible)
                    {
                        if (Now->Which_Enemy == 0)
                            Jump_Speed = 40;
                        else
                            Jump_Speed = 6;
                        Judge_Jump = true;
                        if (!Buff[5])
                        {
                            Me_HP -= 1;
                            if (StoreHeart)
                            {
                                Me_HP += 1;
                                Mix_PlayChannel(-1, music24, 0);
                                StoreHeart = false;
                            }
                            Mix_PlayChannel(-1, music12, 0);
                            InvicibleTime = MaxInvicibleTime;
                            BeingInvicible = true;
                            if (Buff[8])
                                CreateStar();
                        }
                        else
                        {
                            int r = rand() % 4;
                            if (0 <= r && r < 3)
                            {
                                Me_HP -= 1;
                                Mix_PlayChannel(-1, music12, 0);
                                InvicibleTime = MaxInvicibleTime;
                                BeingInvicible = true;
                                if (Buff[8])
                                    CreateStar();
                            }
                        }
                    }
                }
            }
        }
        Now = Now->Next;
    }
}

void Judge_BulletOnEnemy(OB **List, ENEMY **EList)
{
    if (*List == NULL)
        return;
    OB *Now = *List;
    while (Now != NULL)
    {
        ENEMY *ENow = *EList;
        while (ENow != NULL)
        {
            if (ENow->EnemyRect.y < 720)
            {
                if (ENow->Which_Enemy == 0)
                    if ((230 < Now->ObjectRect.x && Now->ObjectRect.x < 280 && Now->ObjectRect.y + 40 < ENow->EnemyRect.y + 70) || (430 < Now->ObjectRect.x && Now->ObjectRect.x < 470 && Now->ObjectRect.y + 40 < ENow->EnemyRect.y + 70))
                    {
                        ENow = ENow->Next;
                        continue;
                    }
                if (SDL_HasIntersection(&(Now->ObjectRect), &(ENow->EnemyRect)))
                {
                    if (!Buff[12] || ENow->Which_Enemy == 0)
                        Now->Hp = 0;
                    CreateFire(Now->ObjectRect.x, ENow->EnemyRect.y);
                    if (ENow->Which_Enemy != 0)
                        ENow->Hp -= Now->Damage;
                    else
                        BOSS_BLOOD -= Now->Damage;
                    if (ENow->Hp <= 0)
                    {
                        CreateDie(ENow->EnemyRect.x, ENow->EnemyRect.y);
                        if (ENow->Which_Enemy == 4)
                            Add_Enemy_Node(&Enemy3, Create_DropEnemy_Node(5, 30, 1, 0, 0, 1, 2, ENow->EnemyRect.x, ENow->EnemyRect.y + 50, false, true, false, 3, 34, 50));
                        else
                            CreateMoney(ENow->EnemyRect.x + ENow->W / 2, ENow->EnemyRect.y + ENow->H / 2);
                        switch (ENow->Music)
                        {
                        case 1:
                            Mix_PlayChannel(-1, music9, 0);
                            break;
                        case 2:
                            Mix_PlayChannel(-1, music10, 0);
                            break;
                        case 3:
                            Mix_PlayChannel(-1, music11, 0);
                            break;
                        case 4:
                            Mix_PlayChannel(-1, music13, 0);
                            break;
                        case 5:
                            Mix_PlayChannel(-1, music14, 0);
                            break;
                        case 6:
                            Mix_PlayChannel(-1, music25, 0);
                            break;
                        case 7:
                            Mix_PlayChannel(-1, music29, 0);
                            break;
                        default:
                            break;
                        }
                        Remove_Enemy_Node(EList);
                    }
                    break;
                }
            }
            ENow = ENow->Next;
        }
        Now = Now->Next;
    }
    RemoveNode(List);
}
void Collect_Money(ENEMY **List)
{
    ENEMY *Now = *List;
    while (Now != NULL)
    {
        if (SDL_HasIntersection(&MeRect, &(Now->EnemyRect)))
        {
            Now->Hp = 0;
            if (Now->Which_Enemy != -5)
                switch (rand() % 4 + 1)
                {
                case 1:
                    Mix_PlayChannel(-1, music19, 0);
                    break;
                case 2:
                    Mix_PlayChannel(-1, music20, 0);
                    break;
                case 3:
                    Mix_PlayChannel(-1, music21, 0);
                    break;
                case 4:
                    Mix_PlayChannel(-1, music22, 0);
                    break;
                case 5:
                    Mix_PlayChannel(-1, music23, 0);
                    break;
                default:
                    break;
                }
            if (Now->Which_Enemy == -4)
                Cu += 1;
            else if (Now->Which_Enemy == -3)
                Ag += 1;
            else if (Now->Which_Enemy == -2)
                Au += 1;
            else if (Now->Which_Enemy == -5)
            {
                if (Me_HP < MAXHP)
                {
                    Me_HP += 1;
                    Mix_PlayChannel(-1, music24, 0);
                }
                else if (Buff[7] && !(StoreHeart))
                {
                    StoreHeart = true;
                }
                else
                    Now->Hp = 1;
            }
        }
        Now = Now->Next;
        if (Cu == 100)
        {
            Ag += 1;
            Cu = 0;
        }
        if (Ag == 100)
        {
            Au += 1;
            Ag = 0;
        }
        if (Au == 100)
        {
            Pt += 1;
            Au = 0;
        }
    }
    Remove_Enemy_Node(List);
}
int Judge_CanInteract(ENEMY **List)
{
    if (*List == NULL)
        return 0;
    ENEMY *Now = *List;
    while (Now != NULL)
    {
        if (Now->EnemyRect.y < 720)
        {
            if (SDL_HasIntersection(&MeRect, &(Now->EnemyRect)))
            {
                CanInteract = true;
                if (!Interacting)
                {
                    if (Now->Which_Enemy == 22 || Now->Which_Enemy == 24)
                        LOAD_TIP(Now->EnemyRect.x + Now->W, Now->EnemyRect.y - 30);
                    else if (Now->Which_Enemy == 21)
                        LOAD_CHAT3(280, Now->EnemyRect.y - 40);
                    else if (Now->Which_Enemy == 23)
                        LOAD_CHAT4(300, Now->EnemyRect.y - 40);
                }
                return (Now->Which_Enemy - 20);
            }
            else
                CanInteract = false;
        }
        Now = Now->Next;
    }
    return 0;
}

void ControlFPS(clock_t StartTime)
{
    FDurTime = clock() - StartTime;
    if (FDurTime < 40)
        SDL_Delay(40 - FDurTime);
    FDurTime = clock() - StartTime;
}

void Free(SDL_Surface *SurList[], int n)
{
    for (int i = 0; i <= n; i++)
    {
        SDL_FreeSurface(SurList[i]);
    }
}

void QUIT()
{
    SDL_DestroyWindow(Window);
    SDL_DestroyRenderer(Renderer);
    TTF_CloseFont(MoneyFont);
    TTF_CloseFont(ScoreFont);
    for (int i = 1; i <= 11; i++)
        free(Items[i]);
    Free(BulletSurface, 1);
    Free(DieSurface, 8);
    Free(CuSurface, 8);
    Free(AgSurface, 8);
    Free(AuSurface, 8);
    Free(FireSurface, 3);
    Free(EmptySurface, 5);
    Free(LandSurface, 2);
    Free(PlatSurface, 1);
    Free(LoveSurface, 1);
    Free(StarSurface, 1);
    Free(Enemy1LSurface, 3);
    Free(Enemy1RSurface, 3);
    Free(Enemy2LSurface, 2);
    Free(Enemy2RSurface, 2);
    Free(Enemy3Surface, 2);
    Free(Enemy4Surface, 3);
    Free(NPCRSurface, 4);
    Free(NPCLSurface, 4);
    Free(ItemsSurface, 6);
    Free(TipSurface, 5);
    Free(FinallineSurface, 1);
    Free(Finalline2Surface, 1);
    Free(RandomBuffSurface, 7);
    Free(EnemyBulletSurface, 2);
    Free(BossBodySurface, 9);
    Free(BossEyeSurface, 1);
    Free(BossItemSurface, 11);
    Free(BossBulletSurface, 3);
    Mix_FreeChunk(music1);
    Mix_FreeChunk(music2);
    Mix_FreeChunk(music3);
    Mix_FreeChunk(music4);
    Mix_FreeChunk(music5);
    Mix_FreeChunk(music6);
    Mix_FreeChunk(music7);
    Mix_FreeChunk(music8);
    Mix_FreeChunk(music9);
    Mix_FreeChunk(music10);
    Mix_FreeChunk(music11);
    Mix_FreeChunk(music12);
    Mix_FreeChunk(music13);
    Mix_FreeChunk(music14);
    Mix_FreeChunk(music15);
    Mix_FreeChunk(music16);
    Mix_FreeChunk(music17);
    Mix_FreeChunk(music18);
    Mix_FreeChunk(music19);
    Mix_FreeChunk(music20);
    Mix_FreeChunk(music21);
    Mix_FreeChunk(music22);
    Mix_FreeChunk(music23);
    Mix_FreeChunk(music24);
    Mix_FreeChunk(music25);
    Mix_FreeChunk(music26);
    Mix_FreeChunk(music27);
    Mix_FreeChunk(music28);
    Mix_FreeChunk(music29);
    Mix_FreeMusic(BGM[1]);
    Mix_FreeMusic(BGM[2]);
    Mix_FreeMusic(BGM[3]);
    Mix_FreeMusic(BGM[4]);
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}