#ifndef AI_H
#define AI_H

#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include "game_window.h"

// Callback setter for return button
void SetCallBackAIGame(void (*Copy_ptr)(void));

QT_BEGIN_NAMESPACE
namespace Ui { class game_window; }
QT_END_NAMESPACE

class TestGameLogic; // Forward declaration for testing

class ai : public game_window
{
    Q_OBJECT
    friend class TestGameLogic;

public:
    explicit ai(QWidget *parent = nullptr);
    ~ai();

    // Board state
    char board[3][3];

    // Game logic utilities
    bool checkWin(QString player);
    bool isBoardFull();

private slots:
    void on_RESET_clicked();
    void on_Return_Prof_clicked();
    void cellClicked();
    void resetGame();
    void makeAIMove();

private:
    Ui::game_window *ui;

    // UI grid buttons
    QPushButton *cells[3][3];

    // Game state
    QString currentPlayer;
    QString aiToken;
    QString playerToken;

    // Internal logic
    void initializeBoard();
    void printBoard();
    int minimax(char board[3][3], int depth, bool isMaximizing);
    int evaluate(char board[3][3]);
};

#endif // AI_H
