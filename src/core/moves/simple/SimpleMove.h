/**
 * @file
 * This file contains the declaration of MoveSimple, which is the abstract
 * base class for MoveSimples used in mcmc inference.
 *
 * Simple moves are moves that work only on a single DAG node, e.g. on scalers. 
 * However, the DAG node might also contain a vector of values.
 *
 * @brief Declaration of MoveSimple
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-23 18:42:52 +0100 (Fri, 23 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-19, version 1.0
 *
 * $Id$
 */

#ifndef SimpleMove_H
#define SimpleMove_H

#include "Move.h"

#include <vector>

namespace RevBayesCore {

    class SimpleMove : public Move {
    
    public:
        virtual                                 ~SimpleMove(void);                                                         //!< Destructor
    
        // functions you have to override
        virtual SimpleMove*                     clone(void) const = 0;                                                              //!< Clone the MoveSimple to get an independent copy
    
        // methods you may want to overwrite
        virtual void                            swapNode(DagNode *oldN, DagNode *newN);

    protected:
        SimpleMove(DagNode *n, double w, bool autoTune = false);                                                                    //!< constructor
        
        void                                    acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double                                  performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void                                    rejectMove(void);                                                                   //!< Reject the InferenceMoveSimple
    
        virtual double                          performSimpleMove(void) = 0;                                                        //!< Perform the InferenceMoveSimple
        virtual void                            rejectSimpleMove(void) = 0;
        virtual void                            acceptSimpleMove(void); // non-pure virtual accept function
    
        bool                                    changed;
        DagNode*                                theNode;

    };
    
}

#endif
