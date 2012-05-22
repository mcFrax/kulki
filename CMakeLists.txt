cmake_minimum_required(VERSION 2.0)

PROJECT(kulki)
FIND_PACKAGE(Qt4 REQUIRED)

SET(kulki_SOURCES main.cpp
	main_window.cpp
	ball.cpp ballcolor.cpp ballitem.cpp balltypes.cpp
	board.cpp square.cpp )

SET(kulki_HEADERS main_window.hpp 
	ball.hpp board.hpp boardimplementation.hpp square.hpp )

QT4_WRAP_CPP(kulki_HEADERS_MOC ${kulki_HEADERS})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(kulki ${kulki_SOURCES}
    ${kulki_HEADERS_MOC})
TARGET_LINK_LIBRARIES(kulki ${QT_LIBRARIES})