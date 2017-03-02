#ifndef TILE_H
#define TILE_H

#include <QPoint>
#include <QPolygon>
#include "datatypes.h"


class Tile
{
public:
    Tile(int _posX, int _posY, int col, int row, int index, bool isHexagon);
    QPoint GetCenter();
    QPolygon GetTilePolygon();
    QPoint GetSnakePoint(int index);

    TileState GetTileState();
    void SetTileState(TileState state);
    int GetIndex();
    int GetGridX();
    int GetGridY();

private:
    QPoint points[7];
    QPoint snakePoints[12];
    QPoint center;
    QPolygon poly;

    TileState ts;
    int gridX, gridY;
    int index;
};

#endif // TILE_H
