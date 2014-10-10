/**
 * @file
 * This file contains the declaration of the RevLanguage wrapper of the base class of all moves.
 *
 * @brief Declaration of RlMoves
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-06 20:14:22 +0200 (Mon, 06 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id: Real.h 1746 2012-08-06 18:14:22Z hoehna $
 */

#ifndef RlMove_H
#define RlMove_H

#include "Move.h"
#include "TypedDagNode.h"
#include "WorkspaceObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Move : public WorkspaceObject<RevBayesCore::Move> {
        
    public:
        
                                                    Move(void);                                                             //!< Default constructor (0.0)
                                                    Move(const Move& x);                                                    //!< Copy constructor
        
        // Overloaded operators
        
        // Basic utility functions
        virtual Move*                               clone(void) const = 0;                                                  //!< Clone object
        static const std::string&                   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        virtual const MemberRules&                  getMemberRules(void) const;                                             //!< Get member rules (const)
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable

        RevPtr<const Variable>                      weight;

    };
    
}

#endif
