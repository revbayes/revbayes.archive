#ifndef AbstractOldMove_H
#define AbstractOldMove_H

#include "Move.h"

namespace RevBayesCore {
    
    class DagNode;
    
    /**
     * Base class for all move within an MCMC (and all other variants).
     *
     * The base class of all moves only provides the interface for the call to propose a new move.
     * All specifics are implemented in the derived classes.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-26, version 1.0
     *
     */
    class AbstractOldMove : public Move {
        
    public:
        virtual                                                ~AbstractOldMove(void);                              //!< Destructor
        
        // pure virtual public methods
        void                                                    perform(double lHeat, double pHeat);                //!< Perform the move.
        
        virtual void                                            accept(void) = 0;
        virtual void                                            autoTune(void) = 0;                                 //!< Automatic tuning of the move.
        virtual AbstractOldMove*                                clone(void) const = 0;
        virtual const std::set<DagNode*>&                       getDagNodes(void) const = 0;                        //!< Get the nodes vector
        virtual const std::string&                              getMoveName(void) const = 0;                        //!< Get the name of the move for summary printing
        virtual double                                          getUpdateWeight(void) const = 0;                    //!< Get update weight of InferenceMove
        virtual bool                                            isGibbs(void) const = 0;
        virtual double                                          performOld(double &pr) = 0;
        virtual void                                            performGibbs(void) = 0;
        virtual void                                            printSummary(std::ostream &o) const = 0;            //!< Print the move summary
        virtual void                                            reject(void) = 0;
        virtual void                                            swapNode(DagNode *oldN, DagNode *newN) = 0;         //!< Swap the pointers to the variable on which the move works on.
        
    protected:
        AbstractOldMove();                                                                                          //!< Constructor
        
    };
    
}


#endif
