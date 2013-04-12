/**
 * @file
 * This file contains the declaration of the Uncorrelated LogNormal relaxed clock model with different T92 substitution matrices for different branches of the tree. 
 * This test runs an MCMC analysis on a phylogenetic model using the UCLN Relaxed Clock with Branch Heterogeneous T92 substitution matrices.
 * The data is read as an alignment from file. The tree is estimated as well!
 *
 *
 * @brief Declaration of the UCLN Relaxed Clock + BH T92 model test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-05
 *
 * $Id$
 */


#ifndef TestUCLNRelaxedClockBHT92Model_H
#define TestUCLNRelaxedClockBHT92Model_H

#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class Tree;
    class AbstractCharacterData;
    
    class TestUCLNRelaxedClockBHT92Model {
        
    public:
        TestUCLNRelaxedClockBHT92Model(const std::string &alignFile, const std::string &treeFn, int gen);
        virtual                                ~TestUCLNRelaxedClockBHT92Model(void);                                                            //!< Virtual destructor
        
        bool                                    run();
        
    private:
        
        // members
        std::string                             alignmentFilename;
        std::string                             treeFilename;
        int                                     mcmcGenerations;
        
    };
    
}

#endif
