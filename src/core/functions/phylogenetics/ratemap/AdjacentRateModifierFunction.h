//
//  AdjacentRateModifierFunction.hpp
//  revbayes-proj
//
//  Created by Michael Landis on 2/3/17.
//  Copyright © 2017 Michael Landis. All rights reserved.
//

#ifndef AdjacentRateModifierFunction_h
#define AdjacentRateModifierFunction_h

#include "CharacterHistoryRateModifier.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <iostream>
#include <vector>

namespace RevBayesCore {
    
    class AdjacentRateModifierFunction : public TypedFunction<CharacterHistoryRateModifier> {
        
    public:
        AdjacentRateModifierFunction(const TypedDagNode<double>* f, const TypedDagNode<int>* w, const TypedDagNode<RbVector<RbVector<double> > >* c, size_t ns, size_t nc);
        AdjacentRateModifierFunction(const AdjacentRateModifierFunction& m);
        virtual ~AdjacentRateModifierFunction(void);                                                                                                  //!< Virtual destructor
        
        // public member functions
        AdjacentRateModifierFunction*                                     clone(void) const;                                                          //!< Create an independent clone
        void                                                              update(void);
        
    protected:
        void                                                              swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<double>*                                       factor;
        const TypedDagNode<int>*                                          width;
        const TypedDagNode<RbVector<RbVector<double> > >*                 context_matrix;
        
        std::string context_type;
        
        
    };
}

#endif /* AdjacentRateModifierFunction_hpp */
