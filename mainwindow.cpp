#include <iostream>
#include <QMessageBox>
#include <QPixmap>
#include <fstream>

#include "mainwindow.h"

//####################################################
//            Constructeur / destructeur            //
//####################################################

    MainWindow::MainWindow(QWidget *parent):
        QMainWindow(parent),
        _background("original"),
        _newGameButton(new QPushButton(tr("New game"))),
        _loadGameButton(new QPushButton(tr("Load game"))),
        _saveGameButton(new QPushButton(tr("Save game"))),
        _moves(DEFAULT_MOVES),
        _labelMoves(new QLabel)

    {
        setWindowIcon(QIcon("240px-15-puzzle.png"));    // mettre l'icone de l'application

        createActions();
        createMenu();
        createToolBar();
        updateMovesLabel();

        _GameWidget = new GameWidget(this, 3, _background);
        setCentralWidget(_GameWidget);

        adjustSize();
        this->show();
    }

    MainWindow::~MainWindow(){}

//####################################################
//                      New Game                    //
//####################################################

    void MainWindow::newGame()
    {
        DialogNewGame * dialog = new DialogNewGame();
        int ok = dialog->exec();
        if (ok)
        {
            _GameWidget->deleteLater();
            _GameWidget = new GameWidget(this, dialog->getLargeur(), _background, true);
            this->setCentralWidget(_GameWidget);
            adjustSize();

            _moves = DEFAULT_MOVES;
            updateMovesLabel();
        }
    }

//####################################################
//                     Load Game                    //
//####################################################

    std::vector<std::string> split(std::string str, char del)
    {
        std::string temp = "";
        std::vector<std::string> res;

        for(int i=0; i<(int)str.size(); i++){
            if(str[i] != del) temp += str[i];
            else {
                if(temp != "") res.push_back(temp);
                temp = "";
            }
        }
        res.push_back(temp);
        return res;
    }

    void MainWindow::loadGame()
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Load a new game ?"), tr("Abandon the current game to load a new one ?"),
                                      QMessageBox::Yes|QMessageBox::No);

        if(reply == QMessageBox::Yes)
        {
            QString filename = QFileDialog::getOpenFileName(this, tr("Load a game ..."), "", tr("Saves files (*.save)"));
            std::ifstream fichier(filename.toStdString());  //Ouverture d'un fichier en lecture

            // déclarations de tous ce qu'on va charger
                int largeurPuzzle, nombreCoupsJoues, column = 0;
                QPoint* trou = new QPoint;
                QGridLayout* grille = new QGridLayout;
                vector<boutonStruct*> vectorBoutons, vectorBoutonsPossibles;

            if(fichier)
            {
                std::string ligne;
                int ligneCourante (0);

                while(getline(fichier, ligne)) //Tant qu'on n'est pas à la fin, on lit
                {
                    ligneCourante++;
                    if(ligneCourante == 1)  // si c'est la premiere ligne, on recupere la taille du puzzle et le nombre de coups joués
                    {
                        std::vector<std::string> data = split(ligne, ' ');
                        largeurPuzzle = stoi(data.at(0));
                        nombreCoupsJoues = stoi(data.at(1));
                    }
                    else
                    {
                        std::vector<std::string> boutonsRow = split(ligne, ' ');
                        column = 0;

                        for(int i(0) ; i<largeurPuzzle ; i++)
                        {
                            int valeurDuBouton = stoi(boutonsRow[i]);

                            if(valeurDuBouton == -1) // si ca correspond au trou
                            {
                                trou->setX(ligneCourante-2);
                                trou->setY(column);
                            }
                            else // si ca correspond a un bouton
                            {
                                /* Selon les consignes, les boutons sont numérotés à partir de 0,
                                 * mais dans le jeu sont numérotés à partir de 1.
                                 * Je trouve ça plus logique de les sauvegarder tel quels,
                                 * et non avec -1 sur toutes les valeurs de boutons.
                                 * Mais s'il fallait absolument respecter les consignes il faut juste
                                 * incrémenter "valeurDuBouton" avant de creer les boutons et la
                                 * décrémenter lorsqu'on sauvegarde
                                 */

//                                valeurDuBouton++;

                                boutonStruct* newBouton = new boutonStruct;
                                    newBouton->button = new QPushButton(QString::number(valeurDuBouton));
                                        newBouton->button->setStyleSheet(QString("background-color: midnightblue; font:Bold; font-size:20px;"));
                                        newBouton->button->setMinimumSize(100,100);
                                        newBouton->button->setDown(true);
                                    newBouton->row = ligneCourante-2;
                                    newBouton->column = column;
                                    newBouton->valeurDuBouton = valeurDuBouton;

                                grille->addWidget(newBouton->button, ligneCourante-2,column);
                                vectorBoutons.push_back(newBouton);
                            }
                            column++;
                        }
                    }
                }
                // une fois le fichier lu en entier

                // ajout des boutons deplacable dans vectorBoutonsPossibles

                    int x = trou->x();
                    int y = trou->y();
                    for(auto bouton : vectorBoutons)    // on verifie à chaque fois que ça ne sorte pas de la grille
                    {
                        if(((bouton->row == x-1) && (bouton->column == y)) && (x-1 >= 0))
                            vectorBoutonsPossibles.push_back(bouton);

                        if(((bouton->row == x+1) && (bouton->column == y))  && (x+1 <= largeurPuzzle-1))
                            vectorBoutonsPossibles.push_back(bouton);

                        if(((bouton->row == x) && (bouton->column == y-1))  && (y-1 >= 0))
                            vectorBoutonsPossibles.push_back(bouton);

                        if(((bouton->row == x) && (bouton->column == y+1))  && (y+1 <= largeurPuzzle-1))
                            vectorBoutonsPossibles.push_back(bouton);
                    }

                _moves = nombreCoupsJoues-1;    // -1 parce que updateMovesLabel() l'incrémente
                updateMovesLabel();

                _GameWidget->deleteLater();
                _GameWidget = new GameWidget(this,grille,largeurPuzzle,trou,vectorBoutons,vectorBoutonsPossibles,_background);
                this->setCentralWidget(_GameWidget);
                adjustSize();
            }
            else
            {
                std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
            }
        }

    }

//####################################################
//                     Save Game                    //
//####################################################

    void MainWindow::saveGame()
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("Save the game ..."), "", tr("Saves files (*.save)"));

        if (filename.isEmpty())  return; // si le nom du fichier est vide on arrete
        else
        {
           std::ofstream fichierSAVE;
           fichierSAVE.open (filename.toStdString(), std::ofstream::out);

           std::string fichierDeSauvegarde = "";
           auto largeur = _GameWidget->getLargeur();
           auto grille = _GameWidget->getGrille();
           auto trou = _GameWidget->getTrou();
           auto boutons = _GameWidget->getBoutons();
           int valeurDuBouton;

           fichierDeSauvegarde += std::to_string(largeur) + " " + std::to_string(_moves) + "\n"; // premiere ligne du fichier (taille coups)

           for (int i = 0; i < largeur; ++i)
           {
               for (int j = 0; j < largeur; ++j)
               {
                   if (i == trou->x() && j == trou->y())    // si ca correspond au trou on met -1
                       fichierDeSauvegarde += std::to_string(-1) + " ";
                   else
                   {
                       auto boutonDansGrille = grille->itemAtPosition(i,j)->widget();  // on reconnais le bouton à l'emplacement (i,j)
                       for(auto bouton : boutons)   // et on le recherche dans la liste des boutons pour avoir la valeur du bouton
                       {
                           if(boutonDansGrille == bouton->button)
                               valeurDuBouton = bouton->valeurDuBouton;
                       }
                       fichierDeSauvegarde += std::to_string(valeurDuBouton) + " ";
                   }
               }
               fichierDeSauvegarde += "\n"; // une fois qu'on a fais toute la ligne, on va à la ligne
           }
           fichierSAVE << fichierDeSauvegarde;  // on écrit dans le fichier

           fichierSAVE.close();
        }
    }

//####################################################
//            creer les actions des menu            //
//####################################################

    void MainWindow::createActions()
    {
        _newGameAction = new QAction(tr("New Game"), this);
        _newGameAction->setShortcut(QKeySequence::New); // New = CTRL+N
        connect(_newGameAction, &QAction::triggered, this, &MainWindow::newGame);

        _loadGameAction = new QAction(tr("Load"), this);
        _loadGameAction->setShortcut(QKeySequence::Open); // Load = CTRL+O
        connect(_loadGameAction, &QAction::triggered, this, &MainWindow::loadGame);

        _saveGameAction = new QAction(tr("Save"), this);
        _saveGameAction->setShortcut(QKeySequence::Save); // Save = CTRL+S
        connect(_saveGameAction, &QAction::triggered, this, &MainWindow::saveGame);

        _quitAction = new QAction(tr("Quit"), this);
        _quitAction->setShortcut(QKeySequence::Quit); // Quit = CTRL+Q
        connect(_quitAction, &QAction::triggered, this, &QApplication::quit);

        _originalBackgroundAction = new QAction(tr("Original"), this);
        _forestBackgroundAction   = new QAction(tr("Forest"), this);
        _treeBackgroundAction     = new QAction(tr("Tree"), this);
        _networkBackgroundAction  = new QAction(tr("Network"), this);
        connect(_originalBackgroundAction,  &QAction::triggered, this, &MainWindow::setBackgroundOriginal);
        connect(_forestBackgroundAction,    &QAction::triggered, this, &MainWindow::setBackgroundForest);
        connect(_treeBackgroundAction,      &QAction::triggered, this, &MainWindow::setBackgroundTree);
        connect(_networkBackgroundAction,   &QAction::triggered, this, &MainWindow::setBackgroundNetwork);

        _aboutQt = new QAction(tr("About Qt"));
        _aboutTaquin = new QAction(tr("About Taquin"));
        connect(_aboutQt, &QAction::triggered, this, &MainWindow::aboutQt);
        connect(_aboutTaquin, &QAction::triggered, this, &MainWindow::aboutTaquin);
    }

//####################################################
//                 creer les menus                  //
//####################################################

    void MainWindow::createMenu()
    {
        QMenu *fileMenu = menuBar()->addMenu(tr("File"));
        fileMenu->addAction(_newGameAction);
        fileMenu->addAction(_loadGameAction);
        fileMenu->addAction(_saveGameAction);
        fileMenu->addSeparator();
        fileMenu->addAction(_quitAction);

        QMenu *editMenu = menuBar()->addMenu(tr("Background"));
        editMenu->addAction(_originalBackgroundAction);
        editMenu->addAction(_forestBackgroundAction);
        editMenu->addAction(_treeBackgroundAction);
        editMenu->addAction(_networkBackgroundAction);

        QMenu *aboutMenu = menuBar()->addMenu(QIcon::fromTheme("help-browser"), "");
        aboutMenu->addAction(_aboutQt);
        aboutMenu->addAction(_aboutTaquin);
    }

//####################################################
//                creer la ToolBar                  //
//####################################################

    void MainWindow::createToolBar()
    {
        _toolBar = new QToolBar;
        addToolBar(Qt::RightToolBarArea,_toolBar);
        _toolBar->setMovable(false);

        _toolBar->addWidget(_newGameButton);
        _toolBar->addWidget(_loadGameButton);
        _toolBar->addWidget(_saveGameButton);
        _toolBar->addWidget(_labelMoves);
        connect(_newGameButton, &QPushButton::clicked, this, &MainWindow::newGame);
        connect(_loadGameButton, &QPushButton::clicked, this, &MainWindow::loadGame);
        connect(_saveGameButton, &QPushButton::clicked, this, &MainWindow::saveGame);
    }

//####################################################
//                      About                       //
//####################################################

    void MainWindow::aboutQt()
    {
        QMessageBox * message = new QMessageBox;
        message->aboutQt(this,tr("About Qt"));
    }

    void MainWindow::aboutTaquin()
    {
        QMessageBox * message = new QMessageBox;
        message->setIcon(QMessageBox::Information);
        message->about(this,tr("About Taquin"), tr("This program was created by Corentin CUVELIER for the L3 IT's Qt project."));
    }

//####################################################
//                    Background                    //
//####################################################

    void MainWindow::setBackgroundOriginal()
    {
        _background = "original";
        _GameWidget->setBackgroundOriginal();
    }
    void MainWindow::setBackgroundForest()
    {
        _background = "forest";
        _GameWidget->setBackgroundForest();
    }
    void MainWindow::setBackgroundTree()
    {
        _background = "tree";
        _GameWidget->setBackgroundTree();
    }
    void MainWindow::setBackgroundNetwork()
    {
        _background = "network";
        _GameWidget->setBackgroundNetwork();
    }
