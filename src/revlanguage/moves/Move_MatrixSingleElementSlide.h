/* 
 * File:   Move_MatrixSingleElementSlide.h
 * Author: nl
 *
 * Created on 13 juillet 2014, 18:13
 */

#ifndef Move_MatrixSingleElementSlide_H
#define Move_MatrixSingleElementSlide_H

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Move_MatrixSingleElementSlide : public Move {
        
    public:
        
        Move_MatrixSingleElementSlide(void);                                                                                      //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Move_MatrixSingleElementSlide*     clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassType(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        RevPtr<const RevVariable>                      v;
        RevPtr<const RevVariable>                      lambda;
        RevPtr<const RevVariable>                      weight;
        RevPtr<const RevVariable>                      tune;
        
    };
    
}


#endif	/* MOVE_MATRIXSINGLEELEMENTSLIDE_H */

