#ifndef SNAKE_H
#define SNAKE_H

#include "food.h"

class snake
{
public:
    snake(int x, int y);

    void IncreaseSize();
private:

    QVector<Food*> body;
};

#endif // SNAKE_H
