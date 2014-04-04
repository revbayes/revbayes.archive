/**
 * @file
 * This file contains the declaration of DPPAllocateAuxGibbsMove, which performs the DPP move based on Neal (2000) Algorithm 8
 *
 * @brief Declaration of DPPAllocateAuxGibbsMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-20, version 1.0
 *
 * $Id: DPPAllocateAuxGibbsMove.h $
 */

#ifndef DPPAllocateAuxGibbsMove_H
#define DPPAllocateAuxGibbsMove_H

#include <ostream>

#include "DirichletProcessPriorDistribution.h"
#include "SimpleMove.h"

namespace RevBayesCore {

    template <class valueType>
    class DPPAllocateAuxGibbsMove : public Move {
    
    public:
        DPPAllocateAuxGibbsMove(StochasticNode< std::vector<valueType> >* v, int na, double w);                                                                      //!< Internal constructor
    
        // Basic utility functions
        DPPAllocateAuxGibbsMove<valueType>*						clone(void) const;                                                                  //!< Clone object
        void                                                    swapNode(DagNode *oldN, DagNode *newN);
		const std::string&                                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing

    protected:
        void													performGibbsMove(void);                                                            //!< Perform move
        void													acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double													performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void													rejectMove(void);                                                                   //!< Reject the InferenceMoveSimple
    
    private:
		double													getLnProbabilityForMove(void);
		
        StochasticNode<std::vector<valueType> >*				variable;
		int														numAuxCat;
		
 
    };
    
}



#endif

