//
//  RlPrecisionMatrixSimpleMove.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__RlPrecisionMatrixSimpleMove__
#define __revbayes__RlPrecisionMatrixSimpleMove__

#include <iostream>

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    class PrecisionMatrixSimpleMove : public Move {
        
    public:
        
        PrecisionMatrixSimpleMove(void);                                                                                      //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual PrecisionMatrixSimpleMove*                        clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                                     //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        RbPtr<const Variable>                       mat;
        RbPtr<const Variable>                       lambda;
        RbPtr<const Variable>                       weight;
        RbPtr<const Variable>                       tune;
        
    };
    
}


#endif /* defined(__revbayes__RlPrecisionMatrixSimpleMove__) */
