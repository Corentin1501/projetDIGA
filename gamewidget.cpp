#include "gamewidget.h"

#include <iostream>

//####################################################
//            Constructeur / destructeur            //
//####################################################

    GameWidget::GameWidget(QWidget *parent, int largeur) :
        QWidget(parent),
        _background(nullptr),
        _largeurGrille(largeur),
        _grille(nullptr)
//        _sideButtons(nullptr)
    {
//        _layoutH = new QHBoxLayout;


        _grille = creerGrille(_largeurGrille);

        this->setLayout(_grille);
//        _layoutH->addLayout(_grille);

//        _sideButtons = createSideButton();
//        _layoutH->addLayout(_sideButtons);
    }

    GameWidget::~GameWidget() {}


//####################################################
//              New / Load / Save Game              //
//####################################################

//    void GameWidget::newGame()
//    {
//        DialogNewGame * dialog = new DialogNewGame();

//        auto ok = dialog->exec();

//        if (ok)
//        {
//            _grille = creerGrille(dialog->getLargeur());
//            _moves = DEFAULT_MOVES;
//        }
//    }

//    void GameWidget::loadGame()
//    {
//    //    QString filename = QFileDialog::getOpenFileName(this, tr("Load a game ..."), QString(), tr("Saves files (*.save)"));

//    }

//    void GameWidget::saveGame(){

//    }

//####################################################
//            Creer gridlayout de boutons           //
//####################################################

    QGridLayout* GameWidget::creerGrille(int largeur)
    {
        QGridLayout* grille = new QGridLayout;
        int valeurDuBouton(0);

        if(_background == nullptr){
            for (int i = 0; i < largeur; ++i) {
                for (int j = 0; j < largeur; ++j) {
                    valeurDuBouton++;
                    if(valeurDuBouton != largeur*largeur){
                        QPushButton *button = new QPushButton(QString::number(valeurDuBouton));
                        button->resize(100,100);

                        connect(button, &QPushButton::clicked, this, &GameWidget::boutonClique);
                        grille->addWidget(button, i, j);
                        button->setStyleSheet("background-color: blue;");
                    }
                }
            }
        }
        return grille;
    }

//####################################################
//              Creer boutons lateraux              //
//####################################################

//    QVBoxLayout* GameWidget::createSideButton(){
//        QVBoxLayout* layout = new QVBoxLayout;

//        _newGame = new QPushButton(tr("New game"));
//        connect(_newGame, &QPushButton::clicked, this, &GameWidget::newGame);
//        layout->addWidget(_newGame);
//        _loadGame = new QPushButton(tr("Load game"));
//        connect(_loadGame, &QPushButton::clicked, this, &GameWidget::loadGame);
//        layout->addWidget(_loadGame);
//        _saveGame = new QPushButton(tr("Save game"));
//        connect(_saveGame, &QPushButton::clicked, this, &GameWidget::saveGame);
//        layout->addWidget(_saveGame);

//        _labelMoves = new QLabel(tr("Number of moves : ") + QString::number(_moves));
//        layout->addWidget(_labelMoves);

//        layout->addStretch();

//        return layout;
//    }

//####################################################
//                Changer arriere plan              //
//####################################################

    void GameWidget::boutonClique(bool){}
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
