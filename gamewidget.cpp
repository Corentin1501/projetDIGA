#include "gamewidget.h"

#include <iostream>

//####################################################
//            Constructeur / destructeur            //
//####################################################

    GameWidget::GameWidget(QWidget *parent, int largeur) :
        QWidget(parent),
        _background(nullptr),
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

        if(_background == nullptr)
        {
            for (int i = 0; i < largeur; ++i)
            {
                for (int j = 0; j < largeur; ++j)
                {
                    valeurDuBouton++;
                    if(valeurDuBouton != largeur*largeur)
                    {
                        QPushButton *button = new QPushButton(QString::number(valeurDuBouton));
                        button->resize(100,100);

                        connect(button, &QPushButton::clicked, this, &GameWidget::boutonClique);
                        grille->addWidget(button, i, j);
                        button->setStyleSheet("background-color: red; font:Bold; font-size:20px;");
                        button->setMinimumSize(100,100);
                        _vectorBoutons.push_back(button);
                        button->setEnabled(false);



                        if(((i == largeur-2) && (j==largeur-1)) || ((i == largeur-1) && (j==largeur-2)))
                            button->setEnabled(true);
                    }
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
