cmake_minimum_required(VERSION 2.0)

PROJECT(kulki)
FIND_PACKAGE(Qt4 REQUIRED)

SET(kulki_SOURCES main.cpp
	main_window.cpp
	ball.cpp ballcolor.cpp ballitem.cpp balltypes.cpp
	board.cpp square.cpp highlightitem.cpp
	pointsearneditem.cpp boardimplementation.cpp player.cpp 
	humanplayer.cpp aiplayer.cpp settings.cpp playerinfoitem.cpp
	playericonitem.cpp gloss.cpp )

SET(kulki_HEADERS main_window.hpp 
	ballitem.hpp board.hpp boardimplementation.hpp 
	highlightitem.hpp pointsearneditem.hpp player.hpp aiplayer.hpp 
	playerinfoitem.hpp playericonitem.hpp)

SET(kulki_RESOURCES kulki.qrc)

QT4_WRAP_CPP(kulki_HEADERS_MOC ${kulki_HEADERS})
QT4_ADD_RESOURCES(kulki_RESOURCES_RCC ${kulki_RESOURCES})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(kulki ${kulki_SOURCES}
    ${kulki_HEADERS_MOC}
    ${kulki_RESOURCES_RCC} )
    
SET(CMAKE-CXX-FLAGS ${CMAKE-CXX-FLAGS} -O3 -flto)
    
#~ TARGET_LINK_LIBRARIES(kulki ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(kulki -fuse-linker-plugin -flto -fwhole-program ${QT_LIBRARIES})