//
//  PowermixFunction.h
//  RevBayes
//
//  Created by Fredrik Ronquist on 2014-03-07.
//  Copyright (c) 2014 Swedish Museum of Natural History. All rights reserved.
//

#ifndef PowermixFunction_h
#define PowermixFunction_h

#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <iostream>
#include <vector>

namespace RevBayesCore {
    
    class PowermixFunction : public TypedFunction<RbVector<double> > {
        
    public:
        PowermixFunction(const std::vector<const TypedDagNode< RbVector<double> > *> &args);
        virtual                                            ~PowermixFunction(void);                                                     //!< Virtual destructor
        
        // public member functions
        PowermixFunction*                                   clone(void) const;                                                          //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        std::vector<const TypedDagNode< RbVector<double> >* >    parameters;
        
    };

}

#endif
