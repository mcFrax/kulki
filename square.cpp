#include <QGraphicsSceneHoverEvent>

#include "square.hpp"
#include "board.hpp"
#include "ball.hpp"
#include "ballitem.hpp"

#include "debugtools.hpp"

const qreal Square::xSize = 36;
const qreal Square::ySize = 36;

Square::Square(uint xPos, uint yPos, Board* b)
	: QGraphicsRectItem(0, 0, xSize, ySize, 0, b), xPos(xPos), yPos(yPos),
		board(b), highlight(0)
{
	setPos(xPos*xSize, yPos*ySize);
	setAcceptHoverEvents(1);
	for (int i = 0; i < 5; ++i)
		neighbours[i] = 0;
}

void Square::setNeighbours(Square* l, Square* t, Square* r, Square* b)
{
	neighbours[0] = l;
	neighbours[1] = t;
	neighbours[2] = r;
	neighbours[3] = b;
	//neighbours[4] = 0;
}

//~ void Square::setHighlight(bool hl, Square::Side side)
//~ {
	//~ static const qreal r = Square::ySize/3.0;
	//~ static const qreal coords[4][2] = {
		//~ {0-r, Square::ySize/2.0-r},
		//~ {Square::xSize/2.0-r, 0-r},
		//~ {Square::xSize-r, Square::ySize/2.0-r},
		//~ {Square::xSize/2.0-r, Square::ySize-r},
	//~ };
	//~ 
	//~ if (side == none)
		//~ hl = 0;
	//~ if (bool(highlight) == hl && highlightedSide == side)
		//~ return;
		//~ 
	//~ if (highlight){
		//~ delete highlight;
		//~ highlight = 0;
	//~ }
	//~ if (hl){
		//~ QGraphicsEllipseItem* el = board->addEllipse(
				//~ coords[side][0], coords[side][1],
				//~ r*2, r*2);
		//~ el->setPos(pos());
		//~ el->setBrush(QBrush(QColor(0,0,0,127)));
		//~ el->setPen(Qt::NoPen);
		//~ el->setAcceptedMouseButtons(0);
		//~ highlight = el;
		//~ highlightedSide = side;
	//~ }
//~ }

//~ static inline bool between(qreal x, qreal min, qreal max)
//~ {
	//~ return min < x && x <= max;
//~ }

//~ Square::Side Square::side(QPointF p)
//~ {
	//~ static const qreal clickableBorder = 8;
	//~ 
	//~ //srodek 
	//~ if (between(p.x(), clickableBorder, Square::xSize-clickableBorder)
			//~ && between(p.y(), clickableBorder, Square::ySize-clickableBorder))
		//~ return none;
	//~ 
	//~ //brzeg
	//~ bool b1 = p.x() < p.y();
	//~ bool b2 = Square::xSize-p.x() < p.y();
	//~ 
	//~ //wiem, ze to oblesne, ale najprostsze:
	//~ if (b1)
		//~ if (b2)
			//~ return bottom;
		//~ else
			//~ return left;
	//~ else
		//~ if (b2)
			//~ return right;
		//~ else
			//~ return top;
//~ }

//~ void Square::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
//~ {
	//~ Side s = side(event->pos());
	//~ if (board->isLegal(this, neighbours[s]))
		//~ setHighlight(1, s);
	//~ else
		//~ setHighlight(0);
//~ }
//~ 
//~ void Square::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
//~ {
	//~ for (int i = 0; i < 4; ++i) {
		//~ if (highlights[i])
			//~ highlights[i].deactivate();
	//~ }
//~ }

//~ void Square::mousePressEvent(QGraphicsSceneMouseEvent * event)
//~ {
	//~ Side s = side(event->pos());
	//~ if (s == none)
		//~ return;
	//~ board->move(this, neighbours[s]);
//~ }

Square* Square::getNeighbour(Square::Side s)
{
	return neighbours[s];
}

Square*const* Square::getNeighbours() const
{
	return neighbours;
}

Ball* Square::getBall()
{
	return ball;
}

Board* Square::getBoard()
{
	return board;
}

BallColor Square::ballColor()
{
	return ball->getColor();
	//~ if (ball)
		//~ return ball->getColor();
	//~ else
		//~ return BallColor::none;
}

void Square::takeBall(int animDelay)
{
	if (ball) return;
	int fall = 1;
	for (Square* s = this->neighbours[top]; s != 0; s = s->neighbours[top]) {
		if (s->ball) {
			s->ball->placeOnSquare(this, fall, animDelay);
			return;
		}
		++fall;
	}
	Ball::getNew(board->gameSetup(), this, fall, animDelay);
}