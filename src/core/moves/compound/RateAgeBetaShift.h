#ifndef RateAgeBetaShift_H
#define RateAgeBetaShift_H

#include <map>
#include <ostream>
#include <set>
#include <string>

#include "AbstractMove.h"
#include "StochasticNode.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    class RateAgeBetaShift : public AbstractMove {
        
    public:
        RateAgeBetaShift( StochasticNode<TimeTree> *tr, std::vector<StochasticNode<double> *> n, double delta, bool t, double weight);                                                         //!<  constructor
        virtual                                ~RateAgeBetaShift(void);                                                             //!< Destructor
        
        // Basic utility functions
        RateAgeBetaShift*                       clone(void) const;                                                                  //!< Clone object
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        void                                    printSummary(std::ostream &o) const;                                                //!< Print the move summary
        
    protected:
        void                                    performMove(double lHeat, double pHeat);                                 //!< Perform move
        void                                    resetMoveCounters(void);                                                                //!< Reset the counters such as numAccepted.
        void                                    swapNodeInternal(DagNode *oldN, DagNode *newN);                                             //!< Swap the pointers to the variable on which the move works on.
        void                                    tune(void);
        
    private:
        void                                    reject(void);
        
        // member variables
        StochasticNode<TimeTree>*               tree;
        std::vector<StochasticNode<double>* >   rates;
        double                                  delta;
        
        // stored objects to undo proposal
        TopologyNode*                           storedNode;
        double                                  storedAge;
        std::vector<double>                     storedRates;
        
        size_t                                  numAccepted;
        
    };
    
}

#endif
