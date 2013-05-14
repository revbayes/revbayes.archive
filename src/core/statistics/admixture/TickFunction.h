//
//  IncrementFunction.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/30/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__IncrementFunction__
#define __rb_mlandis__IncrementFunction__



//#include "Statistic.h"
#include "ConstantNode.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class TickFunction : public TypedFunction<int> {
        
    public:
        TickFunction(int v, int i);                                                                                   //!< Default constructor
        TickFunction(const TickFunction& t);                                                                                      //!< Copy constructor
        virtual                                ~TickFunction(void);                                                                  //!< Destructor
        
        TickFunction&                          operator=(const TickFunction& t);
        
        // Basic utility functions
        TickFunction*                          clone(void) const;                                                                          //!< Clone object
        void                                   update(void);                                                                               //!< Clone the function
        void                                   tick(void);
        
    protected:
        void                                   swapParameterInternal(const DagNode *oldP, const DagNode *newP);                            //!< Implementation of swaping parameters
    
    private:
        // members
        //const ConstantNode<double>* nd;
        int interval;
        
    };
    
}


#endif /* defined(__rb_mlandis__IncrementFunction__) */
