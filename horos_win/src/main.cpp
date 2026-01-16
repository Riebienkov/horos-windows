#include "DatabaseManager.h"
#include "MainWindow.h"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // Initialize Database in AppData
  QString appData =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir().mkpath(appData);
  DatabaseManager::instance().initialize(appData + "/horos_database.sqlite");

  // Set application-wide stylesheet for the Dark Theme
  a.setStyleSheet("QMainWindow { background-color: #121212; color: #E0E0E0; }"
                  "QMenuBar { background-color: #1E1E1E; color: #E0E0E0; }"
                  "QStatusBar { background-color: #1E1E1E; color: #888; }");

  MainWindow w;
  w.show();
  return a.exec();
}
