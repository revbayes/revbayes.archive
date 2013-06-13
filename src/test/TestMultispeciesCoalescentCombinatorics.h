/**
 * @file
 * This file contains the declaration of the time dependent Birth-Death model test class. 
 * This test runs an MCMC analysis on a birth-death model when we have the tree observed and want to estimate the 
 * diversification rates which are varying over time. We use a ln-Brownian motion for the rates function.
 *
 *
 * @brief Declaration of the B-D model test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-05
 *
 * $Id:$
 */


#ifndef TestMultispeciesCoalescentCombinatorics_H
#define TestMultispeciesCoalescentCombinatorics_H

namespace RevBayesCore {
    
    class TestMultispeciesCoalescentCombinatorics {
        
    public:
        TestMultispeciesCoalescentCombinatorics();
        virtual                         ~TestMultispeciesCoalescentCombinatorics(void);                                                            //!< Virtual destructor
        
        bool                            run();
                
    };
    
}

#endif
