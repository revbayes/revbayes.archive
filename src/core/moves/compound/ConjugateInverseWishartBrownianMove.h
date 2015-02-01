#ifndef ConjugateInverseWishartBrownianMove_H
#define ConjugateInverseWishartBrownianMove_H

#include <map>
#include <ostream>
#include <set>
#include <string>

#include "AbstractMove.h"
#include "MatrixReal.h"
#include "StochasticNode.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    class ConjugateInverseWishartBrownianMove : public AbstractMove {
        
    public:
        ConjugateInverseWishartBrownianMove(StochasticNode<MatrixReal>* s, TypedDagNode<double>* k, TypedDagNode<int>* d, double w);                                                         //!<  constructor
        ConjugateInverseWishartBrownianMove(const ConjugateInverseWishartBrownianMove &m);                                                                                //!< Copy constructor
        virtual                                ~ConjugateInverseWishartBrownianMove(void);                                                             //!< Destructor
        
        // overloaded operators
        ConjugateInverseWishartBrownianMove&    operator=(const ConjugateInverseWishartBrownianMove &m);                                               //!< Assignment operator
        
        // Basic utility functions
        ConjugateInverseWishartBrownianMove*    clone(void) const;                                                                  //!< Clone object
        const std::set<DagNode*>&               getDagNodes(void) const;
        const std::string&                      getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        void                                    printSummary(std::ostream &o) const;                                                //!< Print the move summary
        void                                    swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the pointers to the variable on which the move works on.
        
    protected:
        void                                    performMove(double lHeat, double pHeat);                                 //!< Perform move
        void                                    resetMoveCounters(void);                                                                //!< Reset the counters such as numAccepted.
        void                                    tune(void);
        
    private:
        void                                    reject(void);
        
        // member variables
        StochasticNode<MatrixReal>*             sigma;
        TypedDagNode<double>*                   kappa;
        TypedDagNode<int>*                      df;
        
        std::set<DagNode*>                      nodes;
        size_t                                  numAccepted;
        
    };
    
}

#endif
