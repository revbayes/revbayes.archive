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


#ifndef TestGtrModelFixedTree_H
#define TestGtrModelFixedTree_H

#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class Tree;
    class AbstractCharacterData;

    class TestGtrModelFixedTree {
    
    public:
        TestGtrModelFixedTree(const std::string &alignFile, const std::string &treeFile, int gen);
        virtual                         ~TestGtrModelFixedTree(void);                                                            //!< Virtual destructor
    
        bool                            run();
    
    private:
    
        // members
        std::string                     alignmentFilename;
        std::string                     treeFilename;
        int                             mcmcGenerations;

    };
    
}

#endif
