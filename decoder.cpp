#include <QDebug>
#include <QFile>
#include <QFileInfo>

#include "decoder.h"

Decoder::Decoder()
{
}

void Decoder::start(QString inputCodec, QString outputCodec, bool saveOld)
{
    qDebug() << "Input codec:" << inputCodec << " Output codec:" << outputCodec << " Save old:" << saveOld;

    if (saveOld) {
        foreach (QString fileName, fileList) {
            QString tmpName(fileName);
            tmpName.append("-tmp");
            QString query("iconv -f \"%1\" -t \"%2\" \"%3\" > \"%4\"");
            query.replace("%1" ,inputCodec);
            query.replace("%2" ,outputCodec);
            query.replace("%3" ,fileName);
            query.replace("%4" ,tmpName);
            std::system(query.toStdString().c_str());
            QFile file(fileName);
            QString newName(fileName);
            newName.append("-original");
            file.rename(newName);
            file.close();
            QFile::rename(tmpName, fileName);
            QFile tmpFile(tmpName);
            tmpFile.remove();
            tmpFile.close();
        }
    } else {
        foreach (QString fileName, fileList) {
            QString tmpName(fileName);
            tmpName.append("-tmp");
            QString query("iconv -f \"%1\" -t \"%2\" \"%3\" > \"%4\"");
            query.replace("%1" ,inputCodec);
            query.replace("%2" ,outputCodec);
            query.replace("%3" ,fileName);
            query.replace("%4" ,tmpName);
            std::system(query.toStdString().c_str());
            QFile file(fileName);
            file.remove();
            file.close();
            QFile::rename(tmpName, fileName);
        }
    }
}

void Decoder::addFileLink(QString link)
{
    fileList.append(link);
}

void Decoder::clear()
{
    fileList.clear();
}
