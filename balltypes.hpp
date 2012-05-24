#pragma once
#include "ball.hpp"

class ColorBall : public Ball
{
	public:
		//~ ColorBall(const BallColor&);
		ColorBall(const BallColor&, Square*, int falling = 0, int animDelay = 0);
		uint getPointValue();
};

class JokerBall : public Ball
{
	public:
		//~ JokerBall();
		JokerBall(Square*, int falling = 0, int animDelay = 0);
		uint getPointValue();
};

class CameleonBall : public ColorBall
{
	public:
		//~ CameleonBall();
		CameleonBall(Square*, int falling = 0, int animDelay = 0);
		void update(uint plyNumber);
};

class DoubleBall : public ColorBall
{
	public:
		//~ DoubleBall(const BallColor&);
		DoubleBall(const BallColor&, Square*, int falling = 0, int animDelay = 0);
		int applyPointModificator(const int& points);
};

class SkullBall : public ColorBall //or maybe it should be joker?
{
	public:
		//~ SkullBall(const BallColor&);
		SkullBall(const BallColor&, Square*, int falling = 0, int animDelay = 0);
		uint getPointValue();
		int applyPointModificator(const int& points);
};

class HourglassBall : public ColorBall
{
	private:
		uint points;
	public:
		//~ HourglassBall(const BallColor&);
		HourglassBall(const BallColor&, Square*, int falling = 0, int animDelay = 0);
		void update(uint plyNumber);
		uint getPointValue();
};