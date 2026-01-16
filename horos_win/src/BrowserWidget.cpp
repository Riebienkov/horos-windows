#include "BrowserWidget.h"
#include "ViewerWidget.h"
#include <QDebug>
#include <QHeaderView>
#include <QSqlError>
#include <QVBoxLayout>

BrowserWidget::BrowserWidget(QWidget *parent) : QWidget(parent) { setupUI(); }

void BrowserWidget::setupUI() {
  QVBoxLayout *layout = new QVBoxLayout(this);

  m_studyTree = new QTreeView(this);
  m_model = new QSqlTableModel(this);
  m_model->setTable("studies");
  m_model->select();

  // Set column headers
  m_model->setHeaderData(0, Qt::Horizontal, tr("Study UID"));
  m_model->setHeaderData(1, Qt::Horizontal, tr("Patient Name"));
  m_model->setHeaderData(2, Qt::Horizontal, tr("Patient ID"));
  m_model->setHeaderData(3, Qt::Horizontal, tr("Date"));

  m_studyTree->setModel(m_model);
  m_studyTree->header()->setSectionResizeMode(QHeaderView::Stretch);
  m_studyTree->setEditTriggers(QAbstractItemView::NoEditTriggers);

  connect(m_studyTree, &QTreeView::doubleClicked,
          [this](const QModelIndex &index) {
            QString studyUID =
                m_model->data(m_model->index(index.row(), 0)).toString();
            ViewerWidget *viewer = new ViewerWidget(studyUID);
            viewer->setWindowTitle("Horos Viewer - " + studyUID);
            viewer->resize(800, 800);
            // For "minimim" demo, we just simulate getting a list of images
            QStringList dummyImages;
            for (int i = 0; i < 50; ++i)
              dummyImages << QString("image_%1.dcm").arg(i);

            viewer->view()->setImageStack(dummyImages);
            viewer->show();
          });

  layout->addWidget(m_studyTree);
}
