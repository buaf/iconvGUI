#ifndef DECODER_H
#define DECODER_H

#include <QStringList>

class Decoder
{
private:
    QStringList fileList;

public:
    Decoder();
    void start(QString inputCodec, QString outputCodec, bool saveOld);
    void addFileLink(QString link);
    void clear();
};

#endif // DECODER_H
