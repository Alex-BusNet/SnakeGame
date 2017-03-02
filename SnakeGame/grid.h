#ifndef GRID_H
#define GRID_H

#include "tile.h"
#include <QGradient>
#include <QPainter>

class Grid
{
public:
    Grid(int sizeX, int sizeY, bool lunatic);

    void DrawGrid(QPainter &paint);

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();

    int GetScore();
    bool isGameOver();
    bool isVictory();
    void UpdateSnake();

    Tile* GetTileAt(int index);
    Tile* GetTileAt(int x, int y);

private:
    QVector<Tile*> board;
    QVector<int> snake;
    QRect *border;
    int sizeX, sizeY;
    int currentTile, lastTile;
    int score;
    bool gameOver, victory, draw;
    void GrowSnake();
    QList<int> GetEmptyNeighbors(int tile);
    int GetSnakeDirection();
};

#endif // GRID_H
