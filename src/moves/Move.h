/**
 * @file
 * This file contains the declaration of Move, which is the abstract
 * base class for moves used in mcmc inference.
 *
 * @brief Declaration of Move
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef Move_H
#define Move_H

#include "MemberObject.h"

class RandomNumberGenerator;
class VectorString;

class Move : public MemberObject {

    public:
        virtual                    ~Move(void) {}                                                                               //!< Destructor

        // Basic utility functions
        virtual RbObject*           clone(void) const = 0;                                                                      //!< Clone the object
        virtual const VectorString& getClass(void) const;                                                                       //!< Get class vector

        // Member variable rules
        virtual const MemberRules&  getMemberRules(void) const = 0;                                                             //!< Get member rules
        void                        setVariable(const std::string& name, DAGNode* var);                                         //!< Only allow constants

        // Member method inits
        virtual const MethodTable&  getMethodInits(void) const;                                                                 //!< Get method inits

        // Move functions you have to override
        virtual void                acceptMove(void) = 0;                                                                       //!< Accept the move
        virtual void                performMove(double& lnPriorRatio, double& lnLikelihoodRatio, double& lnHastingsRatio) = 0;  //!< Perform the move
        virtual void                rejectMove(void) = 0;                                                                       //!< Reject the move

        // Move functions you should not override
        double                      getAcceptanceProbability(void);                                                             //!< Get acceptance probability
        double                      getUpdateWeight(void) const;                                                                //!< Get update weight
        void                        resetCounters(void);                                                                        //!< Reset numTried/numAccepted
        void                        setUpdateWeight(double weight);                                                             //!< Set update weight

	protected:
        virtual RbObject*           convertTo(const std::string& type) const;                                                   //!< Convert to type
        virtual bool                isConvertibleTo(const std::string& type) const;                                             //!< Is convertible to type and dim?
                                    Move(const MemberRules& memberRules);                                                       //!< Parser constructor

        // Member method call (throw an error: no member methods)
        virtual const RbObject*     executeOperation(const std::string& name, std::vector<DAGNode*>& args);                     //!< Execute method

        // Hidden member variables (not visible to parser)
        int                         numAccepted;                                                                                //!< Number of times accepted
        int                         numTried;                                                                                   //!< Number of times tried
};

#endif
