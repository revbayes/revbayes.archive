/**
 * @file
 * This file contains the declaration of the Uncorrelated Lognormal Relaxed Clock model test class. 
 * 
 *
 *
 * @brief Declaration of the ucln model test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-05
 *
 * $Id:$
 */


#ifndef TestUCLNRelaxedClock_H
#define TestUCLNRelaxedClock_H

#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class TimeTree;
    class AbstractCharacterData;

    class TestUCLNRelaxedClock {
		
    public:
        TestUCLNRelaxedClock(const std::string &aFn, const std::string &tFn, int gen);
        virtual                         ~TestUCLNRelaxedClock(void);                                                            //!< Virtual destructor
		
        bool                            run();
		
    private:    
        // members
		std::string                     alignmentFilename;
		std::string                     treeFilename;
        int                             mcmcGenerations;
		
    };
	
}

#endif
