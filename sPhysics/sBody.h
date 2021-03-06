#pragma once
#include <vector>
#include "sObject.h"
//#include "sWorld.h"

using std::vector;



class sWorld;
class sBody;

struct sBodyState
{
	float32 angle;
	float32 angularVelocity;
	b2Vec2 position;
	b2Vec2 linearVelocity;
	int32 body_id;
	void *userData;

	sBodyState()
	{
		zero();
	}

	void zero()
	{
		angle = 0;
		angularVelocity = 0;
		position.Set(0,0);
		linearVelocity.Set(0,0);
	}

	sBodyState interpolate(sBodyState state, float32 t)
	{
		sBodyState result;
		result.angle = angle + (state.angle - angle) * t;
		result.angularVelocity = angularVelocity + (state.angularVelocity - angularVelocity) * t;
		result.position = position + t * (state.position - position);
		result.linearVelocity = linearVelocity + t * (state.linearVelocity - linearVelocity);
		return result;
	}

	void copy(sBodyState &state)
	{
		angle = state.angle;
		angularVelocity = state.angularVelocity;
		position = state.position;
		linearVelocity = state.linearVelocity;
	}

};

struct sRayCastOutput
{
	bool found;
	b2Vec2 point;
	float fraction;
	b2Vec2 normal;
	sBody *body;
	b2Fixture *fixture;
};

// Body types, matched to box2d types for convenience
enum sBodyType
{
	STATIC_BODY = b2_staticBody,
	DYNAMIC_BODY = b2_dynamicBody,
	KINEMATIC_BODY = b2_kinematicBody
};

class sBody : public sObject
{

public:

	sBody() : sObject(BODY_OBJECT)
	{
		m_density = 1;
		m_bodyType = DYNAMIC_BODY;
		m_state.angle = 0;
		m_state.angularVelocity = 0;
		m_state.position = b2Vec2(0,0);
		m_state.linearVelocity = b2Vec2(0,0);
		m_linearDamping = 0.1f;
		m_angularDamping = 0.1f;
	}
	sBody(sBody &body) : sObject(body)
	{
		_copy(body);
	}
	~sBody()
	{
		if(m_inWorld){
			removeFromWorld(*m_world);
		}
	}
	void copy(const sBody &body)
	{
		sObject::copy(body);
		_copy(body);
	}



	//========================================================================================
	// Getters & Setters
	//========================================================================================


	//-------------------------------------------------------------------------------
	// Type

	void setType(sBodyType type)
	{
		m_bodyType = type;

		// TODO: needs testing to see if it actually works
		if(m_inWorld)m_body->SetType(b2BodyType(type));
	}
	sBodyType getType()
	{
		return m_bodyType;
	}


	//-------------------------------------------------------------------------------
	// State

	// TODO: possibly change this so that deriving classes can use derived states
	sBodyState getState() const
	{
		if(m_inWorld){
			sBodyState state;
			state.position = m_body->GetPosition();
			state.angle = m_body->GetAngle();
			state.angularVelocity = m_body->GetAngularVelocity();
			state.linearVelocity = m_body->GetLinearVelocity();
			return state;
		}
		return m_state;
	}
	void setState(sBodyState state)
	{
		if(m_inWorld){
			m_body->SetTransform(state.position, state.angle);
			m_body->SetLinearVelocity(state.linearVelocity);
			m_body->SetAngularVelocity(state.angularVelocity);
		} else {
			m_state = state;
		}
	}
	void zeroState()
	{
		sBodyState z;
		setState(z);
	}


	//-------------------------------------------------------------------------------
	// Position

	void setPosition(b2Vec2 position)
	{
		if(!m_inWorld){
			m_state.position = position;
		} else {
			m_body->SetTransform(position, m_body->GetAngle());
		}
	}
	void setPosition(float32 x, float32 y)
	{
		setPosition(b2Vec2(x, y));
	}
	b2Vec2 getPosition()
	{
		return m_inWorld ? m_body->GetPosition() : m_state.position;
	}


	//-------------------------------------------------------------------------------
	// Angle

	void setAngle(float32 angle)
	{
		if(!m_inWorld){
			m_state.angle = angle;
		} else {
			m_body->SetTransform(m_body->GetPosition(), angle);
		}
	}
	float32 getAngle()
	{
		return m_inWorld ? m_body->GetAngle() : m_state.angle;
	}


	//-------------------------------------------------------------------------------
	// Velocity

	void setLinearVelocity(b2Vec2 velocity)
	{
		if(!m_inWorld){
			m_state.linearVelocity = velocity;
		} else {
			m_body->SetLinearVelocity(velocity);
		}
	}
	b2Vec2 getLinearVelocity()
	{
		return m_inWorld ? m_body->GetLinearVelocity() : m_state.linearVelocity;
	}


	//-------------------------------------------------------------------------------
	// Angular Velocity

	void setAngularVelocity(float32 velocity)
	{
		if(!m_inWorld){
			m_state.angularVelocity = velocity;
		} else {
			m_body->SetAngularVelocity(velocity);
		}
	}
	float32 getAngularVelocity()
	{
		return m_inWorld ? m_body->GetAngularVelocity(): m_state.angularVelocity;
	}


	//-------------------------------------------------------------------------------
	// Linear Damping

	void setLinearDamping(float32 damping)
	{
		m_linearDamping = damping;
		if(m_inWorld)m_body->SetLinearDamping(damping);
	}
	float32 getLinearDamping()
	{
		return m_linearDamping;
	}

	//-------------------------------------------------------------------------------
	// Angular Damping

	void setAngularDamping(float32 damping)
	{
		m_angularDamping = damping;
		if(m_inWorld)m_body->SetAngularDamping(damping);
	}
	float32 getAngularDamping()
	{
		return m_angularDamping;
	}

	//-------------------------------------------------------------------------------
	// Forces

	void applyForce(const b2Vec2 &force, const b2Vec2 &point)
	{
		if(m_inWorld){
			m_body->ApplyForce(force, point, true);
		}
	}
	void applyForceToCenter(const b2Vec2 &force)
	{
		if(m_inWorld){
			m_body->ApplyForceToCenter(force, true);
		}
	}
	void applyTorque(float torque)
	{
		if(m_inWorld){
			m_body->ApplyTorque(torque, true);
		}
	}

	
	//-------------------------------------------------------------------------------
	// Impulses

	void applyImpulse(const b2Vec2 &force, const b2Vec2 &point)
	{
		if(m_inWorld){
			m_body->ApplyLinearImpulse(force, point, true);
		}
	}

	void applyAngularImpulse(float angularImpulse)
	{
		if(m_inWorld){
			m_body->ApplyAngularImpulse(angularImpulse, true);
		}
	}


	b2Vec2 GetLinearVelocityFromWorldPoint(const b2Vec2 &point)
	{
		if(m_inWorld){
			return m_body->GetLinearVelocityFromWorldPoint(point);
		} else {
			return b2Vec2_zero;
		}
	}

	//-------------------------------------------------------------------------------
	// Awake

	bool isAwake()
	{
		if(!m_inWorld) return false;
		return m_body->IsAwake();
	}
	void setAwake(bool awake)
	{
		if(!m_inWorld) return;
		m_body->SetAwake(awake);
	}

	//-------------------------------------------------------------------------------
	// Bounding Box

	b2AABB getAABB()
	{
		b2AABB aabb;
		aabb.lowerBound.Set(100000,100000);
		aabb.upperBound.Set(-100000,-100000);

		// TODO: Allow out of world check?
		if(!m_inWorld) return aabb;
		b2Fixture* fixture = m_body->GetFixtureList();
		while (fixture != NULL)
		{
			aabb.Combine(aabb, fixture->GetAABB(0));
			fixture = fixture->GetNext();
		}
		return aabb;
	}

	//-------------------------------------------------------------------------------
	// RayCast

	sRayCastOutput rayCast(b2Vec2 v1, b2Vec2 v2, float maxFraction = 1)
	{
		b2RayCastOutput b2_output;
		b2RayCastInput b2_input;
		sRayCastOutput output;
		output.found = false;

		b2_input.maxFraction = maxFraction;
		b2_input.p1 = v1;
		b2_input.p2 = v2;

		b2Fixture* fixture = m_body->GetFixtureList();
		while (fixture != NULL)
		{
			//fixture->
			int l = fixture->GetShape()->GetChildCount();
			//for(int i = 0; i < l; i++){
				if(fixture->RayCast(&b2_output, b2_input, 0)){
					if(b2_input.maxFraction > b2_output.fraction){
						b2_input.maxFraction = b2_output.fraction;
						output.fraction = b2_output.fraction;
						output.normal = b2_output.normal;
						output.found = true;
						output.fixture = fixture;
						output.body = this;
						output.point = v1 + output.fraction * (v2 - v1);
					}
				}
			//}
			fixture = fixture->GetNext();
		}
		return output;
	}


	//========================================================================================

	b2Body *m_body;  // temp public
	//string userString;

protected:
	

	virtual void addToWorld(sWorld &world);
	virtual void removeFromWorld(sWorld &world);

	sBodyType m_bodyType;
	float32 m_density;
	sBodyState m_state;

	float32 m_linearDamping;
	float32 m_angularDamping;

	// Body definition pointer, must be set by deriving class
	// This is pointer, so that many instances can share the same definition
	b2BodyDef *m_bodyDef;               
	vector<b2FixtureDef*> m_fixtureDefs;
	vector<b2Fixture*> m_fixtures;

private :

	void _copy(const sBody &body)
	{
		m_density = body.m_density;
		m_bodyType = body.m_bodyType;
		m_fixtureDefs = body.m_fixtureDefs;
		m_bodyDef = body.m_bodyDef;
		m_angularDamping = body.m_angularDamping;
		m_linearDamping = body.m_linearDamping;
		setState(body.getState());
	}
	
};