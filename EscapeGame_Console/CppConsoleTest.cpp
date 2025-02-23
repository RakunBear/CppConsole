#include <iostream>
#include <conio.h>

/*
    탈출게임
    10 * 10 이차원 배열로 맵 만듦. (출력)
    WASD 키로 플레이어 (0)가 상하좌우로 이동
    플레이어 시작지점 {0, 0}, 탈출지점은 {0, 0} 을 제외한 랜덤설정
    플레이어가 탈출지점에 도착하면 "탈출에 성공했습니다. 승리!" 출력하고 종료.
    ++
    1.난이도 선택 기능 추가(easy, normal, hard)
        1_1. 맵사이즈 변화
        1_2. 적 수량 증가
    2. 적 등장 및 이동
*/
//---------------------------

int main()
{
    srand(time(NULL));
    const int RIMIT_LOOP = 10;
    const int LEVEL_SIZE = 3;

    const int MAP_EASY = 10;
    const int MAP_NORMAL = 15;
    const int MAP_HARD = 20;

    const int MOPCOUNT_EASY = 3;
    const int MOPCOUNT_NORMAL = 10;
    const int MOPCOUNT_HARD = 20;

    const int PLAYER_BASEPOS = 0;
    // { MAP_SIZE, MONSTER_COUNT }
    const int mapLevelArr[LEVEL_SIZE][2] = {
        { MAP_EASY, MOPCOUNT_EASY },
        { MAP_NORMAL, MOPCOUNT_NORMAL },
        { MAP_HARD, MOPCOUNT_HARD } };

    const char TILE_F = '.';
    const char TILE_P = 'O';
    const char TILE_E = 'E';
    const char TILE_M = 'M';
    const char TILE_X = 'X';

    const char* const UI_TITLE =
        "==== Welcom Escape Miro ====\n" \
        "[[ Plz, Select Level of Difficulty ]]\n";
    const char* const UI_TITLE_SELECT[LEVEL_SIZE] = {
        "->  EASY      \n"
        "    NORMAL    \n"
        "    HARD      \n",

        "    EASY      \n"
        "->  NORMAL    \n"
        "    HARD      \n",

        "    EASY      \n"
        "    NORMAL    \n"
        "->  HARD      \n"
    };
    const char* const UI_LEVEL[LEVEL_SIZE] = {
        "LEVEL: EASY    ",
        "LEVEL: NORMAL  ",
        "LEVEL: HARD    "
    };

    char inputKey;

    char** map;
    int mapSize;

    int* monsterPosX, * monsterPosY;
    int monsterCount;

    int exitPosX, exitPosY;

    int playerPosX, playerPosY;
    char playerCurTile = TILE_F;
    int curLevel;

    // Title : Selete Level
    {
        bool isPass = false;
        curLevel = 0;
        do {
            // <<Show UI>>
            std::cout << UI_TITLE;
            std::cout << UI_TITLE_SELECT[curLevel];
            // <<User Input>>
            inputKey = _getch();
            std::cout << inputKey;
            // <<Check Key>>
            switch (inputKey)
            {
            case 'a': case 'A':
            case 'w': case 'W':
                if (curLevel > 0)
                {
                    --curLevel;
                }
                break;
            case 'd': case 'D':
            case 's': case 'S':
                if (curLevel < LEVEL_SIZE - 1)
                {
                    ++curLevel;
                }
                break;
            case ' ': case '\n':
                isPass = true;
                break;
            default:
                break;
            }

            // <<Clear Window>>
            system("cls");
        } while (!isPass);

        // <<Set Level>>
        mapSize = mapLevelArr[curLevel][0];
        monsterCount = mapLevelArr[curLevel][1];
    }

    //===========

    // Before Gaem Loop
    // <<Set Map>>
    map = new char* [mapSize];
    for (int i = 0; i < mapSize; ++i)
    {
        map[i] = new char[mapSize];
    }

    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            map[i][j] = TILE_F;
        }
    }

    // Set Player (Player should first be set to position {0,0})
    playerPosX = PLAYER_BASEPOS;
    playerPosY = PLAYER_BASEPOS;
    map[PLAYER_BASEPOS][PLAYER_BASEPOS] = TILE_P;

    // Set ExitPos
    do {
        exitPosX = rand() * 12 % mapSize;
        exitPosY = rand() * 12 % mapSize;
    } while (map[exitPosY][exitPosX] != TILE_F);
    map[exitPosY][exitPosX] = TILE_E;

    // Set MonsterPos
    monsterPosX = new int[monsterCount];
    monsterPosY = new int[monsterCount];

    for (int i = 0; i < monsterCount; ++i)
    {
        do {
            monsterPosX[i] = rand() * 21 % mapSize;
            monsterPosY[i] = rand() * 21 % mapSize;
        } while (map[monsterPosY[i]][monsterPosX[i]] != TILE_F);
        map[monsterPosY[i]][monsterPosX[i]] = TILE_M;
    }

    //==========
    // Game Loop
    while (true)
    {
        // <<Show UI>>
        std::cout << UI_LEVEL[curLevel];
        std::cout << "Cur Tile : " << playerCurTile << "\n";

        // <<Draw Map>>
        for (int i = 0; i < mapSize; ++i)
        {
            for (int j = 0; j < mapSize; ++j)
            {
                std::cout << map[i][j];
            }
            std::cout << "\n";
        }

        // <<Check Collision with Player>>
        if (playerCurTile == TILE_M)
        {
            map[playerPosY][playerPosX] = TILE_X;
            std::cout << "몬스터에게 잡혔다!.\n 패배;\n";
            break;
        }

        // <<Check Exit with Player>>
        if (playerPosX == exitPosX && playerPosY == exitPosY)
        {
            std::cout << "탈출에 성공했습니다.\n 승리!\n";
            break;
        }

        // <<Set MonsterMove>>
        {
            int dir, count = 0;
            for (int i = 0; i < monsterCount; ++i)
            {
                count = 0;
                for (int j = 0; j < RIMIT_LOOP; ++j)
                {
                    ++count;
                    dir = rand() * 12 % 5;
                    if (dir == 0)
                    {
                        break;
                    }
                    else if (dir == 1) // Up
                    {
                        if (monsterPosY[i] <= 0 || (map[monsterPosY[i] - 1][monsterPosX[i]] != TILE_F))
                            continue;

                        map[monsterPosY[i]][monsterPosX[i]] = TILE_F;
                        --monsterPosY[i];
                    }
                    else if (dir == 2) // Left
                    {
                        if (monsterPosX[i] <= 0 || (map[monsterPosY[i]][monsterPosX[i] - 1] != TILE_F))
                            continue;

                        map[monsterPosY[i]][monsterPosX[i]] = TILE_F;
                        --monsterPosX[i];
                    }
                    else if (dir == 3) // Down
                    {
                        if ((monsterPosY[i] >= mapSize - 1) || (map[monsterPosY[i] + 1][monsterPosX[i]] != TILE_F))
                            continue;

                        map[monsterPosY[i]][monsterPosX[i]] = TILE_F;
                        ++monsterPosY[i];
                    }
                    else if (dir == 4) // Right
                    {
                        if ((monsterPosX[i] >= mapSize - 1) || (map[monsterPosY[i]][monsterPosX[i] + 1] != TILE_F))
                            continue;

                        map[monsterPosY[i]][monsterPosX[i]] = TILE_F;
                        ++monsterPosX[i];
                    }

                    break;
                }

                map[monsterPosY[i]][monsterPosX[i]] = TILE_M;
            }
        }

        // <<Set Player Move>>
        {
            bool isPass;
            do {
                isPass = true;
                inputKey = _getch();
                switch (inputKey)
                {
                case 'W':
                case 'w':
                    if (playerPosY > 0)
                    {
                        map[playerPosY][playerPosX] = TILE_F;
                        --playerPosY;
                        playerCurTile = map[playerPosY][playerPosX];
                        map[playerPosY][playerPosX] = TILE_P;
                    }
                    break;
                case 'A':
                case 'a':
                    if (playerPosX > 0)
                    {
                        map[playerPosY][playerPosX] = TILE_F;
                        --playerPosX;
                        playerCurTile = map[playerPosY][playerPosX];
                        map[playerPosY][playerPosX] = TILE_P;

                    }
                    break;
                case 'S':
                case 's':
                    if (playerPosY < mapSize - 1)
                    {
                        map[playerPosY][playerPosX] = TILE_F;
                        ++playerPosY;
                        playerCurTile = map[playerPosY][playerPosX];
                        map[playerPosY][playerPosX] = TILE_P;
                    }
                    break;
                case 'D':
                case 'd':
                    if (playerPosX < mapSize - 1)
                    {
                        map[playerPosY][playerPosX] = TILE_F;
                        ++playerPosX;
                        playerCurTile = map[playerPosY][playerPosX];
                        map[playerPosY][playerPosX] = TILE_P;
                    }
                    break;
                default:
                    isPass = false;
                    break;
                }
            } while (!isPass);
        }

        // <<Clear Windown>>
        system("cls");
    }

    // ===========
    // Finishing Work
    // <<Delete Memory>>
    delete[] monsterPosX;
    delete[] monsterPosY;

    for (int i = 0; i < mapSize; ++i)
    {
        delete[] map[i];
    }
    delete[] map;

    return 0;
}