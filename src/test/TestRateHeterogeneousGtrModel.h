/**
 * @file
 * This file contains the declaration of the Gtr model test class. 
 * This test runs an MCMC analysis on a phylogenetic model using the GTR subsitution model.
 * The data is read as an alignment from file. The tree is estimated as well!
 *
 *
 * @brief Declaration of the Gtr+G model test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-05
 *
 * $Id$
 */


#ifndef TestRateHeterogeneousGtrModel_H
#define TestRateHeterogeneousGtrModel_H

#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class Tree;
    class AbstractCharacterData;
    
    class TestRateHeterogeneousGtrModel {
        
    public:
        TestRateHeterogeneousGtrModel(const std::string &alignFile, const std::string &treeFn, int gen);
        virtual                                ~TestRateHeterogeneousGtrModel(void);                                                            //!< Virtual destructor
        
        bool                                    run();
        
    private:
        
        // members
        std::string                             alignmentFilename;
        std::string                             treeFilename;
        int                                     mcmcGenerations;
        
    };
    
}

#endif
