#ifndef AbstractMove_H
#define AbstractMove_H

#include "Move.h"
#include "RbOrderedSet.h"

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
        virtual                                                ~AbstractMove(void);                                                 //!< Destructor
        
        // public methods
        const std::vector<DagNode*>&                            getDagNodes(void) const;                                            //!< Get the nodes vector
        const RbOrderedSet<DagNode*>&                           getAffectedNodes(void) const;                                       //!< Get the nodes vector
        void                                                    swapNode(DagNode *oldN, DagNode *newN);                             //!< Swap the pointers to the variable on which the move works on.
        
        
        // pure virtual public methods
        virtual AbstractMove*                                   clone(void) const = 0;
        virtual const std::string&                              getMoveName(void) const = 0;                                        //!< Get the name of the move for summary printing
        virtual void                                            printSummary(std::ostream &o) const = 0;                            //!< Print the move summary
        
        // functions you should not override
        void                                                    addNode(DagNode* p);                                                //!< add a node to the proposal
        void                                                    autoTune(void);                                                     //!< Automatic tuning of the move.
        void                                                    decrementTriedCounter(void);                        //!< Get update weight of InferenceMove
        virtual size_t                                          getNumberAccepted(void) const;                                      //!< Get update weight of InferenceMove
        size_t                                                  getNumberTried(void) const;                                         //!< Get the number of tries for this move since the last reset
        virtual double                                          getUpdateWeight(void) const;                                        //!< Get update weight of move
        bool                                                    isActive(unsigned long gen) const;                                  //!< Is the move active at the generation 'gen'?
        void                                                    performMcmcStep(double lHeat, double pHeat);                        //!< Perform the move.
        void                                                    performHillClimbingStep(double lHeat, double pHeat);                //!< Perform the move.
        void                                                    removeNode(DagNode* p);                                             //!< remove a node from the proposal
        void                                                    resetCounters(void);                                                //!< Reset the counters such as numTried.
        
    protected:
        AbstractMove(double w, bool autoTune = false);                                              //!< Constructor
        AbstractMove(const std::vector<DagNode*> &d, double w, bool autoTune = false);                                              //!< Constructor
        AbstractMove(const AbstractMove &m);                                                                                        //!< Constructor
        
        // overloaded operators
        AbstractMove&                                           operator=(const AbstractMove &m);                                   //!< Assignment operator
        
        // pure virtual protected methods
        virtual void                                            performMcmcMove(double lHeat, double pHeat) = 0;                    //!< Perform the move.
        virtual void                                            performHillClimbingMove(double lHeat, double pHeat);            //!< Perform the move.
        virtual void                                            swapNodeInternal(DagNode *oldN, DagNode *newN) = 0;                 //!< Swap the pointers to the variable on which the move works on.
        virtual void                                            tune(void) = 0;                                                     //!< Specific tuning of the move
        
        // virtual methods
        virtual void                                            resetMoveCounters(void);                                            //!< Reset the counters such as numTried and numAccepted.
        
        // parameters
        std::vector<DagNode*>                                   nodes;
        RbOrderedSet<DagNode*>                                  affected_nodes;                                                      //!< The affected nodes by this move.
        double                                                  weight;
        bool                                                    auto_tuning;
        unsigned int                                            num_tried;                                                           //!< Number of times tried
                
    };
    
}


#endif
