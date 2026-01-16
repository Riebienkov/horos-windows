#ifndef DICOMREADER_H
#define DICOMREADER_H

#include <QDataStream>
#include <QFile>
#include <QString>
#include <QVector>

struct DicomImage {
  QVector<uint16_t> pixelData;
  int width = 0;
  int height = 0;
  float windowWidth = 2000.0f;
  float windowLevel = 400.0f;
  bool valid = false;
};

class DicomReader {
public:
  static DicomImage load(const QString &filePath) {
    DicomImage img;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
      return img;

    // Skip preamble (128) + prefix (4)
    if (!file.seek(132))
      return img;

    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);

    // STUB: Basic DICOM parser looking for Pixel Data (7FE0, 0010)
    // In a real app, use DCMTK. Here we do a simplified scan.
    while (!in.atEnd()) {
      uint16_t group, element;
      in >> group >> element;

      char vr[2];
      in.readRawData(vr, 2);

      uint16_t length16;
      in >> length16;
      uint32_t length = length16;

      // Handle long VRs (OB, OW, SQ, UN, UT)
      if (vr[0] == 'O' || vr[0] == 'S' || vr[0] == 'U') {
        in >> length; // 4 byte length follows 2 reserved bytes
      }

      if (group == 0x0028 && element == 0x0010) { // Rows
        uint16_t val;
        in >> val;
        img.height = val;
      } else if (group == 0x0028 && element == 0x0011) { // Columns
        uint16_t val;
        in >> val;
        img.width = val;
      } else if (group == 0x7FE0 && element == 0x0010) { // Pixel Data
        img.pixelData.resize(length / 2);
        in.readRawData((char *)img.pixelData.data(), length);
        img.valid = true;
        break;
      } else {
        file.seek(file.pos() + length);
      }

      // Safety break for corrupted files
      if (file.pos() > 50 * 1024 * 1024)
        break;
    }

    // Final fallback if parsing failed but it looks like a DICOM
    if (!img.valid && img.width > 0 && img.height > 0) {
      // Maybe it's a simple raw format? Log error.
    }

    return img;
  }
};

#endif // DICOMREADER_H
