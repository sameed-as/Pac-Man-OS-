#include <iostream>
#include <SFML/Graphics.hpp>
#include <semaphore.h>
using namespace std;

sem_t sem_window, sem_player;

struct Player
{
    sf::Texture player_text;
    sf::Sprite player_sprite;
    sf::Image full_texture;
    float x, y;
    int text_x, text_y;
    char dir, last_dir;
    int lives;

public:
    Player()
    {
        full_texture;
        full_texture.loadFromFile("res/Pac-man.png");
        text_x = 457;
        text_y = 0;
        sf::IntRect rect(text_x, text_y, 16, 16);
        player_text.loadFromImage(full_texture, rect);
        player_sprite.setTexture(player_text);
        x = (16 * 60);
        y = (11 * 60);
        player_sprite.setPosition(x, y);
        player_sprite.setScale(3, 3);

        dir = 'n';
        last_dir = dir;
        lives = 3;
    }
    void draw(sf::RenderWindow &window)
    {
        // player_sprite.setPosition(x, y);
        sem_wait(&sem_player);
        int rot = player_sprite.getRotation();
        float tempx = player_sprite.getPosition().x, tempy = player_sprite.getPosition().y;
        for (int i = 1; i <= lives; i++)
        {
            player_sprite.setPosition(60 * (i - 1), 0);
            player_sprite.setRotation(0);
            // sem_post(&sem_player);

            sem_wait(&sem_window);
            window.draw(player_sprite);
            sem_post(&sem_window);
        }
        // sem_wait(&sem_player);
        player_sprite.setRotation(rot);
        player_sprite.setPosition(tempx, tempy);

        sem_wait(&sem_window);
        window.draw(player_sprite);
        sem_post(&sem_window);
        sem_post(&sem_player);
    }
    void dirchange(sf::Event event)
    {
        if (event.key.code == sf::Keyboard::Left)
        {
            last_dir = dir;
            dir = 'l';
        }
        else if (event.key.code == sf::Keyboard::Right)
        {
            last_dir = dir;
            dir = 'r';
        }
        else if (event.key.code == sf::Keyboard::Up)
        {
            last_dir = dir;
            dir = 'u';
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            last_dir = dir;
            dir = 'd';
        }
    }
    void move(float dt, sf::RenderWindow &window, int **maze)
    {
        
        sem_wait(&sem_player);
        static float animation_time = 0;
        static float dir_timer = 0;
        int speed = 500;
        int offsetx, offsety;
        float tempSpeed = (dt * speed) / 2;
        sf::Texture win;
        sem_wait(&sem_window);
        win.create(window.getSize().x, window.getSize().y);
        win.update(window);
        int windowy = window.getSize().y, windowx = window.getSize().x;
        sem_post(&sem_window);
        sf::Image img = win.copyToImage();

        animation_time += dt;
        dir_timer += dt;
        if (dir_timer >= 0.5)
        {
            last_dir = 'n';
            dir_timer = 0;
        }
        if (dir == 'n')
        {
            animation_time = 0;
        }
        if (animation_time >= 0.1)
        {
            text_x += 16;
            if (text_x >= 500)
            {
                text_x = 457;
            }
            sf::IntRect rect(text_x, text_y, 16, 16);
            player_text.loadFromImage(full_texture, rect);
            player_sprite.setTexture(player_text);
            animation_time = 0;
        }

        // cout << player_sprite.getGlobalBounds().width << endl;
        if (dir == 'd')
        {
            // float tempSpeed = dt * speed;
            // sem_wait(&sem_player);
            y += tempSpeed;
            // sem_post(&sem_player);
            offsety = 0;
            offsetx = 0;
            if (y + player_sprite.getGlobalBounds().height + 10 > windowy)
            {
                if (y > windowy)
                {
                    // sem_wait(&sem_player);
                    y = 0 - player_sprite.getGlobalBounds().height;
                    player_sprite.setPosition(x + 48, y);
                    sem_post(&sem_player);
                    // sem_post(&sem_player);
                    return;
                }
                offsety = y + player_sprite.getGlobalBounds().height - windowy + 1;
            }
            if (x - 10 < 0)
            {
                offsetx = x - 0;
            }
            else if (x + player_sprite.getGlobalBounds().width + 10 > windowx)
            {
                offsetx = x + player_sprite.getGlobalBounds().width - windowx;
            }

            // if ((img.getPixel(x - offsetx, y + 48 - offsety).toInteger() != 2869901567) && (img.getPixel(x + 48 - offsetx, y + 48 - offsety).toInteger() != 2869901567))
            // cout << "start " << (int)((x - offsetx) / 60) << ", ";
            if ((maze[(int)((y + 48 - offsety) / 60)][(int)((x - offsetx) / 60)] == 0) && (maze[(int)((y + 48 - offsety) / 60)][(int)((x + 48 - offsetx) / 60)] == 0))
            {
                if (player_sprite.getRotation() != 90)
                {
                    player_sprite.setRotation(90);
                    player_sprite.setOrigin(0, 0);
                    x += 0;
                    y += 0;
                    player_sprite.setPosition(x + 48, y);
                }
                else
                {
                    player_sprite.move(0, tempSpeed);
                }
            }
            else
            {
                y -= tempSpeed;
                dir = last_dir;
                dir_timer = 0;
            }
            // cout << "end\n";
            // player_sprite.move(0, tempSpeed);
        }
        else if (dir == 'u')
        {
            // float tempSpeed = dt * speed;
            y -= tempSpeed;
            offsety = 0;
            offsetx = 0;
            if (y - 10 < 0)
            {
                if (y + 48 < 0)
                {
                    y = windowy;
                    player_sprite.setPosition(x, y + 48);
                    sem_post(&sem_player);
                    return;
                }
                offsety = y - 0;
            }
            if (x - 10 < 0)
            {
                offsetx = x - 0;
            }
            else if (x + player_sprite.getGlobalBounds().width + 10 > windowx)
            {
                offsetx = x + player_sprite.getGlobalBounds().width - windowx;
            }

            // if ((img.getPixel(x - offsetx, y - offsety).toInteger() != 2869901567) && (img.getPixel(x + 48 - offsetx, y - offsety).toInteger() != 2869901567))
            if ((maze[(int)((y + 0 - offsety) / 60)][(int)((x - offsetx) / 60)] == 0) && (maze[(int)((y + 0 - offsety) / 60)][(int)((x + 48 - offsetx) / 60)] == 0))
            {
                if (player_sprite.getRotation() != -90)
                {
                    player_sprite.setRotation(-90);
                    player_sprite.setOrigin(0, 0);
                    x += 0;
                    y += 0;
                    player_sprite.setPosition(x, y + 48);
                }
                else
                {
                    player_sprite.move(0, -tempSpeed);
                }
            }
            else
            {
                y += tempSpeed;
                dir = last_dir;
                dir_timer = 0;
            }

            // player_sprite.move(0, -tempSpeed);
        }
        else if (dir == 'l')
        {
            // float tempSpeed = dt * speed;
            x -= tempSpeed;
            offsety = 0;
            offsetx = 0;
            if (x - 10 < 0)
            {
                if (x + 48 < 0)
                {
                    x = windowx;
                    player_sprite.setPosition(x + 48, y + 48);
                    sem_post(&sem_player);
                    return;
                }
                offsetx = x - 0;
            }
            if (y - 10 < 0)
            {
                offsety = y - 0;
            }
            else if (y + player_sprite.getGlobalBounds().height + 10 > windowy)
            {
                offsety = y + player_sprite.getGlobalBounds().height - windowy + 1;
            }
            // if ((img.getPixel(x - offsetx, y + 48 - offsety).toInteger() != 2869901567) && (img.getPixel(x - offsetx, y - offsety).toInteger() != 2869901567))
            if ((maze[(int)((y + 48 - offsety) / 60)][(int)((x + 0 - offsetx) / 60)] == 0) && (maze[(int)((y - offsety) / 60)][(int)((x + 0 - offsetx) / 60)] == 0))
            {
                if (player_sprite.getRotation() != 180)
                {
                    player_sprite.setRotation(180);
                    player_sprite.setOrigin(0, 0);
                    x += 0;
                    y += 0;
                    player_sprite.setPosition(48 + x, y + 48);
                }
                else
                {
                    player_sprite.move(-tempSpeed, 0);
                }
            }
            else
            {

                x += tempSpeed;
                dir = last_dir;
                dir_timer = 0;
            }
        }
        else if (dir == 'r')
        {
            // float tempSpeed = dt * speed;
            x += tempSpeed;
            offsety = 0;
            offsetx = 0;
            if (x + player_sprite.getGlobalBounds().width + 10 > windowx)
            {
                if (x > windowx)
                {
                    x = 0 - player_sprite.getGlobalBounds().width;
                    player_sprite.setPosition(x, y);
                    sem_post(&sem_player);
                    return;
                }
                offsetx = x + player_sprite.getGlobalBounds().width - windowx + 1;
                // cout << x + 48 - offsetx << ", " << window.getSize().x << endl;
            }
            if (y - 10 < 0)
            {
                offsety = y - 0;
            }
            else if (y + player_sprite.getGlobalBounds().height + 10 > windowy)
            {
                offsety = y + player_sprite.getGlobalBounds().height - windowy + 1;
            }
            // if ((img.getPixel(x + 48 - offsetx, y - offsety).toInteger() != 2869901567) && (img.getPixel(x + 48 - offsetx, y + 48 - offsety).toInteger() != 2869901567))
            if ((maze[(int)((y + 48 - offsety) / 60)][(int)((x + 48 - offsetx) / 60)] == 0) && (maze[(int)((y - offsety) / 60)][(int)((x + 48 - offsetx) / 60)] == 0))
            {
                if (player_sprite.getRotation() != 0)
                {
                    player_sprite.setRotation(0);
                    player_sprite.setOrigin(0, 0);
                    x += 0;
                    y += 0;
                    player_sprite.setPosition(x, y);
                }
                else
                {
                    player_sprite.move(tempSpeed, 0);
                }
            }
            else
            {
                x -= tempSpeed;
                dir = last_dir;
                dir_timer = 0;
            }
            // player_sprite.move(tempSpeed, 0);
        }
        // cout << x << ", " << y << endl;
        sem_post(&sem_player);
        // window.draw(player_sprite);
    }
};