/*
 * Misc_Random.hpp
 *
 * Revision: October 2018
 * Author: Thassyo Pinto - thassyo@ieee.org
 *
 * A collection of functions for creating random doubles.
 */

#ifndef MISC_RANDOM_HPP_
#define MISC_RANDOM_HPP_

// Standard libraries
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>

/**
 * Seeds the random number generator with the indicated seed.
 */
inline void seed(unsigned int seed){
#ifdef DEBUG
    std::cout << "Seed set: " << seed << std::endl;
#endif
    srand(seed);
}

/**
 * Seeds the random number generator based on the current time.
 */
inline void seed(){
    time_t current_time = time(NULL);
#ifdef DEBUG
    std::cout << "Seeded by time " << current_time << std::endl;
#endif
    srand(current_time);
}

/**
 * Returns a random double in the interval [0, 1].
 *
 * @return A random double in the range [0, 1].
 */
inline double randDouble(){
    double random = (double)rand()/(double)RAND_MAX;
#ifdef DEBUG
    std::cout << "randDouble: " << random << std::endl;
#endif
    return random;
}

/**
 * Returns a random double in the interval [0, max].
 *
 * @param max A double indicating the maximum possible value for the randomly generated number.
 * @return A random double in the range [0, max].
 */
inline double randDouble(double max){
    double random = randDouble() * max;
#ifdef DEBUG
    std::cout << "randDouble(max): " << random << std::endl;
#endif
    return random;
}

/**
 * Returns a random double in the interval [min, max].
 *
 * @param min A double indicating the minimum possible value for the randomly generated number.
 * @param max A double indicating the maximum possible value for the randomly generated number.
 * @return A random double in the range [min, max].
 */
inline double randDouble(double min, double max){
    double random = randDouble() * (max - min) + min;
#ifdef DEBUG
    std::cout << "randDouble(min,max): " << random << std::endl;
#endif
    return random;
}

/**
 * Returns a random integer in the interval [min, max].
 *
 * @param min An integer indicating the minimum possible value for the randomly generated number.
 * @param max An integer indicating the maximum possible value for the randomly generated number.
 * @return A random integer in the range [min, max].
 */
inline int randInt(int min, int max){
    int randomIndex = int(randDouble(min, max));
    while(randomIndex >= max){
        randomIndex = int(randDouble(min, max));
    }
#ifdef DEBUG
    std::cout << "randInt: " << randomIndex << std::endl;
#endif
    return randomIndex;
}

/**
 * Returns a random integer in the interval [0, max].
 *
 * @param max An integer indicating the maximum possible value for the randomly generated number.
 * @return A random integer in the range [0, max].
 */
inline int randInt(int max){
    return randInt(0, max);
}

/**
 * Gaussian random function to produce a real number according to a Gaussian distribution.
 *
 * Original implementation came from the sferes2 framework:
 * https://github.com/jbmouret/sferes2
 *
 * @param m The median of the Gaussian distribution.
 * @param v The deviation (the default is 1, the standard deviation).
 */
inline double randGaussian(double m=0.0,double v=1.0)
{
    double factor = sqrt(-2.0f * log(randDouble()));
    float angle  = 2.0f * M_PI * randDouble();
    double random =  double(m + v * factor * cos(angle));
#ifdef DEBUG
    std::cout << "randGaussian: " << random << std::endl;
#endif
    return random;
}

/**
 * Generates a random index in the range [min, max].
 *
 * When generating a double in [0, max], there exists some non-zero
 * probability that the number will be exactly max.
 *
 * Another way to solve this issue is to generate number with
 * (double)rand()/((double)RAND_MAX-1);
 * but there are some reports that this can still generate a 1.0,
 * even though it should not.
 *
 * Thus I chose to go with the while loop.
 *
 * @param min An unsigned integer indicating the minimum possible value for the randomly generated number.
 * @param max An unsigned integer the maximum possible value for the randomly generated number.
 * @return An unsigned integer in the range [min, max].
 */
inline size_t randIndex(size_t min, size_t max){
    size_t randomIndex = size_t(randDouble(min, max));
    while(randomIndex >= max){
        randomIndex = size_t(randDouble(min, max));
    }
#ifdef DEBUG
    std::cout << "randIndex: " << randomIndex << std::endl;
#endif
    return randomIndex;
}

/**
 * Generates a random index in the range [0, max].
 *
 * Overloaded version of randIndex.
 *
 * @param max An unsigned integer the maximum possible value for the randomly generated number.
 * @return An unsigned integer in the range [0, max].
 */
inline size_t randIndex(size_t max){
    return randIndex(0, max);
}

#endif /* MISC_RANDOM_HPP_ */
