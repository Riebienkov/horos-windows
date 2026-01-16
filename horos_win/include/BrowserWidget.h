#ifndef BROWSERWIDGET_H
#define BROWSERWIDGET_H

#include <QSqlTableModel>
#include <QTreeView>
#include <QWidget>

class BrowserWidget : public QWidget {
  Q_OBJECT

public:
  explicit BrowserWidget(QWidget *parent = nullptr);

private:
  void setupUI();
  QTreeView *m_studyTree;
  QSqlTableModel *m_model;
};

#endif // BROWSERWIDGET_H
