#ifndef ScaleSingleACLNRatesMove_H
#define ScaleSingleACLNRatesMove_H

#include "RbVector.h"
#include "SimpleMove.h"
#include "StochasticNode.h"

#include <ostream>
#include <vector>
#include <string>

namespace RevBayesCore {
    
     class ScaleSingleACLNRatesMove : public SimpleMove {
        
    public:
        ScaleSingleACLNRatesMove(StochasticNode< RbVector<double> >* v, double l, bool t, double w);                         //!< Constructor
        
        // Basic utility functions
        ScaleSingleACLNRatesMove*                   clone(void) const;                                                                  //!< Clone this object.
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        void                                        swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        double                                      performSimpleMove(void);                                                            //!< Perform move
        void                                        printParameterSummary(std::ostream &o) const;
        void                                        rejectSimpleMove(void);
        void                                        acceptSimpleMove(void);
        void                                        tune(void);
        void                                        touch( DagNode *toucher );
        
    private:
        
        StochasticNode< RbVector<double> >*         variable;
		
        double                                      lambda;                                                                             //!< The scale parameter of the move (larger lambda -> larger proposals).
        size_t                                      index;                                                                              //!< The index of the last modified element.
		double                                      storedValue;                                                                        //!< The stored value of the last modified element.
//        size_t										rootID;
    };
    
}

#endif

