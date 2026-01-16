#include "DICOMScanner.h"
#include "DatabaseManager.h"
#include <QDebug>
#include <QDirIterator>
#include <QRandomGenerator>
#include <QThread>

// Note: In a real implementation, we would include DCMTK headers here:
// #include "dcmtk/dcmdata/dctk.h"

DICOMScanner::DICOMScanner(QObject *parent) : QObject(parent) {}

void DICOMScanner::scanFolder(const QString &path) {
  QDirIterator it(path, QDir::Files | QDir::NoSymLinks,
                  QDirIterator::Subdirectories);
  QStringList files;
  while (it.hasNext()) {
    files << it.next();
  }

  int total = files.size();
  int current = 0;

  for (const QString &file : files) {
    if (isDicomFile(file)) {
      processFile(file);
    }
    current++;
    emit progressUpdated(current, total);
  }

  emit scanFinished();
}

bool DICOMScanner::isDicomFile(const QString &filePath) {
  // A more robust check would read the "DICM" prefix at offset 128
  return filePath.endsWith(".dcm", Qt::CaseInsensitive);
}

void DICOMScanner::processFile(const QString &filePath) {
  qDebug() << "Indexing file:" << filePath;

  /*
     MOCK EXTRACTION (Simulating DCMTK)
     In Phase 2, we will replace this with real DcmFileFormat::loadFile calls.
  */

  // We'll use the filename as a mock UID for demonstration in the table
  QString mockStudyUID = "MOCK_STUDY_" + QFileInfo(filePath).baseName();
  QString mockPatientName = "Patient_" + QFileInfo(filePath).baseName();
  QString mockID =
      "ID_" + QString::number(QRandomGenerator::global()->generate() % 1000);
  QString mockDate = "2024-01-16";

  DatabaseManager::instance().addStudy(mockStudyUID, mockPatientName, mockID,
                                       mockDate);

  emit studyAdded(mockStudyUID);
}
