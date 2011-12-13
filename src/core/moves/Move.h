/**
 * @file
 * This file contains the declaration of Move, which is the abstract
 * base class for moves used in mcmc inference.
 *
 * @brief Declaration of Move
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef Move_H
#define Move_H

#include "ConstantMemberObject.h"

class RandomNumberGenerator;
class StochasticNode;
class VectorString;

const std::string Move_name = "Move";

class Move : public ConstantMemberObject {

    public:
    virtual                                ~Move(void);                                                                 //!< Destructor

        // Basic utility functions
        virtual std::string                 briefInfo(void) const;                                                  //!< Get a brief info about the move
        virtual Move*                       clone(void) const = 0;                                                  //!< Clone the object
        virtual const VectorString&         getClass(void) const;                                                   //!< Get class vector
        virtual void                        printValue(std::ostream& o) const;
    
        // Member variable rules
        virtual RbPtr<const MemberRules>    getMemberRules(void) const;                                             //!< Get member rules

        // Member methods
        virtual RbPtr<const MethodTable>    getMethods(void) const;                                                 //!< Get methods
        void                                setMemberVariable(const std::string& name, RbPtr<Variable> var);        //!< set the member variables

        // Move functions you have to override
        virtual void                        acceptMove(void) = 0;                                                   //!< Accept the move
        virtual double                      performMove(double& lnProbabilityRatio) = 0;                            //!< Perform the move
        virtual void                        rejectMove(void) = 0;                                                   //!< Reject the move

        // Move functions you should not override
        double                              getAcceptanceRatio(void) const;                                         //!< Get acceptance ratio
        std::vector<RbPtr<VariableNode> >&  getDagNodes(void) { return nodes;}                                      //!< Get the nodes vector
        double                              getUpdateWeight(void) const;                                            //!< Get update weight of move
//        void                            setDagNodes(std::vector<StochasticNode*> n);                              //!< Set the nodes vector
        void                                replaceDagNodes(std::vector<RbPtr<VariableNode> > &n);                         //!< Set the nodes vector
        void                                resetCounters(void);                                                    //!< Reset numTried/numAccepted

	protected:
        Move(RbPtr<const MemberRules> memberRules);                                                                   //!< Default constructor
        Move(const Move& m);                                                                                        //!< Copy constructor

        RbPtr<RbLanguageObject>             executeOperationSimple(const std::string& name, const RbPtr<Environment>& args); //!< Map method call to internal functions

    
        // Hidden member variables
        int                                 numAccepted;                                                            //!< Number of times accepted
        int                                 numTried;                                                               //!< Number of times tried
        std::vector<RbPtr<VariableNode> >   nodes;                                                                  //!< The nodes on which the move works
};

#endif
