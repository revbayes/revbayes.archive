//
//  ChooseFunction.h
//  revbayes-proj
//
//  Created by Michael Landis on 10/21/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#ifndef ChooseFunction_h
#define ChooseFunction_h

#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    /**
     * \brief Round down to the next integer.
     *
     * The floor function rounds a real number down to the next integer number.
     *
     * \copyright (c) Copyright 2009-2015 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2012-08-13
     *
     */
    class ChooseFunction : public TypedFunction<long> {
        
    public:
        ChooseFunction(const TypedDagNode<long> *a, const TypedDagNode<long> *b);
        
        ChooseFunction*                      clone(void) const;                                                  //!< Create a clon.
        void                                update(void);                                                       //!< Recompute the value
        
    protected:
        void                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters
        
    private:
        const TypedDagNode<long>*         n;
        const TypedDagNode<long>*         k;
    };
}


#endif /* ChooseFunction_h */
