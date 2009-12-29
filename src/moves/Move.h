/**
 * @file
 * This file contains the declaration of Move, which is the abstract
 * base class for moves.
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

#include "RbComplex.h"

#include <string>
#include <vector>

class VariableNode;
class RandomNumberGenerator;
class StringVector;

class Move : public RbComplex {

    public:
        virtual                 ~Move(void) {}                      //!< Virtual destructor, just in case

        // Move functions
        void                    acceptMove(void);				    //!< Accept the move, update statistics
        double                  getAcceptanceProbability(void);     //!< Get acceptance probability
        double                  performMove(void);                  //!< Perform the move, update statistics
        void                    rejectMove(void);                   //!< Reject the move

        // Basic utility functions
        const StringVector&     getClass(void) const;               //!< Get class vector

    protected:
                                Move(VariableNode* node, RandomNumberGenerator* rgen);      //!< Constructor

        // Functions you have to override
        virtual double          perform(void) = 0;                  //!< Perform the move, update statistics
        virtual void            accept(void) = 0;                   //!< Accept the move, update statistics
        virtual void            reject(void) = 0;                   //!< Reject the move

        // Member variables
		RandomNumberGenerator*  rng;                                //!< Random number generator (ptr)
        int                     numAccepted;                        //!< Number accepted moves
        int                     numTried;                           //!< Number of times move was tried
        VariableNode*           theNode;                            //!< The node to change (ptr)
};

#endif
