#include <QtTest/QtTest>
#include "ai.h"

class TestGameLogic : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        // Called before the first test function
    }

    void cleanupTestCase() {
        // Called after the last test function
    }

    void init() {
        // Called before each test function
    }

    void cleanup() {
        // Called after each test function
    }

    // Test empty board state
    void testEmptyBoard() {
        ai testAI(nullptr);
        // Access board through a test-friendly method or make board public for testing
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                testAI.board[i][j] = ' ';
        QVERIFY(!testAI.checkWin("X"));
        QVERIFY(!testAI.checkWin("O"));
        QVERIFY(!testAI.isBoardFull());
    }

    // Test horizontal win conditions
    void testHorizontalWinConditions() {
        ai testAI(nullptr);

        // Test X wins in first row
        testAI.board[0][0] = 'X'; testAI.board[0][1] = 'X'; testAI.board[0][2] = 'X';
        testAI.board[1][0] = 'O'; testAI.board[1][1] = ' '; testAI.board[1][2] = 'O';
        testAI.board[2][0] = ' '; testAI.board[2][1] = ' '; testAI.board[2][2] = ' ';
        QVERIFY(testAI.checkWin("X"));
        QVERIFY(!testAI.checkWin("O"));

        // Test O wins in second row
        testAI.board[0][0] = 'X'; testAI.board[0][1] = ' '; testAI.board[0][2] = 'X';
        testAI.board[1][0] = 'O'; testAI.board[1][1] = 'O'; testAI.board[1][2] = 'O';
        testAI.board[2][0] = ' '; testAI.board[2][1] = 'X'; testAI.board[2][2] = ' ';
        QVERIFY(testAI.checkWin("O"));
        QVERIFY(!testAI.checkWin("X"));

        // Test X wins in third row
        testAI.board[0][0] = 'O'; testAI.board[0][1] = ' '; testAI.board[0][2] = 'O';
        testAI.board[1][0] = ' '; testAI.board[1][1] = 'O'; testAI.board[1][2] = ' ';
        testAI.board[2][0] = 'X'; testAI.board[2][1] = 'X'; testAI.board[2][2] = 'X';
        QVERIFY(testAI.checkWin("X"));
        QVERIFY(!testAI.checkWin("O"));
    }

    // Test vertical win conditions
    void testVerticalWinConditions() {
        ai testAI(nullptr);

        // Test X wins in first column
        testAI.board[0][0] = 'X'; testAI.board[0][1] = 'O'; testAI.board[0][2] = ' ';
        testAI.board[1][0] = 'X'; testAI.board[1][1] = ' '; testAI.board[1][2] = 'O';
        testAI.board[2][0] = 'X'; testAI.board[2][1] = 'O'; testAI.board[2][2] = ' ';
        QVERIFY(testAI.checkWin("X"));
        QVERIFY(!testAI.checkWin("O"));

        // Test O wins in second column
        testAI.board[0][0] = 'X'; testAI.board[0][1] = 'O'; testAI.board[0][2] = 'X';
        testAI.board[1][0] = ' '; testAI.board[1][1] = 'O'; testAI.board[1][2] = ' ';
        testAI.board[2][0] = 'X'; testAI.board[2][1] = 'O'; testAI.board[2][2] = ' ';
        QVERIFY(testAI.checkWin("O"));
        QVERIFY(!testAI.checkWin("X"));

        // Test X wins in third column
        testAI.board[0][0] = 'O'; testAI.board[0][1] = ' '; testAI.board[0][2] = 'X';
        testAI.board[1][0] = ' '; testAI.board[1][1] = 'O'; testAI.board[1][2] = 'X';
        testAI.board[2][0] = 'O'; testAI.board[2][1] = ' '; testAI.board[2][2] = 'X';
        QVERIFY(testAI.checkWin("X"));
        QVERIFY(!testAI.checkWin("O"));
    }

    // Test diagonal win conditions
    void testDiagonalWinConditions() {
        ai testAI(nullptr);

        // Test X wins main diagonal (top-left to bottom-right)
        testAI.board[0][0] = 'X'; testAI.board[0][1] = 'O'; testAI.board[0][2] = ' ';
        testAI.board[1][0] = ' '; testAI.board[1][1] = 'X'; testAI.board[1][2] = 'O';
        testAI.board[2][0] = 'O'; testAI.board[2][1] = ' '; testAI.board[2][2] = 'X';
        QVERIFY(testAI.checkWin("X"));
        QVERIFY(!testAI.checkWin("O"));

        // Test O wins anti-diagonal (top-right to bottom-left)
        testAI.board[0][0] = 'X'; testAI.board[0][1] = ' '; testAI.board[0][2] = 'O';
        testAI.board[1][0] = ' '; testAI.board[1][1] = 'O'; testAI.board[1][2] = 'X';
        testAI.board[2][0] = 'O'; testAI.board[2][1] = 'X'; testAI.board[2][2] = ' ';
        QVERIFY(testAI.checkWin("O"));
        QVERIFY(!testAI.checkWin("X"));
    }

    // Test tie/draw game conditions
    void testTieGame() {
        ai testAI(nullptr);
        char draw[3][3] = {
            {'X', 'O', 'X'},
            {'O', 'O', 'X'},
            {'X', 'X', 'O'}
        };
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                testAI.board[i][j] = draw[i][j];
        QVERIFY(!testAI.checkWin("X"));
        QVERIFY(!testAI.checkWin("O"));
        QVERIFY(testAI.isBoardFull());
    }

    // Test board full detection
    void testBoardFullDetection() {
        ai testAI(nullptr);

        // Test partially filled board
        testAI.board[0][0] = 'X'; testAI.board[0][1] = 'O'; testAI.board[0][2] = ' ';
        testAI.board[1][0] = ' '; testAI.board[1][1] = 'X'; testAI.board[1][2] = 'O';
        testAI.board[2][0] = 'O'; testAI.board[2][1] = ' '; testAI.board[2][2] = 'X';
        QVERIFY(!testAI.isBoardFull());

        // Test completely filled board
        testAI.board[0][0] = 'X'; testAI.board[0][1] = 'O'; testAI.board[0][2] = 'X';
        testAI.board[1][0] = 'O'; testAI.board[1][1] = 'X'; testAI.board[1][2] = 'O';
        testAI.board[2][0] = 'O'; testAI.board[2][1] = 'X'; testAI.board[2][2] = 'X';
        QVERIFY(testAI.isBoardFull());
    }

    // Test minimax evaluation function
    void testMinimaxEvaluation() {
        ai testAI(nullptr);

        // Test AI winning position evaluation
        char aiWinBoard[3][3] = {
            {'O', 'O', 'O'},
            {'X', 'X', ' '},
            {' ', ' ', ' '}
        };
        // Assuming AI is 'O' and player is 'X'
        int score = testAI.evaluate(aiWinBoard);
        QVERIFY(score > 0); // AI should have positive score when winning

        // Test player winning position evaluation
        char playerWinBoard[3][3] = {
            {'X', 'X', 'X'},
            {'O', 'O', ' '},
            {' ', ' ', ' '}
        };
        score = testAI.evaluate(playerWinBoard);
        QVERIFY(score < 0); // Player should have negative score when winning

        // Test neutral position evaluation
        char neutralBoard[3][3] = {
            {'X', 'O', ' '},
            {' ', ' ', ' '},
            {' ', ' ', ' '}
        };
        score = testAI.evaluate(neutralBoard);
        QVERIFY(score == 0); // Should be neutral
    }

    // Test edge cases and invalid inputs
    void testEdgeCases() {
        ai testAI(nullptr);

        // Test single move scenarios
        testAI.board[0][0] = 'X'; testAI.board[0][1] = ' '; testAI.board[0][2] = ' ';
        testAI.board[1][0] = ' '; testAI.board[1][1] = ' '; testAI.board[1][2] = ' ';
        testAI.board[2][0] = ' '; testAI.board[2][1] = ' '; testAI.board[2][2] = ' ';
        QVERIFY(!testAI.checkWin("X"));
        QVERIFY(!testAI.checkWin("O"));
        QVERIFY(!testAI.isBoardFull());

        // Test almost full board
        testAI.board[0][0] = 'X'; testAI.board[0][1] = 'O'; testAI.board[0][2] = 'X';
        testAI.board[1][0] = 'O'; testAI.board[1][1] = 'X'; testAI.board[1][2] = 'O';
        testAI.board[2][0] = 'O'; testAI.board[2][1] = 'X'; testAI.board[2][2] = ' ';
        QVERIFY(!testAI.isBoardFull());
    }

    // Test game state consistency
    void testGameStateConsistency() {
        ai testAI(nullptr);

        // Test that a winning board cannot also be a draw
        testAI.board[0][0] = 'X'; testAI.board[0][1] = 'X'; testAI.board[0][2] = 'X';
        testAI.board[1][0] = 'O'; testAI.board[1][1] = 'O'; testAI.board[1][2] = ' ';
        testAI.board[2][0] = ' '; testAI.board[2][1] = ' '; testAI.board[2][2] = ' ';

        bool xWins = testAI.checkWin("X");
        bool oWins = testAI.checkWin("O");
        bool boardFull = testAI.isBoardFull();

        QVERIFY(xWins);
        QVERIFY(!oWins);
        QVERIFY(!boardFull);

        // Both players cannot win simultaneously
        QVERIFY(!(xWins && oWins));
    }

    // Test multiple win conditions (should not happen in valid game)
    void testInvalidGameStates() {
        ai testAI(nullptr);

        // Test board with impossible state (both players winning)
        // This shouldn't happen in a real game, but we test edge cases
        testAI.board[0][0] = 'X'; testAI.board[0][1] = 'X'; testAI.board[0][2] = 'X';
        testAI.board[1][0] = 'O'; testAI.board[1][1] = 'O'; testAI.board[1][2] = 'O';
        testAI.board[2][0] = ' '; testAI.board[2][1] = ' '; testAI.board[2][2] = ' ';

        // Both should be detected as winners (invalid state)
        QVERIFY(testAI.checkWin("X"));
        QVERIFY(testAI.checkWin("O"));
    }
};

QTEST_MAIN(TestGameLogic)
#include "test_ai.moc"
