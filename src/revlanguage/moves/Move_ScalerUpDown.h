/**
 * @file
 * This file contains the declaration of RevLanguage wrapper of VectorSingleElementScaleMove.
 *
 * @brief Declaration of Move_ScalerUpDown
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-06 20:14:22 +0200 (Mon, 06 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 */

#ifndef Move_ScalerUpDown_H
#define Move_ScalerUpDown_H

#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Move_ScalerUpDown : public Move {
        
    public:
        
        Move_ScalerUpDown(void);                                                                                      //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Move_ScalerUpDown*                  clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        RevPtr<const RevVariable>                      scaleUp;
        RevPtr<const RevVariable>                      scaleDown;
        RevPtr<const RevVariable>                      scaleFactor;
        RevPtr<const RevVariable>                      weight;
        RevPtr<const RevVariable>                      tune;
        
    };
    
}

#endif
