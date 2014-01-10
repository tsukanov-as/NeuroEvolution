#pragma once
#include "sSimpleBody.h"


class sCircle : public sSimpleBody
{

private:


public:

	sCircle(float32 radius = 1, float32 x = 0, float32 y = 0)
	{
		m_fixtureDefs.push_back(&s_fixtureDef);
		s_fixtureDef.shape = &s_shape;
		m_bodyDef = &s_bodyDef;
		setRadius(radius);
		setPosition(b2Vec2(x, y));
	}


	void addToWorld(b2World &world)
	{
		s_shape.m_radius = m_radius;
		sBody::addToWorld(world);
	}

	void setRadius(float32 radius)
	{
		m_radius = radius;
	}

	float32 getRadius()
	{
		return m_radius;
	}


protected:



	float32 m_radius;

	static b2CircleShape s_shape;
	static b2FixtureDef s_fixtureDef;
	static b2BodyDef s_bodyDef;
};