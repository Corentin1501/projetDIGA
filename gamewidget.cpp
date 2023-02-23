#include "gamewidget.h"
#include "mainwindow.h"

#include <iostream>
#include <algorithm>
#include <random>


//####################################################
//            Constructeur / destructeur            //
//####################################################

    GameWidget::GameWidget(QMainWindow *parent, int largeur, std::string bg, bool random) :
        QWidget(parent),
        _largeurGrille(largeur),
        _background(bg),
        _positionTrou(new QPoint(largeur-1,largeur-1)),
        _victoire(false)
    {
        _grille = creerGrille(_largeurGrille, random);

        if(_background == "forest") setBackgroundForest();
        else if(_background == "tree") setBackgroundTree();
        else if(_background == "network") setBackgroundNetwork();
        else setBackgroundOriginal();

        this->setLayout(_grille);
    }

    GameWidget::GameWidget(QMainWindow *parent, QGridLayout* grille, int largeur, QPoint* trou,
                           vector<boutonStruct*> boutons,vector<boutonStruct*> boutonsPossibles,std::string bg):
        QWidget(parent),
        _largeurGrille(largeur),
        _grille(grille),
        _background(bg),
        _positionTrou(trou),
        _victoire(false)
    {
        for(auto bouton : boutons){
            _vectorBoutons.push_back(bouton);
            connect(bouton->button, &QPushButton::clicked, this, &GameWidget::boutonClique);
        }
        for(auto bouton : boutonsPossibles)
            _vectorBoutonsPossibles.push_back(bouton);

        if(_background == "forest") setBackgroundForest();
        else if(_background == "tree") setBackgroundTree();
        else if(_background == "network") setBackgroundNetwork();
        else setBackgroundOriginal(); // si c'est n'importe quoi d'autre, on met l'original

        this->setLayout(_grille);
    }

    GameWidget::~GameWidget() {}

//####################################################
//            Creer gridlayout de boutons           //
//####################################################

    QGridLayout* GameWidget::creerGrille(int largeur, bool random)
    {
        QGridLayout* grille = new QGridLayout;
        int valeurDuBouton(0);

        for (int i = 0; i < largeur; ++i)
        {
            for (int j = 0; j < largeur; ++j)
            {
                valeurDuBouton++;
                if(valeurDuBouton != largeur*largeur)
                {
                    boutonStruct* newButton = new boutonStruct;
                    newButton->valeurDuBouton = valeurDuBouton;
                    newButton->button = new QPushButton(QString::number(valeurDuBouton));
                        newButton->button->setStyleSheet(QString("background-color: midnightblue; font:Bold; font-size:20px;"));
                        newButton->button->setMinimumSize(100,100);
                    connect(newButton->button, &QPushButton::clicked, this, &GameWidget::boutonClique);
                    _vectorBoutons.push_back(newButton);
                }
            }
        }

        if(random)  // si l'on doit mettre l'image en désordre
        {
            unsigned seed = std::chrono::system_clock::now()
                                    .time_since_epoch()
                                    .count();
            std::shuffle (_vectorBoutons.begin(), _vectorBoutons.end(), std::default_random_engine(seed));
        }

        int k (0);
        //Générer un trou aléatoire

//        std::srand(std::time(nullptr));
//        int x = std::rand() % (largeur + 1);
//        int y = std::rand() % (largeur + 1);
//        _positionTrou->setX(x);
//        _positionTrou->setY(y);

        for (int i = 0; i < largeur; ++i)
        {
            for (int j = 0; j < largeur; ++j)
            {
                if((i+1)*(j+1) != largeur*largeur)  // pour ignorer la derniere case qui manque
                {
                    _vectorBoutons[k]->row = i;
                    _vectorBoutons[k]->column = j;
                    if(((i == largeur-2) && (j==largeur-1)) || ((i == largeur-1) && (j==largeur-2)))
                        _vectorBoutonsPossibles.push_back(_vectorBoutons[k]);

                    grille->addWidget(_vectorBoutons[k++]->button, i, j);
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
            QPushButton * buttonSender = qobject_cast<QPushButton*>(QObject::sender()); // Identification du bouton

            // si le bouton a le droit d'etre bougé
            bool peutEtreBouge (false);
            boutonStruct* boutonIdentifie = new boutonStruct;
            for(auto bouton : _vectorBoutonsPossibles)
            {
                if (bouton->button == buttonSender)
                {
                    peutEtreBouge = true;
                    boutonIdentifie = bouton;
                }
            }
            if (peutEtreBouge)
            {
                bouger(boutonIdentifie);      // changement de place du bouton et du trou

                // mettre a jour le nombre de coups
                    MainWindow* mainWindow = qobject_cast<MainWindow*>(parentWidget()); // Récupération de la fenêtre principale
                    mainWindow->updateMovesLabel(); // Modification du label de la fenêtre principale


                verifierSiVictoire();

                _vectorBoutonsPossibles.clear(); // on enleve la possibilité de bouger les boutons

                if(!_victoire)  // et si on a pas gagné, on va rajouter des boutons dans la liste des boutons deplacables
                {
                    int x(_positionTrou->x());
                    int y(_positionTrou->y());

                    // ajout des boutons autour du nouveau trou dans le vector des boutons autorisés
                    for(auto bouton : _vectorBoutons)
                    {
                        if((x-1 >= 0) && ((bouton->row == x-1) && (bouton->column == y)))
                        {
                            _vectorBoutonsPossibles.push_back(bouton);
                        }
                        if(((bouton->row == x+1) && (bouton->column == y))  && (x+1 <= _largeurGrille-1))
                        {
                            _vectorBoutonsPossibles.push_back(bouton);
                        }
                        if(((bouton->row == x) && (bouton->column == y-1))  && (y-1 >= 0))
                        {
                            _vectorBoutonsPossibles.push_back(bouton);
                        }
                        if(((bouton->row == x) && (bouton->column == y+1))  && (y+1 <= _largeurGrille-1))
                        {
                            _vectorBoutonsPossibles.push_back(bouton);
                        }
                    }
                }
            }
        }
    }

    void GameWidget::bouger(boutonStruct* bouton)
    {
        _grille->removeWidget(bouton->button);
        _grille->addWidget(bouton->button,_positionTrou->x(), _positionTrou->y());

        int buttonRow = bouton->row;
        int buttonColumn = bouton->column;

        bouton->row = _positionTrou->x();
        bouton->column = _positionTrou->y();

        _positionTrou->setX(buttonRow);
        _positionTrou->setY(buttonColumn);
    }

//####################################################
//                 Check si victoire                //
//####################################################

    void GameWidget::verifierSiVictoire()
    {
        vector<int> ordreBoutonsDansGrille; // ordre des boutons (juste leurs valeurs)

        // recuperation de l'ordre des boutons dans la grille

            for (int i = 0; i < _largeurGrille; ++i)
            {
                for (int j = 0; j < _largeurGrille; ++j)
                {
                    if((i == _positionTrou->x()) && (j == _positionTrou->y())) // si c'est le trou
                    {
                        ordreBoutonsDansGrille.push_back(-1);
                    }
                    else    // si c'est un bouton
                    {
                        auto boutonDansGrille = _grille->itemAtPosition(i,j)->widget();  // on reconnais le bouton à l'emplacement (i,j)
                        for(auto bouton : _vectorBoutons)   // et on le recherche dans la liste des boutons pour avoir la valeur du bouton
                        {
                            if(boutonDansGrille == bouton->button) ordreBoutonsDansGrille.push_back(bouton->valeurDuBouton);
                        }
                    }
                }
            }

        // verification de l'ordre des boutons

            bool dansLOrdre (true);
            int nombreCasesSansTrou (_largeurGrille * _largeurGrille -1);
            int boutonPrecedent (ordreBoutonsDansGrille[0]);

            for(int i(1) ; i<nombreCasesSansTrou ; i++)
            {
                if (boutonPrecedent > ordreBoutonsDansGrille[i])
                    dansLOrdre = false;
                boutonPrecedent = ordreBoutonsDansGrille[i];
            }

        if(dansLOrdre) // si c'est dans l'ordre, alors c'est la victoire
        {
            MainWindow* mainWindow = qobject_cast<MainWindow*>(parentWidget()); // Récupération de la fenêtre principale
            int moves = mainWindow->getMoves(); // Récupération du nombre de coups joués

            QMessageBox * messageVictoire = new QMessageBox;
            messageVictoire->setIcon(QMessageBox::Information);
            messageVictoire->about(this,tr("End !"), tr("Congratulations, you won in ") + QString::number(moves) + tr(" moves !"));

            _victoire = true;
        }


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
            int quotient = bouton->valeurDuBouton / _largeurGrille;
            int modulo = bouton->valeurDuBouton % _largeurGrille;
            int row = (modulo == 0)? quotient-1 : quotient;
            int column = (modulo == 0)? _largeurGrille -1 : modulo-1;

            bouton->button->setIcon(QPixmap::fromImage(image.copy(column*width, row*height, width, height)));
            bouton->button->setIconSize(QSize(100, 100));
            bouton->button->setText("");
        }
    }
    void GameWidget::setBackgroundOriginal()
    {
        for(auto bouton : _vectorBoutons)
        {
            bouton->button->setText((QString::number(bouton->valeurDuBouton)));
            bouton->button->setIcon(QIcon());
        }
    }
    void GameWidget::setBackgroundForest()  { chargerImage(QImage("foret.jpg")); }
    void GameWidget::setBackgroundTree()    { chargerImage(QImage("arbre.jpg")); }
    void GameWidget::setBackgroundNetwork() { chargerImage(QImage("reseau.jpeg")); }

//####################################################
//                Affichage des boutons             //
//####################################################


    void GameWidget::afficherBoutons()
    {
        std::cout << "######### LISTE DES BOUTONS #########\n";
        for(auto bouton : _vectorBoutons)
        {
            std::cout << "Bouton " << std::to_string(bouton->valeurDuBouton);
            std::cout << " (" << std::to_string(bouton->row) << "," << std::to_string(bouton->column) << ")" << std::endl;
        }
        std::cout << "==== Trou (" << std::to_string(_positionTrou->x()) << "," << std::to_string(_positionTrou->y()) << ") ====\n";
    }
    void GameWidget::afficherBoutonsPossibles()
    {
        std::cout << "######### LISTE DES BOUTONS POSSIBLES #########\n";
        for(auto bouton : _vectorBoutonsPossibles)
        {
            std::cout << "Bouton " << std::to_string(bouton->valeurDuBouton);
            std::cout << " (" << std::to_string(bouton->row) << "," << std::to_string(bouton->column) << ")" << std::endl;
        }
        std::cout << "==== Trou (" << std::to_string(_positionTrou->x()) << "," << std::to_string(_positionTrou->y()) << ") ====\n";
    }
