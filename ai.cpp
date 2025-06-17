#include "ai.h"
#include "ui_game_window.h"
#include "nextgamesettings.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include "database.h"
#include "sign_in_up.h"

void (*CallBackAIGame)(void);

ai::ai(QWidget *parent)
    : game_window(parent)
    , ui(new Ui::game_window)
{
    ui->setupUi(this);
    initializeBoard();
    printBoard();

    connect(ui->RESET, &QPushButton::clicked, this, &ai::on_RESET_clicked);
    connect(ui->Return_Prof, &QPushButton::clicked, this, &ai::on_Return_Prof_clicked);

    this->setStyleSheet(
        "background-image: url(:/res/CSS/f9090857-a77b-48ce-a267-c997b3b21055.png);"
        "background-repeat: no-repeat;"
        "background-position: center;"
        "background-size: cover;"
    );

    ui->RESET->setStyleSheet(
        "QPushButton {"
        " background-color: #a8dadc;"
        " color: black;"
        " font-weight: bold;"
        " border-radius: 8px;"
        " padding: 10px;"
        "}"
        "QPushButton:hover {"
        " background-color: #74c69d;"
        "}"
    );

    ui->Return_Prof->setStyleSheet(
        "QPushButton {"
        " background-color: #f4a261;"
        " color: white;"
        " font-weight: bold;"
        " border-radius: 8px;"
        " padding: 10px;"
        "}"
        "QPushButton:hover {"
        " background-color: #e76f51;"
        "}"
    );

    ui->gridLayout->setSpacing(10);
    ui->gridLayout->setContentsMargins(20, 20, 20, 20);

    if (currenttoken == 'X' || currenttoken == 'O') {
        playerToken = QString(currenttoken);
        aiToken = (playerToken == "X") ? "O" : "X";
    } else {
        playerToken = "X";
        aiToken = "O";
    }

    currentPlayer = playerToken;

    if (playerToken == "O") {
        QTimer::singleShot(500, this, &ai::makeAIMove);
    }
}

ai::~ai()
{
    delete ui;
}

void ai::initializeBoard()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells[i][j] = new QPushButton("", this);
            cells[i][j]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            cells[i][j]->setProperty("row", i);
            cells[i][j]->setProperty("col", j);
            cells[i][j]->setStyleSheet(
                "QPushButton {"
                " background-color: #f2e6d9;"
                " color: #4d2e1f;"
                " font-size: 24px;"
                " font-weight: bold;"
                " border: 2px solid #a67c52;"
                " border-radius: 10px;"
                "}"
                "QPushButton:pressed {"
                " background-color: #ddb892;"
                "}"
            );

            ui->gridLayout->addWidget(cells[i][j], i, j);
            connect(cells[i][j], &QPushButton::clicked, this, &ai::cellClicked);
            board[i][j] = ' ';
        }
    }
}

void ai::printBoard()
{
    QFont font;
    font.setPointSize(26);
    font.setBold(true);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells[i][j]->setText(board[i][j] == ' ' ? "" : QString(board[i][j]));
            cells[i][j]->setFont(font);
        }
    }
}

void ai::cellClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    int row = clickedButton->property("row").toInt();
    int col = clickedButton->property("col").toInt();

    if (currentPlayer == playerToken && board[row][col] == ' ' && !checkWin(playerToken) && !checkWin(aiToken) && !isBoardFull()) {
        board[row][col] = playerToken.at(0).toLatin1();
        printBoard();

        QFont font = clickedButton->font();
        font.setPointSize(26);
        font.setBold(true);
        clickedButton->setFont(font);

        if(doesUserDatabaseExist(Amir));
        else createUserDatabase(Amir);

        if (checkWin(playerToken)) {
            QMessageBox::information(this, "Game ended", playerToken + " wins!");
            saveGameToUserDatabase(Amir, "You Won");
            resetGame();
        } else if (isBoardFull()) {
            QMessageBox::information(this, "Game ended", "It's a draw!");
            saveGameToUserDatabase(Amir, "Draw");
            resetGame();
        } else {
            currentPlayer = aiToken;
            QTimer::singleShot(500, this, &ai::makeAIMove);
        }
    }
}

void ai::resetGame()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = ' ';

    currentPlayer = playerToken;
    printBoard();

    if (currentPlayer == aiToken)
        QTimer::singleShot(500, this, &ai::makeAIMove);
}

bool ai::isBoardFull()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == ' ')
                return false;
    return true;
}

bool ai::checkWin(QString player)
{
    char token = player.at(0).toLatin1();
    for (int i = 0; i < 3; ++i)
        if ((board[i][0] == token && board[i][1] == token && board[i][2] == token) ||
            (board[0][i] == token && board[1][i] == token && board[2][i] == token))
            return true;

    if ((board[0][0] == token && board[1][1] == token && board[2][2] == token) ||
        (board[0][2] == token && board[1][1] == token && board[2][0] == token))
        return true;

    return false;
}

int ai::evaluate(char board[3][3])
{
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == aiToken.at(0).toLatin1()) return 10;
            else if (board[row][0] == playerToken.at(0).toLatin1()) return -10;
        }
        if (board[0][row] == board[1][row] && board[1][row] == board[2][row]) {
            if (board[0][row] == aiToken.at(0).toLatin1()) return 10;
            else if (board[0][row] == playerToken.at(0).toLatin1()) return -10;
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == aiToken.at(0).toLatin1()) return 10;
        else if (board[0][0] == playerToken.at(0).toLatin1()) return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == aiToken.at(0).toLatin1()) return 10;
        else if (board[0][2] == playerToken.at(0).toLatin1()) return -10;
    }

    return 0;
}

int ai::minimax(char board[3][3], int depth, bool isMaximizing)
{
    int score = evaluate(board);
    if (score == 10 || score == -10) return score;
    if (isBoardFull()) return 0;

    if (isMaximizing) {
        int best = -1000;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] == ' ') {
                    board[i][j] = aiToken.at(0).toLatin1();
                    best = std::max(best, minimax(board, depth + 1, false));
                    board[i][j] = ' ';
                }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] == ' ') {
                    board[i][j] = playerToken.at(0).toLatin1();
                    best = std::min(best, minimax(board, depth + 1, true));
                    board[i][j] = ' ';
                }
        return best;
    }
}

void ai::makeAIMove()
{
    int bestMoveVal = -1000;
    int bestMoveRow = -1;
    int bestMoveCol = -1;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ') {
                board[i][j] = aiToken.at(0).toLatin1();
                int moveVal = minimax(board, 0, false);
                board[i][j] = ' ';
                if (moveVal > bestMoveVal) {
                    bestMoveRow = i;
                    bestMoveCol = j;
                    bestMoveVal = moveVal;
                }
            }

    if (bestMoveRow != -1 && bestMoveCol != -1) {
        board[bestMoveRow][bestMoveCol] = aiToken.at(0).toLatin1();
        printBoard();

        if(doesUserDatabaseExist(Amir));
        else createUserDatabase(Amir);

        if (checkWin(aiToken)) {
            QMessageBox::information(this, "Game Over", aiToken + " wins!");
            saveGameToUserDatabase(Amir, "AI Won");
            resetGame();
        } else if (isBoardFull()) {
            QMessageBox::information(this, "Game Over", "It's a draw!");
            saveGameToUserDatabase(Amir, "Draw");
            resetGame();
        } else {
            currentPlayer = playerToken;
        }
    }
}

void SetCallBackAIGame(void (*Copy_ptr)(void)){
    CallBackAIGame = Copy_ptr;
}

void ai::on_RESET_clicked()
{
    ResetGameScreen();
    resetGame();
}

void ai::on_Return_Prof_clicked()
{
    resetGame();
    close();
    CallBackAIGame();
}
