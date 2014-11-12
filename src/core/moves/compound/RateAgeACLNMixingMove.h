//
//  RateAgeACLNMixingMove.h
//

#ifndef RateAgeACLNMixingMove_H
#define RateAgeACLNMixingMove_H

#include <map>
#include <ostream>
#include <set>
#include <string>

#include "CompoundMove.h"
#include "StochasticNode.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    class RateAgeACLNMixingMove : public CompoundMove {
        
    public:
        RateAgeACLNMixingMove( std::vector<DagNode*> n, double ep, bool t, double w);                                                         //!<  constructor
        
        // Basic utility functions
        RateAgeACLNMixingMove*                  clone(void) const;                                                                  //!< Clone object
        void                                    swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                  performCompoundMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectCompoundMove(void);
        void                                    tune(void);
        
    private:
        
        // member variables
        StochasticNode<TimeTree>*				tree;
        StochasticNode< RbVector<double> >*	rates;
		StochasticNode< double >*				rootRate;
        double									epsilon;
        
        // stored objects to undo proposal
        double									storedC;
		double									storedRootAge;
        
    };
    
}

#endif /* defined(RateAgeACLNMixingMove_H) */
