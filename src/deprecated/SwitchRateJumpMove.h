#ifndef SwitchRateJumpMove_H
#define SwitchRateJumpMove_H

#include "RbVector.h"
#include "SimpleMove.h"
#include "StochasticNode.h"

#include <ostream>
#include <vector>
#include <string>

namespace RevBayesCore {
    
    /**
     * @brief Move that switches between a jump and no-jump.
     *
     * 
     * 
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     *
     */
    class SwitchRateJumpMove : public SimpleMove {
        
    public:
        SwitchRateJumpMove(StochasticNode<RbVector<double> >* n, double w);                                                          //!< Constructor
        
        // Basic utility functions
        SwitchRateJumpMove*                         clone(void) const;                                                                  //!< Clone this object.
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        void                                        swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        double                                      performSimpleMove(void);                                                            //!< Perform move
        void                                        rejectSimpleMove(void);
        void                                        acceptSimpleMove(void);
        void                                        touch( DagNode *toucher );
        
    private:
        
        StochasticNode<RbVector<double> >*          variable;
        
        size_t                                      index;                                                                              //!< The index of the last modified element.
		double                                      storedValue;                                                                        //!< The stored value of the last modified element.
        TypedDistribution<double>*                  valueDistribution;                                                                  //!< The distribution of the values if a jump occurred.
        
    };
    
}

#endif

