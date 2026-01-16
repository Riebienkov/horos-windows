#include "PACSPanel.h"
#include <QHBoxLayout>
#include <QHeaderView>

PACSPanel::PACSPanel(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);

  QHBoxLayout *searchLayout = new QHBoxLayout();
  m_searchEdit = new QLineEdit(this);
  m_searchEdit->setPlaceholderText("Search Patient Name...");
  m_searchBtn = new QPushButton("Search PACS", this);
  searchLayout->addWidget(m_searchEdit);
  searchLayout->addWidget(m_searchBtn);

  m_resultsTable = new QTableWidget(0, 4, this);
  m_resultsTable->setHorizontalHeaderLabels(
      {"Patient Name", "ID", "Date", "Description"});
  m_resultsTable->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  m_resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_resultsTable->setStyleSheet("background-color: #1e1e1e; color: white;");

  m_retrieveBtn = new QPushButton("Retrieve Selected Study", this);
  m_retrieveBtn->setEnabled(false);

  layout->addLayout(searchLayout);
  layout->addWidget(m_resultsTable);
  layout->addWidget(m_retrieveBtn);

  connect(m_searchBtn, &QPushButton::clicked, this,
          &PACSPanel::onSearchClicked);
  connect(m_retrieveBtn, &QPushButton::clicked, this,
          &PACSPanel::onRetrieveClicked);
  connect(&NetworkManager::instance(), &NetworkManager::queryFinished, this,
          &PACSPanel::updateResults);
}

void PACSPanel::onSearchClicked() {
  PACSNode node = {"Central PACS", "127.0.0.1", 104, "HOROS_PACS"};
  NetworkManager::instance().query(node, m_searchEdit->text());
}

void PACSPanel::onRetrieveClicked() {
  // Retrieval logic
}

void PACSPanel::updateResults(const QList<DicomQueryResult> &results) {
  m_resultsTable->setRowCount(0);
  for (const auto &res : results) {
    int row = m_resultsTable->rowCount();
    m_resultsTable->insertRow(row);
    m_resultsTable->setItem(row, 0, new QTableWidgetItem(res.patientName));
    m_resultsTable->setItem(row, 1, new QTableWidgetItem(res.patientID));
    m_resultsTable->setItem(row, 2, new QTableWidgetItem(res.studyDate));
    m_resultsTable->setItem(row, 3, new QTableWidgetItem(res.studyDescription));
  }
}
