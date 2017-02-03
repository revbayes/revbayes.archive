//
//  GeographyRateModifierFunction.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/17/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__GeographyRateModifierFunction__
#define __rb_mlandis__GeographyRateModifierFunction__

#include "TimeAtlas.h"
#include "CharacterHistoryRateModifier.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <iostream>
#include <vector>

namespace RevBayesCore {
    
    class GeographyRateModifierFunction : public TypedFunction<CharacterHistoryRateModifier> {
        
    public:
        GeographyRateModifierFunction(const TypedDagNode<double>* dp, const TimeAtlas* ta,  bool uadj=false, bool uav=false, bool udd=false);
        virtual ~GeographyRateModifierFunction(void);                                                                                                  //!< Virtual destructor
        
        // public member functions
        GeographyRateModifierFunction*                                     clone(void) const;                                                          //!< Create an independent clone
        void                                                                    update(void);
        
    protected:
        void                                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<double>*                                             distancePower;
        
        
    };
}


#endif /* defined(__rb_mlandis__GeographyRateModifierFunction__) */
