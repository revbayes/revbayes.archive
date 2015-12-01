#ifndef MetropolisHastingsMove_H
#define MetropolisHastingsMove_H

#include "AbstractMove.h"

#include <set>
#include <vector>

namespace RevBayesCore {
    
    class Proposal;
    
    /**
     * Base class for all Metropolis-Hastings within an MCMC. 
     *
     * The base class of all moves only provides the interface for the call to propose a new move.
     * Here the perform methods actually does the accept/reject step.
     * All specifics are implemented in the derived classes.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-26, version 1.0
     *
     */
    class MetropolisHastingsMove : public AbstractMove {
        
    public:
        MetropolisHastingsMove(Proposal *p, double w, bool autoTune = false);                                           //!< Constructor
        MetropolisHastingsMove(const MetropolisHastingsMove &m);                                                        //!< Copy constructor
        virtual                                                ~MetropolisHastingsMove(void);                           //!< Destructor

        // overloaded operators
        MetropolisHastingsMove&                                 operator=(const MetropolisHastingsMove &m);             //!< Assignment operator
        
        // pure virtual public methods
        virtual MetropolisHastingsMove*                         clone(void) const;
        const std::string&                                      getMoveName(void) const;                                //!< Get the name of the move for summary printing
        Proposal&                                               getProposal(void);                                      //!< Get the proposal of the move
        void                                                    printSummary(std::ostream &o) const;                    //!< Print the move summary
        void                                                    tune(void);                                             //!< Specific tuning of the move
        
    protected:
        //protected methods that are overwritten from the base class
        void                                                    performMove(double lHeat, double pHeat);                //!< Perform the move.
        void                                                    resetMoveCounters(void);                                //!< Reset the counters such as numAccepted.
        virtual void                                            swapNodeInternal(DagNode *oldN, DagNode *newN);         //!< Swap the pointers to the variable on which the move works on.
        
    private:
        
        // parameters
        unsigned int                                            numAccepted;                                            //!< Number of times accepted
        Proposal*                                               proposal;                                               //!< The proposal distribution
    };
    
}


#endif
