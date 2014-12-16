/* 
 * File:   Move_ConjugateInverseWishartBrownian.h
 * Author: nl
 *
 * Created on 23 juillet 2014, 17:44
 */

#ifndef MOVE_BROWNIANINVERSEWISHARTSLIDING_H
#define	MOVE_BROWNIANINVERSEWISHARTSLIDING_H

#include "ConjugateInverseWishartBrownianMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Move_ConjugateInverseWishartBrownian : public Move {
        
    public:
        
        Move_ConjugateInverseWishartBrownian(void);                                                                                         //!< Default constructor
        
        // Basic utility functions
        virtual Move_ConjugateInverseWishartBrownian*               clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal Move.
        static const std::string&                   getClassType(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        RevPtr<const RevVariable>                      process;
        RevPtr<const RevVariable>                      sigma;
        RevPtr<const RevVariable>                      kappa;
        RevPtr<const RevVariable>                      df;
//        RevPtr<const RevVariable>                      tau;
        
    };
    
}

#endif	/* MOVE_BROWNIANINVERSEWISHARTSLIDING_H */

