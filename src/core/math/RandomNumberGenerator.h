/**
 * @file
 * This file contains the declaration of RandomNumberGenerator, which is
 * used to generate uniform(0,1) random variables.
 *
 * @brief Declaration of RandomNumberGenerator
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id:$
 */

#ifndef RandomNumberGenerator_H
#define RandomNumberGenerator_H

#include <vector>

class RandomNumberGenerator {

    public:

		                            RandomNumberGenerator(void);                          //!< Default constructor using time seed
    		                        RandomNumberGenerator(std::vector<unsigned int> s);   //!< Constructor explicitly setting seeds

        // Regular functions
        void						setSeed(std::vector<unsigned int> s);                 //!< Set the seeds of the RNG
		double                      uniform01(void);                                      //!< Get a random [0,1) var

	private:
		std::vector<unsigned int>	seed;											      //!< Vector of two seed values
};

#endif

