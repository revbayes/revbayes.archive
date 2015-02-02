/* 
 * File:   RealNodeContainerTranslationMove.h
 * Author: nl
 *
 * Created on 2 août 2014, 21:32
 */

#ifndef REALNODECONTAINERTRANSLATIONMOVE_H
#define	REALNODECONTAINERTRANSLATIONMOVE_H

#include "SimpleMove.h"
#include "StochasticNode.h"

#include "RealNodeContainer.h"
#include <ostream>
#include <string>


namespace RevBayesCore {
    

    class RealNodeContainerTranslationMove : public SimpleMove {
        
    public:
        RealNodeContainerTranslationMove(StochasticNode< RbVector<double> >* n, double l, bool t, double w);                         //!< Constructor
        
        // Basic utility functions
        RealNodeContainerTranslationMove*           clone(void) const;                                                                  //!< Clone this object.
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        void                                        swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        double                                  performSimpleMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectSimpleMove(void);
        void                                    acceptSimpleMove(void);
        void                                    tune(void);
        void                                    touch( DagNode *toucher );
        
    private:
        
        StochasticNode< RbVector<double> >*         variable;
        double                                      lambda;                         
        RbVector<double>                            storedValue;
    };
    
}

#endif	/* REALNODECONTAINERTRANSLATIONMOVE_H */

