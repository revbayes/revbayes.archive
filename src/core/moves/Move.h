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

#include "MemberObject.h"
#include "Natural.h"
#include "RealPos.h"

class RandomNumberGenerator;


class Move : public MemberObject {

    public:

        // Basic utility functions
        virtual Move*                           clone(void) const = 0;                                                                  //!< Clone the object
        static const std::string&               getClassName(void);                                                                     //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                                 //!< Get class type spec
        virtual void                            printValue(std::ostream& o) const;
    
        // Member variable rules
        virtual const MemberRules&              getMemberRules(void) const;                                                             //!< Get member rules

        // Member methods
        virtual const MethodTable&              getMethods(void) const;                                                                 //!< Get methods

        // Move functions you have to override
        virtual void                            acceptMove(void) = 0;                                                                   //!< Accept the move
        virtual std::vector<StochasticNode*>&   getDagNodes(void) = 0;                                                                  //!< Get the nodes vector
        virtual double                          performMove(double& probRatio) = 0;                                                     //!< Perform the move
        virtual void                            rejectMove(void) = 0;                                                                   //!< Reject the move
        virtual void                            replaceDagNodes(std::vector<StochasticNode*> &n) = 0;                                   //!< Set the nodes vector

        // Move functions you should not override
        virtual void                            addDagNode(StochasticNode* d);                                                                 //!< Add a DAG node to this move
        double                                  getAcceptanceRatio(void) const;                                                         //!< Get acceptance ratio
        double                                  getUpdateWeight(void) const;                                                            //!< Get update weight of move
        void                                    resetCounters(void);                                                                    //!< Reset numTried/numAccepted

	protected:
        Move(const MemberRules& memberRules);                                                                                           //!< Default constructor

        virtual RbPtr<RbLanguageObject>         executeOperationSimple(const std::string& name, const std::vector<RbPtr<Argument> >& args);     //!< Map method call to internal functions

        
        std::vector<StochasticNode*>            nodes;
    
    
};

#endif
