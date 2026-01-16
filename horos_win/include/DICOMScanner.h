#ifndef DICOMSCANNER_H
#define DICOMSCANNER_H

#include <QFileInfo>
#include <QObject>
#include <QStringList>

class DICOMScanner : public QObject {
  Q_OBJECT

public:
  explicit DICOMScanner(QObject *parent = nullptr);

  // Scans a folder recursively and indexes DICOM files
  void scanFolder(const QString &path);

signals:
  void progressUpdated(int current, int total);
  void studyAdded(const QString &uid);
  void scanFinished();

private:
  void processFile(const QString &filePath);
  bool isDicomFile(const QString &filePath);
};

#endif // DICOMSCANNER_H
