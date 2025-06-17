#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

#include "mainwindow.h"
#include "game_window.h"
#include "sign_in_up.h"
#include "ai.h"

void return_to_profile_from_game();
void return_to_change_info();


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // --- Auto-create 'Games' table in the default database ---
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mo_games.db"); // Change to your main DB file if needed
    if (db.open()) {
        QSqlQuery query(db);
        if (!query.exec("CREATE TABLE IF NOT EXISTS Games ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "winner VARCHAR(50), "
                        "date TIMESTAMP DEFAULT CURRENT_TIMESTAMP)")) {
            qDebug() << "Error creating Games table:" << query.lastError().text();
        }
    } else {
        qDebug() << "Error opening database for Games table creation:" << db.lastError().text();
    }
    // --------------------------------------------------------

    MainWindow w;
    SetCallBackGame(return_to_profile_from_game);
    SetCallBackAIGame(return_to_profile_from_game);
    SetCallBackChangeInfo(return_to_change_info);
    w.show();
    return a.exec();
}

void return_to_change_info(){
    Sign_In_Up* newChangeInfo;
    newChangeInfo = new Sign_In_Up();
    newChangeInfo->show();
}

void return_to_profile_from_game(){
    Profile* newprofwind;
    newprofwind = new Profile();
    newprofwind->show();
}
