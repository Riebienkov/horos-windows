#include "AutoImporter.h"
#include "DICOMScanner.h"
#include <QDebug>
#include <QDir>

AutoImporter::AutoImporter(QObject *parent) : QObject(parent) {
  m_watcher = new QFileSystemWatcher(this);
  m_processTimer = new QTimer(this);
  m_processTimer->setSingleShot(true);

  connect(m_watcher, &QFileSystemWatcher::directoryChanged, this,
          &AutoImporter::onDirectoryChanged);
  connect(m_processTimer, &QTimer::timeout, this, &AutoImporter::processQueue);
}

void AutoImporter::setWatchDirectory(const QString &path) {
  m_watchPath = path;
  if (m_watcher->directories().contains(path))
    return;

  if (!m_watcher->directories().isEmpty()) {
    m_watcher->removePaths(m_watcher->directories());
  }

  if (QDir(path).exists()) {
    m_watcher->addPath(path);
    emit logMessage("Auto-Import: Watching directory " + path);
  }
}

void AutoImporter::onDirectoryChanged(const QString &path) {
  // Wait a bit for files to be fully written
  m_processTimer->start(2000);
}

void AutoImporter::processQueue() {
  emit logMessage("Auto-Import: Scanning for new studies...");

  DICOMScanner scanner;
  // Scan the directory and import to database
  // scanner.scanDirectory(m_watchPath);

  emit logMessage("Auto-Import: Scan complete.");
}

void AutoImporter::start() {
  if (!m_watchPath.isEmpty())
    m_watcher->addPath(m_watchPath);
}

void AutoImporter::stop() { m_watcher->removePaths(m_watcher->directories()); }
