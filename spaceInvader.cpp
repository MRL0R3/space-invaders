#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <fstream>
#include <iomanip>
using namespace std;

int difficulty = 0; // 0 برای عادی، 1 برای سخت
int numBullets = 1;
int screenWidth = 100;       // عرض جدید کنسول
int screenHeight = 40;       // ارتفاع جدید کنسول
const int maxEnemies = 9;    // تعداد کل دشمن‌ها (3 ردیف × 3 ستون)
const int enemySpacing = 15; // فاصله بین دشمن‌ها در هر ردیف
const int NORMAL = 0;
const int HARD = 1;

struct Player
{
    int x, y;
    const char *design[2];
    int width;
    int lives; // تعداد جان‌ها
    int score;
    string username;
};

Player player[100];

// تعریف استراکت برای تیر
struct Bullet
{
    int x, y;
    int isActive;
};
// تعریف استراکت برای دشمن
struct Enemy
{
    int x, y;
    bool active;
    const char *design[3];       // طراحی جدید برای انمی‌ها که کوچکتر است
    Bullet bullet;               // هر دشمن یک تیر دارد
    unsigned long lastShotTime;  // زمان آخرین شلیک
    const int shootCooldown = 0; // زمان خنثی‌سازی در میلی‌ثانیه
};
// تعریف استراکت برای سفینه
struct Spaceship
{
    int x, y;
    bool active;
    const char *design[3];
    Bullet bullet;             // Bullet for the spaceship
    unsigned long respawnTime; // Time to respawn after being hit
};

// تعریف استراکت برای موانع (Barriers)
struct Barrier
{
    int x, y;
    int width = 6;     // عرض جدید
    int height = 4;    // ارتفاع جدید
    bool active;       // Indicates if the barrier is still intact
    bool blocks[6][4]; // Represents individual blocks of the barrier (width x height)
};
void gotoxy(int x, int y);
// تعریف استراکت برای امتیاز
struct Score
{
    int points;

    void draw()
    {
        gotoxy(screenWidth - 20, 1);
        cout << "Score: " << points;
    }

    void increase(int amount)
    {
        points += amount;
        draw();
    }
};

void setColor(int color);
void moveCursor(int x, int y);
// تابع برای تنظیم اندازه کنسول
void setConsoleSize(int width, int height);
// تابع برای جابه‌جایی مکان مکان‌نما
void gotoxy(int x, int y);
// تابع برای دریافت ابعاد کنسول
void getConsoleSize();
// تابع برای رسم دورچین
void drawBoundary();
// تعریف استراکت برای بازیکن// تابع برای رسم بازیکن
void drawPlayer(struct Player *player);
// تابع برای پاک کردن بازیکن از صفحه
void erasePlayer(struct Player *player);
// تابع برای حرکت بازیکن
void movePlayer(struct Player *player, char direction);
// تابع برای شلیک تیر
void shootBullet(struct Player *player, struct Bullet *bullet);
// تابع برای حرکت تیر
void moveBullet(struct Bullet *bullet);
// تابع برای رسم دشمنان
void drawEnemies(Enemy enemies[], int maxEnemies);
// تابع برای حرکت دشمنان
void moveEnemies(Enemy enemies[], int maxEnemies, int &direction);
// تابع برای شلیک تیر دشمن به صورت تصادفی
void enemyShoot(Enemy enemies[], int maxEnemies);
// تابع برای حرکت تیر دشمن‌ها
void moveEnemyBullets(Enemy enemies[], int maxEnemies);
// تابع برای بررسی برخورد تیر با دشمنان
bool checkCollisionWithEnemies(struct Bullet *bullet, Enemy enemies[], int maxEnemies, Score &score);
// تابع برای بررسی برخورد تیرهای دشمن با بازیکن
bool checkCollisionWithPlayerBullets(struct Player *player, Enemy enemies[], int maxEnemies);
// تابع برای بررسی برخورد دشمن با بازیکن
bool checkCollisionWithPlayer(struct Player *player, Enemy enemies[], int maxEnemies);
// تابع برای رسم فضاپیما
void drawSpaceship(Spaceship &spaceship);
// تابع برای پاک کردن فضاپیما
void eraseSpaceship(Spaceship &spaceship);
// تابع برای حرکت فضاپیما
void moveSpaceship(Spaceship &spaceship);
// تابع برای اسپاون فضاپیما
void spawnSpaceship(Spaceship &spaceship, unsigned long &lastSpawnTime);
// تابع برای حذف فضاپیما بعد از رسیدن به آخر صفحه
void checkAndRemoveSpaceship(Spaceship &spaceship);
// تابع برای بررسی برخورد تیرهای فضاپیما با بازیکن
// Function to check collision with the spaceship
bool checkCollisionWithSpaceship(struct Bullet *bullet, Spaceship &spaceship, Score &score);
// تابع برای بررسی برخورد تیرهای دشمن با موانع
bool checkBulletCollisionWithBarrier(Bullet *bullet, Barrier &barrier);
// تابع برای رسم موانع
void drawBarrier(Barrier &barrier);
// تابع برای بررسی اینکه آیا فضاپیما باید شلیک کند
void checkSpaceshipShoot(Spaceship &spaceship, unsigned long &lastShootTime);
void moveSpaceshipBullet(Spaceship &spaceship, Player &player);
void hideCursor();
void gameOverAnimation(int screenWidth, int screenHeight);
bool allEnemiesDestroyed(Enemy enemies[], int maxEnemies);
// Function to check collision between spaceship bullets and the player
bool checkSpaceshipBulletCollisionWithPlayer(Spaceship &spaceship, Player &player);
// Function to check collision between spaceship bullets and barriers
bool checkSpaceshipBulletCollisionWithBarrier(Spaceship &spaceship, Barrier barriers[], int numBarriers);
void printLogo();
void printCentered(const string &str, int width);
void clearScreen();
void printCenteredText(const string &str, int width);
void mainMenu(int selected);
void pauseMenu(int selected);
// انتخاب درجه سختی
int selectDifficulty();
void newGame();
void loadGame();
void showLeaderboard();
void howToPlay();
void setDifficulty(int difficultyLevel);
void show_game(Barrier barrier[], struct Player *player, Spaceship &spaceship);
void loadLeaderboard(const string &filename, Player leaderboard[], int &count, int maxEntries);
void savePlayerData(const string &filename, const Player &player);
void bublesort(Player leaderboard[], int count);
bool checkEnemyCollisionWithBarrier(Enemy enemies[], int maxEnemies, Barrier barriers[], int numBarriers);
void spawnSpaceship(Spaceship &spaceship, unsigned long &lastSpawnTime, bool &spaceshipDestroyed, unsigned long &lastDestructionTime);
// تابع اصلی

int main()
{

    // Example of enemy bullet settings
    SetConsoleOutputCP(CP_UTF8);
    hideCursor();
    srand(static_cast<unsigned int>(time(0))); // Initialize random seed
    setConsoleSize(800, 600);                  // Setting console size
    getConsoleSize();
    system("cls");

    srand(time(0));

    int selected = 0;
    bool running = true;

    clearScreen();
    printLogo();

    cout << "\nPress ENTER to enter the main menu..." << endl;

    while (_getch() != '\r')
        ;
    system("cls");

    while (running)
    {
        clearScreen();
        printLogo();
        mainMenu(selected);

        char key = _getch();

        if (key == 'w' || key == 'W' || key == 72)
        {
            selected = (selected - 1 + 6) % 6;
        }
        else if (key == 's' || key == 'S' || key == 80)
        {
            selected = (selected + 1) % 6;
        }
        else if (key == '\r')
        {
            if (selected == 0)
            {
                system("cls");
                newGame();
            }
            else if (selected == 1)
            {
                showLeaderboard();
            }
            else if (selected == 2)
            {
                system("cls");
                howToPlay();
            }
            else if (selected == 3)
            {
                running = false; // Exit game
            }
            system("pause");
        }
        else if (key == 27)
        { // If ESC is pressed
            int pauseSelected = 0;
            system("cls");
            while (true)
            {

                // pauseMenu(pauseSelected); // نمایش منوی Pause

                moveCursor(0, 0);

                char pauseKey = _getch();

                if (pauseKey == 'w' || pauseKey == 'W' || pauseKey == 72)
                {
                    pauseSelected = (pauseSelected - 1 + 3) % 3;
                }
                else if (pauseKey == 's' || pauseKey == 'S' || pauseKey == 80)
                {
                    pauseSelected = (pauseSelected + 1) % 3;
                }
                else if (pauseKey == '\r')
                {
                    if (pauseSelected == 0)
                    {

                        // Logic for saving game goes here
                        mainMenu(selected);
                        running = false; // Exit the game after saving
                        return 0;
                    }
                    else if (pauseSelected == 1)
                    {
                        cout << "Restarting game..." << endl;
                        system("cls");
                        newGame(); // Restart the game by calling newGame function
                    }
                    else if (pauseSelected == 2)
                    {
                        system("cls");
                        break; // Resume the game and go back to the game loop
                    }
                }
            }
        }
    }

    const char *playerDesign[] = {
        "  █  ",
        "█████"};

    struct Player player = {30, screenHeight - 4, {playerDesign[0], playerDesign[1]}, 5, 3}; // number of player lives = 3
    struct Bullet bullet = {0, 0, 0};
    Score score = {0}; // امتیاز اولیه 0

    // طراحی مختلف برای ردیف‌ها
    const char *enemyDesign1[] = {
        "  ███  ",
        " █████ ",
        "  ███  ",
    };

    const char *enemyDesign2[] = {
        "  ███ ",
        " ████ ",
        " ███  ",
    };

    const char *enemyDesign3[] = {
        " ███   ",
        " █████ ",
        "   ███ ",
    };

    // ایجاد 9 دشمن (3 ردیف × 3 ستون) با فاصله بیشتر بین ردیف‌ها
    Enemy enemies[maxEnemies] = {
        {screenWidth / 6, 10, true, {enemyDesign1[0], enemyDesign1[1], enemyDesign1[2]}, {0, 0, 0}, 0},
        {screenWidth / 6 + enemySpacing, 10, true, {enemyDesign2[0], enemyDesign2[1], enemyDesign2[2]}, {0, 0, 0}, 0},
        {screenWidth / 6 + 2 * enemySpacing, 10, true, {enemyDesign3[0], enemyDesign3[1], enemyDesign3[2]}, {0, 0, 0}, 0},
        {screenWidth / 6, 14, true, {enemyDesign1[0], enemyDesign1[1], enemyDesign1[2]}, {0, 0, 0}, 0},
        {screenWidth / 6 + enemySpacing, 14, true, {enemyDesign2[0], enemyDesign2[1], enemyDesign2[2]}, {0, 0, 0}, 0},
        {screenWidth / 6 + 2 * enemySpacing, 14, true, {enemyDesign3[0], enemyDesign3[1], enemyDesign3[2]}, {0, 0, 0}, 0},
        {screenWidth / 6, 18, true, {enemyDesign1[0], enemyDesign1[1], enemyDesign1[2]}, {0, 0, 0}, 0},
        {screenWidth / 6 + enemySpacing, 18, true, {enemyDesign2[0], enemyDesign2[1], enemyDesign2[2]}, {0, 0, 0}, 0},
        {screenWidth / 6 + 2 * enemySpacing, 18, true, {enemyDesign3[0], enemyDesign3[1], enemyDesign3[2]}, {0, 0, 0}, 0}};

    // تعریف سفینه جدید
    Spaceship spaceship = {0, 0, false, {"  ███  ", " █████", "   █  "}, {0, 0, 0}, 0}; // Initialize bullet as inactive
    unsigned long lastSpawnTime = 0;                                                    // زمان آخرین اسپاون
    unsigned long lastShootTime = 0;                                                    // زمان آخرین شلیک فضاپیما
    unsigned long lastDestructionTime = 0;                                              // زمان آخرین تخریب سفینه
    const int respawnDelay = 60;                                                        // 60 seconds for respawn delay

    // Initialize barriers
    // Initialize barriers
    const int numBarriers = 3; // Number of barriers
    Barrier barriers[numBarriers] = {
        {20, screenHeight - 10, 6, 4, true, {true}}, // Barrier at position (20, height - 10) with width 6 and height 4
        {40, screenHeight - 10, 6, 4, true, {true}}, // Barrier at position (40, height - 10) with width 6 and height 4
        {60, screenHeight - 10, 6, 4, true, {true}}  // Barrier at position (60, height - 10) with width 6 and height 4
    };

    return 0;
}

void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void moveCursor(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// تابع برای تنظیم اندازه کنسول
void setConsoleSize(int width, int height)
{
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); // Get current console window size
    MoveWindow(console, r.left, r.top, width, height, TRUE);

    // Set the console buffer size
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT sr;
    sr.Left = 0;
    sr.Top = 0;
    sr.Right = width / 8 - 1;    // Considering average character width
    sr.Bottom = height / 16 - 1; // Considering average character height
    SetConsoleWindowInfo(hConsole, TRUE, &sr);

    // Set the console screen buffer size
    COORD coord;
    coord.X = sr.Right + 1;
    coord.Y = sr.Bottom + 1;
    SetConsoleScreenBufferSize(hConsole, coord);
}

// تابع برای جابه‌جایی مکان مکان‌نما
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// تابع برای دریافت ابعاد کنسول
void getConsoleSize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

// تابع برای رسم دورچین
void drawBoundary()
{
    gotoxy(0, 0);
    cout << "╔";
    for (int x = 1; x < screenWidth - 1; ++x)
    {
        cout << "═";
    }
    cout << "╗";

    gotoxy(0, screenHeight - 1);
    cout << "╚";
    for (int x = 1; x < screenWidth - 1; ++x)
    {
        cout << "═";
    }
    cout << "╝";

    for (int y = 1; y < screenHeight - 1; ++y)
    {
        gotoxy(0, y);
        cout << "║";
        gotoxy(screenWidth - 1, y);
        cout << "║";
    }
}

// تابع برای رسم بازیکن
void drawPlayer(struct Player *player)
{
    for (int i = 0; i < 2; i++)
    {
        gotoxy(player->x, player->y + i);
        cout << player->design[i];
    }
}

// تابع برای پاک کردن بازیکن از صفحه
void erasePlayer(struct Player *player)
{
    for (int i = 0; i < 2; i++)
    {
        gotoxy(player->x, player->y + i);
        cout << "                    "; // فضای خالی برای پاک کردنb
    }
}

// تابع برای حرکت بازیکن
void movePlayer(struct Player *player, char direction)
{
    if (direction == 'a' || direction == 75 || direction == 'A') // Move left
    {
        if (player->x > 1) // Ensure player doesn't go outside the left boundary
        {
            erasePlayer(player);
            player->x--;
            drawPlayer(player);
        }
    }
    else if (direction == 'd' || direction == 77 || direction == 'D') // Move right
    {
        if (player->x + player->width < screenWidth - 1) // Ensure player doesn't go outside the right boundary
        {
            erasePlayer(player);
            player->x++;
            drawPlayer(player);
        }
    }
    int counter = 0;
    while (_kbhit())

    {
        if (counter % 5 == 0)
        {
            Sleep(1);
        }
        counter++;
        erasePlayer(player);
        drawPlayer(player);
        char ch = _getch();
        if (ch == 'a' || ch == 'd' || ch == 75 || ch == 77 || ch == 'A' || ch == 'D')
        {
            if (direction == 'a' || direction == 75 || direction == 'A') // Move left
            {

                if (player->x > 1) // Ensure player doesn't go outside the left boundary
                {
                    erasePlayer(player);
                    player->x--;
                    drawPlayer(player);
                }
            }
            else if (direction == 'd' || direction == 77 || direction == 'D') // Move right
            {
                if (player->x + player->width < screenWidth - 1) // Ensure player doesn't go outside the right boundary
                {
                    erasePlayer(player);
                    player->x++;
                    drawPlayer(player);
                }
            }
        }
    }
}
// تابع برای شلیک تیر
void shootBullet(struct Player *player, struct Bullet *bullet)
{
    bullet->x = player->x + 2; // تیر دقیقاً به وسط سفینه شلیک می‌شود (با توجه به طراحی)
    bullet->y = player->y - 1; // موقعیت تیر در بالای سفینه
    bullet->isActive = 1;
}

// تابع برای حرکت تیر
void moveBullet(struct Bullet *bullet)
{
    if (bullet->isActive)
    {
        // Erase the bullet from its current position
        gotoxy(bullet->x, bullet->y);
        cout << " ";

        // Move the bullet up
        bullet->y--;

        // Check if the bullet is off-screen
        if (bullet->y < 1)
        {                             // Top of the screen
            bullet->isActive = false; // Deactivate the bullet
        }
        else
        {
            // Draw the bullet at its new position
            gotoxy(bullet->x, bullet->y);
            cout << "|";
        }
    }
}

// تابع برای رسم دشمنان
void drawEnemies(Enemy enemies[], int maxEnemies)
{

    for (int i = 0; i < maxEnemies; ++i)
    {
        if (enemies[i].active)
        {
            for (int j = 0; j < 3; ++j)
            {
                gotoxy(enemies[i].x, enemies[i].y + j);
                cout << enemies[i].design[j];
            }
        }
    }
}

// تابع برای حرکت دشمنان
void moveEnemies(Enemy enemies[], int maxEnemies, int &direction)
{
    bool reachedBoundary = false;

    // Erase enemies from their current position
    for (int i = 0; i < maxEnemies; ++i)
    {
        if (enemies[i].active)
        {
            for (int j = 0; j < 3; ++j)
            {
                gotoxy(enemies[i].x, enemies[i].y + j);
                cout << "       "; // Clear the enemy's previous position (adjust width as needed)
            }
        }
    }

    // Move enemies
    for (int i = 0; i < maxEnemies; ++i)
    {
        if (enemies[i].active)
        {
            enemies[i].x += direction; // Move in the current direction

            // If any enemy reaches the left or right boundary, mark for movement down
            if (enemies[i].x >= screenWidth - 15 || enemies[i].x <= 1)
            {
                reachedBoundary = true;
            }
        }
    }

    // If any enemy reaches the boundary, move all enemies down and change direction
    if (reachedBoundary)
    {
        for (int i = 0; i < maxEnemies; ++i)
        {
            if (enemies[i].active)
            {
                enemies[i].y += 1; // Move down
            }
        }
        direction = -direction; // Reverse direction
    }

    // Redraw enemies at new positions
    for (int i = 0; i < maxEnemies; ++i)
    {
        if (enemies[i].active)
        {
            for (int j = 0; j < 3; ++j)
            {
                gotoxy(enemies[i].x, enemies[i].y + j);
                cout << enemies[i].design[j];
            }
        }
    }
}
bool checkEnemyCollisionWithBarrier(Enemy enemies[], int maxEnemies, Barrier barriers[], int numBarriers)
{
    for (int i = 0; i < maxEnemies; ++i)
    {
        if (enemies[i].active)
        {
            for (int j = 0; j < numBarriers; ++j)
            {
                if (barriers[j].active)
                {
                    // Check if the enemy is within the barrier's bounds
                    if (enemies[i].x + 7 >= barriers[j].x && // Enemy width is 7
                        enemies[i].x <= barriers[j].x + barriers[j].width &&
                        enemies[i].y + 3 >= barriers[j].y && // Enemy height is 3
                        enemies[i].y <= barriers[j].y + barriers[j].height)
                    {

                        // Destroy the barrier
                        barriers[j].active = false;
                        // Erase the barrier from the screen
                        for (int k = 0; k < barriers[j].height; ++k)
                        {
                            gotoxy(barriers[j].x, barriers[j].y + k);
                            cout << string(barriers[j].width, ' '); // Clear the barrier
                        }

                        // Destroy the enemy
                        enemies[i].active = false;
                        // Erase the enemy from the screen
                        for (int k = 0; k < 3; ++k)
                        { // Enemy height is 3
                            gotoxy(enemies[i].x, enemies[i].y + k);
                            cout << "       "; // Clear the enemy (adjust width as needed)
                        }

                        return true; // Collision detected
                    }
                }
            }
        }
    }
    return false; // No collision detected
}
// تابع برای شلیک تیر دشمن به صورت تصادفی
void enemyShoot(Enemy enemies[], int maxEnemies)
{
    unsigned long currentTime = time(0) * 1000; // Current time in milliseconds
    for (int i = 0; i < maxEnemies; ++i)
    {
        if (enemies[i].active && !enemies[i].bullet.isActive)
        {
            // Randomly decide to shoot (e.g., 10% chance)
            if (rand() % 200 < 2) // Adjust the shooting rate here (0-9, 2 means 20% chance)
            {
                // Check cooldown
                if (currentTime - enemies[i].lastShotTime >= enemies[i].shootCooldown)
                {
                    // Set bullet position just below the enemy
                    enemies[i].bullet.x = enemies[i].x + (strlen(enemies[i].design[0]) / 2); // Center bullet over enemy
                    enemies[i].bullet.y = enemies[i].y + 3;                                  // Adjust based on enemy design height
                    enemies[i].bullet.isActive = true;
                    enemies[i].lastShotTime = currentTime; // Update last shot time
                }
            }
        }
    }
}

// تابع برای حرکت تیر دشمن‌ها
void moveEnemyBullets(Enemy enemies[], int maxEnemies)
{
    for (int i = 0; i < maxEnemies; ++i)
    {
        if (enemies[i].bullet.isActive)
        {
            gotoxy(enemies[i].bullet.x, enemies[i].bullet.y);
            cout << " ";
            enemies[i].bullet.y++;

            if (enemies[i].bullet.y >= screenHeight - 1)
            { // اگر تیر به پایین برسد
                enemies[i].bullet.isActive = 0;
            }
            else
            {
                gotoxy(enemies[i].bullet.x, enemies[i].bullet.y);
                cout << "*"; // نمایش تیر دشمن
            }
        }
    }
}

// تابع برای بررسی برخورد تیر با دشمنان
bool checkCollisionWithEnemies(struct Bullet *bullet, Enemy enemies[], int maxEnemies, Score &score)
{
    // Check if the bullet is active
    if (!bullet->isActive)
        return false;

    for (int i = 0; i < maxEnemies; ++i)
    {
        if (enemies[i].active)
        {
            for (int j = 0; j < 3; ++j) // Check each part of the enemy
            {
                int enemyX = enemies[i].x;
                int enemyY = enemies[i].y + j;

                // Check if the bullet hits any part of the enemy
                if (bullet->x >= enemyX && bullet->x < enemyX + strlen(enemies[i].design[0]) && bullet->y == enemyY)
                {
                    // Determine which row the enemy is in and assign the appropriate score
                    int row = (enemies[i].y - 10) / 4; // Calculate the row (0 for first row, 1 for second, 2 for third)
                    int points = 0;

                    switch (row)
                    {
                    case 0:
                        points = 30; // First row
                        break;
                    case 1:
                        points = 20; // Second row
                        break;
                    case 2:
                        points = 10; // Third row
                        break;
                    default:
                        points = 0; // Default (should not happen)
                        break;
                    }

                    // پاک کردن دشمن برخورد کرده
                    for (int k = 0; k < 3; ++k)
                    {
                        gotoxy(enemies[i].x, enemies[i].y + k);
                        cout << "                        "; // فضای خالی برای پاک کردن
                    }

                    enemies[i].active = false; // Deactivate the enemy
                    bullet->isActive = false;  // Deactivate the bullet
                    score.increase(points);    // Increase the score based on the row
                    return true;               // Exit after the first hit
                }
            }
        }
    }
    return false; // هیچ برخوردی شناسایی نشد
}
// تابع برای نمایش جان‌ها
void drawLives(struct Player *player)
{
    gotoxy(1, 1);
    cout << "Lives: " << player->lives;
}

// تابع برای بررسی برخورد تیرهای دشمن با بازیکن
bool checkCollisionWithPlayerBullets(struct Player *player, Enemy enemies[], int maxEnemies)
{
    for (int i = 0; i < maxEnemies; ++i)
    {
        if (enemies[i].bullet.isActive)
        {
            // Check if the bullet is within the player's x bounds
            if (enemies[i].bullet.x >= player->x && enemies[i].bullet.x <= player->x + player->width)
            {
                // Check if the bullet is within the player's y bounds
                if (enemies[i].bullet.y >= player->y && enemies[i].bullet.y <= player->y + 1) // Player height is 2 rows
                {
                    enemies[i].bullet.isActive = false; // Deactivate the bullet
                    player->lives--;                    // Reduce player lives
                    drawLives(player);                  // Update lives display
                    return true;                        // Collision detected
                }
            }
        }
    }
    return false; // No collision detected
}
// تابع برای بررسی برخورد دشمن با بازیکن
bool checkCollisionWithPlayer(struct Player *player, Enemy enemies[], int maxEnemies)
{
    for (int i = 0; i < maxEnemies; ++i)
    {
        if (enemies[i].active)
        {
            for (int j = 0; j < 3; ++j)
            {
                int enemyX = enemies[i].x;
                int enemyY = enemies[i].y + j;

                // Check if the enemy hits any part of the player
                if (player->x + 1 >= enemyX && player->x + 1 < enemyX + player->width && player->y == enemyY)
                {
                    player->lives--;
                    return true;
                }
            }
        }
    }
    return false;
}

// تابع برای رسم فضاپیما
void drawSpaceship(Spaceship &spaceship)
{
    // Draw the spaceship's body
    if (spaceship.active)
    {
        for (int i = 0; i < 3; ++i)
        {
            gotoxy(spaceship.x, spaceship.y + i);
            cout << spaceship.design[i];
        }
    }
}

// تابع برای پاک کردن فضاپیما
void eraseSpaceship(Spaceship &spaceship)
{
    if (spaceship.active)
    {
        for (int i = 0; i < 3; ++i)
        {
            gotoxy(spaceship.x, spaceship.y + i);
            cout << "                "; // فضای خالی برای پاک کردن
        }
    }
}

// تابع برای حرکت فضاپیما
void moveSpaceship(Spaceship &spaceship)
{
    if (spaceship.active)
    {
        // قبل از حرکت فضاپیما از صفحه حذف می‌شود
        eraseSpaceship(spaceship);
        spaceship.x++;            // حرکت به سمت راست
        drawSpaceship(spaceship); // دوباره رسم فضاپیما در موقعیت جدید
    }
}

// تابع برای اسپاون فضاپیما
void spawnSpaceship(Spaceship &spaceship, unsigned long &lastSpawnTime)
{
    unsigned long currentTime = time(0); // زمان جاری به ثانیه

    if (currentTime - lastSpawnTime >= 30)
    { // اگر 30 ثانیه گذشته باشد
        if (!spaceship.active)
        {
            spaceship.x = 2; // مکان ابتدایی فضاپیما
            spaceship.y = 5; // مکان ابتدایی فضاپیما
            spaceship.active = true;
            lastSpawnTime = currentTime;
        }
    }
}

// تابع برای حذف فضاپیما بعد از رسیدن به آخر صفحه
void checkAndRemoveSpaceship(Spaceship &spaceship)
{
    if (spaceship.active && spaceship.x >= screenWidth - 18)
    {                              // If spaceship reaches the end of the screen
        eraseSpaceship(spaceship); // Erase spaceship before deactivating
        spaceship.active = false;
        spaceship.bullet.isActive = false; // Deactivate spaceship's bullet
    }
}
// تابع برای بررسی برخورد تیرهای فضاپیما با بازیکن
// Function to check collision with the spaceship
bool checkCollisionWithSpaceship(struct Bullet *bullet, Spaceship &spaceship, Score &score)
{
    if (bullet->isActive && spaceship.active) // Check if bullet is active and spaceship is active
    {
        // Check if bullet is within the x bounds of the spaceship
        if (bullet->x >= spaceship.x + 2 && bullet->x <= spaceship.x + 4) // Only middle area
        {
            // Check if bullet is within the y bounds of the spaceship
            if (bullet->y >= spaceship.y && bullet->y <= spaceship.y + 2) // Check y range
            {
                // Clear the spaceship from the screen
                for (int i = 0; i < 3; ++i)
                {
                    gotoxy(spaceship.x, spaceship.y + i);
                    cout << "                "; // Clear space for spaceship
                }

                bullet->isActive = false;          // Deactivate player bullet
                spaceship.active = false;          // Deactivate spaceship (destroy it)
                spaceship.bullet.isActive = false; // Deactivate spaceship's bullet

                // Random score between 75 to 200
                int randomScore = rand() % 126 + 75; // Generates random score
                score.increase(randomScore);         // Increase score
                return true;                         // Collision detected
            }
        }
    }
    return false; // No collision detected
}

// تابع برای بررسی برخورد تیرهای دشمن با موانع
bool checkBulletCollisionWithBarrier(Bullet *bullet, Barrier &barrier)
{
    if (bullet->isActive && barrier.active) // Only check if the barrier is active
    {
        // Calculate the relative position of the bullet within the barrier
        int relativeX = bullet->x - barrier.x;
        int relativeY = bullet->y - barrier.y;

        // Check if the bullet is within the bounds of the barrier
        if (relativeX >= 0 && relativeX < barrier.width &&
            relativeY >= 0 && relativeY < barrier.height)
        {
            // Check if the specific block is active
            if (barrier.blocks[relativeX][relativeY])
            {
                // Deactivate the specific block
                barrier.blocks[relativeX][relativeY] = false;

                // Clear the block from the screen
                gotoxy(bullet->x, bullet->y);
                cout << " ";

                bullet->isActive = false; // Deactivate the bullet
                return true;              // Collision detected
            }
        }
    }
    return false; // No collision detected
}
// تابع برای رسم موانع
void drawBarrier(Barrier &barrier)
{
    if (barrier.active)
    {
        for (int i = 0; i < barrier.height; i++)
        {
            gotoxy(barrier.x, barrier.y + i);
            for (int j = 0; j < barrier.width; j++)
            {
                if (barrier.blocks[j][i]) // Only draw active blocks
                {
                    cout << "█";
                }
                else
                {
                    cout << " "; // Draw space for inactive blocks
                }
            }
        }
    }
}

// تابع برای بررسی اینکه آیا فضاپیما باید شلیک کند
void checkSpaceshipShoot(Spaceship &spaceship, unsigned long &lastShootTime)
{
    unsigned long currentTime = time(0) * 1000; // Current time in milliseconds

    // Check if enough time has passed to shoot
    if (currentTime - lastShootTime >= 2000) // 2 seconds interval
    {
        if (spaceship.active)
        {
            spaceship.bullet.x = spaceship.x + 2; // Center bullet
            spaceship.bullet.y = spaceship.y + 3; // Just below spaceship
            spaceship.bullet.isActive = true;     // Activate bullet
        }
        lastShootTime = currentTime; // Update last shoot time
    }
}

void moveSpaceshipBullet(Spaceship &spaceship, Player &player)
{
    if (spaceship.bullet.isActive)
    {
        gotoxy(spaceship.bullet.x, spaceship.bullet.y);
        cout << " ";          // Clear previous bullet position
        spaceship.bullet.y++; // Move the bullet down

        // Deactivate the bullet if it passes the player's y-position
        if (spaceship.bullet.y >= player.y + 1)
        {
            spaceship.bullet.isActive = false;
        }
        else
        {
            gotoxy(spaceship.bullet.x, spaceship.bullet.y);
            cout << "*"; // Draw the bullet
        }
    }
}

void hideCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Get the standard output handle
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo); // Get the current cursor information
    cursorInfo.bVisible = false;                 // Set the cursor visibility to false
    SetConsoleCursorInfo(hConsole, &cursorInfo); // Apply the changes
}

void gameOverAnimation(int screenWidth, int screenHeight)
{
    // Clear the screen
    system("cls");

    // Center coordinates for the animation
    int centerX = screenWidth / 2 - 30; // Adjust for the width of the text
    int centerY = screenHeight / 2 - 6;

    // ASCII Art for "GAME OVER"
    const char *gameOverArt[] = {
        "  ________                        ________                     ",
        " /  _____/_____    _____   ____   \\_____  \\___  __ ___________ ",
        "/   \\  ___\\__  \\  /     \\_/ __ \\   /   |   \\  \\/ // __ \\_  __ \\",
        "\\    \\_\\  \\/ __ \\|  Y Y  \\  ___/  /    |    \\   /\\  ___/|  | \\/",
        " \\______  (____  /__|_|  /\\___  > \\_______  /\\_/  \\___  >__|   ",
        "        \\/     \\/      \\/     \\/          \\/          \\/       "};

    setColor(10);

    // نمایش پیام "GAME OVER"
    for (int i = 0; i < 6; ++i) // تعداد خطوط ASCII Art
    {
        gotoxy(centerX, centerY + i);
        cout << gameOverArt[i];
    }

    setColor(7);
    // تأخیر قبل از اتمام
    Sleep(2000); // Pause before exiting
}

bool allEnemiesDestroyed(Enemy enemies[], int maxEnemies)
{
    for (int i = 0; i < maxEnemies; ++i)
    {
        if (enemies[i].active)
        {
            return false; // At least one enemy is still active
        }
    }
    return true; // All enemies are destroyed
}

// Function to check collision between spaceship bullets and the player
bool checkSpaceshipBulletCollisionWithPlayer(Spaceship &spaceship, Player &player)
{
    if (spaceship.bullet.isActive)
    {
        // Check if the bullet is within the player's x bounds
        if (spaceship.bullet.x >= player.x && spaceship.bullet.x <= player.x + player.width)
        {
            // Check if the bullet is within the player's y bounds
            if (spaceship.bullet.y >= player.y && spaceship.bullet.y <= player.y + 2) // Player height is 2 rows
            {
                spaceship.bullet.isActive = false; // Deactivate the bullet
                player.lives--;                    // Reduce player lives
                drawLives(&player);                // Update lives display
                return true;                       // Collision detected
            }
        }
    }
    return false; // No collision detected
}
// Function to check collision between spaceship bullets and barriers
bool checkSpaceshipBulletCollisionWithBarrier(Spaceship &spaceship, Barrier barriers[], int numBarriers)
{
    if (spaceship.bullet.isActive)
    {
        for (int i = 0; i < numBarriers; i++)
        {
            if (barriers[i].active)
            {
                // Calculate the relative position of the bullet within the barrier
                int relativeX = spaceship.bullet.x - barriers[i].x;
                int relativeY = spaceship.bullet.y - barriers[i].y;

                // Check if the bullet is within the bounds of the barrier
                if (relativeX >= 0 && relativeX < barriers[i].width &&
                    relativeY >= 0 && relativeY < barriers[i].height)
                {
                    // Check if the specific block is active
                    if (barriers[i].blocks[relativeX][relativeY])
                    {
                        // Deactivate the specific block
                        barriers[i].blocks[relativeX][relativeY] = false;

                        // Clear the block from the screen
                        gotoxy(spaceship.bullet.x, spaceship.bullet.y);
                        cout << " ";

                        spaceship.bullet.isActive = false; // Deactivate the bullet
                        return true;                       // Collision detected
                    }
                }
            }
        }
    }
    return false; // No collision detected
}

void printLogo()
{
    string spaceInvaderText[] = {
        "███████╗██████╗  █████╗  ██████╗███████╗  ██╗███╗   ██╗██╗   ██╗ █████╗ ██████╗ ███████╗██████╗ ",
        "██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝  ██║████╗  ██║██║   ██║██╔══██╗██╔══██╗██╔════╝██╔══██╗",
        "███████╗██████╔╝███████║██║     █████╗    ██║██╔██╗ ██║██║   ██║███████║██║  ██║█████╗  ██████╔╝",
        "╚════██║██╔═══╝ ██╔══██║██║     ██╔══╝    ██║██║╚██╗██║╚██╗ ██╔╝██╔══██║██║  ██║██╔══╝  ██╔══██╗",
        "███████║██║     ██║  ██║╚██████╗███████╗  ██║██║ ╚████║ ╚████╔╝ ██║  ██║██████╔╝███████╗██║  ██║",
        "╚══════╝╚═╝     ╚═╝  ╚═╝ ╚═════╝╚══════╝  ╚═╝╚═╝  ╚═══╝  ╚═══╝  ╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝"};

    int width = 100;
    int height = 6;
    int padding = 2;

    cout << "┌";
    for (int i = 0; i < width - 2; i++)
    {
        cout << "─";
    }
    cout << "┐" << endl;

    for (int i = 0; i < height; i++)
    {
        cout << "│";
        cout << " " << spaceInvaderText[i] << " ";
        cout << "│" << endl;
    }

    cout << "└";
    for (int i = 0; i < width - 2; i++)
    {
        cout << "─";
    }
    cout << "┘" << endl;
}

void printCentered(const string &str, int width)
{
    int padding = (width - str.length()) / 2;
    cout << string(padding, ' ') << str << endl;
}

void clearScreen()
{
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hConsole, coord);
#else
    cout << "\033[H\033[J";
#endif
}

void printCenteredText(const string &str, int width)
{
    int padding = (width - str.length()) / 2;
    cout << string(padding, ' ') << str;
}

void mainMenu(int selected)
{
    string options[6] = {"New game", "Leaderboard", "How to Play", "Exit"};
    int width = 40;

    for (int i = 0; i < 4; i++)
    {
        if (i == selected)
        {
            setColor(2);

            cout << "┌────────────────────────────────────────┐" << endl;
            printCenteredText(">> " + options[i] + " <<", width);
            cout << endl
                 << "└────────────────────────────────────────┘" << endl;
        }
        else
        {

            setColor(8);
            cout << "┌────────────────────────────────────────┐" << endl;
            printCenteredText("   " + options[i] + "   ", width);
            cout << endl
                 << "└────────────────────────────────────────┘" << endl;
        }
        setColor(7);
    }
}
void pauseMenu(int selected)
{
    system("cls");
    string options[3] = {"Resume Game", "Restart Game", "Exit"};
    int width = 40;

    // ابتدا لوگو را چاپ می‌کنیم
    printLogo();

    // حالا منوی Pause را نمایش می‌دهیم
    for (int i = 0; i < 3; i++)
    {
        moveCursor(0, i * 3 + 8); // جابجایی کرسر به پایین‌تر از لوگو

        if (i == selected)
        {
            setColor(2); // رنگ انتخاب شده (سبز)
            cout << "┌────────────────────────────────────────┐" << endl;
            printCenteredText(">> " + options[i] + " <<", width);
            cout << endl
                 << "└────────────────────────────────────────┘" << endl;
        }
        else
        {
            setColor(8); // رنگ عادی (خاکی)
            cout << "┌────────────────────────────────────────┐" << endl;
            printCenteredText("   " + options[i] + "   ", width);
            cout << endl
                 << "└────────────────────────────────────────┘" << endl;
        }
        setColor(7); // بازگشت به رنگ معمولی
    }
}

// انتخاب درجه سختی
int selectDifficulty()
{
    string options[2] = {"Normal", "Hard"};
    int selected = 0; // Default to Normal
    bool selecting = true;

    while (selecting)
    {
        clearScreen(); // Clear the screen and redraw the menu

        // Display the options with a border
        int width = 40;
        for (int i = 0; i < 2; i++)
        {
            if (i == selected)
            {
                setColor(2);
                cout << "┌────────────────────────────────────────┐" << endl;
                printCenteredText(">> " + options[i] + " <<", width);
                cout << endl
                     << "└────────────────────────────────────────┘" << endl;
            }
            else
            {
                setColor(8);
                cout << "┌────────────────────────────────────────┐" << endl;
                printCenteredText("   " + options[i] + "   ", width);
                cout << endl
                     << "└────────────────────────────────────────┘" << endl;
            }
            setColor(7);
        }

        char key = _getch(); // Get user input

        if (key == 'w' || key == 'W' || key == 72)
        {
            selected = (selected - 1 + 2) % 2; // Move up (wrap around)
        }
        else if (key == 's' || key == 'S' || key == 80)
        {
            selected = (selected + 1) % 2; // Move down (wrap around)
        }
        else if (key == '\r')
        {
            selecting = false; // Exit the loop when Enter is pressed
        }
    }

    return selected; // Return the selected difficulty (0 for Normal, 1 for Hard)
}

void newGame()
{

    bool gameOver = false;
    system("cls");
    const char *playerDesign[] = {
        "  █  ",
        "█████"};

    struct Player player = {30, screenHeight - 4, {playerDesign[0], playerDesign[1]}, 5, 3}; // number of player lives = 3
    struct Bullet bullet = {0, 0, 0};
    Score score = {0}; // امتیاز اولیه 0

    // طراحی مختلف برای ردیف‌ها
    const char *enemyDesign1[] = {
        "  ███  ",
        " █████ ",
        "  ███  ",
    };

    const char *enemyDesign2[] = {
        "  ███ ",
        " ████ ",
        " ███  ",
    };

    const char *enemyDesign3[] = {
        " ███   ",
        " █████ ",
        "   ███ ",
    };

    // ایجاد 9 دشمن (3 ردیف × 3 ستون) با فاصله بیشتر بین ردیف‌ها
    Enemy enemies[maxEnemies] = {
        // First row enemies
        {screenWidth / 6, 10, true, {enemyDesign1[0], enemyDesign1[1], enemyDesign1[2]}, {0, 0, 0}, 0},
        {screenWidth / 6 + enemySpacing, 10, true, {enemyDesign1[0], enemyDesign1[1], enemyDesign1[2]}, {0, 0, 0}, 0},
        {screenWidth / 6 + 2 * enemySpacing, 10, true, {enemyDesign1[0], enemyDesign1[1], enemyDesign1[2]}, {0, 0, 0}, 0},

        // Second row enemies
        {screenWidth / 6, 14, true, {enemyDesign2[0], enemyDesign2[1], enemyDesign2[2]}, {0, 0, 0}, 0},
        {screenWidth / 6 + enemySpacing, 14, true, {enemyDesign2[0], enemyDesign2[1], enemyDesign2[2]}, {0, 0, 0}, 0},
        {screenWidth / 6 + 2 * enemySpacing, 14, true, {enemyDesign2[0], enemyDesign2[1], enemyDesign2[2]}, {0, 0, 0}, 0},

        // Third row enemies
        {screenWidth / 6, 18, true, {enemyDesign3[0], enemyDesign3[1], enemyDesign3[2]}, {0, 0, 0}, 0},
        {screenWidth / 6 + enemySpacing, 18, true, {enemyDesign3[0], enemyDesign3[1], enemyDesign3[2]}, {0, 0, 0}, 0},
        {screenWidth / 6 + 2 * enemySpacing, 18, true, {enemyDesign3[0], enemyDesign3[1], enemyDesign3[2]}, {0, 0, 0}, 0}};
    // تعریف سفینه جدید
    Spaceship spaceship = {0, 0, false, {"  ███  ", " █████", "   █  "}, {0, 0, 0}, 0}; // Initialize bullet as inactive
    unsigned long lastShootTime = 0;                                                    // Time when the last bullet was shot
    bool spaceshipDestroyed = false;       // check if the spaceship was destroyed
    unsigned long lastSpawnTime = 0;       // Time when the spaceship was last spawned
    unsigned long lastDestructionTime = 0; // Time when the spaceship was last destroyed

    const int respawnDelay = 60;                                                        // 60 seconds for respawn delay

    // Initialize barriers
    // Initialize barriers
    const int numBarriers = 3; // Number of barriers
    Barrier barriers[numBarriers] = {
        {20, screenHeight - 10, 6, 4, true, {true}}, // Barrier at position (20, height - 10) with width 6 and height 4
        {40, screenHeight - 10, 6, 4, true, {true}}, // Barrier at position (40, height - 10) with width 6 and height 4
        {60, screenHeight - 10, 6, 4, true, {true}}  // Barrier at position (60, height - 10) with width 6 and height 4
    };

    string playerName;
    system("cls");

    // وارد کردن نام بازیکن
    while (true)
    {
        clearScreen();
        cout << "Enter your name: ";
        getline(cin, playerName);
        player.username = playerName;
        if (!playerName.empty())
        {
            break;
        }
        else
        {
            cout << "Name cannot be empty! Please enter your name." << endl;
            this_thread::sleep_for(chrono::milliseconds(1000)); // Pause for a moment
        }
    }

    int difficultyLevel = selectDifficulty(); // دریافت درجه سختی از تابع
    if (difficultyLevel == 1)
    {
        player.lives = 1; // اگر درجه سختی هارد انتخاب شده باشد، جان بازیکن برابر با 1 می‌شود
    }
    else
    {
        player.lives = 3; // اگر درجه سختی نرمال انتخاب شده باشد، جان بازیکن برابر با 3 می‌شود
    }
    system("cls");
    for (int i = 0; i < numBarriers; i++)
    {
        for (int j = 0; j < barriers[i].width; j++)
        {
            for (int k = 0; k < barriers[i].height; k++)
            {
                barriers[i].blocks[j][k] = true; // Set all blocks to active
            }
        }
    }

    // Draw barriers
    for (int i = 0; i < numBarriers; i++)
    {
        drawBarrier(barriers[i]);
    }

    // Modify enemy behavior based on difficulty

    int direction = 1; // جهت حرکت دشمن‌ها (1 برای راست، -1 برای چپ)
    int pauseSelected = 0;
    drawBoundary();
    drawPlayer(&player);
    drawEnemies(enemies, maxEnemies);
    score.draw();
    drawLives(&player);
    int counter = 0;
    int speed = 60;

    while (player.lives > 0 && !gameOver)
    {
        spawnSpaceship(spaceship, lastSpawnTime, spaceshipDestroyed, lastDestructionTime);
        if (counter % 50 == 0)
            show_game(barriers, &player, spaceship);
        counter++;

        // Handle input
        if (_kbhit())
        {
            char ch = _getch();
            if (ch == 'a' || ch == 'd' || ch == 75 || ch == 77 || ch == 'A' || ch == 'D')
            {
                movePlayer(&player, ch);
            }
            else if (ch == ' ')
            {
                if (!bullet.isActive)
                {
                    shootBullet(&player, &bullet);
                }
            }
            else if (ch == 27)
            {
                system("cls");
                bool is_key_pressed = false;
                while (!is_key_pressed)
                {

                    pauseMenu(pauseSelected); // نمایش منوی Pause

                    moveCursor(0, 0);

                    char pauseKey = _getch();

                    if (pauseKey == 'w' || pauseKey == 'W' || pauseKey == 72)
                    {
                        pauseSelected = (pauseSelected - 1 + 3) % 3;
                    }
                    else if (pauseKey == 's' || pauseKey == 'S' || pauseKey == 80)
                    {
                        pauseSelected = (pauseSelected + 1) % 3;
                    }
                    else if (pauseKey == '\r')
                    {
                        if (pauseSelected == 0)
                        {
                            system("cls");
                            is_key_pressed = true;
                            show_game(barriers, &player, spaceship);
                            // Resume the game and go back to the game loop
                        }
                        else if (pauseSelected == 1)
                        {
                            cout << "Restarting game..." << endl;
                            system("cls");
                            newGame();
                            player.lives = 0; // Restart the game by calling newGame function
                        }
                        else if (pauseSelected == 2)
                        {
                            gameOver = true;
                            break;
                        }
                    }
                }
                // break;
            }
        }
        if (checkCollisionWithSpaceship(&bullet, spaceship, score))
        {
            spaceship.active = false; // Deactivate the spaceship
            spaceshipDestroyed = true; // Mark the spaceship as destroyed
            lastDestructionTime = time(0); // Record the time of destruction
        }
        moveBullet(&bullet);
        moveEnemies(enemies, maxEnemies, direction);
        enemyShoot(enemies, maxEnemies); // شلیک تصادفی تیر از دشمن
        moveEnemyBullets(enemies, maxEnemies);

        // Spawn spaceship
        if (!spaceship.active && (time(0) - lastDestructionTime >= respawnDelay)) // Check if it's time to respawn
        {
            spaceship.x = 2;         // Reset position
            spaceship.y = 5;         // Reset position
            spaceship.active = true; // Respawn the spaceship
        }

        drawSpaceship(spaceship); // Draw spaceship
        if (checkEnemyCollisionWithBarrier(enemies, maxEnemies, barriers, numBarriers))
        {
            // Barrier has been destroyed, no further action needed
        }
        // Move spaceship
        moveSpaceship(spaceship);
        checkAndRemoveSpaceship(spaceship); // Remove spaceship after reaching the end

        // Check if spaceship should shoot
        checkSpaceshipShoot(spaceship, lastShootTime);
        // Move spaceship bullet
        moveSpaceshipBullet(spaceship, player);

        // Check for collision between spaceship bullet and player
        if (checkSpaceshipBulletCollisionWithPlayer(spaceship, player))
        {
            // Handle player hit by spaceship bullet
            if (player.lives <= 0)
            {
                // restart_game();
                gameOverAnimation(screenWidth, screenHeight);

                break; // Exit the game loop
            }
        }
        // Check for bullet collision with barriers
        for (int i = 0; i < numBarriers; i++)
        {
            if (bullet.isActive && checkBulletCollisionWithBarrier(&bullet, barriers[i]))
            {
                // Redraw the barrier after collision
                drawBarrier(barriers[i]);
                break; // Exit the loop if the bullet has been deactivated
            }
        }

        for (int i = 0; i < maxEnemies; i++)
        {
            if (enemies[i].bullet.isActive)
            {
                for (int j = 0; j < numBarriers; j++)
                {
                    if (checkBulletCollisionWithBarrier(&enemies[i].bullet, barriers[j]))
                    {
                        // Redraw the barrier after collision
                        drawBarrier(barriers[j]);
                        break; // Exit the loop if the bullet has been deactivated
                    }
                }
            }
        }
        for (int i = 0; i < maxEnemies; i++)
        {
            if (enemies[i].bullet.isActive)
            {
                for (int j = 0; j < numBarriers; j++)
                {
                    if (checkBulletCollisionWithBarrier(&spaceship.bullet, barriers[j]))
                    {
                        // Redraw the barrier after collision
                        drawBarrier(barriers[j]);
                        break; // Exit the loop if the bullet has been deactivated
                    }
                }
            }
        }

        // Check collision between player bullets and enemies
        if (checkCollisionWithEnemies(&bullet, enemies, maxEnemies, score))
        {
            moveBullet(&bullet);
            speed -= 6;
        }

        // Check collision between player's bullet and spaceship
        if (checkCollisionWithSpaceship(&bullet, spaceship, score))
        {
            // Handle spaceship destruction
            lastDestructionTime = time(0); // Record the time when the spaceship was destroyed
        }

        // Check for player lives and update accordingly

        if (checkCollisionWithPlayer(&player, enemies, maxEnemies))
        {
            drawLives(&player);
        }

        // Check for collision between spaceship bullet and player
        if (checkSpaceshipBulletCollisionWithPlayer(spaceship, player))
        {

            if (player.lives <= 0)
            {
                gameOverAnimation(screenWidth, screenHeight);
                Sleep(3000);
                break; // Exit the game loop
            }
        }

        if (allEnemiesDestroyed(enemies, maxEnemies))
        {
            gameOverAnimation(screenWidth, screenHeight);
            Sleep(3000); // Pause for 3 seconds to allow the animation to be seen
            break;       // Exit the game loop
        }
        if (player.lives <= 0)
        {
            gameOverAnimation(screenWidth, screenHeight);
            Sleep(3000);
            break; // Exit the game loop
        }

        score.draw();

        Sleep(speed);
    }
    player.score = score.points;
    // saveplayerdata
    savePlayerData("leaderboard.txt", player);
    gotoxy(screenWidth / 2 - 5, screenHeight / 2);
    system("cls");
}

void loadGame()
{
    cout << "Loading game..." << endl;
}

void howToPlay()
{
    string greenColor = "\033[38;5;82m";
    string resetColor = "\033[0m";
    string lines[] = {
        u8"╓─────────────────────────────────────────────────────────────────────────╖",
        u8"║ In this game, you will control a spaceship to fight off aliens.         ║",
        u8"║ Your goal is to destroy as many aliens as possible without getting hit. ║",
        u8"╙─────────────────────────────────────────────────────────────────────────╜",
        u8"╓─────────────────────────────────────────────────────────────────╖        ",
        u8"║      Instructions:                                              ║",
        u8"╟─────────────────────────────────────────────────────────────────╣",
        u8"║ 1. Use 'A' or Left Arrow to move the spaceship left.            ║",
        u8"║ 2. Use 'D' or Right Arrow to move the spaceship right.          ║",
        u8"║ 3. Press 'Space' to shoot lasers at the aliens.                 ║",
        u8"║ 4. Avoid enemy lasers and stay alive as long as you can!        ║",
        u8"║ 5. The Score bar shows the scores you can make.                 ║",
        u8"║    Aliens have different scores:                                ║",
        u8"║    - 'FIRST ROW' has 10 score                                   ║",
        u8"║    - 'SECOND ROW' has 20 score                                  ║",
        u8"║    - 'THIRD ROW' has 30 score                                   ║",
        u8"║    - 'SPACESHIP'has a random score between 50 to 300 points.    ║",
        u8"╟─────────────────────────────────────────────────────────────────╢",

        u8"║ Tips:                                                           ║",
        u8"╟─────────────────────────────────────────────────────────────────╣",
        u8"║ - Focus on dodging enemy lasers while aiming your shots.        ║",
        u8"║ - Destroy all aliens on the screen to proceed to the next level ║",
        u8"║ - Collect power-ups to upgrade your spaceship.                  ║",
        u8"╟─────────────────────────────────────────────────────────────────╣",
        u8"║ Press 'B' to go back to the main menu.                          ║",
        u8"╙─────────────────────────────────────────────────────────────────╜"};

    for (int currentLine = 0; currentLine < sizeof(lines) / sizeof(lines[0]); currentLine++)
    {
        cout << greenColor;

        for (char c : lines[currentLine])
        {
            cout << c;
            this_thread::sleep_for(chrono::milliseconds(10));
        }

        cout << resetColor;
        cout << endl;
    }
    while (true)
    {
        if (_kbhit())
        {
            char key = _getch();
            if (key == 'b' || key == 'B')
            {
                break;
            }
        }
    }
    system("cls");
}
void setDifficulty(int difficultyLevel)
{
    difficulty = difficultyLevel;
    if (difficulty == 1)
    {
        numBullets = 2; // در حالت سخت، تعداد تیرها دو برابر می‌شود
    }
    else
    {
        numBullets = 1; // در حالت عادی، یک تیر
    }
}
void show_game(Barrier barrier[], struct Player *player, Spaceship &spaceship)
{
    system("cls");
    drawBoundary();           // Static - draw once
    drawPlayer(player);       // Update Player
    drawLives(player);        // Update Lives UI
    drawSpaceship(spaceship); // Update Spaceship

    for (int i = 0; i < 3; i++)
    {
        drawBarrier(barrier[i]); // Update Barriers
    }
}

void savePlayerData(const string &filename, const Player &player)
{
    ofstream file("leaderboard.txt", ios::app); // Open file in append mode
    if (file.is_open())
    {
        file << player.username << " " << player.score << endl;
        file.close();
    }
    else
    {
        cout << "Error: Unable to open the file for saving data!" << endl;
    }
}
void loadLeaderboard(const string &filename, Player leaderboard[], int &count, int maxEntries)
// تابع بارگذاری امتیازها از فایل
{
    ifstream file("leaderboard.txt");
    count = 0;

    if (file.is_open())
    {
        while (file >> leaderboard[count].username >> leaderboard[count].score)
        {
            bool exists = false;
            for (int i = 0; i < count; ++i)
            {
                if (leaderboard[i].username == leaderboard[count].username)
                {
                    // If user exists, keep the highest score
                    if (leaderboard[count].score > leaderboard[i].score)
                    {
                        leaderboard[i].score = leaderboard[count].score;
                    }
                    exists = true;
                    break;
                }
            }
            if (!exists)
            {
                count++;
            }
            if (count >= maxEntries)
            {
                break;
            }
        }
        file.close();

        // Sort leaderboard after loading
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = i + 1; j < count; j++)
            {
                if (leaderboard[j].score > leaderboard[i].score)
                {
                    swap(leaderboard[i], leaderboard[j]);
                }
            }
        }
    }
    else
    {
        cout << "Error: Unable to open the file for loading data!" << endl;
    }
}

// تابع مرتب‌سازی امتیازات
void bubbleSort(Player player[], int size)
{
    for (int i = 0; i < size - 1; ++i)
    {
        for (int j = 0; j < size - i - 1; ++j)
        {
            // Sort in descending order by score
            if (player[j].score < player[j + 1].score)
            {
                Player temp = player[j];
                player[j] = player[j + 1];
                player[j + 1] = temp;
            }
        }
    }
}

void showLeaderboard()
{
    ifstream file("leaderboard.txt");
    Player players[100]; // Use a fixed-size array
    int count = 0;       // Counter for players

    if (file.is_open())
    {
        while (file >> players[count].username >> players[count].score)
        {
            bool exists = false;
            for (int i = 0; i < count; ++i)
            {
                if (players[i].username == players[count].username)
                {
                    // If username exists, update with higher score
                    if (players[count].score > players[i].score)
                    {
                        players[i].score = players[count].score;
                    }
                    exists = true;
                    break;
                }
            }
            if (!exists)
            {
                count++;
            }
            if (count >= 100)
            {
                cout << "Warning: leaderboard exceeds the predefined size." << endl;
                break;
            }
        }
        file.close();
    }
    else
    {
        cout << "Error: Unable to open the leaderboard file." << endl;
        return;
    }

    // Sort leaderboard by score in descending order
    bubbleSort(players, count);

    // Clear screen
    cout << "\033[H\033[J"; // This clears the screen (works in most terminals)

    // Display leaderboard header with green color
    cout << "\033[32m" // Green
         << "╔══════════════════════════════════════════════╗" << "\033[0m" << endl;
    cout << "\033[32m"
         << "║                Leaderboard                   ║" << "\033[0m" << endl;
    cout << "\033[32m"
         << "╚══════════════════════════════════════════════╝" << "\033[0m" << endl;
    // Display leaderboard table with gray background
    cout << "\033[90m" // Gray text color
         << left << setw(8) << "Rank" << setw(15) << "Name" << setw(15) << "Score" << "\033[0m" << endl;
    cout << "\033[90m"
         << "-------------------------------------------------" << "\033[0m" << endl;

    if (count == 0)
    {
        cout << "\033[90m" << "No leaderboard data available." << "\033[0m" << endl;
    }
    else
    {
        for (int i = 0; i < count; ++i)
        {
            cout << "\033[90m" // Gray text color for rows
                 << left << setw(8) << i + 1
                 << setw(15) << players[i].username
                 << setw(15) << players[i].score
                 << "\033[0m" << endl;
        }
    }

    cout << "\033[0m" << "\nPress Enter to return to the main menu..." << endl;
    cin.ignore();
    cin.get();
}
void spawnSpaceship(Spaceship &spaceship, unsigned long &lastSpawnTime, bool &spaceshipDestroyed, unsigned long &lastDestructionTime)
{
    unsigned long currentTime = time(0); // Current time in seconds

    if (spaceshipDestroyed)
    {
        // If the spaceship was destroyed, wait 50-60 seconds before respawning
        if (currentTime - lastDestructionTime >= 50 + rand() % 11) // Random delay between 50-60 seconds
        {
            spaceship.active = true;
            spaceship.x = 2; // Reset position
            spaceship.y = 5; // Reset position
            spaceshipDestroyed = false;
            lastSpawnTime = currentTime; // Update last spawn time
        }
    }
    else
    {
        // If the spaceship is not active, respawn it every 20-30 seconds
        if (!spaceship.active && (currentTime - lastSpawnTime >= 20 + rand() % 11)) // Random delay between 20-30 seconds
        {
            spaceship.active = true;
            spaceship.x = 2;             // Reset position
            spaceship.y = 5;             // Reset position
            lastSpawnTime = currentTime; // Update last spawn time
        }
    }
}