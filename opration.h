#ifndef OPRATION_H
#define OPRATION_H
#include <QString>

class Opration
{
    QString model;
    bool loaddone;
public:
    Opration();
    QString getmodel();
    bool setmodel(QString str);
    bool isload();
    bool setloaddone(bool flag);

};

#endif // OPRATION_H
