/**
 * @file
 * This file contains the declaration of MoveCompound, which is the abstract
 * base class for MoveCompounds used in mcmc inference.
 *
 * Compound moves are moves that work only on a single DAG node, e.g. on scalers. 
 * However, the DAG node might also contain a vector of values.
 *
 * @brief Declaration of MoveCompound
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

#ifndef CompoundMove_H
#define CompoundMove_H

#include "Move.h"

#include <vector>

namespace RevBayesCore {

    class CompoundMove : public Move {
    
    public:
        virtual                                 ~CompoundMove(void);                                                         //!< Destructor
    
        // functions you have to override
        virtual CompoundMove*                     clone(void) const = 0;                                                              //!< Clone the MoveCompound to get an independent copy
    
        // methods you may want to overwrite
        virtual void                            swapNode(DagNode *oldN, DagNode *newN);

    protected:
        CompoundMove(std::vector<DagNode*> n, double w, bool autoTune = false);                                                                    //!< constructor
        
        void                                    acceptMove(void);                                                                   //!< Accept the InferenceMoveCompound
        double                                  performMove(double& probRatio);                                                     //!< Perform the InferenceMoveCompound
        void                                    rejectMove(void);                                                                   //!< Reject the InferenceMoveCompound
    
        virtual double                          performCompoundMove(void) = 0;                                                        //!< Perform the InferenceMoveCompound
        virtual void                            rejectCompoundMove(void) = 0;
        virtual void                            acceptCompoundMove(void); // non-pure virtual accept function
    
        bool                                    changed;
        std::vector<DagNode*>                   theNodes;

    };
    
}

#endif
