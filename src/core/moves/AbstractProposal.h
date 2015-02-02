#ifndef AbstractProposal_H
#define AbstractProposal_H

#include "Cloneable.h"

#include <set>
#include <vector>

namespace RevBayesCore {
    
    class DagNode;
    
    /**
     * Base class for all AbstractProposal distribution.
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
    class AbstractProposal : public Cloneable {
        
    public:
        virtual                                                ~AbstractProposal(void) {}                                                                       //!< Destructor
        
        // pure virtual public methods
        virtual void                                            cleanProposal(void) = 0;                                                                //!< Cleanup AbstractProposal
        virtual AbstractProposal*                               clone(void) const = 0;                                                                  //!< Make a deep copy
        virtual double                                          doProposal(void) = 0;                                                                   //!< Actually do the AbstractProposal.
        virtual const std::set<DagNode*>&                       getNodes(void) const;                                                               //!< Get the vector of nodes for which the AbstractProposal is drawing new values.
        virtual const std::set<DagNode*>&                       getAuxillaryNodes(void) const;
        virtual const std::string&                              getProposalName(void) const = 0;                                                        //!< Get the name of this AbstractProposal used for printing out info.
        virtual void                                            prepareProposal(void) = 0;                                                              //!< Propose a new state
        virtual void                                            printParameterSummary(std::ostream &o) const = 0;                                       //!< Print the parameter summary
        virtual void                                            swapNode(DagNode *oldN, DagNode *newN) = 0;                                             //!< Swap the pointers to the variable on which the move works on.
        virtual void                                            tune(double r) = 0;                                                                     //!< Tune the parameters of the AbstractProposal.
        virtual void                                            undoProposal(void) = 0;                                                                 //!< Undo the AbstractProposal.
        
    protected:
        AbstractProposal() {}                                                                                                                                   //!< Constructor
        std::set<DagNode*>                                      nodes;
        std::set<DagNode*>                                      auxillaryNodes;
    };
    
}

#endif
