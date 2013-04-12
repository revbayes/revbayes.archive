/**
 * @file
 * This file contains the declaration of the Bayes factor test class. 
 * We use stepping stone sampling and path sampling to approximate the marginal likelihood of a model.
 *
 *
 * @brief Declaration of the Bayes factor test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-05
 *
 * $Id$
 */


#ifndef TestBayesFactor_H
#define TestBayesFactor_H

#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class Tree;
    class AbstractCharacterData;
    
    class TestBayesFactor {
        
    public:
        TestBayesFactor(int gen);
        virtual                                ~TestBayesFactor(void);                                                            //!< Virtual destructor
        
        bool                                    run();
        
    private:
        
        int                                     mcmcGenerations;
        
    };
    
}

#endif
