/* 
 * File:   Move_MultivariatePhyloProcessTranslation.h
 * Author: nl
 *
 * Created on 16 juillet 2014, 19:46
 */

#ifndef MOVE_MULTIVARIATEBROWNIANTRANSLATION_H
#define	MOVE_MULTIVARIATEBROWNIANTRANSLATION_H

#include "MultivariatePhyloProcessTranslationMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Move_MultivariatePhyloProcessTranslation : public Move {
        
    public:
        
        Move_MultivariatePhyloProcessTranslation(void);                                                                                         //!< Default constructor
        
        // Basic utility functions
        virtual Move_MultivariatePhyloProcessTranslation*               clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal Move.
        static const std::string&                   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        RevPtr<const Variable>                      process;
        RevPtr<const Variable>                      lambda;
        RevPtr<const Variable>                      tuning;
        
    };
    
}

#endif	/* MOVE_MULTIVARIATEBROWNIANTRANSLATION_H */

