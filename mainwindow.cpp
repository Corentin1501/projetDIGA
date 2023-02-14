#include "mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include <QPixmap>
#include <fstream>


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
        _labelMoves(new QLabel(tr("Number of moves : ") + QString::number(_moves)))

    {
        createActions();
        createMenu();
        createToolBar();

        _GameWidget = new GameWidget(this, 3, _background);
        setCentralWidget(_GameWidget);
    }

    MainWindow::~MainWindow(){}

//####################################################
//               New / Load / Save Game             //
//####################################################

    void MainWindow::newGame()
    {
        DialogNewGame * dialog = new DialogNewGame();
        auto ok = dialog->exec();
        if (ok)
        {
            _GameWidget->deleteLater();
            _GameWidget = new GameWidget(this, dialog->getLargeur(), _background);
            this->setCentralWidget(_GameWidget);
            adjustSize();
        }
    }

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
        QString filename = QFileDialog::getOpenFileName(this, tr("Load a game ..."), QString(), tr("Saves files (*.save)"));

        std::ifstream fichier(filename.toStdString());  //Ouverture d'un fichier en lecture

        int largeurPuzzle;
        int nombreCoupsJoues;
        int row = 0, column = 0;
        QPoint* trou = new QPoint;
        QGridLayout* grille = new QGridLayout;
        vector<QPushButton*> vectorBoutons;
        vector<QPushButton*> vectorBoutonsPossibles;

        if(fichier)
        {
            std::string ligne;

            int ligneCourante = 0;
            while(getline(fichier, ligne)) //Tant qu'on n'est pas à la fin, on lit
            {
                ligneCourante++;

                if(ligneCourante == 1)  // si c'est la premiere ligne, on recupere la taille du puzzle et le nombre de coups joués
                {
                    std::vector<std::string> data = split(ligne, ';');
                    largeurPuzzle = stoi(data.at(0));
                    nombreCoupsJoues = stoi(data.at(1));
                }
                else
                {
                   int valeurDuBouton = stoi(ligne);

                   if(valeurDuBouton == -1) // si ca correspond au trou
                   {
                       trou->setX(column);
                       trou->setY(row);
                   }
                   else // si ca correspond a un bouton
                   {
                       QPushButton* bouton = new QPushButton(QString::number(valeurDuBouton));
                       grille->addWidget(bouton, row,column);
                       connect(bouton, &QPushButton::clicked, _GameWidget, &GameWidget::boutonClique);
                       bouton->setStyleSheet(QString("background-color: grey;"
                                                     "font:Bold;"
                                                     "font-size:20px;"));
                       bouton->setMinimumSize(100,100);
                       bouton->setDown(true);

                       vectorBoutons.insert(vectorBoutons.begin()+valeurDuBouton, bouton);
                   }

                   if(column == largeurPuzzle-1)  //
                   {
                       row++;
                       column=0;
                   }
                   else
                       column++;
                }

            }
            // une fois le fichier lu en entier
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

    void MainWindow::saveGame(){

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
        _quitAction->setShortcut(Qt::Key_Q); // Quit = CTRL+Q
        // Juste la touche Q pour l'instant, c'ets plus rapide (Mais sinon c'est QKeySequence::Quit)
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

        QMenu *aboutMenu = menuBar()->addMenu(tr("About"));
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
        _toolBar->setAllowedAreas(Qt::RightToolBarArea);
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
        message->aboutQt(this,"About Qt");
    }

    void MainWindow::aboutTaquin()
    {
        QMessageBox * message = new QMessageBox;
        message->setIconPixmap(QPixmap("240px-15-puzzle.png"));
        message->about(this,"About Taquin", "This program was created by Corentin CUVELIER for the L3 IT's Qt project.");
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
