#include "Graphics.cpp"
#include "Player.h"
#include "Ghost.h"
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <semaphore.h>

using namespace sf;
using namespace std;

RenderWindow *window;
Event e;
Graphics graphics;
CircleShape circle;
sf::Clock game_clock, ghost_clock[4], player_clock;
float dt, gt[4], pt;
Player pac_man;
Ghost *ghost;
sf::Texture tile;
sf::Sprite tilespr;
int **maze_arr, **coin_arr;
int grid1[16][32] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
    {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
    {1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1},
    {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

int grid2[16][32] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
sf::Image sprite_sheet;
sf::Texture coin_tex, power_tex;
sf::Sprite coin_spr, power_spr;
int x, y;

struct ghost_inp
{
    int ind;
};

struct player_inp
{
    sf::RenderWindow *win;
    float dtime;
};

void intialize_window()
{

    ////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////

    // queue<int> que;
    // coin_arr[11][16] = 1;
    // que.push(11);
    // que.push(16);
    // int row, col;
    // while (!que.empty())
    // {
    //     row = que.front();
    //     que.pop();
    //     col = que.front();
    //     que.pop();
    //     if (row + 1 < y && coin_arr[row + 1][col] == 0 && maze_arr[row + 1][col] == 0)
    //     {
    //         coin_arr[row + 1][col] = 1;
    //         que.push(row + 1);
    //         que.push(col);
    //     }
    //     if (row - 1 >= 0 && coin_arr[row - 1][col] == 0 && maze_arr[row - 1][col] == 0)
    //     {
    //         coin_arr[row - 1][col] = 1;
    //         que.push(row - 1);
    //         que.push(col);
    //     }
    //     if (col + 1 < x && coin_arr[row][col + 1] == 0 && maze_arr[row][col + 1] == 0)
    //     {
    //         coin_arr[row][col + 1] = 1;
    //         que.push(row);
    //         que.push(col + 1);
    //     }
    //     if (col - 1 >= 0 && coin_arr[row][col - 1] == 0 && maze_arr[row][col - 1] == 0)
    //     {
    //         coin_arr[row][col - 1] = 1;
    //         que.push(row);
    //         que.push(col - 1);
    //     }
    // }

    cout << "Array initialized";
}

void update()
{
    while (window->pollEvent(e))
    {
        if (e.type == Event::Closed)
            window->close();
        else if (e.type == Event::KeyPressed)
        {
            // sem_wait(&sem_player);
            pac_man.dirchange(e);
            // sem_post(&sem_player);
        }
    }
}

void *coin_creation(void *arg)
{
    sf::Time timing = sf::seconds(0.05f);
    while (window->isOpen())
    {
        queue<int> que;
        sem_wait(&sem_coin);
        if (coin_arr[11][16] != 2)
            coin_arr[11][16] = 1;
        sem_post(&sem_coin);
        que.push(11);
        que.push(16);
        int row, col;
        while (!que.empty())
        {
            sf::sleep(timing);
            row = que.front();
            que.pop();
            col = que.front();
            que.pop();
            sem_wait(&sem_coin);
            if (row + 1 < y && coin_arr[row + 1][col] == 0 && maze_arr[row + 1][col] == 0)
            {
                if (row + 1 == 1 && col == 1 || row + 1 == 1 && col == 30 || row + 1 == 14 && col == 1 || row + 1 == 14 && col == 30)
                    coin_arr[row + 1][col] = 3;
                else
                    coin_arr[row + 1][col] = 1;
                que.push(row + 1);
                que.push(col);
            }
            if (row - 1 >= 0 && coin_arr[row - 1][col] == 0 && maze_arr[row - 1][col] == 0)
            {
                if (row - 1 == 1 && col == 1 || row - 1 == 1 && col == 30 || row - 1 == 14 && col == 1 || row - 1 == 14 && col == 30)
                    coin_arr[row - 1][col] = 3;
                else
                    coin_arr[row - 1][col] = 1;
                que.push(row - 1);
                que.push(col);
            }
            if (col + 1 < x && coin_arr[row][col + 1] == 0 && maze_arr[row][col + 1] == 0)
            {
                if (row == 1 && col + 1 == 1 || row == 1 && col + 1 == 30 || row == 14 && col + 1 == 1 || row == 14 && col + 1 == 30)
                    coin_arr[row][col + 1] = 3;
                else
                    coin_arr[row][col + 1] = 1;
                que.push(row);
                que.push(col + 1);
            }
            if (col - 1 >= 0 && coin_arr[row][col - 1] == 0 && maze_arr[row][col - 1] == 0)
            {
                if (row == 1 && col - 1 == 1 || row == 1 && col - 1 == 30 || row == 14 && col - 1 == 1 || row == 14 && col - 1 == 30)
                    coin_arr[row][col - 1] = 3;
                else
                    coin_arr[row][col - 1] = 1;
                que.push(row);
                que.push(col - 1);
            }
            sem_post(&sem_coin);
        }
        for (int i = 0; i < y; i++)
        {
            for (int j = 0; j < x; j++)
            {
                if (i == 1 && j - 1 == 1 || i == 1 && j - 1 == 30 || i == 14 && j - 1 == 1 || i == 14 && j - 1 == 30)
                    coin_arr[i][j - 1] = 3;
            }
        }
    }
}

void *coin_eating(void *args)
{
    float pacx, pacy, pach, pacw, windowy, windowx;
    while (window->isOpen())
    {
        sem_wait(&sem_player);
        pacx = pac_man.x;
        pacy = pac_man.y;
        pach = pac_man.player_sprite.getGlobalBounds().height;
        pacw = pac_man.player_sprite.getGlobalBounds().width;
        sem_post(&sem_player);
        sem_wait(&sem_window);
        windowx = window->getSize().x;
        windowy = window->getSize().y;
        sem_post(&sem_window);
        if ((int)((pacy + pach) / 60) >= windowy || (int)(pacy / 60) < 0)
            continue;
        if ((int)((pacx + pacw) / 60) >= windowx || (int)(pacx / 60) < 0)
            continue;
        if ((int)(pacy / 60) == (int)((pacy + pach) / 60))
        {
            if ((int)(pacx / 60) == (int)((pacx + pacw) / 60))
            {
                sem_wait(&sem_coin);
                if (coin_arr[(int)(pacy / 60)][(int)(pacx / 60)] == 1)
                {
                    coin_arr[(int)(pacy / 60)][(int)(pacx / 60)] = 2;
                }
                else if (coin_arr[(int)(pacy / 60)][(int)(pacx / 60)] == 3)
                {
                    for (int i = 0; i < 4; i++)
                        ghost[i].can_die = true;
                    coin_arr[(int)(pacy / 60)][(int)(pacx / 60)] = 2;
                }
                sem_post(&sem_coin);
            }
        }
    }
}

void *ppellet_eater(void *arg)
{
    float pacx, pacy, pach, pacw, windowy, windowx;
    while (window->isOpen())
    {
        sem_wait(&sem_player);
        pacx = pac_man.x;
        pacy = pac_man.y;
        pach = pac_man.player_sprite.getGlobalBounds().height;
        pacw = pac_man.player_sprite.getGlobalBounds().width;
        sem_post(&sem_player);
        sem_wait(&sem_window);
        windowx = window->getSize().x;
        windowy = window->getSize().y;
        sem_post(&sem_window);
        if ((int)((pacy + pach) / 60) >= windowy || (int)(pacy / 60) < 0)
            continue;
        if ((int)((pacx + pacw) / 60) >= windowx || (int)(pacx / 60) < 0)
            continue;
        if ((int)(pacy / 60) == (int)((pacy + pach) / 60))
        {
            if ((int)(pacx / 60) == (int)((pacx + pacw) / 60))
            {
                sem_wait(&sem_coin);
                if (coin_arr[(int)(pacy / 60)][(int)(pacx / 60)] == 3)
                {
                    for (int i = 0; i < 4; i++)
                        ghost[i].can_die = true;
                    coin_arr[(int)(pacy / 60)][(int)(pacx / 60)] = 2;
                    sem_post(&sem_coin);
                    sem_wait(&sem_power);
                    pac_man.powered = true;
                }
                else
                {
                    sem_post(&sem_coin);
                }
            }
        }
    }
}

/*void render()
{

    window->clear();

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            if (maze_arr[i][j] == 1)
            {
                tilespr.setPosition(j * 60, i * 60);
                window->draw(tilespr);
            }
        }
    }
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            if (coin_arr[j][i] == 1)
                coin_spr.setPosition((60 * i) + 28, (60 * j) + 28);
            window->draw(coin_spr);
        }
    }

    pac_man.draw(*window);
    for (int i = 0; i < 4; i++)
    {

        ghost[i].draw(*window);
    }

    window->display();
}*/

void *permit_key_giver(void *args)
{
    ghost_inp *inp = (ghost_inp *)args;
    ghost[inp->ind].permit_key();
}

void *speed_boost(void *args)
{
    ghost_inp *inp = (ghost_inp *)args;
    sf::Time temp = sf::seconds(5);
    while (window->isOpen())
    {
        sem_wait(&sem_speed);
        ghost[inp->ind].speed *= 2;
        sleep(5);
        ghost[inp->ind].speed /= 2;
        sem_post(&sem_speed);
        sleep(sf::milliseconds(100));
    }
}

void *ghost_movement(void *args)
{
    float temp = 0;
    while (window->isOpen())
    {
        ghost_inp *inp = (ghost_inp *)args;
        sem_wait(&sem_player_m);
        ghostcount++;
        if (ghostcount == 1)
            sem_wait(&sem_player);
        sem_post(&sem_player_m);
        gt[inp->ind] = ghost_clock[inp->ind].restart().asSeconds();
        ghost[inp->ind].dir_changer(pac_man.x, pac_man.y, maze_arr, gt[inp->ind]);
        ghost[inp->ind].move(gt[inp->ind]);
    }
}

void *player_move(void *args)
{
    float timer = 0;
    while (window->isOpen())
    {
        timer += dt;
        // cout << "hellooooo\n";
        // sem_wait(&sem_window);
        // sem_wait(&sem_player);
        // window->setActive(true);
        pt = player_clock.restart().asSeconds();
        pac_man.move(pt, *window, maze_arr);
        // sem_post(&sem_player);
        // cout << "moving " << timer << endl;
        timer = 0;
        // window->setActive(false);
        // sem_post(&sem_window);
    }
}

void *Game_thread(void *args)
{
    window = nullptr;
    dt = 0.f;
    graphics.loadFromFile("config/graphics.txt");
    // this->gridSize = 100.f;
    circle.setRadius(10.f);
    circle.setFillColor(Color::Red);
    circle.setPosition(50.f, 50.f);

    tile.loadFromFile("res/tile.png");
    tilespr.setTexture(tile);
    tilespr.setScale(3, 3);

    sprite_sheet.loadFromFile("res/Pac-man.png");
    sf::IntRect rect(11, 11, 2, 2);
    coin_tex.loadFromImage(sprite_sheet, rect);
    coin_spr.setTexture(coin_tex);
    coin_spr.setScale(3, 3);
    rect.top = 24;
    rect.left = 8;
    rect.height = 8;
    rect.width = 8;
    power_tex.loadFromImage(sprite_sheet, rect);
    power_spr.setTexture(power_tex);
    power_spr.setScale(3, 3);

    window = new RenderWindow(graphics.res, graphics.wind_name);
    window->setFramerateLimit(graphics.frameRateLimit);

    x = 32;
    y = 16;
    int check = rand() % 2;
    coin_arr = new int *[y];
    maze_arr = new int *[16];
    ghost = new Ghost[4]{{1, x, y}, {2, x, y}, {3, x, y}, {4, x, y}};

    for (int i = 0; i < y; i++)
    {
        // maze_arr[i] = new int[x];
        coin_arr[i] = new int[x]{0};
    }
    for (int i = 0; i < 16; i++)
    {
        maze_arr[i] = new int[32];
        for (int j = 0; j < 32; j++)
        {
            if (check == 0)
                maze_arr[i][j] = grid1[i][j];
            else if (check == 1)
                maze_arr[i][j] = grid1[i][j];
        }
    }
    pthread_t tid1, tid2[4], tid3, tid4, tid5, tid6, tid7;
    ghost_inp ginp[4];
    sem_init(&sem_window, 0, 1);
    sem_init(&sem_player, 0, 1);
    sem_init(&sem_coin, 0, 1);
    sem_init(&sem_player_m, 0, 1);
    sem_init(&sem_blue_ghost, 0, 1);
    sem_init(&sem_power, 0, 1);
    sem_init(&sem_key, 0, 2);
    sem_init(&sem_permit, 0, 2);
    sem_init(&sem_speed, 0, 2);
    // player_inp pinp;
    pthread_create(&tid1, NULL, coin_creation, NULL);
    pthread_create(&tid4, NULL, coin_eating, NULL);
    pthread_create(&tid3, NULL, player_move, NULL);
    pthread_create(&tid5, NULL, ppellet_eater, NULL);
    for (int i = 0; i < 4; i++)
    {
        // ginp[i].win = window;
        ginp[i].ind = i;
        pthread_create(&tid2[i], NULL, ghost_movement, (void *)&ginp[i]);
        pthread_create(&tid6, NULL, permit_key_giver, (void *)&ginp[i]);
        pthread_create(&tid7, NULL, speed_boost, (void *)&ginp[i]);
    }

    float timer = 0;

    while (window->isOpen())
    {
        // cout<<"helllooe\n";
        // timer += dt;
        // if(timer <= 2)
        // {
        //     continue;
        // }
        // timer = 0;
        // pinp.dtime = dt;
        // pinp.win = window;
        // pac_man.move(dt, *window);
        // ghost[0].dir_changer(*window, pac_man.x, pac_man.y, maze_arr, x, y);
        // sleep(1);
        sem_wait(&sem_window);
        dt = game_clock.restart().asSeconds();
        update();
        // render();
        window->clear();
        sem_post(&sem_window);
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 32; j++)
            {
                if (maze_arr[i][j] == 1)
                {
                    tilespr.setPosition(j * 60, i * 60);
                    sem_wait(&sem_window);
                    window->draw(tilespr);
                    sem_post(&sem_window);
                }
            }
        }
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                sem_wait(&sem_coin);
                if (coin_arr[j][i] == 1)
                {
                    sem_post(&sem_coin);
                    coin_spr.setPosition((60 * i) + 28, (60 * j) + 28);
                    sem_wait(&sem_window);
                    window->draw(coin_spr);
                    sem_post(&sem_window);
                }
                else if (coin_arr[j][i] == 3)
                {
                    sem_post(&sem_coin);
                    power_spr.setPosition((60 * i) + 18, (60 * j) + 18);
                    // power_spr.setPosition(0, 60);
                    sem_wait(&sem_window);
                    window->draw(power_spr);
                    sem_post(&sem_window);
                }
                else
                {
                    sem_post(&sem_coin);
                }
            }
        }
        if (ghost[0].can_die == true)
            timer += dt;
        // sem_wait(&sem_player);
        // cout << "drawing " << timer << endl;
        pac_man.draw(*window);
        // timer = 0;
        // sem_post(&sem_player);
        for (int i = 0; i < 4; i++)
        {
            ghost[i].draw(*window, timer);
        }
        if (timer >= 5)
        {
            timer = 0;
        }

        sem_wait(&sem_window);
        window->display();
        sem_post(&sem_window);
    }
    pthread_join(tid1, NULL);
    for (int i = 0; i < 4; i++)
    {
        pthread_join(tid2[i], NULL);
    }
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);
}

int main()
{
    srand(time(0));
    pthread_t game_tid;
    pthread_create(&game_tid, NULL, Game_thread, NULL);
    pthread_join(game_tid, NULL);
    // void *s = nullptr;
    // Game_thread(s);
}
