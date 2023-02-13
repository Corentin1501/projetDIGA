#include "mainwindow.h"
#include <iostream>

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




        _GameWidget = new GameWidget(this);
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
            std::cout<<dialog->getLargeur() << std::endl;
            _GameWidget = new GameWidget(this,dialog->getLargeur());
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

    void MainWindow::createActions() {
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
//        connect(_originalBackgroundAction, &QAction::triggered, _GameWidget, &GameWidget::setBackgroundOriginal);
        _forestBackgroundAction = new QAction(tr("Forest"), this);
    //    connect(_forestBackgroundAction, &QAction::triggered, _GameWidget, &GameWidget::setBackgroundForest);
        _treeBackgroundAction = new QAction(tr("Tree"), this);
    //    connect(_treeBackgroundAction, &QAction::triggered, _GameWidget, &GameWidget::setBackgroundTree);
        _networkBackgroundAction = new QAction(tr("Network"), this);
    //    connect(_networkBackgroundAction, &QAction::triggered, _GameWidget, &GameWidget::setBackgroundNetwork);
    }

//####################################################
//                 creer les menus                  //
//####################################################

    void MainWindow::createMenu() {
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
    }

//####################################################
//                 creer les menus                  //
//####################################################

    void MainWindow::createToolBar() {
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

