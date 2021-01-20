#ifndef _PHYSICS_H_
#define _PHYSICS_H_

//=============================================
//http://geomalgorithms.com/a03-_inclusion.html
//=============================================

#include "defs.h"

class Body
{
public:
	fPoint* v = nullptr;
	fPoint center;
	int sides;
	float radius;
	float angle;
	float rotation;
	float speed;
	vertexbool* b = nullptr;

	Body(float _x,float _y,int _sides, float _radius, float _angle, float _rotation, float _speed) :
		center{ _x,_y }, sides(_sides), radius(_radius), angle(_angle), rotation(_rotation), speed(_speed)
	{
		v = new fPoint[sides];
		b = new vertexbool[360];
		ResetPos(_x, _y);
	}
	~Body();

	void ResetPos(float _x, float _y);

	void Update(float dt);

	void Draw(SDL_Renderer* renderer);

	void Collision(Body* b2);
};

class Physics
{
public:

	Body* b1 = new Body(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 7, 100, 0, 0, 0);
	Body* b2 = new Body(800, 300, 3, 50, 0, 0, 0);

	bool collision[2] = { false,false };

	Physics();

	~Physics();

	void Update(float dt, Mouse _mouse, int* _keyboard);

	void Draw(SDL_Renderer* renderer);
};

#endif
