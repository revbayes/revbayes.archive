/* 
 * File:   Move_RealNodeValTreeTranslation.h
 * Author: nl
 *
 * Created on 2 août 2014, 21:30
 */

#ifndef MOVE_REALNODEVALTREETRANSLATION_H
#define	MOVE_REALNODEVALTREETRANSLATION_H

#include "RealNodeContainerTranslationMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Move_RealNodeValTreeTranslation : public Move {
        
    public:
        
        Move_RealNodeValTreeTranslation(void);                                                                                         //!< Default constructor
        
        // Basic utility functions
        virtual Move_RealNodeValTreeTranslation*               clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal Move.
        static const std::string&                   getClassType(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        RevPtr<const RevVariable>                      process;
        RevPtr<const RevVariable>                      lambda;
        RevPtr<const RevVariable>                      tuning;
        
    };
    
}

#endif	/* MOVE_REALNODEVALTREETRANSLATION_H */

