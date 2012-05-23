#include <QGraphicsSceneMouseEvent>

#include "board.hpp"
#include "boardimplementation.hpp"
#include "square.hpp"
#include "ball.hpp"

#include "debugtools.hpp"

using namespace std;

//klasa do "udawanych" zamian - przy sprawdzaniu ruchow
//dziala identycznie z Board::BoardInfo, ale zamienia s1 z s2
class SwapPretender
//!<Udaje Board::BoardInfo z zamiana dwoch Square'ow
{
	private:
		Square** array;
		uint arrayWidth;
		uint arrayHeight;
		Square* s1;
		Square* s2;
	public:
		SwapPretender(Square** array, uint arrayWidth, uint arrayHeight,
				Square* s1, Square* s2)
			: array(array), arrayWidth(arrayWidth), 
					arrayHeight(arrayHeight), s1(s1), s2(s2)
		{
		}
		BallColor operator () (uint x, uint y) const
		{
			Square* square = array[y*arrayWidth+x];
			if (square == s1)
				square = s2;
			else if (square == s2)
				square = s1;
			return square->ballColor();
		}
		uint width() const
		{
			return arrayWidth;
		}
		uint height() const
		{
			return arrayHeight;
		}
		bool contains(int x, int y) const
		{
			return x >= 0 && x < arrayWidth
				&& y >= 0 && y < arrayHeight;
		}
};

Board* Board::newBoard(const GameSetup& s, QObject * parent)
//!<Konstruktor
{
	return new BoardImplementation(s, parent);
}

Board::Board(const GameSetup& s, QObject * parent)
//!<Konstruktor
	: QGraphicsScene(0, 0, s.width*Square::xSize,
		s.height*Square::ySize, parent), setup(s), state(animatingMove),
		curPlayer(0)
{
}

Player* Board::currentPlayer()
//!<Zwraca biezacego gracza
{
	return curPlayer;
}

const Board::GameSetup& Board::gameSetup()
//!<Zwraca gameSetup
{
	return setup;
}

Board::State Board::getState()
//!<Zwraca aktualny stan planszy/gry
{
	return state;
}

bool Board::inBoard(int x, int y)
//!<Sprawdza, czy wspolrzedne (x, y) mieszcza sie w planszy
{
	return x >= 0 && x < setup.width
		&& y >= 0 && y < setup.height;
}

bool Board::isLegal(Square* s1, Square* s2) const
//!<sprawdza, czy ruch - zamiana s1 z s2 - jest legalny
{
	return legalMoves.find(make_pair(s1, s2)) != legalMoves.end();
}

Board::BoardInfo::BoardInfo(Square** array, uint width, uint height)
//!<Kontruktor
	: array(array), arrayWidth(width), arrayHeight(height)
{
}

BallColor Board::BoardInfo::operator () (uint x, uint y) const
//!<Square na pozycji (x, y)
{
	return array[y*arrayWidth+x]->ballColor();
}

uint Board::BoardInfo::width() const
//!<Szerokosc (w polach)
{
	return arrayWidth;
}

uint Board::BoardInfo::height() const
{
//!<Wyskokosc (w polach)
	return arrayHeight;
}

bool Board::BoardInfo::contains(int x, int y) const
//!<Sprawdza, czy wspolrzedne (x, y) mieszcza sie w planszy
{
	return x >= 0 && x < arrayWidth
		&& y >= 0 && y < arrayHeight;
}

BoardImplementation::BoardImplementation(const GameSetup& s, QObject * parent)
//!<Konstruktor
	: Board(s, parent)
{
	BallColor::createTable(setup.colors);
	squares = new Square*[s.width*s.height];
	
	//creating squares
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			squares[iy*s.width+ix] = new Square(ix, iy, this);
		}
	}
	
	//setting squares neighbours
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			square(ix, iy)->setNeighbours(
					ix == 0 ? 0 : square(ix-1, iy),
					iy == 0 ? 0 : square(ix, iy-1),
					ix == s.width-1  ? 0 : square(ix+1, iy),
					iy == s.height-1 ? 0 : square(ix, iy+1)
					);
		}
	}
	
	//placing balls
	#warning here must go avoiding triplets on start
	#warning possibly by making continous fall till its good
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			Ball::getNew(setup, square(ix, iy), setup.height+2);
		}
	}
	
	computeLegalMoves();
	state = waitingForPlayer;
}

BoardImplementation::~BoardImplementation()
//!<Destruktor
{
	delete squares;
}

void BoardImplementation::mousePressEvent(QGraphicsSceneMouseEvent * event)
//!<overrides mousePressEvent
{
	switch (state) {
		case waitingForMove :
			QGraphicsScene::mousePressEvent(event);
			return;
		default:
			event->ignore();
	}
}

void BoardImplementation::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
//!<overrides mouseMoveEvent
{
	switch (state) {
		case waitingForMove :
			QGraphicsScene::mouseMoveEvent(event);
			return;
		default:
			event->ignore();
	}
}

bool BoardImplementation::move(Square* s1, Square* s2)
//!Wykonanie ruchu - zamiany kulki na s1 z kulka na s2 - przez biezacego gracza
{
	if (state != waitingForMove || !isLegal(s1, s2))
		return 0;
	state = animatingMove;
	Ball* b1 = s1->getBall();
	Ball* b2 = s2->getBall();
	b1->placeOnSquare(s2);
	b2->placeOnSquare(s1);
	
	check();
	return 1;
}

//dzieki template'owi moge uzywac i BoardInfo i SwapPretender,
//bez polimorfizmu i bez copy'iego paste'a
template <class BoardInfo_or_SwapPretender>
uint countSame(uint sX, uint sY, bool xM, bool yM, BallColor bc,
		const BoardInfo_or_SwapPretender& board)
//!<zlicza ile jest kulek na (sX,sY) i sasiednich polach typu (sX+k*xM, sY+k*yM) koloru bc
{
	if (board(sX, sY) != bc)
		return 0;
	
	uint res = 1;
	int ix = sX+xM;
	int iy = sY+yM;
	while (board.contains(ix, iy) && board(ix, iy) == bc){
		++res;
		ix += xM;
		iy += yM;
	}
	ix = sX-xM;
	iy = sY-yM;
	while (board.contains(ix, iy) && board(ix, iy) == bc){
		++res;
		ix -= xM;
		iy -= yM;
	}
	return res;
}

template <class BoardInfo_or_SwapPretender>
static bool checkForTriple(uint x, uint y, const Board::GameSetup& setup,
		const BoardInfo_or_SwapPretender& board)
//!<Sprawdza, czy jest jakas trojka (rzad), zawierajacy kulke na (x,y)
{
	for (uint c = 0; c < setup.colors; ++c){
		if (countSame(x, y, 1, 0, c, board) >= setup.rowLength
				|| countSame(x, y, 0, 1, c, board) >= setup.rowLength)
			return 1;
	}
	return 0;
}

void BoardImplementation::computeLegalMoves(const int dx, const int dy)
//!<Wylicza legalne zamiany typu <x,y> z <x+dx, y+dy>
{
	for ( uint iy = 0; iy < setup.height-dy; ++iy){
		for ( uint ix = 0; ix < setup.width-dx; ++ix){
			//symulacja zamiany, i sprawdzenie, czy cos daje
			SwapPretender pret(squares, setup.width, setup.height, 
					square(ix, iy), square(ix+dx, iy+dy));
			if (checkForTriple(ix, iy, setup, pret)
					|| checkForTriple(ix+dx, iy+dy, setup, pret)){
				legalMoves.insert(make_pair(square(ix, iy), 
						square(ix+dx, iy+dy)));
				legalMoves.insert(make_pair(square(ix+dx, iy+dy), 
						square(ix, iy)));
			}
		}
	}
}

bool BoardImplementation::computeLegalMoves()
//!<Wylicza wszystkie legalne zamiany
{
	legalMoves.clear();
	computeLegalMoves(1, 0);
	computeLegalMoves(0, 1);
	return !legalMoves.empty();
}

Square* BoardImplementation::square(uint x, uint y)
//!<Square na pozycji (x, y)
{
	return squares[y*setup.width+x];
}

void BoardImplementation::check()
//!<Sprawdzenie planszy (po ruchu) i odpowiednia zmiana stanu
{
	//~ state = checking;
	//~ #warning here goes check
	//~ if (0)
		//~ state = falling; //cleaning? something?
	//~ else if (1)
		//~ state = normal;
	//~ else
		//~ state = locked;
}

void BoardImplementation::refill()
//!<zepchniecie kulek (grawitacja) + utworzenie nowych (zapelnienie planszy)
{
	for ( int iy = setup.height-1; iy >= 0 ; --iy){
		for ( int ix = 0; ix < setup.width; ++ix){
			square(ix, iy)->takeBall();
		}
	}
}

#warning more documentation needed here (or there)
void BoardImplementation::setCurrentPlayer(Player* p)
//!<Oddaje nastepny ruch graczowi p.
{
	if (curPlayer)
		return;
		
	curPlayer = p;
	if (state == waitingForPlayer)
		state = waitingForMove;
}