#pragma once

#include <iostream>

#define LINECHECK std::cerr << __FILE__":"<<__LINE__<<std::endl;
#define PRINT(x) std::cerr << __FILE__":"<<__LINE__<<':'<< #x " = "<<( x )<<std::endl;