#include "opration.h"

Opration::Opration()
{
    loaddone = false;
    model = "";
}

QString Opration::getmodel(){
    return this->model;
}


bool Opration::setmodel(QString str){
    this->model = str;
    return true;
}

bool Opration::isload(){
    return this->loaddone;
}

bool Opration::setloaddone(bool flag){
    this->loaddone = flag;
    return true;
}
