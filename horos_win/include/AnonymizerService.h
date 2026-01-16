#ifndef ANONYMIZERSERVICE_H
#define ANONYMIZERSERVICE_H

#include <QString>
#include <QStringList>

class AnonymizerService {
public:
  struct Config {
    bool removeName = true;
    bool removeID = true;
    bool removeBirthDate = true;
    QString replacementName = "ANONYMOUS";
  };

  static bool anonymize(const QString &inputPath, const QString &outputPath,
                        const Config &config);
};

#endif // ANONYMIZERSERVICE_H
