#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>

using namespace std;

struct Ghost
{
    sf::Image full_image;
    sf::Texture ghost_text, blue_text;
    sf::Sprite ghost_sprite;
    float x, y;
    int x_lim, y_lim;
    int **path;
    char ghost_dir;
    bool can_die = false;
    bool permit = false, key = false;
    int speed = 100;
    sf::Clock blueck;
    float blue_time;

public:
    Ghost()
    {
        full_image.loadFromFile("res/Pac-man.png");
    }
    Ghost(int choice, int x_lim, int y_lim)
    {
        full_image.loadFromFile("res/Pac-man.png");
        sf::IntRect rect(457 + (16 * 9), 64, 16, 16);
        blue_text.loadFromImage(full_image, rect);
        rect.height = 16;
        rect.width = 16;
        x = (14 * 60) + 6;
        y = (7 * 60) + 6;
        switch (choice)
        {
        case 1:
            rect.top = 64;
            rect.left = 457;
            ghost_text.loadFromImage(full_image, rect);
            x += 0;
            y += 0;
            ghost_sprite.setPosition(x, y);
            break;
        case 2:
            rect.top = 64 + 16;
            rect.left = 457;
            ghost_text.loadFromImage(full_image, rect);
            x += 60 * 3;
            y += 0;
            ghost_sprite.setPosition(x, y);
            break;
        case 3:
            rect.top = 64 + 32;
            rect.left = 457;
            ghost_text.loadFromImage(full_image, rect);
            x += 0;
            y += 60 * 2;
            ghost_sprite.setPosition(x, y);
            break;
        case 4:
            rect.top = 64 + 48;
            rect.left = 457;
            ghost_text.loadFromImage(full_image, rect);
            x += 60 * 3;
            y += 60 * 2;
            ghost_sprite.setPosition(x, y);
            break;
        default:
            break;
        }
        this->x_lim = x_lim;
        this->y_lim = y_lim;
        path = new int *[y_lim];
        for (int i = 0; i < y_lim; i++)
        {
            path[i] = new int[x_lim]{};
        }
        ghost_dir = 'n';
        ghost_sprite.setTexture(ghost_text);
        ghost_sprite.setScale(3, 3);
    }
    void draw(sf::RenderWindow &window, float& dtime)
    {
        // ghost_sprite.setPosition(x, y);
        if (dtime >= 5.0f)
        {
            can_die = false;
            // dtime = 0;
        }
        // sem_wait(&sem_blue_ghost);
        if (can_die == false)
        {
            // sem_post(&sem_blue_ghost);
            ghost_sprite.setTexture(ghost_text);
            sem_wait(&sem_window);
            window.draw(ghost_sprite);
            sem_post(&sem_window);
        }
        else
        {
            sem_post(&sem_blue_ghost);
            ghost_sprite.setTexture(blue_text);
            sem_wait(&sem_window);
            window.draw(ghost_sprite);
            sem_post(&sem_window);
        }
    }
    void dir_changer(float x_play, float y_play, int **maze_arr, float dt)
    {
        static float timing = 0;
        timing += dt;
        if (timing <= 0.005)
        {
            // for (int i = 0; i < y_lim; i++)
            // {
            //     for (int j = 0; j < x_lim; j++)
            //     {
            //         // cout << path[i][j] << ", ";
            //         if (path[i][j] == 1)
            //         {
            //             ghost_sprite.setPosition(j * 60, i * 60);
            //             window.draw(ghost_sprite);
            //         }
            //     }
            //     // cout << endl;
            // }
            sem_wait(&sem_player_m);
            ghostcount--;
            if (ghostcount == 0)
                sem_post(&sem_player);
            sem_post(&sem_player_m);
            return;
        }
        timing = 0;

        // sf::Time timing = sf::seconds(0.05f);
        int x_player = x_play, y_player = y_play;
        sem_wait(&sem_player_m);
        ghostcount--;
        if (ghostcount == 0)
            sem_post(&sem_player);
        sem_post(&sem_player_m);
        queue<int> que;
        que.push(this->y / 60);
        que.push(this->x / 60);
        char visited[y_lim][x_lim] = {};
        for (int i = 0; i < y_lim; i++)
        {
            for (int j = 0; j < x_lim; j++)
            {
                path[i][j] = 0;
            }
        }
        // path[y_lim][x_lim] = {};
        int row, col;

        while (!que.empty())
        {
            // sf::sleep(timing);
            row = que.front();
            que.pop();
            col = que.front();
            que.pop();
            if (row == y_player / 60 && col == x_player / 60)
            {
                break;
            }
            if (row + 1 < y_lim && visited[row + 1][col] == 0 && maze_arr[row + 1][col] != 1)
            {
                visited[row + 1][col] = 'u';
                que.push(row + 1);
                que.push(col);
            }
            if (row - 1 >= 0 && visited[row - 1][col] == 0 && maze_arr[row - 1][col] != 1)
            {
                visited[row - 1][col] = 'd';
                que.push(row - 1);
                que.push(col);
            }
            if (col + 1 < x_lim && visited[row][col + 1] == 0 && maze_arr[row][col + 1] != 1)
            {
                visited[row][col + 1] = 'l';
                que.push(row);
                que.push(col + 1);
            }
            if (col - 1 >= 0 && visited[row][col - 1] == 0 && maze_arr[row][col - 1] != 1)
            {
                visited[row][col - 1] = 'r';
                que.push(row);
                que.push(col - 1);
            }
        }
        path[row][col] = 1;
        // cout << row << ", " << col << endl;
        while (1)
        {
            if (row == (int)(this->y / 60) && col == (int)(this->x / 60))
                break;
            switch (visited[row][col])
            {
            case 'u':
                row--;
                path[row][col] = 1;
                break;
            case 'd':
                row++;
                path[row][col] = 1;
                break;
            case 'l':
                col--;
                path[row][col] = 1;
                break;
            case 'r':
                col++;
                path[row][col] = 1;
                break;
            }
        }
        path[row][col] = 1;
        // cout << "Path\n";
        // for (int i = 0; i < y_lim; i++)
        // {
        //     for (int j = 0; j < x_lim; j++)
        //     {
        //         if (visited[i][j] == 0)
        //             cout << " , ";
        //         else
        //             cout << visited[i][j] << ", ";
        //     }
        //     cout << endl;
        // }
    }
    void moving(float dt)
    {

        switch (ghost_dir)
        {
        case 'u':
            if ((int)(this->y) / 60 == (int)(this->y + 48) / 60 && path[(int)((this->y + 48) / 60)][(int)((this->x + 48) / 60)] == 1)
                ghost_dir = 'n';
            break;
        case 'd':
            if ((int)(this->y) / 60 == (int)(this->y + 48) / 60 && path[(int)((this->y + 48) / 60)][(int)((this->x + 48) / 60)] == 1)
                ghost_dir = 'n';
            break;
        case 'l':
            if ((int)(this->x) / 60 == (int)(this->x + 48) / 60 && path[(int)((this->y + 48) / 60)][(int)((this->x + 48) / 60)] == 1)
                ghost_dir = 'n';
            break;
        case 'r':
            if ((int)(this->x) / 60 == (int)(this->x + 48) / 60 && path[(int)((this->y + 48) / 60)][(int)((this->x + 48) / 60)] == 1)
                ghost_dir = 'n';
            break;
        }
        if (ghost_dir != 'n')
        {
            return;
        }
        if ((int)((this->y) / 60) - 1 >= 0 && path[(int)((this->y + 48) / 60) - 1][(int)((this->x + 48) / 60)] == 1 && path[(int)((this->y + 48) / 60) - 1][(int)((this->x) / 60)] == 1)
        {
            path[(int)((this->y + 48) / 60)][(int)((this->x + 48) / 60)] = 0;
            ghost_dir = 'u';
        }

        else if ((int)((this->y) / 60) + 1 < y_lim && path[(int)((this->y) / 60) + 1][(int)((this->x + 48) / 60)] == 1 && path[(int)((this->y) / 60) + 1][(int)((this->x) / 60)] == 1)
        {
            path[(int)((this->y + 48) / 60)][(int)((this->x + 48) / 60)] = 0;
            ghost_dir = 'd';
        }

        else if ((int)((this->x + 48) / 60) - 1 >= 0 && path[(int)((this->y + 48) / 60)][(int)((this->x + 48) / 60) - 1] == 1 && path[(int)((this->y) / 60)][(int)((this->x + 48) / 60) - 1] == 1)
        {
            path[(int)((this->y + 48) / 60)][(int)((this->x + 48) / 60)] = 0;
            ghost_dir = 'l';
        }

        else if ((int)((this->x) / 60) + 1 < x_lim && path[(int)((this->y + 48) / 60)][(int)((this->x) / 60) + 1] == 1 && path[(int)((this->y) / 60)][(int)((this->x) / 60) + 1] == 1)
        {
            path[(int)((this->y + 48) / 60)][(int)((this->x + 48) / 60)] = 0;
            ghost_dir = 'r';
        }
        else
        {
            ghost_dir = 'n';
        }
        // ghost_sprite.setPosition(x, y);
    }

    void move(float dtime)
    {
        if (!key && !permit)
            return;
        
        // int speed = 100;
        moving(dtime);
        switch (ghost_dir)
        {
        case 'u':
            y -= speed * dtime;
            // ghost_sprite.move(0, speed * dt);
            ghost_sprite.move(0, -speed * dtime);
            break;
        case 'd':
            y += speed * dtime;
            // ghost_sprite.move(0, speed * dt);
            ghost_sprite.move(0, speed * dtime);
            break;
        case 'r':
            x += speed * dtime;
            // ghost_sprite.move(speed * dt, 0);
            ghost_sprite.move(speed * dtime, 0);
            break;
        case 'l':
            x -= speed * dtime;
            // ghost_sprite.move(speed * dt, 0);
            ghost_sprite.move(-speed * dtime, 0);
            break;
        }
    }

    void permit_key()
    {
        sf::Time waiter = sf::seconds(5);
        sf::Clock tempck;
        float temp_tmr = 0;
        while (!permit || !key)
        {

            if (rand() % 5 == 1)
            {
                sem_wait(&sem_permit);
                permit = true;
                while (!key)
                {
                    if (rand() % 5 == 1)
                    {
                        sem_wait(&sem_key);
                        key = true;
                    }
                }
            }
        }
        sleep(waiter);
        sem_post(&sem_key);
        sem_post(&sem_permit);
    }


};