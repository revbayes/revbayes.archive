/**
 * @file
 * This file contains the declaration of InferenceMoveSimple, which is the abstract
 * base class for InferenceMoveSimples used in mcmc inference.
 *
 * Simple moves are moves that work only on a single DAG node, e.g. on scalers. 
 * However, the DAG node might also contain a vector of values.
 *
 * @brief Declaration of InferenceMoveSimple
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-23 18:42:52 +0100 (Fri, 23 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: InferenceMoveSimple.h 1358 2012-03-23 17:42:52Z hoehna $
 */

#ifndef InferenceMoveSimple_H
#define InferenceMoveSimple_H

#include "InferenceMove.h"

#include <vector>

class InferenceMoveSimple : public InferenceMove {
    
public:
    virtual                                 ~InferenceMoveSimple(void);                                                         //!< Destructor
    
    // functions you have to override
    virtual InferenceMoveSimple*            clone(void) const = 0;                                                              //!< Clone the MoveSimple to get an independent copy
        
protected:
    InferenceMoveSimple();                                                                                                      //!< Default constructor
        
    void                                    acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
    double                                  performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
    void                                    rejectMove(void);                                                                   //!< Reject the InferenceMoveSimple
    
    virtual double                          performSimpleMove(void) = 0;                                                        //!< Perform the InferenceMoveSimple
    virtual void                            rejectSimpleMove(void) = 0;
    
    bool                                    changed;
};

#endif
