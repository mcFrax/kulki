#pragma once

#include "board.hpp"

class Player;
class QWidget;

void recordHighScore(Player*, const Board::GameSetup&);

void showHighScores();