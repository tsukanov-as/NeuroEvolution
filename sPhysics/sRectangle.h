#pragma once
#include "sSimpleBody.h"


class sRectangle : public sSimpleBody
{

private:


public:

	sRectangle(float32 width = 1, float32 height = 1, float32 x = 0, float32 y = 0, float32 angle = 0)
	{
		m_fixtureDefs.push_back(&s_fixtureDef);
		s_fixtureDef.shape = &s_shape;
		m_bodyDef = &s_bodyDef;
		setSize(width, height);
		setPosition(b2Vec2(x, y));
		setAngle(angle);
	}




	void setSize(b2Vec2 size)
	{
		m_size = size;
	}
	void setSize(float32 width, float32 height)
	{
		setSize(b2Vec2(width, height));
	}
	b2Vec2 getSize()
	{
		return m_size;
	}


protected:

	void addToWorld(b2World &world)
	{
		printf("rectanle add to world \n");
		s_shape.SetAsBox(m_size.x, m_size.y);		
		sSimpleBody::addToWorld(world);
	}

	b2Vec2 m_size;

	static b2PolygonShape s_shape;
	static b2FixtureDef s_fixtureDef;
	static b2BodyDef s_bodyDef;
};

b2PolygonShape sRectangle::s_shape;
b2FixtureDef sRectangle::s_fixtureDef;
b2BodyDef sRectangle::s_bodyDef;