/**
 * @file
 * This file contains the declaration of the performance test of the specific versus generic char-evo-model classes. 
 * This test runs an MCMC analysis on a phylogenetic model using the GTR subsitution model.
 * The data is read as an alignment from file.
 *
 *
 * @brief Declaration of the performance test of the char-evo model test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-05
 *
 * $Id$
 */


#ifndef TestCharEvoModelImplementationPerformance_H
#define TestCharEvoModelImplementationPerformance_H

#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class Tree;
    class AbstractCharacterData;
    
    class TestCharEvoModelImplementationPerformance {
        
    public:
        TestCharEvoModelImplementationPerformance(const std::string &alignFile, const std::string &tFn, int gen);
        virtual                                ~TestCharEvoModelImplementationPerformance(void);                                                            //!< Virtual destructor
        
        bool                                    run();
        
    private:
        
        // members
        std::string                             alignmentFilename;
        std::string                             treeFilename;
        int                                     mcmcGenerations;
        
    };
    
}

#endif
