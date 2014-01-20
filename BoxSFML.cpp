// BoxSFML.cpp : Defines the entry point for the console application.
#include "Sims\RaceCar\RaceSim.h"
#include "sGraphics\sSimulationWindow.h"
#include "sNeuralNet\sNeuralNet.h"


int main()
{
	
	/*
	int runs = 20;
	while(runs--){
		sGenome genome;
		sNeuralNet neuralNet;
		neuralNet.create(genome);

		printf("=================================================\n\n");

		for(float i = -3.f; i < 3.f; i += 0.1f){

			neuralNet.inputs[0].value = i;
			neuralNet.run();
			printf("%f -> %f \n", i, neuralNet.outputs[0].activation());

		}
	}*/

	

	srand(2);

	RaceSim simulation;
	sSimulationWindow window;
	

	simulation.elites = 0;
	simulation.mutationRate = 0.001f;
	simulation.selectionBias = 1.5f;
	simulation.breadingPoolFraction = 0.5;

	simulation.maxRoughness = 15;
	simulation.minRoughness = 15;
	//simulation
	simulation.randomizeEnvironment = true;
	
	window.setSimulation(&simulation);

	window.start();
	
	return 0;

}

