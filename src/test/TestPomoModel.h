/**
 * @file
 * This file contains the declaration for the Pomo model test class.
 * This test runs an MCMC analysis on a phylogenetic model using the Pomo subsitution model.
 * The data is read as an alignment from file. The tree is estimated as well!
 *
 *
 * @brief Declaration of the Pomo model test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-05
 *
 * $Id$
 */


#ifndef TestGtrModel_H
#define TestGtrModel_H

#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class TestPomoModel {
        
    public:
        TestPomoModel(const std::string &alignFile, const std::string &treeFn, int gen);
        
        virtual ~TestPomoModel(void);                             //!< Virtual destructor
        
        bool                                    run();
        
    private:
        
        // members
        std::string                             alignmentFilename;
        std::string                             treeFilename;
        int                                     mcmcGenerations;
        
    };
    
}

#endif
