#include "MainWindow.h"
#include "AutoImporter.h"
#include "BrowserWidget.h"
#include "DICOMScanner.h"
#include "DatabaseManager.h"
#include "ExportDialog.h"
#include "PACSPanel.h"
#include "SettingsDialog.h"
#include <QDockWidget>
#include <QFileDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUI();
  setWindowTitle("Horos | Windows Edition");
  resize(1200, 800);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
  // Basic Menu Bar
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(tr("&Import DICOM..."));
  fileMenu->addSeparator();
  fileMenu->addAction(tr("E&xit"), this, &QWidget::close);

  // Toolbar with Import Button
  QToolBar *toolbar = addToolBar(tr("Main"));
  QAction *importAction = toolbar->addAction(tr("Import Folder"));
  connect(importAction, &QAction::triggered, [this]() {
    QString dir =
        QFileDialog::getExistingDirectory(this, tr("Select DICOM Folder"));
    if (!dir.isEmpty()) {
      DICOMScanner *scanner = new DICOMScanner(this);
      // In a real app, run this in a QThread
      scanner->scanFolder(dir);
      statusBar()->showMessage(tr("Import complete"));
    }
  });

  // Browser Widget
  m_browser = new BrowserWidget(this);
  setCentralWidget(m_browser);

  // PACS Panel in Dock
  QDockWidget *pacsDock = new QDockWidget("PACS Search", this);
  pacsDock->setWidget(new PACSPanel(pacsDock));
  pacsDock->setFeatures(QDockWidget::DockWidgetMovable |
                        QDockWidget::DockWidgetClosable);
  addDockWidget(Qt::RightDockWidgetArea, pacsDock);

  // Auto Importer
  m_autoImporter = new AutoImporter(this);
  m_autoImporter->setWatchFolder(
      "C:/mock/network/folder"); // Mock network folder
  m_autoImporter->startWatching();

  statusBar()->showMessage(tr("Ready"));
}
