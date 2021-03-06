#pragma once
#include "sUniformBody.h"


class sEdgeRectangle : public sUniformBody
{

private:


public:

	sEdgeRectangle(float32 x = 0, float32 y = 0, float32 width = 30, float32 height = 20, float32 angle = 0)
	{
		m_bodyType = STATIC_BODY;
		__fixtureDef.shape = &__shape;
		m_fixtureDefs.push_back(&__fixtureDef);
		m_bodyDef = &__bodyDef;
		m_size = b2Vec2(width, height);
		setPosition(b2Vec2(x, y));
		setAngle(angle);
	}
	sEdgeRectangle(sEdgeRectangle &rect) : sUniformBody(rect)
	{
		_copy(rect);
	}
	void copy(const sEdgeRectangle &rect)
	{
		sUniformBody::copy(rect);
		_copy(rect);
	}
	sEdgeRectangle& operator=(const sEdgeRectangle& rect)
	{
		copy(rect);
		return *this;
	}

	void setSize(b2Vec2 size)
	{
		m_size = size;

		//b2Vec2 points[4];
		//float width = m_size.x;
		//float height = -m_size.y;
		//points[0] = b2Vec2(-width/2, -height/2);
		//points[1] = b2Vec2(-width/2, height/2);
		//points[2] = b2Vec2(width/2, height/2);
		//points[3] = b2Vec2(width/2, -height/2);
		//__shape.CreateLoop(points, 4);

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

	
	void addToWorld(sWorld &world)
	{
		b2Vec2 points[4];
		float width = m_size.x;
		float height = m_size.y;
		points[0] = b2Vec2(-width/2, -height/2);
		points[1] = b2Vec2(-width/2, height/2);
		points[2] = b2Vec2(width/2, height/2);
		points[3] = b2Vec2(width/2, -height/2);
		__shape.CreateLoop(points, 4);
		sUniformBody::addToWorld(world);
	}
	

	void _copy(const sEdgeRectangle &rect)
	{
		m_size = rect.m_size;
	}

	b2Vec2 m_size;

	b2ChainShape __shape;
	b2FixtureDef __fixtureDef;
	b2BodyDef __bodyDef;
};