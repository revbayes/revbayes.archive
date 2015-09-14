//
//  ScalerUpDownMove.h
//

#ifndef ScalerUpDownMove_H
#define ScalerUpDownMove_H

#include <map>
#include <ostream>
#include <set>
#include <string>

#include "CompoundMove.h"
#include "ContinuousStochasticNode.h"

namespace RevBayesCore {
    
    class ScalerUpDownMove : public CompoundMove {
        
    public:
        ScalerUpDownMove( std::vector<DagNode*> n, double sf, bool t, double w);                                                         //!<  constructor
        
        // Basic utility functions
        ScalerUpDownMove*                       clone(void) const;                                                                  //!< Clone object
        void                                    swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                                  performCompoundMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectCompoundMove(void);
        void                                    tune(void);
        
    private:
        
        // member variables
        ContinuousStochasticNode*               scalerVal1;
        ContinuousStochasticNode*               scalerVal2;
        double									scaleFactor;
        
        // stored objects to undo proposal
		double									storedSV1;
		double									storedSV2;
        
    };
    
}

#endif /* defined(ScalerUpDownMove_H) */
