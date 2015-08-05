/**
 * @file
 * This file contains the declaration of the Gtr+Gamma model test class. 
 * This test the likelihood on a phylogenetic model using the GTR subsitution model with rate heterogenity per site.
 * The data is read as an alignment from file.
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


#ifndef TestFilteredStandardLikelihood_H
#define TestFilteredStandardLikelihood_H

#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class Tree;
    class AbstractCharacterData;
    
    class TestFilteredStandardLikelihood {
        
    public:
        TestFilteredStandardLikelihood(const std::string &alignFile, const std::string &tFn);
        virtual                                ~TestFilteredStandardLikelihood(void);                                                            //!< Virtual destructor
        
        bool                                    run();
        
    private:
        
        // members
        std::string                             alignmentFilename;
        std::string                             treeFilename;
        
    };
    
}

#endif
