//
//  Move_RealSymmetricMatrixSimple.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__Move_RealSymmetricMatrixSimple__
#define __revbayes__Move_RealSymmetricMatrixSimple__

#include <iostream>

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    class Move_RealSymmetricMatrixSimple : public Move {
        
    public:
        
        Move_RealSymmetricMatrixSimple(void);                                                                                      //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Move_RealSymmetricMatrixSimple*         clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        RevPtr<const Variable>                      mat;
        RevPtr<const Variable>                      lambda;
        RevPtr<const Variable>                      weight;
        RevPtr<const Variable>                      tune;
        
    };
    
}


#endif /* defined(__revbayes__Move_RealSymmetricMatrixSimple__) */
