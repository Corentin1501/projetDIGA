#ifndef GameWidget_H
#define GameWidget_H

#include <QtWidgets>
#include <vector>

#include "dialognewgame.h"

using namespace std;

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    GameWidget(QWidget *parent = nullptr, int largeur=3);
    ~GameWidget();

public slots:
    void setBackgroundOriginal(bool);
    void setBackgroundForest(bool);
    void setBackgroundTree(bool);
    void setBackgroundNetwork(bool);

private slots:
    void boutonClique(bool);


private:
    QGridLayout * creerGrille(int largeur);
//    QVBoxLayout * createSideButton();

    QImage * _background;
    int _largeurGrille;

//    QHBoxLayout* _layoutH;
    QGridLayout* _grille;
//    QVBoxLayout* _sideButtons;

//    QPushButton* _newGame;
//    QPushButton* _loadGame;
//    QPushButton* _saveGame;

//    int _moves;
//    QLabel * _labelMoves;

};

#endif // GameWidget_H
