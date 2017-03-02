#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QWidget>
#include <qevent.h>
#include "grid.h"

class GameManager : public QWidget
{
    Q_OBJECT
public:
    explicit GameManager(QWidget *parent = 0);

private:
    Grid *grid;
    QTimer* updateTimer;
    QPushButton *easy, *medium, *hard, *lunatic;
    QLabel *currentScore;

    int currentDirection;

    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void endGame();
    void win();
    bool gameEnd;

    void UpdateHighscores();

signals:

public slots:
    void moveSnake();
    void setEasy();
    void setMedium();
    void setHard();
    void setLunatic();

};

#endif // GAMEMANAGER_H
