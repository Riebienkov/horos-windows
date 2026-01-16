#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>

class DatabaseManager : public QObject {
  Q_OBJECT

public:
  static DatabaseManager &instance();
  bool initialize(const QString &path);
  void close();

  // Database Actions
  bool createTables();

  // DICOM Metadata Inserts (Stubs for Phase 1)
  bool addStudy(const QString &uid, const QString &patientName,
                const QString &patientID, const QString &date);
  bool addSeries(const QString &uid, const QString &studyUid, int seriesNumber,
                 const QString &modality);
  bool addImage(const QString &sopUid, const QString &seriesUid,
                int instanceNumber, const QString &path);

private:
  explicit DatabaseManager(QObject *parent = nullptr);
  ~DatabaseManager();
  DatabaseManager(const DatabaseManager &) = delete;
  DatabaseManager &operator=(const DatabaseManager &) = delete;

  QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
