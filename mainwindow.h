#pragma once
#include <QMainWindow>
#include <QApplication>
#include <QFileDialog>
#include <QtWidgets>

#include "gamewidget.h"
#include "dialognewgame.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // accesseurs pour l'affichage du message de victoire
    int getMoves() const { return _moves; }

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

    void updateMovesLabel(){ _labelMoves->setText(tr("Number of moves : ") + QString::number(++_moves)); }

private:
    static const int DEFAULT_MOVES = -1;

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
