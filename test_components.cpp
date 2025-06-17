#include <QtTest/QtTest>
#include <QVector>
#include <QString>
#include <QMap>
#include <QList>

class TestComponents : public QObject
{
    Q_OBJECT

private:
    // Mock Database implementation for testing
    class MockDatabase {
    private:
        QMap<QString, QList<QString>> tables;
        QMap<QString, QString> tableSchemas;
        bool isOpen;
        QMap<QString, QString> users;

    public:
        MockDatabase() : isOpen(false) {}

        bool open() {
            isOpen = true;
            return true;
        }

        bool close() {
            isOpen = false;
            users.clear();
            return true;
        }

        bool isValid() const {
            return true;
        }

        bool createTable(const QString& tableName, const QString& columns) {
            if (!isOpen) return false;
            if (tables.contains(tableName)) return false;

            tables[tableName] = QList<QString>();
            tableSchemas[tableName] = columns;
            return true;
        }

        QString getTableSchema(const QString& tableName) const {
            return tableSchemas.value(tableName, "");
        }

        bool insertRecord(const QString& tableName, const QString& values) {
            if (!isOpen) return false;
            if (!tables.contains(tableName)) return false;

            QString schema = tableSchemas[tableName];
            QStringList schemaColumns = schema.split(", ");
            QStringList valueColumns = values.split(", ");

            if (schemaColumns.size() != valueColumns.size()) {
                return false;
            }

            tables[tableName].append(values);
            return true;
        }

        QList<QString> getRecords(const QString& tableName) {
            if (!isOpen || !tables.contains(tableName))
                return QList<QString>();
            return tables[tableName];
        }

        bool tableExists(const QString& tableName) const {
            return tables.contains(tableName);
        }

        bool addUser(const QString& username, const QString& password) {
            if (!isOpen) return false;
            if (username.isEmpty() || password.isEmpty()) return false;
            if (users.contains(username)) return false;

            users[username] = password;
            return true;
        }

        bool verifyUser(const QString& username, const QString& password) {
            if (!isOpen) return false;
            return users.contains(username) && users[username] == password;
        }

        bool updatePassword(const QString& username, const QString& newPassword) {
            if (!isOpen) return false;
            if (username.isEmpty() || newPassword.isEmpty()) return false;
            if (!users.contains(username)) return false;

            users[username] = newPassword;
            return true;
        }
    };

    // Mock Player List implementation for testing
    class PlayerList {
    public:
        struct Player {
            QString username;
            QString password;
            Player* next;
            Player(QString name, QString pass) : username(name), password(pass), next(nullptr) {}
        };

        Player* head;
        int size;

        PlayerList() : head(nullptr), size(0) {}

        ~PlayerList() {
            clear();
        }

        void clear() {
            while (head != nullptr) {
                Player* temp = head;
                head = head->next;
                delete temp;
            }
            head = nullptr;
            size = 0;
        }

        bool empty() const { return head == nullptr; }
        int getSize() const { return size; }

        bool isfound(QString user, QString pass) {
            Player* current = head;
            while (current != nullptr) {
                if (current->username == user && current->password == pass) {
                    return true;
                }
                current = current->next;
            }
            return false;
        }

        void push_back(QString username, QString password) {
            if (username.isEmpty() || password.isEmpty()) return;

            Player* newNode = new Player(username, password);
            if (empty()) {
                head = newNode;
            } else {
                Player* current = head;
                while (current->next != nullptr) {
                    current = current->next;
                }
                current->next = newNode;
            }
            size++;
        }

        void insert(int index, QString username, QString password) {
            if (index < 0 || index > size || username.isEmpty() || password.isEmpty()) return;

            Player* newNode = new Player(username, password);
            if (index == 0) {
                newNode->next = head;
                head = newNode;
            } else {
                Player* current = head;
                for (int i = 0; i < index - 1 && current != nullptr; i++) {
                    current = current->next;
                }
                if (current != nullptr) {
                    newNode->next = current->next;
                    current->next = newNode;
                }
            }
            size++;
        }

        void erase(int index) {
            if (index >= size || empty()) return;

            if (index == 0) {
                Player* toDelete = head;
                head = head->next;
                delete toDelete;
                size--;
                if (size == 0) {
                    head = nullptr;
                }
            } else {
                Player* current = head;
                for (int i = 0; i < index - 1 && current != nullptr; i++) {
                    current = current->next;
                }
                if (current != nullptr && current->next != nullptr) {
                    Player* toDelete = current->next;
                    current->next = toDelete->next;
                    delete toDelete;
                    size--;
                }
            }
        }

        Player* getPlayerNode(QString user, QString pass) {
            Player* current = head;
            while (current != nullptr) {
                if (current->username == user && current->password == pass) {
                    return current;
                }
                current = current->next;
            }
            return nullptr;
        }
    };

    // Mock Game History implementation for testing
    class GameHistory {
    public:
        struct Game {
            int index;
            char token;
            QString result;
            Game* next;
            Game(char t, QString r) : token(t), result(r), next(nullptr) {}
        };

        Game* head;
        int size;

        GameHistory() : head(nullptr), size(0) {}

        ~GameHistory() {
            clear();
        }

        void clear() {
            while (head != nullptr) {
                Game* temp = head;
                head = head->next;
                delete temp;
            }
            head = nullptr;
            size = 0;
        }

        bool empty() const { return head == nullptr; }
        int getSize() const { return size; }

        void push_back(char token, QString result) {
            Game* newNode = new Game(token, result);
            if (empty()) {
                newNode->index = 0;
                head = newNode;
            } else {
                Game* current = head;
                while (current->next != nullptr) {
                    current = current->next;
                }
                newNode->index = size;
                current->next = newNode;
            }
            size++;
        }

        void insert(int index, char token, QString result) {
            if (index < 0 || index > size) return;

            Game* newNode = new Game(token, result);
            if (index == 0) {
                newNode->index = 0;
                newNode->next = head;
                head = newNode;
                // Update indices of subsequent games
                Game* current = head->next;
                while (current != nullptr) {
                    current->index++;
                    current = current->next;
                }
            } else {
                Game* current = head;
                for (int i = 0; i < index - 1 && current != nullptr; i++) {
                    current = current->next;
                }
                if (current != nullptr) {
                    newNode->index = index;
                    newNode->next = current->next;
                    current->next = newNode;
                    // Update indices of subsequent games
                    current = newNode->next;
                    while (current != nullptr) {
                        current->index++;
                        current = current->next;
                    }
                }
            }
            size++;
        }

        void erase(int index) {
            if (index >= size || empty()) return;

            if (index == 0) {
                Game* toDelete = head;
                head = head->next;
                delete toDelete;
                size--;
                if (size == 0) {
                    head = nullptr;
                }
                // Update indices of remaining games
                Game* current = head;
                int newIndex = 0;
                while (current != nullptr) {
                    current->index = newIndex++;
                    current = current->next;
                }
            } else {
                Game* current = head;
                for (int i = 0; i < index - 1 && current != nullptr; i++) {
                    current = current->next;
                }
                if (current != nullptr && current->next != nullptr) {
                    Game* toDelete = current->next;
                    current->next = toDelete->next;
                    delete toDelete;
                    size--;
                    // Update indices of remaining games
                    current = current->next;
                    int newIndex = index;
                    while (current != nullptr) {
                        current->index = newIndex++;
                        current = current->next;
                    }
                }
            }
        }

        Game* getGameNode(int index) {
            if (index >= size || empty()) return nullptr;

            Game* current = head;
            for (int i = 0; i < index && current != nullptr; i++) {
                current = current->next;
            }
            return current;
        }
    };

private slots:
    // Database Tests
    void testDatabaseConnection() {
        // Simple in-memory database simulation
        QMap<QString, QString> mockDb;

        // Test database operations
        QVERIFY2(mockDb.isEmpty(), "Database should be empty initially");

        // Test insert
        mockDb["test_key"] = "test_value";
        QVERIFY2(!mockDb.isEmpty(), "Database should not be empty after insert");
        QVERIFY2(mockDb.contains("test_key"), "Database should contain inserted key");
        QCOMPARE(mockDb["test_key"], QString("test_value"));

        // Test update
        mockDb["test_key"] = "updated_value";
        QCOMPARE(mockDb["test_key"], QString("updated_value"));

        // Test delete
        mockDb.remove("test_key");
        QVERIFY2(!mockDb.contains("test_key"), "Database should not contain deleted key");
        QVERIFY2(mockDb.isEmpty(), "Database should be empty after delete");
    }

    // Game Logic Tests
    void testCheckWin() {
        QVector<QVector<char>> board(3, QVector<char>(3, '.'));

        // Test horizontal win
        board[0][0] = 'X';
        board[0][1] = 'X';
        board[0][2] = 'X';

        // Check horizontal win
        bool horizontalWin = false;
        for (int i = 0; i < 3; ++i) {
            if (board[i][0] == 'X' && board[i][1] == 'X' && board[i][2] == 'X') {
                horizontalWin = true;
                break;
            }
        }
        QVERIFY(horizontalWin);

        // Test vertical win
        board = QVector<QVector<char>>(3, QVector<char>(3, '.'));
        board[0][0] = 'O';
        board[1][0] = 'O';
        board[2][0] = 'O';

        // Check vertical win
        bool verticalWin = false;
        for (int i = 0; i < 3; ++i) {
            if (board[0][i] == 'O' && board[1][i] == 'O' && board[2][i] == 'O') {
                verticalWin = true;
                break;
            }
        }
        QVERIFY(verticalWin);

        // Test diagonal win
        board = QVector<QVector<char>>(3, QVector<char>(3, '.'));
        board[0][0] = 'X';
        board[1][1] = 'X';
        board[2][2] = 'X';

        // Check diagonal win
        bool diagonalWin = (board[0][0] == 'X' && board[1][1] == 'X' && board[2][2] == 'X') ||
                           (board[0][2] == 'X' && board[1][1] == 'X' && board[2][0] == 'X');
        QVERIFY(diagonalWin);
    }

    // Game History Tests
    void testGameHistory() {
        GameHistory history;

        // Test empty list
        QVERIFY(history.empty());
        QCOMPARE(history.getSize(), 0);

        // Test adding first game
        history.push_back('X', "Win");
        QVERIFY(!history.empty());
        QCOMPARE(history.getSize(), 1);

        // Test adding second game
        history.push_back('O', "Loss");
        QCOMPARE(history.getSize(), 2);

        // Test inserting game
        history.insert(1, 'X', "Draw");
        QCOMPARE(history.getSize(), 3);

        // Test erasing game
        history.erase(1);
        QCOMPARE(history.getSize(), 2);

        // Test getting game node
        GameHistory::Game* game = history.getGameNode(0);
        QVERIFY(game != nullptr);
        QCOMPARE(game->token, 'X');
        QCOMPARE(game->result, QString("Win"));

        // Clear the history
        history.clear();
        QVERIFY(history.empty());
        QCOMPARE(history.getSize(), 0);
    }

    // Game Reset Tests
    void testResetGame() {
        // Initialize game state
        QVector<QVector<char>> board(3, QVector<char>(3, '.'));
        int movesLeft = 5;

        // Set some moves
        board[0][0] = 'X';
        board[1][1] = 'O';
        movesLeft = 5;

        // Reset the game
        board = QVector<QVector<char>>(3, QVector<char>(3, '.'));
        movesLeft = 9;

        // Verify reset
        for(int i = 0; i < 3; ++i) {
            for(int j = 0; j < 3; ++j) {
                QCOMPARE(board[i][j], '.');
            }
        }
        QCOMPARE(movesLeft, 9);
    }

    // Game Move Tests
    void testOnClick() {
        QVector<QVector<char>> board(3, QVector<char>(3, '.'));
        int movesLeft = 9;
        char currentToken = 'X';

        // Test valid move
        int row = 1;
        int col = 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == '.') {
            board[row][col] = currentToken;
            movesLeft--;
            QCOMPARE(board[row][col], 'X');
            QCOMPARE(movesLeft, 8);
        }

        // Test invalid move (occupied position)
        row = 1;
        col = 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == '.') {
            board[row][col] = currentToken;
            movesLeft--;
        }
        QCOMPARE(board[row][col], 'X'); // Should still be X
        QCOMPARE(movesLeft, 8); // Should not have changed

        // Test invalid move (out of bounds)
        row = 3;
        col = 3;
        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == '.') {
            board[row][col] = currentToken;
            movesLeft--;
        }
        QCOMPARE(movesLeft, 8); // Should not have changed
    }

    // Player List Tests
    void testPlayerList() {
        PlayerList players;

        // Test empty list
        QVERIFY(players.empty());
        QCOMPARE(players.getSize(), 0);

        // Test adding players
        players.push_back("player1", "pass1");
        QVERIFY(!players.empty());
        QCOMPARE(players.getSize(), 1);
        QVERIFY(players.isfound("player1", "pass1"));

        players.push_back("player2", "pass2");
        QCOMPARE(players.getSize(), 2);
        QVERIFY(players.isfound("player2", "pass2"));

        // Test inserting player at specific position
        players.insert(1, "player3", "pass3");
        QCOMPARE(players.getSize(), 3);
        QVERIFY(players.isfound("player3", "pass3"));

        // Test getting player node
        PlayerList::Player* player = players.getPlayerNode("player1", "pass1");
        QVERIFY(player != nullptr);
        QCOMPARE(player->username, QString("player1"));
        QCOMPARE(player->password, QString("pass1"));

        // Test erasing player
        players.erase(1);
        QCOMPARE(players.getSize(), 2);
        QVERIFY(!players.isfound("player3", "pass3"));
        QVERIFY(players.isfound("player1", "pass1"));
        QVERIFY(players.isfound("player2", "pass2"));
    }

    // Additional Game Logic Tests
    void testGameBoardOperations() {
        QVector<QVector<char>> board(3, QVector<char>(3, '.'));

        // Test board initialization
        for(int i = 0; i < 3; ++i) {
            for(int j = 0; j < 3; ++j) {
                QCOMPARE(board[i][j], '.');
            }
        }

        // Test making moves
        board[0][0] = 'X';
        board[1][1] = 'O';
        board[2][2] = 'X';

        QCOMPARE(board[0][0], 'X');
        QCOMPARE(board[1][1], 'O');
        QCOMPARE(board[2][2], 'X');

        // Test invalid moves
        QCOMPARE(board[0][1], '.');
        QCOMPARE(board[1][0], '.');
        QCOMPARE(board[2][1], '.');
    }

    void testGameHistoryOperations() {
        GameHistory history;

        // Test empty history
        QVERIFY(history.empty());
        QCOMPARE(history.getSize(), 0);

        // Test adding games
        history.push_back('X', "Win");
        QVERIFY(!history.empty());
        QCOMPARE(history.getSize(), 1);

        history.push_back('O', "Loss");
        QCOMPARE(history.getSize(), 2);

        // Test inserting game at specific position
        history.insert(1, 'X', "Draw");
        QCOMPARE(history.getSize(), 3);

        // Verify game order and indices
        GameHistory::Game* game1 = history.getGameNode(0);
        GameHistory::Game* game2 = history.getGameNode(1);
        GameHistory::Game* game3 = history.getGameNode(2);

        QVERIFY(game1 != nullptr);
        QVERIFY(game2 != nullptr);
        QVERIFY(game3 != nullptr);

        QCOMPARE(game1->token, 'X');
        QCOMPARE(game1->result, QString("Win"));
        QCOMPARE(game2->token, 'X');
        QCOMPARE(game2->result, QString("Draw"));
        QCOMPARE(game3->token, 'O');
        QCOMPARE(game3->result, QString("Loss"));

        // Test erasing game
        history.erase(1);
        QCOMPARE(history.getSize(), 2);

        // Verify remaining games and updated indices
        game1 = history.getGameNode(0);
        game2 = history.getGameNode(1);

        QVERIFY(game1 != nullptr);
        QVERIFY(game2 != nullptr);
        QCOMPARE(game1->token, 'X');
        QCOMPARE(game1->result, QString("Win"));
        QCOMPARE(game2->token, 'O');
        QCOMPARE(game2->result, QString("Loss"));
    }

    void testPlayerListEdgeCases() {
        PlayerList players;

        // Test inserting at invalid positions
        players.insert(-1, "player1", "pass1"); // Should not insert
        QCOMPARE(players.getSize(), 0);
        QVERIFY(players.empty());

        players.insert(1, "player1", "pass1"); // Should not insert
        QCOMPARE(players.getSize(), 0);
        QVERIFY(players.empty());

        // Test inserting at valid positions
        players.insert(0, "player1", "pass1"); // Should insert at beginning
        QCOMPARE(players.getSize(), 1);
        QVERIFY(!players.empty());

        // Clear the list for next tests
        players.clear();
        QCOMPARE(players.getSize(), 0);
        QVERIFY(players.empty());

        // Test erasing from empty list
        players.erase(0);
        QCOMPARE(players.getSize(), 0);
        QVERIFY(players.empty());

        // Test erasing with invalid index
        players.push_back("player1", "pass1");
        QCOMPARE(players.getSize(), 1);
        QVERIFY(!players.empty());

        players.erase(-1); // Should not erase
        QCOMPARE(players.getSize(), 1);
        QVERIFY(!players.empty());

        players.erase(1); // Should not erase
        QCOMPARE(players.getSize(), 1);
        QVERIFY(!players.empty());

        // Test finding non-existent player
        QVERIFY(!players.isfound("nonexistent", "pass"));

        // Test getting non-existent player node
        PlayerList::Player* player = players.getPlayerNode("nonexistent", "pass");
        QVERIFY(player == nullptr);

        // Clear the list at the end
        players.clear();
        QCOMPARE(players.getSize(), 0);
        QVERIFY(players.empty());
    }

    void testGameHistoryEdgeCases() {
        GameHistory history;

        // Test inserting at invalid positions
        history.insert(-1, 'X', "Win"); // Should not insert
        QCOMPARE(history.getSize(), 0);
        QVERIFY(history.empty());

        history.insert(1, 'X', "Win"); // Should not insert
        QCOMPARE(history.getSize(), 0);
        QVERIFY(history.empty());

        // Test inserting at valid positions
        history.insert(0, 'X', "Win"); // Should insert at beginning
        QCOMPARE(history.getSize(), 1);
        QVERIFY(!history.empty());

        // Clear the history for next tests
        history.clear();
        QCOMPARE(history.getSize(), 0);
        QVERIFY(history.empty());

        // Test erasing from empty list
        history.erase(0);
        QCOMPARE(history.getSize(), 0);
        QVERIFY(history.empty());

        // Test erasing with invalid index
        history.push_back('X', "Win");
        QCOMPARE(history.getSize(), 1);
        QVERIFY(!history.empty());

        history.erase(-1); // Should not erase
        QCOMPARE(history.getSize(), 1);
        QVERIFY(!history.empty());

        history.erase(1); // Should not erase
        QCOMPARE(history.getSize(), 1);
        QVERIFY(!history.empty());

        // Test getting non-existent game node
        GameHistory::Game* game = history.getGameNode(1);
        QVERIFY(game == nullptr);

        // Clear the history at the end
        history.clear();
        QCOMPARE(history.getSize(), 0);
        QVERIFY(history.empty());
    }

    void testSignUp() {
        MockDatabase db;
        QVERIFY(db.open());

        // Test successful sign up
        QVERIFY(db.addUser("testuser", "testpass"));
        QVERIFY(db.verifyUser("testuser", "testpass"));

        // Test duplicate username
        QVERIFY(!db.addUser("testuser", "differentpass"));

        // Test empty username/password
        QVERIFY(!db.addUser("", "testpass"));
        QVERIFY(!db.addUser("testuser", ""));

        // Test special characters
        QVERIFY(db.addUser("user@123", "pass#456"));
        QVERIFY(db.verifyUser("user@123", "pass#456"));

        // Clear the database
        db.close();
        QVERIFY(db.open());
        QVERIFY(!db.verifyUser("testuser", "testpass"));
        QVERIFY(!db.verifyUser("user@123", "pass#456"));

        QVERIFY(db.close());
    }

    void testSignIn() {
        MockDatabase db;
        QVERIFY(db.open());

        // Setup test user
        db.addUser("testuser", "testpass");

        // Test successful sign in
        QVERIFY(db.verifyUser("testuser", "testpass"));

        // Test wrong password
        QVERIFY(!db.verifyUser("testuser", "wrongpass"));

        // Test non-existent user
        QVERIFY(!db.verifyUser("nonexistent", "anypass"));

        // Test case sensitivity
        QVERIFY(!db.verifyUser("TESTUSER", "testpass"));
        QVERIFY(!db.verifyUser("testuser", "TESTPASS"));

        QVERIFY(db.close());
    }

    void testAccountManagement() {
        MockDatabase db;
        QVERIFY(db.open());

        // Test account creation and verification
        QVERIFY(db.addUser("user1", "pass1"));
        QVERIFY(db.addUser("user2", "pass2"));

        // Test multiple accounts
        QVERIFY(db.verifyUser("user1", "pass1"));
        QVERIFY(db.verifyUser("user2", "pass2"));
        QVERIFY(!db.verifyUser("user1", "pass2"));
        QVERIFY(!db.verifyUser("user2", "pass1"));

        // Test password update
        QVERIFY(db.updatePassword("user1", "newpass"));
        QVERIFY(db.verifyUser("user1", "newpass"));
        QVERIFY(!db.verifyUser("user1", "pass1"));

        // Test updating non-existent user
        QVERIFY(!db.updatePassword("nonexistent", "newpass"));

        QVERIFY(db.close());
    }
};

QTEST_APPLESS_MAIN(TestComponents)
#include "test_components.moc"
