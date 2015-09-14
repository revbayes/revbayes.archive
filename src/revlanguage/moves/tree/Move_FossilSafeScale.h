//
//  Move_FossilSafeScale.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/9/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__Move_FossilSafeScale__
#define __revbayes_proj__Move_FossilSafeScale__


#include "FossilSafeScaleMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev wrapper class for the rate-age-beta-shift move.
     *
     * This class is the Rev wrapper class for the rate-age-beta-shift move,
     * a move that proposes a new node age and branch rates so that the actual
     * branch lengths remain constant. The new node age is proposed from a scaled
     * beta distribution between its parent age and its oldest child age.
     *
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     */
    class Move_FossilSafeScale : public Move {
        
    public:
        
        Move_FossilSafeScale(void);                                                                                                //!< Default constructor
        
        // Basic utility functions
        virtual Move_FossilSafeScale*              clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal Move.
        static const std::string&                   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        RevPtr<const RevVariable>                      tree;
        RevPtr<const RevVariable>                      lambda;
        RevPtr<const RevVariable>                      tipAges;
        RevPtr<const RevVariable>                      scale;
        RevPtr<const RevVariable>                      tune;
        
    };
    
}

#endif /* defined(__revbayes_proj__Move_FossilSafeScale__) */
