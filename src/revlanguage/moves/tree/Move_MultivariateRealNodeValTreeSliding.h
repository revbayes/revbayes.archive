/* 
 * File:   Move_MultivariateRealNodeValTreeSliding.h
 * Author: nl
 *
 * Created on 16 juillet 2014, 23:22
 */

#ifndef MOVE_MULTIVARIATEPHYLOPROCESSSLIDING_H
#define	MOVE_MULTIVARIATEPHYLOPROCESSSLIDING_H

#include "MultivariateRealNodeContainerSlidingMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Move_MultivariateRealNodeValTreeSliding : public Move {
        
    public:
        
        Move_MultivariateRealNodeValTreeSliding(void);                                                                                         //!< Default constructor
        
        // Basic utility functions
        virtual Move_MultivariateRealNodeValTreeSliding*               clone(void) const;                                                      //!< Clone object
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

#endif	/* MOVE_MULTIVARIATEPHYLOPROCESSSLIDING_H */

