#pragma once
#include "ball.hpp"

class QPixmap;

class ColorBall : public Ball
{
	public:
		ColorBall(const BallColor&, Square*, int falling = 0, int animDelay = 0);
		uint getPointValue();
};

class JokerBall : public Ball
{
	private:
		static QPixmap* specialPixmap;
		static QBrush brush;
	public:
		JokerBall(Square*, int falling = 0, int animDelay = 0);
		uint getPointValue();
};

class ChameleonBall : public ColorBall
{
	private:
		static QPixmap* specialPixmap;
	public:
		ChameleonBall(Square*, int falling = 0, int animDelay = 0);
		void newCheckUpdate();
};

class DoubleBall : public ColorBall
{
	private:
		static QPixmap* specialPixmap;
	public:
		DoubleBall(const BallColor&, Square*, int falling = 0, int animDelay = 0);
		int applyPointModificator(const int& points) const;
};

class SkullBall : public ColorBall //or maybe it should be joker?
{
	private:
		static QPixmap* specialPixmap;
	public:
		SkullBall(const BallColor&, Square*, int falling = 0, int animDelay = 0);
		uint getPointValue();
		int applyPointModificator(const int& points) const;
};

class HourglassBall : public ColorBall
{
	private:
		static QPixmap* specialPixmap;
		uint points;
		bool firstTurn;
	public:
		HourglassBall(const BallColor&, Square*, int falling = 0, int animDelay = 0);
		void newTurnUpdate();
		uint getPointValue();
};