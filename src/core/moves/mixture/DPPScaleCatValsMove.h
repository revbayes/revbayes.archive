/**
 * @file
 * This file contains the declaration of DPPScaleCatValsMove, which performs the DPP move based on Neal (2000) Algorithm 8
 * this move changes the value assigned to each table
 *
 * @brief Declaration of DPPScaleCatValsMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-20, version 1.0
 *
 * $Id: DPPScaleCatValsMove.h $
 */

#ifndef DPPScaleCatValsMove_H
#define DPPScaleCatValsMove_H

#include <ostream>

#include "MoveOld.h"
#include "RbVector.h"
#include "StochasticNode.h"

namespace RevBayesCore {

    class DPPScaleCatValsMove : public MoveOld {
    
    public:
        DPPScaleCatValsMove(StochasticNode< RbVector<double> >* v, double l, double w);                                                                      //!< Internal constructor
    
        // Basic utility functions
        DPPScaleCatValsMove*									clone(void) const;                                                                  //!< Clone object
        void                                                    swapNode(DagNode *oldN, DagNode *newN);
        const std::string&                                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
		bool													isGibbs(void) const;

    protected:
        void													performGibbsMove(void);                                                            //!< Perform move
        void													acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double													performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void													rejectMove(void);                                                                   //!< Reject the InferenceMoveSimple
    
    private:
        double													getCurrentLnProbabilityForMove(void);
		
		double													lambda;                                                                             //!< The scaling parameter of the move  
        StochasticNode< RbVector<double> >*                     variable;
		double													safeExponentiation(double x);
 
    };
    
}



#endif

