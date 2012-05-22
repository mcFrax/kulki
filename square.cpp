#include "square.hpp"
#include "board.hpp"
#include "ball.hpp"
#include "ballitem.hpp"

const qreal Square::xSize = 36;
const qreal Square::ySize = 36;

Square::Square(uint xPos, uint yPos, Board* b)
	: QGraphicsRectItem(0, 0, xSize, ySize, 0, b), xPos(xPos), yPos(yPos),
		board(b)
{
	setPos(xPos*xSize, yPos*ySize);
	setAcceptHoverEvents(1);
}

void Square::setNeighbours(Square* l, Square* t, Square* r, Square* b)
{
	neighbours[0] = l;
	neighbours[1] = t;
	neighbours[2] = r;
	neighbours[3] = b;
}

void Square::setHighlight(bool hl, Square::Side side)
{
	//~ if (highlight){
		//~ delete highlight;
		//~ highlight = 0;
	//~ if (hl){
		//~ QGraphicsEllipseItem* el = new QGraphicsEllipseItem(6, 6,
				//~ Square::xSize-2*xmargin-6, Square::ySize-2*ymargin-6, this);
		//~ el->setBrush(QBrush(QColor(255,255,255,127)));
		//~ el->setPen(Qt::NoPen);
		//~ highlight = el;
	//~ }
}

void Square::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
}

void Square::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
}

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