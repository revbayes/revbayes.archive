/* 
 * File:   RealNodeContainerSlidingMove.h
 * Author: nl
 *
 * Created on 26 juillet 2014, 13:27
 */


#ifndef PHYLOPROCESSSLIDINGMOVE_H
#define	PHYLOPROCESSSLIDINGMOVE_H

#include "SimpleMove.h"
#include "StochasticNode.h"

#include "RealNodeContainer.h"
#include <ostream>
#include <string>


namespace RevBayesCore {
    

    class RealNodeContainerSlidingMove : public SimpleMove {
        
    public:
        RealNodeContainerSlidingMove(StochasticNode<RealNodeContainer>* n, double l, bool t, double w);                         //!< Constructor
        
        // Basic utility functions
        RealNodeContainerSlidingMove*         clone(void) const;                                                                  //!< Clone this object.
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
        
        StochasticNode<RealNodeContainer>*          variable;

        double                                      lambda;                 
        double                                      storedValue;
        size_t                                      nodeindex;
    };
    
}


#endif	/* REALNODECONTAINERSLIDINGMOVE_H */

