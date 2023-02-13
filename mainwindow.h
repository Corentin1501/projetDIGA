#pragma once
#include <QMainWindow>
#include <QApplication>
#include <QFileDialog>
#include <QtWidgets>
#include <QLayout>

#include "gamewidget.h"
#include "dialognewgame.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void newGame();
    void loadGame();
    void saveGame();

    void aboutQt();
    void aboutTaquin();

    void setBackgroundOriginal();
    void setBackgroundForest();
    void setBackgroundTree();
    void setBackgroundNetwork();

private:
    static const int DEFAULT_WIDTH=400;
    static const int DEFAULT_HEIGHT=400;
    static const int DEFAULT_MOVES = 0;

    void createActions();
    void createMenu();
    void createToolBar();

    //### Actions ###
        QAction *_newGameAction;
        QAction *_loadGameAction;
        QAction *_saveGameAction;
        QAction *_quitAction;

        QAction *_originalBackgroundAction;
        QAction *_forestBackgroundAction;
        QAction *_treeBackgroundAction;
        QAction *_networkBackgroundAction;

        QAction *_aboutQt;
        QAction *_aboutTaquin;

    //### Jeu ###
        GameWidget *_GameWidget;
        std::string _background;

    //### ToolBar ###
        QPushButton* _newGameButton;
        QPushButton* _loadGameButton;
        QPushButton* _saveGameButton;
        int _moves;
        QLabel * _labelMoves;

        QToolBar* _toolBar;


};
