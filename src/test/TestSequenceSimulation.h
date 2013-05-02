/**
 * @file
 * This file contains the declaration of the Gtr model test class. 
 * This test runs an MCMC analysis on a phylogenetic model using the GTR subsitution model for a fixed tree.
 * The data is read as an alignment from file.
 *
 *
 * @brief Declaration of the Gtr model test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-05
 *
 * $Id$
 */


#ifndef TestSequenceSimulation_H
#define TestSequenceSimulation_H

#include <string>

namespace RevBayesCore {
        
    class TestSequenceSimulation {
        
    public:
        TestSequenceSimulation(const std::string &treeFile);
        virtual                         ~TestSequenceSimulation(void);                                                            //!< Virtual destructor
        
        bool                            run();
        
    private:
        
        // members
        std::string                     treeFilename;
        
    };
    
}

#endif
