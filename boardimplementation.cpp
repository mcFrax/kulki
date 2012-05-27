#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPixmap>

#include "board.hpp"
#include "boardimplementation.hpp"
#include "square.hpp"
#include "highlightitem.hpp"
#include "ball.hpp"
#include "pointsearneditem.hpp"
#include "player.hpp"

#include "debugtools.hpp"

using namespace std;

//klasa do "udawanych" zamian - przy sprawdzaniu ruchow
//dziala identycznie z Board::BoardInfo, ale zamienia s1 z s2
//!Udaje Board::BoardInfo z zamiana dwoch Square'ow
class SwapPretender
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


//!Konstruktor
BoardImplementation::BoardImplementation(const GameSetup& s, QObject * parent)
	: Board(s, parent), highlights(s.width, s.height), turnNumber(0), swappingBalls(0)
{
	setBackgroundBrush(QBrush(QPixmap(":Background.jpg")));
	
	BallColor::createTable(setup.colors);
	
	//creating squares
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			squares(ix, iy) = new Square(ix, iy, this);
		}
	}
	
	//setting squares neighbours
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			squares(ix, iy)->setNeighbours(
					ix == 0 ? 0 : squares(ix-1, iy),
					iy == 0 ? 0 : squares(ix, iy-1),
					ix == s.width-1  ? 0 : squares(ix+1, iy),
					iy == s.height-1 ? 0 : squares(ix, iy+1)
					);
		}
	}
	
	//creating highlights
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			highlights(ix, iy) = make_pair(
				//na brzegu wstawiam zera (tym ? :)
				//poziomy HighlightItem
				(squares(ix, iy)->getNeighbour(Square::right))
					?new HighlightItem((ix+1)*Square::xSize, 
						(iy+0.5)*Square::ySize, this, squares(ix, iy),
						squares(ix+1, iy), HighlightItem::horizontal)
					:0,
				//pionowy HighlightItem
				(squares(ix, iy)->getNeighbour(Square::bottom))
					?new HighlightItem((ix+0.5)*Square::xSize, 
						(iy+1)*Square::ySize, this, squares(ix, iy),
						squares(ix, iy+1), HighlightItem::vertical)
					:0
			);
		}
	}
	
	//placing balls
	for (uint iy = 0; iy < s.height; ++iy){
		for (uint ix = 0; ix < s.width; ++ix){
			Ball::getNew(setup, squares(ix, iy), iy+1);
		}
	}
	
	computeLegalMoves(0);
	setState(animatingMove);
}

//!Destruktor
BoardImplementation::~BoardImplementation()
{
}

BoardImplementation::Row::Row()
	: sum(0), sumUpToDate(0)
{
}

//!Punkty za rzadek
int BoardImplementation::Row::points() const
{
	int sum = 0;
	for (const_iterator i = begin(); i != end(); ++i)
		sum += (*i)->getPointValue();
	for (const_iterator i = begin(); i != end(); ++i)
		sum = (*i)->applyPointModificator(sum);
	return sum;
}

//!Sprawdza, czy b pasuje do kulek w tym rzadku
bool BoardImplementation::Row::matches(Ball* b) const
{
	for (const_iterator i = begin(); i != end(); ++i)
		if ((*i)->getColor() != b->getColor())
			return 0;
	return 1;
}

void BoardImplementation::Row::emitPointsEarnedItem(Board* b) const
{
	QPointF center(0, 0);
	for (const_iterator i = begin(); i != end(); ++i)
		center += (*i)->getRect().center() / qreal(size());
	new PointsEarnedItem(b, center.x(), center.y(), points());
}

//!overrides QGraphicsScene::mousePressEvent
void BoardImplementation::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	switch (state) {
		case waitingForMove :
			QGraphicsScene::mousePressEvent(event);
			return;
		default:
			event->ignore();
	}
}

//!overrides QGraphicsScene::mouseMoveEvent
void BoardImplementation::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	switch (state) {
		case waitingForMove :
			QGraphicsScene::mouseMoveEvent(event);
			return;
		default:
			event->ignore();
	}
}

//!Wykonanie ruchu - zamiany kulki na s1 z kulka na s2 - przez biezacego gracza
bool BoardImplementation::move(Square* s1, Square* s2)
{
	if (state != waitingForMove || !isLegal(s1, s2))
		return 0;
	setState(animatingMove);
	Ball* b1 = s1->getBall();
	Ball* b2 = s2->getBall();
	b1->detach();
	b2->detach();
	b1->placeOnSquare(s2, s1);
	b2->placeOnSquare(s1, s2);
	
	for (uint iy = 0; iy < setup.height; ++iy){
		for (uint ix = 0; ix < setup.width; ++ix){
			if (highlights(ix, iy).first)
				highlights(ix, iy).first->setVisible(0);
			if (highlights(ix, iy).second)
				highlights(ix, iy).second->setVisible(0);
		}
	}
	
	//~ check(1);
	swappingBalls = 1;
	emit playerMoved(curPlayer);
	return 1;
}

//dzieki template'owi moge uzywac i BoardInfo i SwapPretender,
//bez zbednego polimorfizmu i bez copy'iego paste'a
//!zlicza ile jest kulek koloru bc na (sX,sY) i sasiednich polach typu (sX+k*xM, sY+k*yM)
template <class BoardInfo_or_SwapPretender>
int countSame(uint sX, uint sY, int xM, int yM, BallColor bc,
		const BoardInfo_or_SwapPretender& board)
{
	int res = 0;
	int ix = sX;
	int iy = sY;
	while (board.contains(ix, iy) && board(ix, iy) == bc){
		++res;
		ix += xM;
		iy += yM;
	}
	return res;
}

//!Sprawdza, czy jest jakas trojka (rzad), zawierajacy kulke na (x,y)
template <class BoardInfo_or_SwapPretender>
static bool checkForRow(uint x, uint y, const Board::GameSetup& setup,
		const BoardInfo_or_SwapPretender& board)
{
	for (uint c = 0; c < setup.colors; ++c){
		if (((countSame(x, y, 1, 0, c, board)
				+countSame(x, y, -1, 0, c, board)-1)
				>= int(setup.rowLength))
				||
				((countSame(x, y, 0, 1, c, board)
				+countSame(x, y, 0, -1, c, board)-1)
				>= int(setup.rowLength))){
			return 1;
		}
	}
	return 0;
}

//!Wylicza legalne zamiany typu <x,y> z <x+dx, y+dy>. Przy dx, dy != 0, 1  i dx+dy != 1 moze sie krzaczyc
void BoardImplementation::computeLegalMoves(const int dx, const int dy, bool setVisibility)
{
	for ( uint iy = 0; iy < setup.height-dy; ++iy){
		for ( uint ix = 0; ix < setup.width-dx; ++ix){
			//symulacja zamiany, i sprawdzenie, czy cos daje
			SwapPretender pret(squares, setup.width, setup.height, 
					squares(ix, iy), squares(ix+dx, iy+dy));
			if (checkForRow(ix, iy, setup, pret)
					|| checkForRow(ix+dx, iy+dy, setup, pret)){
				legalMoves.insert(make_pair(squares(ix, iy), 
						squares(ix+dx, iy+dy)));
				legalMoves.insert(make_pair(squares(ix+dx, iy+dy), 
						squares(ix, iy)));
				if (setVisibility){
					if (dx)
						highlights(ix,iy).first->setVisible(1);
					else
						highlights(ix,iy).second->setVisible(1);
				}
			}
		}
	}
}

//!Wylicza wszystkie legalne zamiany
bool BoardImplementation::computeLegalMoves(bool setVisibility)
{
	legalMoves.clear();
	computeLegalMoves(1, 0, setVisibility);
	computeLegalMoves(0, 1, setVisibility);
	return !legalMoves.empty();
}

//!Rozpoczyna nowa ture
void BoardImplementation::newTurn()
{
	total = 0;
	++turnNumber;
	curPlayer = nextPlayer;
	nextPlayer = 0;
	for ( int iy = setup.height-1; iy >= 0 ; --iy){
		for ( int ix = 0; ix < setup.width; ++ix){
			squares(ix, iy)->getBall()->newTurnUpdate();
		}
	}
	if (curPlayer){
		computeLegalMoves(curPlayer->isHuman());
		setState(waitingForMove);
	} else {
		setState(waitingForPlayer);
	}
}

static void putBestToSet(Board::BoardInfo bi, int ix, int iy, 
		const Board::GameSetup& setup, set<pair<int, int> >& rowset,
		int xM, int yM)
{
	//wrzuca do seta pare odpowiadajaca najdluzszemu rzadkowi zawierajacemu kulke
	//na ix, iy (o ile taki jest)
	int best = 0;
	pair<int, int> bestPair;
	for (uint c = 0; c < setup.colors; ++c){
		int l1 = countSame(ix, iy, xM, yM, c, bi);
		int l2 = countSame(ix, iy, -xM, -yM, c, bi);
		if (l1+l2-1 > best){
			best = l1+l2-1;
			if (xM != 0) { //zakladam, ze liczymy po x-ach
				bestPair = make_pair(ix-l2+1, ix+l1-1);
			} else {
				bestPair = make_pair(iy-l2+1, iy+l1-1);
			}
		}
	}
	if (best >= setup.rowLength){
		rowset.insert(bestPair);
	}
}

BoardImplementation::Rows BoardImplementation::findRows()
{
	BoardInfo bi(squares, setup.width, setup.height);
	Rows res;
	for (uint iy = 0; iy < setup.height; ++iy){
		set<pair<int, int> > rowset;
		for (uint ix = 0; ix < setup.width; ++ix){
			putBestToSet(bi, ix, iy, setup, rowset, 1, 0);
		}
		//mamy gotowy set
		for (set<pair<int, int> >::iterator i = rowset.begin(); i != rowset.end(); ++i){
			res.push_back(Row());
			for (int j = i->first; j <= i->second; ++j)
				res.back().push_back(squares(j, iy)->getBall());
		}
	}
	
	for (uint ix = 0; ix < setup.width; ++ix){
		set<pair<int, int> > rowset;
		for (uint iy = 0; iy < setup.height; ++iy){
			putBestToSet(bi, ix, iy, setup, rowset, 0, 1);
		}
		//mamy gotowy set
		for (set<pair<int, int> >::iterator i = rowset.begin(); i != rowset.end(); ++i){
			res.push_back(Row());
			for (int j = i->first; j <= i->second; ++j)
				res.back().push_back(squares(ix, j)->getBall());
		}
	}
	
	return res;
}

void BoardImplementation::ballsNewCheckUpdate()
{
	for ( int iy = setup.height-1; iy >= 0 ; --iy){
		for ( int ix = 0; ix < setup.width; ++ix){
			squares(ix, iy)->getBall()->newCheckUpdate();
		}
	}
}

//!Sprawdzenie planszy (po ruchu/uzupelnieniu) i odpowiednia zmiana stanu
void BoardImplementation::check()
{
	setState(animatingMove);
	if (!swappingBalls){
		ballsNewCheckUpdate();
	}
	swappingBalls = 0;
	const Rows& rows = findRows();
	if (rows.empty()){
		//spadanie sie skonczylo
		if (curPlayer)
			emit playerMoveEnded(curPlayer, total);
		if (!computeLegalMoves(0)){
			setState(locked);
			emit gameEnded();
		} else {
			newTurn();
		}
	} else {
		set<Ball*> balls;
		for (Rows::const_iterator ir = rows.begin(); ir != rows.end(); ++ir){
			for (Row::const_iterator ib = ir->begin(); ib != ir->end(); ++ib)
				balls.insert(*ib);
			total += ir->points();
			ir->emitPointsEarnedItem(this);
		}
		for (set<Ball*>::const_iterator i = balls.begin(); i != balls.end(); ++i)
			(*i)->explode();
		refill();
	}
}

//!zepchniecie kulek (grawitacja) + utworzenie nowych (zapelnienie planszy)
void BoardImplementation::refill()
{
	for ( int iy = setup.height-1; iy >= 0 ; --iy){
		for ( int ix = 0; ix < setup.width; ++ix){
			squares(ix, iy)->takeBall(800);
		}
	}
}

//!Oddaje nastepny ruch graczowi p.
void BoardImplementation::setCurrentPlayer(Player* p)
{
	if (state == waitingForPlayer){
		curPlayer = p;
		computeLegalMoves(curPlayer->isHuman());
		setState(waitingForMove);
	} else if (!nextPlayer) {
		nextPlayer = p;
	}
}