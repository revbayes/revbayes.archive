#ifndef Proposal_H
#define Proposal_H

#include "Cloneable.h"

#include <set>

namespace RevBayesCore {
    
    class DagNode;
    
    /**
     * Base class for all proposal distribution. 
     *
     * The base class of all moves only provides the interface for the call to propose a new move.
     * Here the perform methods actually does the accept/reject step.
     * All specifics are implemented in the derived classes.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-27, version 1.0
     *
     */
    class Proposal : public Cloneable {
        
    public:
        virtual                                                ~Proposal(void) {}                                                                       //!< Destructor
        
        // pure virtual public methods
        virtual void                                            cleanProposal(void) = 0;                                                                //!< Cleanup proposal
        virtual Proposal*                                       clone(void) const = 0;                                                                  //!< Make a deep copy
        virtual double                                          doProposal(void) = 0;                                                                   //!< Actually do the proposal.
        virtual const std::set<DagNode*>&                       getNodes(void) const = 0;                                                               //!< Get the vector of nodes for which the proposal is drawing new values.
        virtual const std::string&                              getProposalName(void) const = 0;                                                        //!< Get the name of this proposal used for printing out info.
        virtual void                                            prepareProposal(void) = 0;                                                              //!< Propose a new state
        virtual void                                            printParameterSummary(std::ostream &o) const = 0;                                       //!< Print the parameter summary
        virtual void                                            swapNode(DagNode *oldN, DagNode *newN) = 0;                                             //!< Swap the pointers to the variable on which the move works on.
        virtual void                                            tune(double r) = 0;                                                                     //!< Tune the parameters of the proposal.
        virtual void                                            undoProposal(void) = 0;                                                                 //!< Undo the proposal.
        
    protected:
        Proposal() {}                                                                                                                                   //!< Constructor
        
        
    };
    
}


#endif
