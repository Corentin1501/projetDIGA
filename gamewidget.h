#pragma once
#include <QtWidgets>
#include <vector>
#include <iostream>

#include "dialognewgame.h"

using namespace std;

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    GameWidget(QWidget *parent = nullptr, int largeur=3, std::string bg="tree");
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
    void echanger(QPushButton * bouton, QPoint * trou);

    std::string     _background;
    int             _largeurGrille;
    QGridLayout*    _grille;

    vector<QPushButton*> _vectorBoutons;
    vector<QPushButton*> _vectorBoutonsPossibles;
    QPoint* _positionTrou;
};
