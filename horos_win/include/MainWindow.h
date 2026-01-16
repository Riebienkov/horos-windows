#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  void setupUI();
  class BrowserWidget *m_browser;
  class AutoImporter *m_autoImporter;
};

#endif // MAINWINDOW_H
