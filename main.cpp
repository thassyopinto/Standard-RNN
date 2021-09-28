/*
 * Main.cpp
 *
 * Revision: November 2018
 * Author: Thassyo Pinto - thassyo@ieee.org
 *
 * This is the main file for the Standard RNN.
 */

// Standard libraries
#include <sstream>

// Local libraries
#include "RNN_NeuralNetwork.hpp"
#include "Misc_Random.hpp"

double VAL_MIN = 0.0;
double VAL_MAX = 1.0;
double WGT_MIN = -1.0;
double WGT_MAX = 1.0;
double MUT_WGT = 0.05;
double MUT_BIA = 0.05;
double MUT_ADD = 0.2;
double NUM_RUN = 50;

int main(int argc, char* argv[]) {

    seed();

    typedef NeuralNetwork<> rnn_t;

    rnn_t myNetwork(atoi(argv[1]), atoi(argv[2]));

    // Sets up the neural network.
    myNetwork.setMinWeight(WGT_MIN);
    myNetwork.setMaxWeight(WGT_MAX);
    myNetwork.setWeightMutRate(MUT_WGT);
    myNetwork.setNeuronMutRate(MUT_BIA);
    myNetwork.setAddNeuronMutRate(MUT_ADD);
    myNetwork.setAddConnectionMutRate(MUT_ADD);
    myNetwork.reset();
    myNetwork.initialize(randDouble(VAL_MIN, VAL_MAX), 0.0);
    myNetwork.randomize();

    // Saves the initial neural network.
    std::ofstream networkFileFirst("std_RNN_ini.csv");
    networkFileFirst << myNetwork << "\n";
    networkFileFirst.close();

    // Runs neural network for [x] timesteps.
    myNetwork.run(NUM_RUN, "std_RNN_activation.csv");

    // Saves the mutated neural network.
    std::ofstream networkFileLast("std_RNN_mut.csv");
    networkFileLast << myNetwork << "\n";
    networkFileLast.close();

    return 0;
}
