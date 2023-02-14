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

    GameWidget::GameWidget(QWidget *parent, QGridLayout* grille, int largeur, QPoint* trou, vector<QPushButton*> boutons,vector<QPushButton*> boutonsPossibles,std::string bg):
        QWidget(parent),
        _background(bg),
        _largeurGrille(largeur),
        _grille(grille),
        _vectorBoutons(boutons),
        _vectorBoutonsPossibles(boutonsPossibles),
        _positionTrou(trou)
    {
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
        else if(_background == "tree") image = QImage("arbre.jpg");
        else if(_background == "network") image = QImage("reseau.jpeg");

        int width = image.width() / largeur;
        int height = image.height() / largeur;

        for (int i = 0; i < largeur; ++i)
        {
            for (int j = 0; j < largeur; ++j)
            {
                valeurDuBouton++;
                if(valeurDuBouton != largeur*largeur)
                {
                    QPushButton *button = new QPushButton(QString::number(valeurDuBouton));

                    connect(button, &QPushButton::clicked, this, &GameWidget::boutonClique);

                    if(!image.isNull()) // si le background n'est pas "original"
                    {
                        button->setIcon(QPixmap::fromImage(image.copy(j*width, i*height, width, height)));
                        button->setIconSize(QSize(100, 100));
                    }

                    button->setStyleSheet(QString("background-color: grey;"
                                                  "font:Bold;"
                                                  "font-size:20px;"));
                    button->setMinimumSize(100,100);
                    button->setDown(true);
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

            // si le bouton a le droit d'etre bougé
            if (std::find(_vectorBoutonsPossibles.begin(), _vectorBoutonsPossibles.end(), boutonSender) != _vectorBoutonsPossibles.end())
            {
                echanger(boutonSender, _positionTrou);      // changement de place du bouton et du trou

                int x(_positionTrou->x());
                int y(_positionTrou->y());


                _vectorBoutonsPossibles.clear();

                // ajout des boutons autour du nouveau trou dans le vector des boutons autorisés
                if (x-1 >= 0)
                {
                    QPushButton* bouton1 = qobject_cast<QPushButton*>(_grille->itemAtPosition(x-1,y)->widget());
                    _vectorBoutonsPossibles.push_back(bouton1);
                }
                if (x+1 <= _largeurGrille-1)
                {
                    QPushButton* bouton2 = qobject_cast<QPushButton*>(_grille->itemAtPosition(x+1,y)->widget());
                    _vectorBoutonsPossibles.push_back(bouton2);
                }
                if (y-1 >= 0)
                {
                    QPushButton* bouton3 = qobject_cast<QPushButton*>(_grille->itemAtPosition(x,y-1)->widget());
                    _vectorBoutonsPossibles.push_back(bouton3);
                }
                if (y+1 <= _largeurGrille-1)
                {
                    QPushButton* bouton4 = qobject_cast<QPushButton*>(_grille->itemAtPosition(x,y+1)->widget());
                    _vectorBoutonsPossibles.push_back(bouton4);
                }
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

    void GameWidget::chargerImage(QImage image)
    {
        int width = image.width() / _largeurGrille;
        int height = image.height() / _largeurGrille;
        for(auto bouton : _vectorBoutons)
        {
            int index = _grille->indexOf(bouton);
            int row, column;
            int _;
            _grille->getItemPosition(index, &row, &column, &_, &_);

            bouton->setIcon(QPixmap::fromImage(image.copy(column*width, row*height, width, height)));
            bouton->setIconSize(QSize(100, 100));
            bouton->setText("");
        }
    }
    void GameWidget::setBackgroundOriginal()
    {
        for(int i(0); i<_vectorBoutons.size(); i++)
        {
            _vectorBoutons.at(i)->setText(QString::number(i+1));
            _vectorBoutons.at(i)->setIcon(QIcon());
        }
    }
    void GameWidget::setBackgroundForest()  { chargerImage(QImage("foret.jpg")); }
    void GameWidget::setBackgroundTree()    { chargerImage(QImage("arbre.jpg")); }
    void GameWidget::setBackgroundNetwork() { chargerImage(QImage("reseau.jpeg")); }






