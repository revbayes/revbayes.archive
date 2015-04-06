//
//  Move_VectorSlide.h
//  revbayes-proj
//
//  Created by Michael Landis on 4/4/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__Move_VectorSlide__
#define __revbayes_proj__Move_VectorSlide__


#include "SimpleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev Wrapper of a scaling move on all elements of a real valued vector.
     *
     * This class is the RevLanguage wrapper of VectorSlide.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     * @since 2013-11-17, version 1.0
     */
    class Move_VectorSlide : public Move {
        
    public:
        
        Move_VectorSlide(void);                                                                                                                   //!< Default constructor
        
        // Basic utility functions
        virtual Move_VectorSlide*                        clone(void) const;                                                                              //!< Clone the object
        void                                        constructInternalObject(void);                                                                  //!< We construct the a new internal move.
        static const std::string&                   getClassType(void);                                                                             //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                         //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                                     //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                        //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                              //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);             //!< Set member variable
        
        RevPtr<const RevVariable>                      x;                                                                                              //!< The variable holding the real valued vector.
        RevPtr<const RevVariable>                      delta;                                                                                         //!< The variable for the tuning parameter.
        RevPtr<const RevVariable>                      tune;                                                                                           //!< The variable telling if to tune or not.
        
    };
    
}


#endif /* defined(__revbayes_proj__Move_VectorSlide__) */
