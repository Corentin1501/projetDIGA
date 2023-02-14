#pragma once
#include <QtWidgets>
#include <vector>
#include <iostream>

#include "dialognewgame.h"

using namespace std;

struct bouton {
    QPushButton* button;
    int valeurDuBouton;
    int row;
    int column;
};

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    GameWidget(QWidget *parent = nullptr, int largeur=3, std::string bg="original");
    GameWidget(QWidget *parent,
               QGridLayout* grille,
               int largeur,
               QPoint* trou,
               vector<QPushButton*> boutons,
               vector<QPushButton*> boutonsPossibles,
               std::string bg);
    ~GameWidget();

public slots:
    void setBackgroundOriginal();
    void setBackgroundForest();
    void setBackgroundTree();
    void setBackgroundNetwork();
    void boutonClique(bool);

private:
    QGridLayout * creerGrille(int largeur);
    void echanger(QPushButton * bouton, QPoint * trou);
    void chargerImage(QImage image);

    std::string     _background;
    int             _largeurGrille;
    QGridLayout*    _grille;

    vector<QPushButton*> _vectorBoutons;
    vector<QPushButton*> _vectorBoutonsPossibles;
    QPoint* _positionTrou;
};
