#ifndef GAME_H
    
    #define GAME_H

    #include <stdint.h>
    #include <stdbool.h>
    #include "videobuffer.h"

    #define SNAKE_SCALE 5
    #define SNAKE_LENGTH 255
    #define SNAKE_START_LENGTH 15

    #define FRUIT_SIZE 5

    typedef struct Game
    {
        bool isAlive;
        uint8_t score;
    } Game;

    typedef struct Coords
    {
        uint8_t x;
        uint8_t y;
    } Coords;

    typedef struct Velocity
    {
        int8_t x;
        int8_t y;
    } Velocity;

    typedef struct Snake
    {
        Coords coords[SNAKE_LENGTH];
        Velocity velocity[SNAKE_LENGTH];
        Velocity prevDirection;
    } Snake;

    typedef struct Fruit
    {
        Coords coords;
    } Fruit;
    

    extern Game game;
    extern Snake snake;
    extern VideoBuffer videoBuffer;
    extern Fruit fruit;

    void moveSnake();

    void initSnake();

    void initGame();

    void stopGame();

    void placeFruit(uint8_t x, uint8_t y);

    void drawSnake();

    void drawFruit(DrawMethod draw);

    void eatFruit();

    void gameUpdate();

#endif