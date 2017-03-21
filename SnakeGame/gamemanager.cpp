#include "gamemanager.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QVBoxLayout>

int mapX = 20, mapY = 10;
GameManager::GameManager(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize((50 * mapX) + 20, (50 * mapY) + 35);

    grid = NULL;

    easy = new QPushButton("Easy");
    medium = new QPushButton("Medium");
    hard = new QPushButton("Hard");
    lunatic = new QPushButton("Lunatic");
    currentScore = new QLabel(this);
    currentScore->setText(QString("Score: 0000000"));

    connect(easy, SIGNAL(clicked(bool)), this, SLOT(setEasy()));
    connect(medium, SIGNAL(clicked(bool)), this, SLOT(setMedium()));
    connect(hard, SIGNAL(clicked(bool)), this, SLOT(setHard()));
    connect(lunatic, SIGNAL(clicked(bool)), this, SLOT(setLunatic()));

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(easy);
    vLayout->addWidget(medium);
    vLayout->addWidget(hard);
    vLayout->addWidget(lunatic);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addSpacerItem(new QSpacerItem(this->width() / 3, this->height(), QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    hLayout->addLayout(vLayout);
    hLayout->addSpacerItem(new QSpacerItem(this->width() / 3, this->height(), QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

    updateTimer = new QTimer();
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(moveSnake()));

    currentScore->setGeometry(this->width() - 100, 5, 100, 20);
    gameEnd = false;
    this->setLayout(hLayout);
}

void GameManager::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);

    if(grid != NULL && !gameEnd)
    {
        grid->DrawGrid(paint);

        currentScore->setText(QString("Score: %1").arg(grid->GetScore()));

        if(grid->isGameOver())
        {
            endGame();
        }
        else if(grid->isVictory())
        {
            win();
        }
    }
}

void GameManager::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_W:
        if(currentDirection == Qt::Key_S)
            break;
        currentDirection = e->key();
        break;
    case Qt::Key_S:
        if(currentDirection == Qt::Key_W)
            break;
        currentDirection = e->key();
        break;
    case Qt::Key_A:
        if(currentDirection == Qt::Key_D)
            break;
        currentDirection = e->key();
        break;
    case Qt::Key_D:
        if(currentDirection == Qt::Key_A)
            break;
        currentDirection = e->key();
        break;
    default:
        break;
    }

}

void GameManager::endGame()
{
    gameEnd = true;
    updateTimer->stop();
    bool newHigh = UpdateHighscores();

    QMessageBox *mbox = new QMessageBox();

    if(newHigh)
        mbox->setText(QString("You Lose.\nNew High Score: %1").arg(grid->GetScore()));
    else
        mbox->setText(QString("You Lose.\nScore: %1").arg(grid->GetScore()));

    mbox->exec();

    easy->setVisible(true);
    medium->setVisible(true);
    hard->setVisible(true);
    lunatic->setVisible(true);
}

void GameManager::win()
{
    gameEnd = true;
    updateTimer->stop();
    bool newHigh = UpdateHighscores();

    QMessageBox *mbox = new QMessageBox();

    if(newHigh)
        mbox->setText(QString("You Win!\nNew High Score: %1").arg(grid->GetScore()));
    else
        mbox->setText(QString("You Win!\nScore: %1").arg(grid->GetScore()));

    mbox->exec();

    easy->setVisible(true);
    medium->setVisible(true);
    hard->setVisible(true);
    lunatic->setVisible(true);
}

bool GameManager::UpdateHighscores()
{
   QFile highscores("../SnakeGame/highscores.json");
   if(!highscores.open(QIODevice::ReadWrite))
   {
       qWarning("Could not open highscores file");
       return false;
   }
   QByteArray byteArr = highscores.readAll();
   QJsonDocument hs = QJsonDocument::fromJson(byteArr);
   QJsonArray scores = hs.array();

   QVector<int> scoreArr;
   bool newHighscore = false;

   for(int i = 0; i < scores.size(); i++)
   {
       scoreArr.push_back(scores.at(i).toInt());
   }
   scores.empty();

   int newScore = grid->GetScore();

   if(scoreArr.isEmpty())
   {
       scoreArr.push_back(newScore);
       newHighscore = true;
   }
   else
   {
       for(int i = 0; i < scoreArr.size(); i++)
       {
           if(newScore > scoreArr.at(i))
           {
               scoreArr.insert(i, newScore);

               if(i == 0)
                   newHighscore = true;

               break;
           }
       }
   }


   for(int i = 0; i < scoreArr.size(); i++)
   {
       scores.push_back(scoreArr[i]);
   }

   hs.setArray(scores);
   highscores.write(hs.toJson());
   highscores.flush();
   highscores.close();

   return newHighscore;
}

void GameManager::moveSnake()
{
    switch(currentDirection)
    {
    case Qt::Key_W:
        grid->MoveUp();
        break;
    case Qt::Key_S:
        grid->MoveDown();
        break;
    case Qt::Key_A:
        grid->MoveLeft();
        break;
    case Qt::Key_D:
        grid->MoveRight();
        break;
    default:
        break;
    }

    this->update();
}

void GameManager::setEasy()
{
    if(grid != NULL)
    {
        delete grid;
        gameEnd = false;
        currentDirection = Qt::Key_0;
    }

    grid = new Grid(mapX, mapY, false);

    easy->setVisible(false);
    medium->setVisible(false);
    hard->setVisible(false);
    lunatic->setVisible(false);

    updateTimer->setInterval(500);
    updateTimer->start();
}

void GameManager::setMedium()
{
    if(grid != NULL)
    {
        delete grid;
        gameEnd = false;
        currentDirection = Qt::Key_0;
    }

    grid = new Grid(mapX, mapY, false);

    easy->setVisible(false);
    medium->setVisible(false);
    hard->setVisible(false);
    lunatic->setVisible(false);

    updateTimer->setInterval(250);
    updateTimer->start();
}

void GameManager::setHard()
{
    if(grid != NULL)
    {
        delete grid;
        gameEnd = false;
        currentDirection = Qt::Key_0;
    }

    grid = new Grid(mapX, mapY, false);

    easy->setVisible(false);
    medium->setVisible(false);
    hard->setVisible(false);
    lunatic->setVisible(false);

    updateTimer->setInterval(125);
    updateTimer->start();
}

void GameManager::setLunatic()
{
    if(grid != NULL)
    {
        delete grid;
        gameEnd = false;
        currentDirection = Qt::Key_0;
    }

    grid = new Grid(mapX, mapY, false);

    easy->setVisible(false);
    medium->setVisible(false);
    hard->setVisible(false);
    lunatic->setVisible(false);

    updateTimer->setInterval(50);
    updateTimer->start();
}
