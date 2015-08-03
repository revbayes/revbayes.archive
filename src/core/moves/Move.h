#ifndef Move_H
#define Move_H

#include "Cloneable.h"

#include <ostream>
#include <set>
#include <vector>

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
    class Move : public Cloneable {
        
    public:
        virtual                                                ~Move(void);                                             //!< Destructor
        
        // pure virtual public methods
        virtual void                                            autoTune(void) = 0;                                     //!< Automatic tuning of the move.
        virtual Move*                                           clone(void) const = 0;                                  //!< Create a deep copy.
        virtual const std::vector<DagNode*>&                    getDagNodes(void) const = 0;                            //!< Get the nodes vector
        virtual const std::string&                              getMoveName(void) const = 0;                            //!< Get the name of the move for summary printing
        virtual double                                          getUpdateWeight(void) const = 0;                        //!< Get update weight of InferenceMove
        virtual bool                                            isActive(unsigned long gen) const = 0;                  //!< Is this move active?
        virtual void                                            perform(double lHeat, double pHeat) = 0;                //!< Perform the move.
        virtual void                                            printSummary(std::ostream &o) const = 0;                //!< Print the move summary
        virtual void                                            resetCounters(void) = 0;                                //!< Reset the counters such as numTried and numAccepted.
        virtual void                                            swapNode(DagNode *oldN, DagNode *newN) = 0;             //!< Swap the pointers to the variable on which the move works on.
        
    protected:
        Move();                                                                                                         //!< Constructor
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Move& x);                                     //!< Overloaded output operator

}


#endif
