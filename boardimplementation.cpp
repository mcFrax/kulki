#include <QPen>
#include <QFont>
#include <QBrush>
#include <QColor>
#include <QPixmap>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsDropShadowEffect>

#include <stdexcept>

#include "board.hpp"
#include "boardimplementation.hpp"
#include "square.hpp"
#include "highlightitem.hpp"
#include "ball.hpp"
#include "pointsearneditem.hpp"
#include "player.hpp"
#include "playerinfoitem.hpp"
#include "playericonitem.hpp"
#include "HighScores.hpp"

#include "settings.hpp"
#include "debugtools.hpp"

using namespace std;

//klasa do "udawanych" zamian - przy sprawdzaniu ruchow
//dziala identycznie z Board::BoardInfo, ale zamienia s1 z s2
//!Udaje Board::BoardInfo z zamiana dwoch Square'ow
class SwapPretender
{
	private:
		Square** array;
		int arrayWidth;
		int arrayHeight;
		Square* s1;
		Square* s2;
	public:
		SwapPretender(Square** array, int arrayWidth, int arrayHeight,
				Square* s1, Square* s2)
			: array(array), arrayWidth(arrayWidth), 
					arrayHeight(arrayHeight), s1(s1), s2(s2)
		{
		}
		BallColor operator () (int x, int y) const
		{
			Square* square = array[y*arrayWidth+x];
			if (square == s1)
				square = s2;
			else if (square == s2)
				square = s1;
			return square->ballColor();
		}
		int width() const
		{
			return arrayWidth;
		}
		int height() const
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
BoardImplementation::BoardImplementation(const GameSetup& s, QGraphicsItem * parent)
	: Board(s, parent), highlights(s.width, s.height)
{
	if (players.empty()){
		QGraphicsSimpleTextItem* t = new QGraphicsSimpleTextItem("No Players", this);
		QFont font;
		font.setPixelSize(24);
		t->setFont(font);
		t->setPen(QPen(Qt::black));
		t->setBrush(Qt::white);
		return;
	}
	
	for (int i = 0; i < players.size(); ++i){
		new PlayerInfoItem(players[i], this, 
				s.width*Square::size() + 3*margin(),
				i*PlayerInfoItem::height() + margin(),
				this);
	}
	
	setPen(Qt::NoPen);
	//~ setPen(QPen(Qt::white, 5.0, Qt::DashLine, Qt::RoundCap));
	
	BallColor::createTable(setup.colors);
	
	//creating squares
	for (int iy = 0; iy < s.height; ++iy){
		for (int ix = 0; ix < s.width; ++ix){
			squares(ix, iy) = new Square(ix, iy, this);
		}
	}
	
	//setting squares neighbours
	for (int iy = 0; iy < s.height; ++iy){
		for (int ix = 0; ix < s.width; ++ix){
			squares(ix, iy)->setNeighbours(
					ix == 0 ? 0 : squares(ix-1, iy),
					iy == 0 ? 0 : squares(ix, iy-1),
					ix == s.width-1  ? 0 : squares(ix+1, iy),
					iy == s.height-1 ? 0 : squares(ix, iy+1)
					);
		}
	}
	
	//placing balls
	for (int iy = 0; iy < s.height; ++iy){
		for (int ix = 0; ix < s.width; ++ix){
			Ball::getNew(setup, squares(ix, iy), -1);
			//~ Ball::getNew(setup, squares(ix, iy), 0, 
				//~ (iy+2)*settings()->value("board/ballPlacingDelay").toInt());
		}
	}
	
	check();
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
	//assert(b->currentPlayer())
	QPointF center(0, 0);
	for (const_iterator i = begin(); i != end(); ++i)
		center += (*i)->square()->center() / qreal(size());
	new PointsEarnedItem(b, center.x(), center.y(), points(), 
			b->currentPlayer()->color());
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
	
	//niszczenie highlightow
	for (int iy = 0; iy < setup.height; ++iy){
		for (int ix = 0; ix < setup.width; ++ix){
			if (highlights(ix, iy).first)
				delete highlights(ix, iy).first;
			if (highlights(ix, iy).second)
				delete highlights(ix, iy).second;
			highlights(ix, iy).first = 0;
			highlights(ix, iy).second = 0;
		}
	}
	
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
	for ( int iy = 0; iy < setup.height-dy; ++iy){
		for ( int ix = 0; ix < setup.width-dx; ++ix){
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
					//tworzenie highlightow
					if (dx)
						highlights(ix,iy).first = new HighlightItem((ix+1)*Square::size(), 
							(iy+0.5)*Square::size(), this, squares(ix, iy),
							squares(ix+1, iy), HighlightItem::horizontal);
					else
						highlights(ix,iy).second = new HighlightItem((ix+0.5)*Square::size(), 
							(iy+1)*Square::size(), this, squares(ix, iy),
							squares(ix, iy+1), HighlightItem::vertical);
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
	if (turnNumber == 1){
		curPlayer = players[0];
	} else {
		for (int i = 0; i < players.size(); ++i){
			if (curPlayer == players[i]){
				if (i == players.size()-1)
					curPlayer = players[0];
				else
					curPlayer = players[i+1];
				break;
			}
		}
	}
		
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
	//jezeli dla kazdego pola wezme najdluzszy rzadek, ktory je zawiera, to ten rzadek na pewno bedzie "dobry", tzn, w szczeg. policze go raz
	BoardInfo bi(squares, setup.width, setup.height);
	Rows res;
	for (int iy = 0; iy < setup.height; ++iy){
		set<pair<int, int> > rowset;
		for (int ix = 0; ix < setup.width; ++ix){
			putBestToSet(bi, ix, iy, setup, rowset, 1, 0);
		}
		//mamy gotowy set
		for (set<pair<int, int> >::iterator i = rowset.begin(); i != rowset.end(); ++i){
			res.push_back(Row());
			for (int j = i->first; j <= i->second; ++j)
				res.back().push_back(squares(j, iy)->getBall());
		}
	}
	
	for (int ix = 0; ix < setup.width; ++ix){
		set<pair<int, int> > rowset;
		for (int iy = 0; iy < setup.height; ++iy){
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
			Ball* b = squares(ix, iy)->getBall();
			if (b)
				b->newCheckUpdate();
		}
	}
}

//!Tworzy napis "Koniec gry" i ew. dopisuje zwyciezce
QGraphicsItem* BoardImplementation::createGameOverItem(Player* winner)
{
	struct GameOverItem : public QGraphicsItem
	{
		GameOverItem(QGraphicsItem* pI) : QGraphicsItem(pI) {}
		QRectF boundingRect() const 
		{
			return childrenBoundingRect();
		}
		void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget * widget = 0)
		{
		}
	};
	
	QGraphicsItem* endItem = new GameOverItem(this);
	
	QGraphicsSimpleTextItem* gameOver = new QGraphicsSimpleTextItem(
			tr("Koniec gry"), endItem);
			
		QFont font;
		font.setPixelSize(82);
		gameOver->setFont(font);
		
		gameOver->setBrush(Qt::white);
		gameOver->setZValue(100);
		gameOver->setGraphicsEffect(new QGraphicsDropShadowEffect(this));
	
	if (winner){
		PlayerIconItem* icon = new PlayerIconItem(winner->color(), endItem);
		icon->blink(1);
		icon->activate();
		QGraphicsSimpleTextItem* winnerText = new QGraphicsSimpleTextItem(
				tr("Wygrywa ")+winner->name()+"!", endItem);
		
		QFont winnerFont;
		winnerFont.setPixelSize(24);
		winnerText->setFont(winnerFont);
		winnerText->setPen(QPen(Qt::black));
		winnerText->setBrush(Qt::white);
		
		qreal off = 
				(gameOver->boundingRect().width()
				-(winnerText->boundingRect().width() + icon->boundingRect().width() + 3)
				) /2;
		if ( off < 0){
			gameOver->setPos(-off, 0);
			icon->setPos(0, gameOver->boundingRect().height() + 3);
			winnerText->setPos(
					icon->boundingRect().width() + 3,
					gameOver->boundingRect().height() + 3);
		} else {
			icon->setPos(off, gameOver->boundingRect().height() + 3);
			winnerText->setPos(
					off + icon->boundingRect().width() + 3,
					gameOver->boundingRect().height() + 3);
		}
	}
		
	return endItem;
}

//!Znajduje zwyciezce. Przy remisie, braku punktow lub jednym graczu zwraca 0.
static Player* getWinner(const QList<Player*> players)
{
	if (players.size() <= 1) return 0;
	int bestPoints = 0;
	Player* best = 0;
	QListIterator<Player*> it(players);
	while(it.hasNext()){
		Player* p = it.next();
		if (p->points() == bestPoints){
			best = 0;
		} else {
			if (p->points() > bestPoints){
				best = p;
				bestPoints = p->points();
			}
		}
	}
	return best;
}

void BoardImplementation::endGame()
{
	Player* winner = getWinner(players);
	
	//tu robie taki duzy napis
	QGraphicsItem* endItem = createGameOverItem(winner);
	
	endItem->setPos(
			(rect().width()  - endItem->boundingRect().width())  /2, 
			(rect().height() - endItem->boundingRect().height()) /2);
	
	//a tutaj jest to, co istotne, czyli stan i signal
	setState(locked);
	emit gameEnded();
	if (players.size() == 1) recordHighScore(players[0], setup);
}

//!Sprawdzenie planszy (po ruchu/uzupelnieniu) i odpowiednia zmiana stanu
void BoardImplementation::check()
{
	setState(animatingMove);
	const Rows& rows = findRows();
	if (rows.empty()){
		if (internalState == preparing){
			internalState = normal;
			//"zapalanie" kulek
			for (int iy = 0; iy < setup.height; ++iy){
				for (int ix = 0; ix < setup.width; ++ix){
					squares(ix, iy)->getBall()->appear(
						(iy+2)*settings()->value("board/ballPlacingDelay").toInt());
				}
			}
			return;
		}
		//spadanie sie skonczylo
		if (curPlayer)
			emit playerMoveEnded(curPlayer, total);
		if ((turnLimit && turnNumber == turnLimit) || !computeLegalMoves(0)){
			endGame();
		} else {
			newTurn();
		}
	} else {
		set<Ball*> balls;
		for (Rows::const_iterator ir = rows.begin(); ir != rows.end(); ++ir){
			for (Row::const_iterator ib = ir->begin(); ib != ir->end(); ++ib)
				balls.insert(*ib);
			uint points = ir->points();
			total += points;
			
			if (curPlayer){
				ir->emitPointsEarnedItem(this);
				curPlayer->pointsVal += points;
				emit pointsEarned(curPlayer, points);
			}
		}
		if (internalState == preparing){
			for (set<Ball*>::const_iterator i = balls.begin(); i != balls.end(); ++i)
				(*i)->explode(true);
		} else {
			for (set<Ball*>::const_iterator i = balls.begin(); i != balls.end(); ++i)
				(*i)->explode();
		}
		gravity();
	}
}

//!zepchniecie kulek (grawitacja)
void BoardImplementation::gravity()
{
	bool noEffect = 1;
	for ( int iy = setup.height-1; iy >= 0 ; --iy){
		for ( int ix = 0; ix < setup.width; ++ix){
			if (internalState == preparing){
				squares(ix, iy)->gravity(true);
			} else {
				if (squares(ix, iy)->gravity(
						settings()->value("ball/fallingDelay").toInt()))
					noEffect = 0;
			}
		}
	}
	
	if (internalState != preparing)
		internalState = falling;
	
	if (noEffect || internalState == preparing)
		refill(settings()->value("ball/appearDelay").toInt()); //wpp refilla odpali animationEnded
}

//!utworzenie nowych kulek (zapelnienie planszy)
void BoardImplementation::refill(int animDelay)
{
	if (internalState != preparing){
		internalState = normal;
		ballsNewCheckUpdate();
	}
	
	bool noEffect = 1;
	for ( int iy = setup.height-1; iy >= 0 ; --iy){
		for ( int ix = 0; ix < setup.width; ++ix){
			if (internalState == preparing){
				squares(ix, iy)->ensureHavingBall();
			} else {
				if (squares(ix, iy)->ensureHavingBall(animDelay))
					noEffect = 0;
			}
		}
	}
	
	if (noEffect || internalState == preparing)
		check(); //wpp checka odpali animationEnded
}