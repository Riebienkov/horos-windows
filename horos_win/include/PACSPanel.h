#ifndef PACSPANEL_H
#define PACSPANEL_H

#include "NetworkManager.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

class PACSPanel : public QWidget {
  Q_OBJECT
public:
  explicit PACSPanel(QWidget *parent = nullptr);

private slots:
  void onSearchClicked();
  void onRetrieveClicked();
  void updateResults(const QList<DicomQueryResult> &results);

private:
  QLineEdit *m_searchEdit;
  QTableWidget *m_resultsTable;
  QPushButton *m_searchBtn;
  QPushButton *m_retrieveBtn;
};

#endif // PACSPANEL_H
