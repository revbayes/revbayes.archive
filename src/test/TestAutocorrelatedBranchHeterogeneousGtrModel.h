
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


#ifndef TestAutocorrelatedBranchHeterogeneousGtrModel_H
#define TestAutocorrelatedBranchHeterogeneousGtrModel_H

#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class Tree;
    class AbstractCharacterData;
    
    class TestAutocorrelatedBranchHeterogeneousGtrModel {
        
    public:
        TestAutocorrelatedBranchHeterogeneousGtrModel(const std::string &alignFile, const std::string &treeFn, int gen);
        virtual                                ~TestAutocorrelatedBranchHeterogeneousGtrModel(void);                                                            //!< Virtual destructor
        
        bool                                    run();
        
    private:
        
        // members
        std::string                             alignmentFilename;
        std::string                             treeFilename;
        int                                     mcmcGenerations;
        
    };
    
}

#endif
