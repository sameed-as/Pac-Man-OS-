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
sf::Texture coin_tex;
sf::Sprite coin_spr;
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
    // while (window->isOpen())
    {
        queue<int> que;
        coin_arr[11][16] = 1;
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
            if (row + 1 < y && coin_arr[row + 1][col] == 0 && maze_arr[row + 1][col] == 0)
            {
                coin_arr[row + 1][col] = 1;
                que.push(row + 1);
                que.push(col);
            }
            if (row - 1 >= 0 && coin_arr[row - 1][col] == 0 && maze_arr[row - 1][col] == 0)
            {
                coin_arr[row - 1][col] = 1;
                que.push(row - 1);
                que.push(col);
            }
            if (col + 1 < x && coin_arr[row][col + 1] == 0 && maze_arr[row][col + 1] == 0)
            {
                coin_arr[row][col + 1] = 1;
                que.push(row);
                que.push(col + 1);
            }
            if (col - 1 >= 0 && coin_arr[row][col - 1] == 0 && maze_arr[row][col - 1] == 0)
            {
                coin_arr[row][col - 1] = 1;
                que.push(row);
                que.push(col - 1);
            }
        }
    }
}

void *coin_eating(void *args)
{
    // while (window->isOpen())
    {
        // if ((int)((pac_man.y + pac_man.player_sprite.getGlobalBounds().height) / 60) >= window->getSize().y || (int)(pac_man.y / 60) < 0)
        //     continue;
        // if ((int)((pac_man.x + pac_man.player_sprite.getGlobalBounds().width) / 60) >= window->getSize().x || (int)(pac_man.x / 60) < 0)
        //     continue;
        // sem_wait(&sem_player);
        if ((int)(pac_man.y / 60) == (int)((pac_man.y + pac_man.player_sprite.getGlobalBounds().height) / 60))
        {
            if ((int)(pac_man.x / 60) == (int)((pac_man.x + pac_man.player_sprite.getGlobalBounds().width) / 60))
            {
                if (coin_arr[(int)(pac_man.y / 60)][(int)(pac_man.x / 60)] == 1)
                {
                    coin_arr[(int)(pac_man.y / 60)][(int)(pac_man.x / 60)] = 2;
                }
            }
        }
        // sem_post(&sem_player);
    }
}

void render()
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
}
void *ghost_movement(void *args)
{
    while(window->isOpen())
    {
        ghost_inp *inp = (ghost_inp *)args;
        gt[inp->ind] = ghost_clock[inp->ind].restart().asSeconds();
        sem_wait(&sem_player);
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
    pthread_t tid1, tid2[4], tid3, tid4;
    ghost_inp ginp[4];
    sem_init(&sem_window, 0, 1);
    sem_init(&sem_player, 0, 1);
    // player_inp pinp;
    pthread_create(&tid1, NULL, coin_creation, NULL);
    pthread_create(&tid4, NULL, coin_eating, NULL);
    pthread_create(&tid3, NULL, player_move, NULL);
    for (int i = 0; i < 4; i++)
    {
        // ginp[i].win = window;
        ginp[i].ind = i;
        pthread_create(&tid2[i], NULL, ghost_movement, (void *)&ginp[i]);
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
                if (coin_arr[j][i] == 1)
                {
                    coin_spr.setPosition((60 * i) + 28, (60 * j) + 28);
                    sem_wait(&sem_window);
                    window->draw(coin_spr);
                    sem_post(&sem_window);
                }
            }
        }
        timer += dt;
        // sem_wait(&sem_player);
        // cout << "drawing " << timer << endl;
        pac_man.draw(*window);
        timer = 0;
        // sem_post(&sem_player);
        for (int i = 0; i < 4; i++)
        {
            ghost[i].draw(*window);
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
}

int main()
{
    pthread_t game_tid;
    pthread_create(&game_tid, NULL, Game_thread, NULL);
    pthread_join(game_tid, NULL);
    // void *s = nullptr;
    // Game_thread(s);
}
