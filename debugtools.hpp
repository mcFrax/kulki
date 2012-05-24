#pragma once

#include <iostream>
#include <QTime>

#define LINECHECK std::cerr << __FILE__":"<<__LINE__<<std::endl;
#define PRINT(x) std::cerr << __FILE__":"<<__LINE__<<':'<< #x " = "<<( x )<<std::endl;
#define TIME_STRING (QTime::currentTime().toString("hh:mm:ss:zzz").toStdString())