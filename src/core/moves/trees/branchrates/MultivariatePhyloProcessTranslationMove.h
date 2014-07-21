/* 
 * File:   MultivariatePhyloProcessTranslationMove.h
 * Author: nl
 *
 * Created on 16 juillet 2014, 19:32
 */

#ifndef MULTIVARIATEPHYLOPROCESSTRANSLATIONMOVE_H
#define	MULTIVARIATEPHYLOPROCESSTRANSLATIONMOVE_H


#include "SimpleMove.h"
#include "StochasticNode.h"

#include "MatrixReal.h"
#include "MultivariatePhyloProcess.h"
#include <ostream>
#include <string>


namespace RevBayesCore {
    
    /**
     * @brief Scaling move of a single element randomly picked from a vector.
     *
     * 
     * This move randomly picks an element of a vector of positive real numbers,
     * proposes a scaling factor and then Slidings the value.
     * The actual scaling factor is computed by sf = exp( lambda * ( u - 0.5 ) )
     * where u ~ Uniform(0,1).
     * It generally makes more sense to apply the scaling move on a vector of positive
     * real numbers but technically it works on negative numbers too. However, 
     * the move will never change the sign of the value and thus is incomplete if applied
     * to variable defined on the whole real line.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     *
     */
    class MultivariatePhyloProcessTranslationMove : public SimpleMove {
        
    public:
        MultivariatePhyloProcessTranslationMove(StochasticNode<MultivariatePhyloProcess>* n, double l, bool t, double w);                         //!< Constructor
        
        // Basic utility functions
        MultivariatePhyloProcessTranslationMove*         clone(void) const;                                                                  //!< Clone this object.
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        void                                        swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        double                                  performSimpleMove(void);                                                            //!< Perform move
        void                                    recursiveTranslate(const TopologyNode& from, int component, double slide);
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectSimpleMove(void);
        void                                    acceptSimpleMove(void);
        void                                    tune(void);
        void                                    touch( DagNode *toucher );
        
    private:
        
        StochasticNode<MultivariatePhyloProcess>*   variable;

        double                                      lambda;                                                                             //!< The Sliding parameter of the move (larger lambda -> larger proposals).
        MultivariatePhyloProcess&                   storedValue;                                                                        //!< The stored value of the last modified element.
        
    };
    
}

#endif	/* MULTIVARIATEPHYLOPROCESSTRANSLATIONMOVE_H */

