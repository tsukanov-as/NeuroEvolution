#include "Sims\RaceCar\Car.h"
#include "Sims\RaceCar\Worm.h"
#include "Sims\RaceCar\RaceSimulation.h"
#include "sGraphics\sSimulationWindow.h"




void runWormSimulation()
{
	sRandom::seed(1241455);


	RaceSimulation<Worm> simulation;
	sSimulationWindow window;


	// Population and selection settings
	simulation.populationSize = 50;
	simulation.elites = 0;
	simulation.mutationRate = 0.007f;
	simulation.selectionBias = 2.f;
	simulation.breadingPoolFraction = 0.9f;


	// Ground
	simulation.worldOffset_y = 0.5f;
	simulation.maxRoughness = 0.1f;
	simulation.minRoughness = 0.1f;
	simulation.randomizeEnvironment = false;
	simulation.worldWidth = 50;

	// Display
	simulation.renderScale = 60;
	window.setSimulation(&simulation);
	window.start();

}


void runCarSimulation()
{
	sRandom::seed(12455);


	RaceSimulation<Car> simulation;
	sSimulationWindow window;


	Car::bodyType = Car::BodyType::Fixed;


	simulation.populationSize = 90;
	simulation.elites = 1;
	simulation.mutationRate = 0.005f;
	simulation.selectionBias = 2.f;
	simulation.breadingPoolFraction = 0.8f;

	simulation.maxRoughness = 20;
	simulation.minRoughness = 20;
	simulation.randomizeEnvironment = true;
	

	window.setSimulation(&simulation);

	window.start();

}

int main()
{
	
	runWormSimulation();
	runCarSimulation();
	
	return 0;

}