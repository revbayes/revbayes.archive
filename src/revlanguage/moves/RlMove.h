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
#include "RbHelpMove.h"
#include "TypedDagNode.h"
#include "WorkspaceToCoreWrapperObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Move : public WorkspaceToCoreWrapperObject<RevBayesCore::Move> {
        
    public:
        
                                                    Move(void);                                                                         //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Move*                               clone(void) const = 0;                                                              //!< Clone object
        virtual void                                constructInternalObject(void) = 0;                                                  //!< We construct the a new internal object.
        static const std::string&                   getClassType(void);                                                                 //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                             //!< Get class type spec
        virtual RevBayesCore::RbHelpMove*           getHelpEntry(void) const;                                                           //!< Get the help entry for this function
        virtual const MemberRules&                  getParameterRules(void) const;                                                      //!< Get member rules (const)
        
        std::vector<std::string>                    getConstructorFunctionAliases(void) const;                                              //!< Get the alternative names used for the constructor function in Rev.
        std::string                                 getConstructorFunctionName(void) const;
        virtual std::vector<std::string>            getMoveAliases(void) const { return std::vector<std::string>(); }   //!< Get the alternative names used for the constructor function in Rev.
        virtual std::string                         getMoveName(void) const = 0;

    protected:
        
        virtual void                                setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);   //!< Set member variable

        virtual std::string                                 getConstructorUsage(void) const { return ""; }
        virtual std::vector<std::string>                    getConstructorDetails(void) const { return std::vector<std::string>(); }
        virtual std::string                                 getConstructorExample(void) const { return ""; }
        virtual std::vector<std::string>                    getHelpAuthor(void) const { return std::vector<std::string>(); }
        virtual std::vector<std::string>                    getHelpDescription(void) const { return std::vector<std::string>(); }
        virtual std::vector<RevBayesCore::RbHelpReference>  getHelpReferences(void) const { return std::vector<RevBayesCore::RbHelpReference>(); }
        virtual std::vector<std::string>                    getHelpSeeAlso(void) const { return std::vector<std::string>(); }
        virtual std::string                                 getHelpTitle(void) const { return ""; }

        
        RevPtr<const RevVariable>                   weight;

    };
    
}

#endif
