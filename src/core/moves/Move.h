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

class RandomNumberGenerator;
class VectorString;

class Move : public MemberObject {

    public:
        virtual                    ~Move(void) {}                                                                               //!< Destructor

        // Basic utility functions
        virtual Move*               clone(void) const = 0;                                                                      //!< Clone the object
        virtual const VectorString& getClass(void) const;                                                                       //!< Get class vector

        // Member variable rules
        virtual const MemberRules&  getMemberRules(void) const;                                                                 //!< Get member rules

        // Member methods
        virtual const MethodTable&  getMethods(void) const;                                                                     //!< Get methods
        DAGNode*                    executeOperation(const std::string& name, ArgumentFrame& args);                             //!< Map method call to internal functions

        // Move functions you have to override
        virtual void                acceptMove(void) = 0;                                                                       //!< Accept the move
        virtual void                performMove(double& lnProbabilityRatio, double& lnHastingsRatio) = 0;                       //!< Perform the move @Fredrik: I think I move should return only the Hasting ratio and the caller (e.g. the Mcmc class) should compute the lnProbabilityRatio. (Sebastian)
        virtual void                rejectMove(void) = 0;                                                                       //!< Reject the move

        // Move functions you should not override
        double                      getAcceptanceRatio(void) const;                                                             //!< Get acceptance ratio
        double                      getUpdateWeight(void) const;                                                                //!< Get update weight of move
        void                        resetCounters(void);                                                                        //!< Reset numTried/numAccepted

	protected:
                                    Move(const MemberRules& memberRules);                                                       //!< Parser constructor

        // Hidden member variables (not visible to parser)
        int                         numAccepted;                                                                                //!< Number of times accepted
        int                         numTried;                                                                                   //!< Number of times tried
};

#endif
