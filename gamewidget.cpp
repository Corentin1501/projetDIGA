#include "gamewidget.h"

#include <iostream>
#include <algorithm>

//####################################################
//            Constructeur / destructeur            //
//####################################################

    GameWidget::GameWidget(QWidget *parent, int largeur, std::string bg) :
        QWidget(parent),
        _background(bg),
        _largeurGrille(largeur),
        _positionTrou(new QPoint(largeur-1,largeur-1))
    {
        _grille = creerGrille(_largeurGrille);
        this->setLayout(_grille);
    }

    GameWidget::~GameWidget() {}

//####################################################
//            Creer gridlayout de boutons           //
//####################################################

    QGridLayout* GameWidget::creerGrille(int largeur)
    {
        QGridLayout* grille = new QGridLayout;
        int valeurDuBouton(0);

        QImage image;
        if(_background == "forest") image = QImage("foret.jpg");
        else if(_background == "tree") image = QImage("arbee.jpg");
        else if(_background == "network") image = QImage("reseau.jpeg");

        int width = image.width() / 3;
        int height = image.height() / 3;

        for (int i = 0; i < largeur; ++i)
        {
            for (int j = 0; j < largeur; ++j)
            {
                valeurDuBouton++;
                if(valeurDuBouton != largeur*largeur)
                {
                    QPushButton *button = new QPushButton(QString::number(valeurDuBouton));

                    connect(button, &QPushButton::clicked, this, &GameWidget::boutonClique);
                    button->setStyleSheet("font:Bold; font-size:20px;");

                    if(image.isNull()) // si le background est "original"
                    {
                        button->setStyleSheet("background-color: red; font:Bold; font-size:20px;");
                    }
                    else
                    {
                        button->setIcon(QPixmap::fromImage(image.copy(j*width, i*height, width, height)));
                        button->setIconSize(QSize(width, height));
                    }

                    button->setMinimumSize(100,100);
                    button->setEnabled(false);
                    _vectorBoutons.push_back(button);

                }
            }
        }

        std::random_shuffle(_vectorBoutons.begin(), _vectorBoutons.end());

        int k = 0;
        for (int i = 0; i < largeur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                if((i+1)*(j+1) != largeur*largeur)
                {
                    if(((i == largeur-2) && (j==largeur-1)) || ((i == largeur-1) && (j==largeur-2)))
                    {
                        _vectorBoutons[k]->setEnabled(true);
                        _vectorBoutonsPossibles.push_back(_vectorBoutons[k]);
                    }
                    grille->addWidget(_vectorBoutons[k++], i, j);
                }
            }
        }


        return grille;
    }

//####################################################
//                  Clic des boutons                //
//####################################################

    void GameWidget::boutonClique(bool)
    {
        if(QObject::sender() != nullptr)
        {

            QPushButton * boutonSender = qobject_cast<QPushButton*>(QObject::sender()); // Identification du bouton

            echanger(boutonSender, _positionTrou);      // changement de place du bouton et du trou

            int x(_positionTrou->x());
            int y(_positionTrou->y());

            for(auto bouton : _vectorBoutonsPossibles)  // desactivation des boutons autour de l'ancien trou
            {
                bouton->setEnabled(false);
            }

            // activation des boutons autour du nouveau trou
            if (x-1 >= 0)
            {
                QPushButton* bouton1 = qobject_cast<QPushButton*>(_grille->itemAtPosition(x-1,y)->widget());
                bouton1->setEnabled(true);
                _vectorBoutonsPossibles.push_back(bouton1);
            }
            if (x+1 <= _largeurGrille-1)
            {
                QPushButton* bouton2 = qobject_cast<QPushButton*>(_grille->itemAtPosition(x+1,y)->widget());
                bouton2->setEnabled(true);
                _vectorBoutonsPossibles.push_back(bouton2);
            }
            if (y-1 >= 0)
            {
                QPushButton* bouton3 = qobject_cast<QPushButton*>(_grille->itemAtPosition(x,y-1)->widget());
                bouton3->setEnabled(true);
                _vectorBoutonsPossibles.push_back(bouton3);
            }
            if (y+1 <= _largeurGrille-1)
            {
                QPushButton* bouton4 = qobject_cast<QPushButton*>(_grille->itemAtPosition(x,y+1)->widget());
                bouton4->setEnabled(true);
                _vectorBoutonsPossibles.push_back(bouton4);
            }
        }
    }

    void GameWidget::echanger(QPushButton * bouton, QPoint * trou)
    {
//        auto boutonItem = _grille->itemAtPosition(x,y);
        int index = _grille->indexOf(bouton);
        int row, column;
        int _;
        _grille->getItemPosition(index, &row, &column, &_, &_);
        _grille->removeWidget(bouton);
        _grille->addWidget(bouton,trou->x(), trou->y());

        trou->setX(row);
        trou->setY(column);
    }

//####################################################
//                Changer arriere plan              //
//####################################################

    void GameWidget::setBackgroundOriginal(bool){
        for (int i = 0; i < _largeurGrille; ++i) {
            for (int j = 0; j < _largeurGrille; ++j) {
                QPushButton* bouton = dynamic_cast<QPushButton*>(_grille->itemAtPosition(i,j)->widget());
                bouton->setStyleSheet("background-color: blue;");
            }
        }
    }
    void GameWidget::setBackgroundForest(bool){}
    void GameWidget::setBackgroundTree(bool){}
    void GameWidget::setBackgroundNetwork(bool){}
