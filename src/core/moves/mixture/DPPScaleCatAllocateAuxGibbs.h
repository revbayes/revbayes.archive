/**
 * @file
 * This file contains the declaration of DPPScaleCatAllocateAuxGibbs, which performs the DPP move based on Neal (2000) Algorithm 8
 * this move changes the value assigned to each table
 *
 * @brief Declaration of DPPScaleCatAllocateAuxGibbs
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-11 14:54:35 +0200 (Fri, 11 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-20, version 1.0
 *
 * $Id: DPPScaleCatAllocateAuxGibbs.h $
 */

#ifndef DPPScaleCatAllocateAuxGibbs_H
#define DPPScaleCatAllocateAuxGibbs_H

#include <ostream>

#include "MoveOld.h"
#include "RbVector.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    class DPPScaleCatAllocateAuxGibbs : public MoveOld {
        
    public:
        DPPScaleCatAllocateAuxGibbs(StochasticNode< RbVector<double> >* v, double l, int na, double w);                                                                      //!< Internal constructor
        
        // Basic utility functions
        DPPScaleCatAllocateAuxGibbs*									clone(void) const;                                                                  //!< Clone object
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
        void                                                    doScaleTablesMove(void);
        void                                                    doAllocateAuxMove(void);
		void													dppNormalizeVector(std::vector<double> &v);
		int														findElementNewTable(double u, std::vector<double> lnProb);
		int														findTableIDForVal(std::vector<double> tvs, double val);

		double													lambda;                                                                             //!< The scaling parameter of the move  
        StochasticNode< RbVector<double> >*                     variable;
		double													safeExponentiation(double x);
		int														numAuxCat;
    };
    
}



#endif

