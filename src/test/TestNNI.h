/**
 * @file
 * This file contains the declaration of the Nearest-Neighbor-Interchange (NNI) test class. 
 * This test runs an MCMC analysis on a birth-death tree withour any data. Hence, we run the model under the prior
 * and should get exactly this tree distribution.
 *
 * @brief Declaration of the NNI test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-05
 *
 * $Id:$
 */


#ifndef TestNNI_H
#define TestNNI_H

#include "BranchLengthTree.h"
#include "RbFileManager.h"
#include "TreeTrace.h"

#include <string>
#include <vector>


namespace RevBayesCore {
    
    class Tree;

    class TestNNI {
    
    public:
        TestNNI(int gen);
        virtual                         ~TestNNI(void);                                                            //!< Virtual destructor
    
        bool                            run();
    
    private:
        TreeTrace<TimeTree>             readTreeTrace(const std::string &fname);
    
        int                             mcmcGenerations;

    };
    
}

#endif
