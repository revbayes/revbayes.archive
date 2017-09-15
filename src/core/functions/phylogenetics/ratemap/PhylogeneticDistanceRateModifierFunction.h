//
//  PhylogeneticDistanceRateModifierFunction.h
//  revbayes-branch-proj
//
//  Created by Michael Landis on 2/24/17.
//  Copyright © 2017 Michael Landis. All rights reserved.
//

#ifndef PhylogeneticDistanceRateModifierFunction_h
#define PhylogeneticDistanceRateModifierFunction_h


#include "CharacterHistoryRateModifier.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <iostream>
#include <vector>

namespace RevBayesCore {
    
    class PhylogeneticDistanceRateModifierFunction : public TypedFunction<CharacterHistoryRateModifier> {
        
    public:
        PhylogeneticDistanceRateModifierFunction(const TypedDagNode<Tree>* t, const TypedDagNode<double>* s);
        PhylogeneticDistanceRateModifierFunction(const PhylogeneticDistanceRateModifierFunction& m);
        virtual ~PhylogeneticDistanceRateModifierFunction(void);                                                                                                  //!< Virtual destructor
        
        // public member functions
        PhylogeneticDistanceRateModifierFunction*                         clone(void) const;                                                          //!< Create an independent clone
        void                                                              update(void);
        
    protected:
        void                                                              swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<Tree>*                                tau;
        const TypedDagNode<double>*                            scale;
        
    };
}

#endif /* PhylogeneticDistanceRateModifierFunction_h */
