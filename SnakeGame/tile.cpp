#include "tile.h"

/*
 *   Snake point breakdown
 *       (for squares)
 *   ------4------5-------
 *   |     |      |      |
 *   |     |      |      |
 *  11     0------1      6
 *   |     |      |      |
 *   |     |      |      |
 *  10     3------2      7
 *   |     |      |      |
 *   |     |      |      |
 *   ------9------8-------
 */

int drawScale = 1;

Tile::Tile(int _posX, int _posY, int col, int row, int index, bool isHexagon)
{
    if(isHexagon)
    {
        //HEXAGONS (Lunatic mode)
        points[0] = QPoint(_posX + (22 * drawScale), _posY - (12 * drawScale));
        points[1] = QPoint(_posX + (44 * drawScale), _posY);
        points[2] = QPoint(_posX + (44 * drawScale), _posY + (25 * drawScale));
        points[3] = QPoint(_posX + (22 * drawScale), _posY + (37 * drawScale));
        points[4] = QPoint(_posX, _posY + (25 * drawScale));
        points[5] = QPoint(_posX, _posY);
        points[6] = QPoint(_posX + (22 * drawScale), _posY - (12 * drawScale));

        center = QPoint(_posX + (22 * drawScale), _posY + (12 * drawScale));

        this->poly << this->points[0]
                << this->points[1]
                << this->points[2]
                << this->points[3]
                << this->points[4]
                << this->points[5]
                << this->points[6];
    }
    else
    {
        //SQUARES
        ///TODO: Make the squares smaller

        this->poly << QPoint(_posX, _posY)
                   << QPoint(_posX + 50, _posY)
                   << QPoint(_posX + 50, _posY + 50)
                   << QPoint(_posX, _posY + 50);

        center = QPoint(_posX + 25, _posY + 25);

        snakePoints[0] = QPoint(_posX + 13, _posY + 13);
        snakePoints[1] = QPoint(_posX + 37, _posY + 13);
        snakePoints[2] = QPoint(_posX + 37, _posY + 37);
        snakePoints[3] = QPoint(_posX + 13, _posY + 37);
        snakePoints[4] = QPoint(_posX + 13, _posY);
        snakePoints[5] = QPoint(_posX + 37, _posY);
        snakePoints[6] = QPoint(_posX + 50, _posY + 13);
        snakePoints[7] = QPoint(_posX + 50, _posY + 37);
        snakePoints[8] = QPoint(_posX + 37, _posY + 50);
        snakePoints[9] = QPoint(_posX + 13, _posY + 50);
        snakePoints[10] = QPoint(_posX, _posY + 37);
        snakePoints[11] = QPoint(_posX, _posY + 13);
    }

    this->index = index;
    this->gridX = col;
    this->gridY = row;
    ts = EMPTY;

}

QPoint Tile::GetCenter()
{
    return center;
}

QPolygon Tile::GetTilePolygon()
{
    return poly;
}

QPoint Tile::GetSnakePoint(int index)
{
    return snakePoints[index];
}

TileState Tile::GetTileState()
{
    return this->ts;
}

void Tile::SetTileState(TileState state)
{
    this->ts = state;
}

int Tile::GetIndex()
{
    return this->index;
}

int Tile::GetGridX()
{
    return this->gridX;
}

int Tile::GetGridY()
{
    return this->gridY;
}
