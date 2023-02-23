#pragma once
#include <QtWidgets>
#include <vector>
#include <iostream>


#include "dialognewgame.h"

using namespace std;

struct boutonStruct {
    QPushButton* button;
    int valeurDuBouton;
    int row;
    int column;
};

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    // constructeur quand on fait "new game" ou à la création de la fenetre
    GameWidget(QMainWindow *parent = nullptr, int largeur=3, std::string bg="original", bool random=false);
    // constructeur quand on charge une partie
    GameWidget(QMainWindow *parent,
               QGridLayout* grille,
               int largeur,
               QPoint* trou,
               vector<boutonStruct*> boutons,
               vector<boutonStruct*> boutonsPossibles,
               std::string bg);

    ~GameWidget();

    // accesseurs
                      int getLargeur()  const { return _largeurGrille; }
                     bool getVictoire() const { return _victoire;      }
                  QPoint* getTrou()     const { return _positionTrou;  }
             QGridLayout* getGrille()   const { return _grille;        }
    vector<boutonStruct*> getBoutons()  const { return _vectorBoutons; }



public slots:
    //### Background ###
        void setBackgroundOriginal();
        void setBackgroundForest();
        void setBackgroundTree();
        void setBackgroundNetwork();

    //### Clic des boutons ###
        void boutonClique(bool);

private:
    QGridLayout * creerGrille(int largeur, bool random);
    void bouger(boutonStruct* bouton);
    void chargerImage(QImage image);

    void afficherBoutons();
    void afficherBoutonsPossibles();

    void verifierSiVictoire();

    void setBoutonsPossible();

    int             _largeurGrille;
    QGridLayout*    _grille;
    std::string     _background;

    vector<boutonStruct *> _vectorBoutons;
    vector<boutonStruct *> _vectorBoutonsPossibles;
    QPoint* _positionTrou;

    bool _victoire;
};
