#pragma once
#include "../../sEvolution/sSimulation.h"
#include "Worm.h"
#include "../../sPhysics/sEdgeRectangle.h"
#include "../../sUtils/perlin.h"

class sSwarmFitness : public sFitnessFunction
{
public: 
	 b2Vec2 target;
	 b2Vec2 deathTarget;
	 float32 distance;
	 float operator()(sPhenotype *phenotype)
	{
		float distance_traveled = phenotype->getPosition().Length();
		float distance_target = (phenotype->getPosition() - target).Length();
		float distance_death = (phenotype->getPosition() - deathTarget).Length();
		return distance - distance_target + distance_traveled * 0.5;
		//return distance_death - distance_target;
	}
};


class SwarmSimulation : public sSimulation
{

public:

	SwarmSimulation()
	{

		ground.setSize(120,70);

		gravity.Set(0,0);
		worlds[0].setGroundBody(&ground);
		worlds[0].add(&ground);

		simulationTime = 100000;

		staticView = true;

		targetDistance = 30;

		target.setIsSensor(true);
		target.setRadius(1);
		target.setLinearDamping(3);
		worlds[0].add(&target);

		deathTarget.setIsSensor(true);
		deathTarget.setRadius(1);
		deathTarget.setLinearDamping(3);
		deathTarget.setCustomColor(b2Color(1,0,0));
		//world.add(&deathTarget);
	}

	~SwarmSimulation()
	{
		for(int i = 0; i < populationSize; i++){
			delete population[i];
		}
	}

	int populationSize;
	float simulationTime;
	float targetDistance;

	sSwarmFitness fitnessFunction;
	sCircle target, deathTarget;

protected:

	void initPhenotypes()
	{


		for(int i = 0; i < populationSize; i++){
			Worm *worm = new Worm;

			worm->bodySense = true;
			worm->directionSense = true;
			worm->fitnessFunction = &fitnessFunction;
			worm->progressTimeout = 3000;
			worm->progressAmount = 0.03f;
			//worm->maxNeuronBias = 0;
			//worm->maxSynapseWeight = 3;
			worm->muscleSpeed = 0.1;
			worm->muscleTorque = 1;
			worm->lateralDamping = 0.8;
			worm->directionTargets.resize(1);
			//worm->additionNeuralInputs.resize(1);
			worm->killOnLackOffProgress = true;

			addPhenotype(worm);
		}
	}


	// Step fucntion
	// Return whether the simulation is finished
	// In this case it's when all phenotypes are no longer active/alive
	bool isFinished()
	{

		if(worlds[0].getBodiesAt(deathTarget.getPosition()).size() > 1){
			for(int i = 0; i < populationSize; i++){

			}
		}
		if(worlds[0].getBodiesAt(target.getPosition()).size() > 1){
			return true;
		}



		fitnessFunction.target = target.getPosition();
		for(int i = 0; i < population.size(); i++){
			((Worm*)population[i])->directionTargets[0] = target.getPosition();
			//((Worm*)population[i])->directionTargets[1] = deathTarget.getPosition();
		}



		for(int i = 0; i < population.size(); i++){
			if(population[i]->alive) return false;
		}

		return true;
	}

	// Build / Rebuild environment
	// Builds the ground based on roughness inputs
	void buildEnvironment()
	{
		float da = population.getGenerationCount() / 20.f + 2.f;
		da = da > b2_pi ? b2_pi : da;
		//float da = b2_pi;
		b2Vec2 p(targetDistance, 0);
		b2Rot a(sRandom::getFloat(-da, da));
		p = b2Mul(a, p);
		p.y *= 0.75;
		target.setPosition(p);
		fitnessFunction.target = p;

		/*
		da = population.getGenerationCount() / 10.f + 0.7f;
		p.Set(targetDistance, 0);
		a.Set(sRandom::getFloat(-da, da) + b2_pi);
		p = b2Mul(a, p);
		deathTarget.setPosition(p);
		fitnessFunction.deathTarget = p;
		*/

		fitnessFunction.distance = targetDistance;


		for(int i = 0; i < populationSize; i++){
			((Worm*)population[i])->directionTargets[0] = target.getPosition();
			//((Worm*)population[i])->directionTargets[1] = deathTarget.getPosition();
		}

	}


private:

	sEdgeRectangle ground;
};