#include "DatabaseManager.h"
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent) {}

DatabaseManager::~DatabaseManager() { close(); }

DatabaseManager &DatabaseManager::instance() {
  static DatabaseManager instance;
  return instance;
}

bool DatabaseManager::initialize(const QString &path) {
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(path);

  if (!m_db.open()) {
    qCritical() << "Error: connection with database failed" << m_db.lastError();
    return false;
  }

  return createTables();
}

void DatabaseManager::close() {
  if (m_db.isOpen()) {
    m_db.close();
  }
}

bool DatabaseManager::createTables() {
  QSqlQuery query;
  bool success = true;

  // Studies Table
  success &= query.exec("CREATE TABLE IF NOT EXISTS studies ("
                        "studyInstanceUID TEXT PRIMARY KEY, "
                        "patientName TEXT, "
                        "patientID TEXT, "
                        "studyDate TEXT, "
                        "studyDescription TEXT, "
                        "modality TEXT)");

  // Series Table
  success &= query.exec("CREATE TABLE IF NOT EXISTS series ("
                        "seriesInstanceUID TEXT PRIMARY KEY, "
                        "studyInstanceUID TEXT, "
                        "seriesNumber INTEGER, "
                        "modality TEXT, "
                        "seriesDescription TEXT, "
                        "FOREIGN KEY(studyInstanceUID) REFERENCES "
                        "studies(studyInstanceUID) ON DELETE CASCADE)");

  // Images Table
  success &= query.exec("CREATE TABLE IF NOT EXISTS images ("
                        "sopInstanceUID TEXT PRIMARY KEY, "
                        "seriesInstanceUID TEXT, "
                        "instanceNumber INTEGER, "
                        "pathString TEXT, "
                        "sliceLocation REAL, "
                        "FOREIGN KEY(seriesInstanceUID) REFERENCES "
                        "series(seriesInstanceUID) ON DELETE CASCADE)");

  if (!success) {
    qCritical() << "Error creating tables" << query.lastError();
  }

  return success;
}

bool DatabaseManager::addStudy(const QString &uid, const QString &patientName,
                               const QString &patientID, const QString &date) {
  QSqlQuery query;
  query.prepare("INSERT OR REPLACE INTO studies (studyInstanceUID, "
                "patientName, patientID, studyDate) "
                "VALUES (:uid, :name, :id, :date)");
  query.bindValue(":uid", uid);
  query.bindValue(":name", patientName);
  query.bindValue(":id", patientID);
  query.bindValue(":date", date);
  return query.exec();
}

bool DatabaseManager::addSeries(const QString &uid, const QString &studyUid,
                                int seriesNumber, const QString &modality) {
  QSqlQuery query;
  query.prepare("INSERT OR REPLACE INTO series (seriesInstanceUID, "
                "studyInstanceUID, seriesNumber, modality) "
                "VALUES (:uid, :suid, :num, :mod)");
  query.bindValue(":uid", uid);
  query.bindValue(":suid", studyUid);
  query.bindValue(":num", seriesNumber);
  query.bindValue(":mod", modality);
  return query.exec();
}

bool DatabaseManager::addImage(const QString &sopUid, const QString &seriesUid,
                               int instanceNumber, const QString &path) {
  QSqlQuery query;
  query.prepare("INSERT OR REPLACE INTO images (sopInstanceUID, "
                "seriesInstanceUID, instanceNumber, pathString) "
                "VALUES (:uid, :suid, :num, :path)");
  query.bindValue(":uid", sopUid);
  query.bindValue(":suid", seriesUid);
  query.bindValue(":num", instanceNumber);
  query.bindValue(":path", path);
  return query.exec();
}
