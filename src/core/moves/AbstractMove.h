#ifndef AbstractMove_H
#define AbstractMove_H

#include "Move.h"

#include <ostream>
#include <set>
#include <vector>

namespace RevBayesCore {
        
    class DagNode;
    
    /**
     * Base class for all moves within an MCMC (and all other variants). 
     *
     * The base class of all moves only provides the interface for the call to propose a new move.
     * All specifics are implemented in the derived classes.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-26, version 1.0
     *
     */
    class AbstractMove : public Move {
        
    public:
        virtual                                                ~AbstractMove(void);                                                                     //!< Destructor
        
        // pure virtual public methods
        virtual AbstractMove*                                   clone(void) const = 0;
        virtual const std::set<DagNode*>&                       getDagNodes(void) const = 0;                                                            //!< Get the nodes vector
        virtual const std::string&                              getMoveName(void) const = 0;                                                            //!< Get the name of the move for summary printing
        virtual void                                            printSummary(std::ostream &o) const = 0;                                                //!< Print the move summary
        virtual void                                            swapNode(DagNode *oldN, DagNode *newN) = 0;                                             //!< Swap the pointers to the variable on which the move works on.
        
        // functions you should not override
        void                                                    autoTune(void);                                                                         //!< Automatic tuning of the move.
        double                                                  getUpdateWeight(void) const;                                                            //!< Get update weight of InferenceMove
        bool                                                    isActive(unsigned long gen) const;                                                      //!< Is the move active at the generation 'gen'?
        void                                                    perform(double lHeat, double pHeat);                                                    //!< Perform the move.
        void                                                    resetCounters(void);                                                                    //!< Reset the counters such as numTried.
        
    protected:
        AbstractMove(double w, bool autoTune = false);                                                                                                  //!< Constructor
        
        
        // pure virtual protected methods
        virtual void                                            performMove(double lHeat, double pHeat) = 0;                                            //!< Perform the move.
        virtual void                                            tune(void) = 0;                                                                         //!< Specific tuning of the move
        
        // virtual methods
        virtual void                                            resetMoveCounters(void);                                                                //!< Reset the counters such as numTried and numAccepted.
        
        // parameters
        double                                                  weight;
        bool                                                    autoTuning;
        unsigned int                                            numTried;                                                                               //!< Number of times tried
                
    };
    
}


#endif
