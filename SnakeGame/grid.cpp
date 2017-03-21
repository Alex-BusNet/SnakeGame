#include "grid.h"
#include <time.h>
#include <QDebug>

QPen outlinePen(Qt::black);
QBrush foodBrush(Qt::red);
QBrush snakeBrush(Qt::blue);
QBrush snakeHead(Qt::green);
QPen borderPen(Qt::gray);


Grid::Grid(int sizeX, int sizeY, bool lunatic)
{
    srand(time(0));
    this->sizeX = sizeX;
    this->sizeY = sizeY;

    int posX = 10, posY = 25;
    int column = 0, row = 0;
    Tile* t;

    border = new QRect(posX, posY, 30 * sizeX, 30 * sizeY);

    for(int i = 0; i < sizeX; i++)
    {
        for(int j = 0; j < sizeY; j++)
        {
            t = new Tile(posX, posY, column, row, board.size(), lunatic);
            board.push_back(t);
            posY += 30;
            row++;
        }

        posX += 30;
        posY = 25;
        row = 0;
        column++;
    }

    board.last()->SetTileState(BODY_START);
    board.at(23)->SetTileState(FOOD);
    currentTile = board.last()->GetIndex();
    lastTile = board.last()->GetIndex();
    snake.push_back(currentTile);
    gameOver = false;
    victory = false;
    draw = true;
    score = 0;

    borderPen.setWidth(3);
}

void Grid::DrawGrid(QPainter &paint)
{
    if(draw)
    {
        QRadialGradient background(QPointF((30 * sizeX) / 2, (30 * sizeY) / 2), 400);
        background.setColorAt(0, Qt::white);
        background.setColorAt(1, Qt::darkCyan);

        paint.setPen(borderPen);
        paint.setBrush(QBrush(background));
        paint.drawRect(*border);

        paint.setPen(outlinePen);
        foreach(Tile* t, board)
        {
            switch(t->GetTileState())
            {
            case FOOD:
                paint.fillRect(QRect(t->GetSnakePoint(0), t->GetSnakePoint(2)), foodBrush);
                break;
            case BODY_START:
                paint.fillRect(QRect(t->GetSnakePoint(9), t->GetSnakePoint(1)), snakeHead);
                break;
            case BODY_HORIZONTAL:
                paint.fillRect(QRect(t->GetSnakePoint(11), t->GetSnakePoint(7)), snakeBrush);
                break;
            case BODY_VERTICAL:
                paint.fillRect(QRect(t->GetSnakePoint(4), t->GetSnakePoint(8)), snakeBrush);
                break;
            case BODY_NORTH_EAST:
                paint.fillRect(QRect(t->GetSnakePoint(4), t->GetSnakePoint(1)), snakeBrush);
                paint.fillRect(QRect(t->GetSnakePoint(0), t->GetSnakePoint(7)), snakeBrush);
                break;
            case BODY_NORTH_WEST:
                paint.fillRect(QRect(t->GetSnakePoint(4), t->GetSnakePoint(1)), snakeBrush);
                paint.fillRect(QRect(t->GetSnakePoint(11), t->GetSnakePoint(2)), snakeBrush);
                break;
            case BODY_SOUTH_EAST:
                paint.fillRect(QRect(t->GetSnakePoint(6), t->GetSnakePoint(3)), snakeBrush);
                paint.fillRect(QRect(t->GetSnakePoint(9), t->GetSnakePoint(2)), snakeBrush);
                break;
            case BODY_SOUTH_WEST:
                paint.fillRect(QRect(t->GetSnakePoint(11), t->GetSnakePoint(3)), snakeBrush);
                paint.fillRect(QRect(t->GetSnakePoint(9), t->GetSnakePoint(1)), snakeBrush);
                break;
            case HEAD_EAST:
                paint.fillRect(QRect(t->GetSnakePoint(11), t->GetSnakePoint(2)), snakeHead);
                break;
            case HEAD_WEST:
                paint.fillRect(QRect(t->GetSnakePoint(6), t->GetSnakePoint(3)), snakeHead);
                break;
            case HEAD_SOUTH:
                paint.fillRect(QRect(t->GetSnakePoint(4), t->GetSnakePoint(2)), snakeHead);
                break;
            default:
                break;
            }
        }
    }
}

void Grid::MoveUp()
{
    if(board.at(currentTile)->GetGridY() - 1 >= 0)
    {
        int prevX = board.at(lastTile)->GetGridX();
        int curX = board.at(currentTile)->GetGridX();

        if(prevX == curX)
        {
            //Already moving up
            if(board.at(currentTile - 1)->GetTileState() == EMPTY && (currentTile != lastTile))
                board.at(currentTile)->SetTileState(BODY_VERTICAL);
            else if(board.at(currentTile - 1)->GetTileState() == FOOD)
            {
                GrowSnake();
                board.at(currentTile)->SetTileState(BODY_VERTICAL);
            }
            else if(currentTile != lastTile)
            {
                gameOver = true;
                return;
            }
        }
        else if(prevX < curX)
        {
            //Moving left to right
            if(board.at(currentTile - 1)->GetTileState() == EMPTY && (currentTile != lastTile))
                board.at(currentTile)->SetTileState(BODY_NORTH_WEST);
            else if(board.at(currentTile - 1)->GetTileState() == FOOD)
            {
                GrowSnake();
                board.at(currentTile)->SetTileState(BODY_NORTH_WEST);
            }
            else if(currentTile != lastTile)
            {
                gameOver = true;
                return;
            }
        }
        else if(prevX > curX)
        {
            //Moving right to left
            if(board.at(currentTile - 1)->GetTileState() == EMPTY && (currentTile != lastTile))
                board.at(currentTile)->SetTileState(BODY_NORTH_EAST);
            else if(board.at(currentTile - 1)->GetTileState() == FOOD)
            {
                GrowSnake();
                board.at(currentTile)->SetTileState(BODY_NORTH_EAST);
            }
            else if(currentTile != lastTile)
            {
                gameOver = true;
                return;
            }
        }

        snake.push_front(currentTile);

        board.at(currentTile - 1)->SetTileState(BODY_START);
        lastTile = currentTile;
        currentTile--;
        UpdateSnake();
    }
    else
    {
        gameOver = true;
    }
}

void Grid::MoveDown()
{
    if(board.at(currentTile)->GetGridY() + 1 < sizeY)
    {
        int prevX = board.at(lastTile)->GetGridX();
        int curX = board.at(currentTile)->GetGridX();

        if(prevX == curX)
        {
            //already moving downward
            if(board.at(currentTile + 1)->GetTileState() == EMPTY && (currentTile != lastTile))
                board.at(currentTile)->SetTileState(BODY_VERTICAL);
            else if(board.at(currentTile + 1)->GetTileState() == FOOD)
            {
                GrowSnake();
                board.at(currentTile)->SetTileState(BODY_VERTICAL);
            }
            else if(currentTile != lastTile)
            {
                gameOver = true;
                return;
            }
        }
        else if(prevX < curX)
        {
            //Moving left to right
            if(board.at(currentTile + 1)->GetTileState() == EMPTY && (currentTile != lastTile))
                board.at(currentTile)->SetTileState(BODY_SOUTH_WEST);
            else if(board.at(currentTile + 1)->GetTileState() == FOOD)
            {
                GrowSnake();
                board.at(currentTile)->SetTileState(BODY_SOUTH_WEST);
            }
            else if(currentTile != lastTile)
            {
                gameOver = true;
                return;
            }
        }
        else if(prevX > curX)
        {
            //Moving right to left
            if(board.at(currentTile + 1)->GetTileState() == EMPTY && (currentTile != lastTile))
                board.at(currentTile)->SetTileState(BODY_SOUTH_EAST);
            else if(board.at(currentTile + 1)->GetTileState() == FOOD)
            {
                GrowSnake();
                board.at(currentTile)->SetTileState(BODY_SOUTH_EAST);
            }
            else if(currentTile != lastTile)
            {
                gameOver = true;
                return;
            }
        }

        snake.push_front(currentTile);

        board.at(currentTile + 1)->SetTileState(HEAD_SOUTH);
        lastTile = currentTile;
        currentTile++;
        UpdateSnake();
    }
    else
    {
        gameOver = true;
    }
}

void Grid::MoveLeft()
{
    if(board.at(currentTile)->GetGridX() > 0)
    {
        int prevY= board.at(lastTile)->GetGridY();
        int curY = board.at(currentTile)->GetGridY();

        if(prevY == curY)
        {
            //Already moving left
            if(board.at(currentTile - sizeY)->GetTileState() == EMPTY && (currentTile != lastTile))
                board.at(currentTile)->SetTileState(BODY_HORIZONTAL);
            else if(board.at(currentTile - sizeY)->GetTileState() == FOOD)
            {
                GrowSnake();
                board.at(currentTile)->SetTileState(BODY_HORIZONTAL);
            }
            else if(currentTile != lastTile)
            {
                gameOver = true;
                return;
            }
        }
        else if(prevY < curY)
        {
            //Moving down
            if(board.at(currentTile - sizeY)->GetTileState() == EMPTY && (currentTile != lastTile))
                board.at(currentTile)->SetTileState(BODY_NORTH_WEST);
            else if(board.at(currentTile)->GetTileState() == FOOD)
            {
                GrowSnake();
                board.at(currentTile)->SetTileState(BODY_NORTH_WEST);
            }
            else if(currentTile != lastTile)
            {
                gameOver = true;
                return;
            }
        }
        else if(prevY > curY)
        {
            //Moving up
            if(board.at(currentTile - sizeY)->GetTileState() == EMPTY && (currentTile != lastTile))
                board.at(currentTile)->SetTileState(BODY_SOUTH_WEST);
            else if(board.at(currentTile - sizeY)->GetTileState() == FOOD)
            {
                GrowSnake();
                board.at(currentTile)->SetTileState(BODY_SOUTH_WEST);
            }
            else if(currentTile != lastTile)
            {
                gameOver = true;
                return;
            }
        }

        snake.push_front(currentTile);

        board.at(currentTile - sizeY)->SetTileState(HEAD_WEST);
        lastTile = currentTile;
        currentTile -= sizeY;
        UpdateSnake();
    }
    else
    {
        gameOver = true;
    }
}

void Grid::MoveRight()
{
    if(board.at(currentTile)->GetGridX() + 1 < sizeX)
    {
        int prevY = board.at(lastTile)->GetGridY();
        int curY = board.at(currentTile)->GetGridY();

        if(prevY == curY)
        {
            //Already Moving right
            if(board.at(currentTile + sizeY)->GetTileState() == EMPTY && (currentTile != lastTile))
                board.at(currentTile)->SetTileState(BODY_HORIZONTAL);
            else if(board.at(currentTile + sizeY)->GetTileState() == FOOD)
            {
                GrowSnake();
                board.at(currentTile)->SetTileState(BODY_HORIZONTAL);
            }
            else if(currentTile != lastTile)
            {
                gameOver = true;
                return;
            }
        }
        else if(prevY < curY)
        {
            //Moving down
            if(board.at(currentTile + sizeY)->GetTileState() == EMPTY && (currentTile != lastTile))
                board.at(currentTile)->SetTileState(BODY_NORTH_EAST);
            else if(board.at(currentTile + sizeY)->GetTileState() == FOOD)
            {
                GrowSnake();
                board.at(currentTile)->SetTileState(BODY_NORTH_EAST);
            }
            else if(currentTile != lastTile)
            {
                gameOver = true;
                return;
            }
        }
        else if(prevY > curY)
        {
            //Moving up
            if(board.at(currentTile + sizeY)->GetTileState() == EMPTY && (currentTile != lastTile))
                board.at(currentTile)->SetTileState(BODY_SOUTH_EAST);
            else if(board.at(currentTile + sizeY)->GetTileState() == FOOD)
            {
                GrowSnake();
                board.at(currentTile)->SetTileState(BODY_SOUTH_EAST);
            }
            else if(currentTile != lastTile)
            {
                gameOver = true;
                return;
            }
        }

        snake.push_front(currentTile);

        board.at(currentTile + sizeY)->SetTileState(HEAD_EAST);
        lastTile = currentTile;
        currentTile += sizeY;
        UpdateSnake();
    }
    else
    {
        gameOver = true;
    }
}

int Grid::GetScore()
{
    return (snake.size() - 1) * 50;
}

bool Grid::isGameOver()
{
    if(gameOver)
        draw = false;

    return gameOver;
}

bool Grid::isVictory()
{
    if(snake.size() == board.size())
    {
        draw = false;
        return true;
    }

    return false;
}

void Grid::UpdateSnake()
{
    //Get last integer from vector and
    //change the tile to EMPTY
    board.at(snake.last())->SetTileState(EMPTY);

    if(snake.size() > 1)
        snake.removeLast();
}

Tile *Grid::GetTileAt(int index)
{
    return board.at(index);
}

Tile *Grid::GetTileAt(int x, int y)
{
    return board.at(y + (sizeY * x));
}

void Grid::GrowSnake()
{
    if(snake.size() > 1)
    {
        int tail = snake.last();
        QList<int> neighbors = GetEmptyNeighbors(tail);

        if(neighbors.isEmpty())
            return;

        int dir = GetSnakeDirection();
        int n;

        if((dir == 0 || dir == 1) && neighbors.size() > 1)
        {
            n = neighbors.at(1);
            board.at(n)->SetTileState(BODY_VERTICAL);
        }
        else
        {
            int dst;
            if(dir == 2) //East
            {
                dst = tail - neighbors.first();
                if(dst == sizeY)
                {
                    board.at(neighbors.first())->SetTileState(BODY_HORIZONTAL);
                    n = neighbors.first();
                }
                else
                {
                    foreach(int i, neighbors)
                    {
                        dst = tail - i;
                        if(dst == -1)
                        {
                            board.at(i)->SetTileState(BODY_SOUTH_EAST);
                            n = i;
                            break;
                        }
                        else if(dst == 1)
                        {
                            board.at(i)->SetTileState(BODY_NORTH_EAST);
                            n = i;
                            break;
                        }
                    }
                }
            }
            else if(dir == 3) //West
            {
                dst = tail - neighbors.last();
                if(dst == -sizeY)
                {
                    board.at(neighbors.last())->SetTileState(BODY_HORIZONTAL);
                    n = neighbors.last();
                }
                else
                {
                    foreach(int i, neighbors)
                    {
                       dst = tail - i;
                       if(dst == -1)
                       {
                           board.at(i)->SetTileState(BODY_SOUTH_WEST);
                           n = i;
                           break;
                       }
                       else if(dst == 1)
                       {
                           board.at(i)->SetTileState(BODY_NORTH_WEST);
                           n = i;
                           break;
                       }
                    }
                }
            }
            else if(dir == 0) //North
            {
                foreach(int i, neighbors)
                {
                   dst = tail - i;
                   if(dst == -1)
                   {
                       board.at(i)->SetTileState(BODY_VERTICAL);
                       n = i;
                       break;
                   }
                   else if(dst == sizeY)
                   {
                       board.at(i)->SetTileState(BODY_NORTH_WEST);
                       n = i;
                       break;
                   }
                   else if(dst == -sizeY)
                   {
                       board.at(i)->SetTileState(BODY_NORTH_EAST);
                       n = i;
                       break;
                   }
                }
            }
            else if(dir == 1) //South
            {
                foreach(int i, neighbors)
                {
                   dst = tail - i;
                   if(dst == 1)
                   {
                       board.at(i)->SetTileState(BODY_VERTICAL);
                       n = i;
                       break;
                   }
                   else if(dst == sizeY)
                   {
                       board.at(i)->SetTileState(BODY_SOUTH_WEST);
                       n = i;
                       break;
                   }
                   else if(dst == -sizeY)
                   {
                       board.at(i)->SetTileState(BODY_SOUTH_EAST);
                       n = i;
                       break;
                   }
                }
            }
        }

        snake.push_back(n);
    }
    else
    {
        snake.push_back(lastTile);
    }

    int i = currentTile;

    while(board.at(i)->GetTileState() != FOOD)
    {
        i = rand() % board.size();

        if(board.at(i)->GetTileState() == EMPTY)
            board.at(i)->SetTileState(FOOD);
    }
}

QList<int> Grid::GetEmptyNeighbors(int tile)
{
    QList<int> neighbors;

    int nodeX = board.at(tile)->GetGridX();
    int nodeY = board.at(tile)->GetGridY();
    Tile* t;
    for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            if((abs(x) == 1 && y != 0) || (abs(y) == 1 && x != 0) || (x == 0 && y == 0))
                continue;

            if(((nodeX + x >= sizeX) || (nodeX + x < 0)) || ((nodeY + y >= sizeY) || (nodeY + y < 0)))
                continue;

            t = GetTileAt(nodeX + x, nodeY + y);
            if(t->GetTileState() == EMPTY)
                neighbors.push_back(t->GetIndex());
        }
    }

    return neighbors;
}

/*
 * 0 = North, 1 = South, 2 = East, 3 = West
 */
int Grid::GetSnakeDirection()
{
    int end = snake.last();

    int nodeX = board.at(end)->GetGridX();
    int nodeY = board.at(end)->GetGridY();

    int tX = board.at(snake.at(snake.size() - 2))->GetGridX();
    int tY = board.at(snake.at(snake.size() - 2))->GetGridY();

    if(nodeX > tX)
    {
        return 2; //Body connected to the east
    }
    else if(nodeX < tX)
    {
        return 3; //Body connected to the west
    }
    else
    {
        if(nodeY > tY)
        {
            return 0; //Body connected to the north
        }
        else
        {
            return 1; //Body connected to the south
        }
    }
}


