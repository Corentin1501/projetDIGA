#include "mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include <QPixmap>


//####################################################
//            Constructeur / destructeur            //
//####################################################

    MainWindow::MainWindow(QWidget *parent):
        QMainWindow(parent),
        _newGameButton(new QPushButton(tr("New game"))),
        _loadGameButton(new QPushButton(tr("Load game"))),
        _saveGameButton(new QPushButton(tr("Save game"))),
        _moves(DEFAULT_MOVES),
        _labelMoves(new QLabel(tr("Number of moves : ") + QString::number(_moves)))

    {
        resize(DEFAULT_WIDTH,DEFAULT_HEIGHT);

        createActions();
        createMenu();
        createToolBar();

        _GameWidget = new GameWidget(this,3,"forest");
        setCentralWidget(_GameWidget);

//        connect(_newGameButton, &QPushButton::clicked, this, &MainWindow::newGame);
//        connect(_loadGameButton, &QPushButton::clicked, this, &MainWindow::loadGame);
//        connect(_saveGameButton, &QPushButton::clicked, this, &MainWindow::saveGame);

//        _sideButtons->addWidget(_newGameButton);
//        _sideButtons->addWidget(_loadGameButton);
//        _sideButtons->addWidget(_saveGameButton);
//        _sideButtons->addWidget(_labelMoves);
//        _sideButtons->addStretch();


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
            _GameWidget = new GameWidget(this,dialog->getLargeur(),_background);
            this->setCentralWidget(_GameWidget);
        }
    }

    void MainWindow::loadGame()
    {
    //    QString filename = QFileDialog::getOpenFileName(this, tr("Load a game ..."), QString(), tr("Saves files (*.save)"));

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
//        connect(_originalBackgroundAction,  &QAction::triggered, this, &MainWindow::setBackgroundOriginal);
//        connect(_forestBackgroundAction,    &QAction::triggered, this, &MainWindow::setBackgroundForest);
//        connect(_treeBackgroundAction,      &QAction::triggered, this, &MainWindow::setBackgroundTree);
//        connect(_networkBackgroundAction,   &QAction::triggered, this, &MainWindow::setBackgroundNetwork);

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

    void MainWindow::setBackgroundOriginal(){

    }
    void MainWindow::setBackgroundForest(){}
    void MainWindow::setBackgroundTree(){}
    void MainWindow::setBackgroundNetwork(){}
