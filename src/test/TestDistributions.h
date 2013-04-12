/**
 * @file
 * This file contains the declaration of the distributions test class. 
 * We test the pdf, cdf, quantile and rv.
 *
 *
 * @brief Declaration of the distributions test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-05
 *
 * $Id$
 */


#ifndef TestDistributions_H
#define TestDistributions_H

#include "RbFileManager.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class TestDistributions {
        
    public:
        TestDistributions();
        virtual                                ~TestDistributions(void);                                                            //!< Virtual destructor
        
        bool                                    run();
                
        
    };
    
}

#endif
