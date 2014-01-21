#pragma once
#include "../sPhysics/sContainer.h"
#include "sGenome.h"

// Virtual class

class sPhenotype : public sContainer, public sStepListener
{
public:

	// Derived phenotypes should implement these
	virtual void build(sWorld &world) = 0;
	virtual void destroy(sWorld &world) = 0;
	virtual float getFitness() = 0;
	virtual void step() = 0;
	virtual b2Vec2 getPosition() = 0;
	virtual b2Vec2 getVelocity() = 0;
	
	sGenome genome;
	int lifeTime;
	bool alive;

	void born(sWorld &world)
	{
		lifeTime = 0;
		alive = true;
		m_isLeader = false;
		build(world);
	}

	void die()
	{
		destroy(*m_world);
		alive = false;
	}

	virtual void onBeforeStep()
	{
		if(alive){
			step();
			lifeTime++;
		}
	}
	virtual void onAfterStep(){};

	void setIsLeader(bool leader)
	{
		m_isLeader = leader;
	}
	bool isLeader()
	{
		return m_isLeader;
	}

	void setIsElite(bool elite)
	{
		m_isElite = elite;
	}
	bool isElite()
	{
		return m_isElite;
	}

protected:

	bool m_isLeader;
	bool m_isElite;

	virtual void addToWorld(sWorld &world)
	{
		born(world);
		sContainer::addToWorld(world);
		world.addStepListener(*this);
	}

	virtual void removeFromWorld(sWorld &world)
	{
		world.removeStepListener(*this);
		sContainer::removeFromWorld(world);
	}

};