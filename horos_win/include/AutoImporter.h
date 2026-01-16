#ifndef AUTOIMPORTER_H
#define AUTOIMPORTER_H

#include <QFileSystemWatcher>
#include <QObject>
#include <QStringList>
#include <QTimer>

class AutoImporter : public QObject {
  Q_OBJECT
public:
  explicit AutoImporter(QObject *parent = nullptr);

  void setWatchDirectory(const QString &path);
  void start();
  void stop();

signals:
  void studyImported(const QString &studyUID);
  void logMessage(const QString &msg);

private slots:
  void onDirectoryChanged(const QString &path);
  void processQueue();

private:
  QFileSystemWatcher *m_watcher;
  QString m_watchPath;
  QStringList m_pendingFiles;
  QTimer *m_processTimer;
};

#endif // AUTOIMPORTER_H
