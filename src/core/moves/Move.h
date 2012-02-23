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
#include "Natural.h"
#include "RealPos.h"

class RandomNumberGenerator;


class Move : public ConstantMemberObject {

    public:
    virtual                                    ~Move(void);                                                                             //!< Destructor

        // Basic utility functions
        virtual Move*                           clone(void) const = 0;                                                                  //!< Clone the object
        static const std::string&               getClassName(void);                                                                     //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                                 //!< Get class type spec
        virtual void                            printValue(std::ostream& o) const;
    
        // Member variable rules
        virtual const MemberRules&              getMemberRules(void) const;                                                             //!< Get member rules

        // Member methods
        virtual const MethodTable&              getMethods(void) const;                                                                 //!< Get methods
        void                                    setMemberVariable(const std::string& name, Variable* var);                              //!< set the member variables

        // Move functions you have to override
        virtual void                            acceptMove(void) = 0;                                                                   //!< Accept the move
        virtual std::vector<StochasticNode*>&   getDagNodes(void) = 0;                                                                  //!< Get the nodes vector
        virtual double                          performMove(double& probRatio) = 0;                                                     //!< Perform the move
        virtual void                            rejectMove(void) = 0;                                                                   //!< Reject the move
        virtual void                            replaceDagNodes(std::vector<StochasticNode*> &n) = 0;                                   //!< Set the nodes vector

        // Move functions you should not override
        double                                  getAcceptanceRatio(void) const;                                                         //!< Get acceptance ratio
        double                                  getUpdateWeight(void) const;                                                            //!< Get update weight of move
        void                                    resetCounters(void);                                                                    //!< Reset numTried/numAccepted

	protected:
        Move(const MemberRules& memberRules);                                                                                           //!< Default constructor
        Move(const Move& m);                                                                                                            //!< Copy constructor

        virtual const RbLanguageObject&         executeOperationSimple(const std::string& name, const std::vector<Argument>& args);     //!< Map method call to internal functions

        // parameters
        RbVariablePtr                           weight;
    
        // Hidden member variables
        Natural                                 numAccepted;                                                                            //!< Number of times accepted
        Natural                                 numTried;                                                                               //!< Number of times tried
        RealPos                                 acceptanceR;
};

#endif
