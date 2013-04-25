/**
 * @file
 * This file contains the declaration of the transition probability test class. 
 * This test computes transition probabilities under different model (JC,F81,HKY,GTR) and compares/checks the results.
 *
 *
 * @brief Declaration of the transition probability test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-05
 *
 * $Id:$
 */


#ifndef TestTransitionProbabilities_H
#define TestTransitionProbabilities_H

namespace RevBayesCore {
    
    class TestTransitionProbabilities {
        
    public:
        TestTransitionProbabilities(void);
        virtual                         ~TestTransitionProbabilities(void);                                                            //!< Virtual destructor
        
        bool                            run();
        
    private:    
        // members
        
    };
    
}

#endif
