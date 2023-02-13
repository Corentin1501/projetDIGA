#ifndef DIALOGNEWGAME_H
#define DIALOGNEWGAME_H

#include <QDialog>

namespace Ui {
class DialogNewGame;
}

class DialogNewGame : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewGame(QWidget *parent = nullptr);
    ~DialogNewGame();

    int getLargeur() const { return _largeur; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogNewGame *ui;

    int _largeur;
};

#endif // DIALOGNEWGAME_H
