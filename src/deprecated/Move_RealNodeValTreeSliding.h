/* 
 * File:   Move_RealNodeValTreeSliding.h
 * Author: nl
 *
 * Created on 26 juillet 2014, 13:31
 */

#ifndef MOVE_REALNODEVALTREESLIDING_H
#define	MOVE_REALNODEVALTREESLIDING_H


#include "RealNodeContainerSlidingMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Move_RealNodeValTreeSliding : public Move {
        
    public:
        
        Move_RealNodeValTreeSliding(void);                                                                                         //!< Default constructor
        
        // Basic utility functions
        virtual Move_RealNodeValTreeSliding*        clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal Move.
        static const std::string&                   getClassType(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                          //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        RevPtr<const RevVariable>                   process;
        RevPtr<const RevVariable>                   lambda;
        RevPtr<const RevVariable>                   tuning;
        
    };
    
}

#endif	/* MOVE_REALNODEVALTREESLIDING_H */

