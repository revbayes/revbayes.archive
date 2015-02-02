//
//  IncrementFunction.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/30/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "TickFunction.h"

using namespace RevBayesCore;

TickFunction::TickFunction(int v, int i) : TypedFunction<int>( new int(v) ), interval(i) {

    // do not add bmag as parameter so it is not called... ??
   // addParameter( nd );
}


TickFunction::TickFunction(const TickFunction &n) : TypedFunction<int>( n ), interval( n.interval ) {
    // no need to add parameters, happens automatically
}


TickFunction::~TickFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



TickFunction* TickFunction::clone( void ) const {
    return new TickFunction( *this );
}

void TickFunction::update(void) {
    *value += interval;
    std::cout << "TickFunction::value = " << *value << "\n";
}

void TickFunction::tick( void ) {
  
}



void TickFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
/*    if (oldP == nd) {
        nd = static_cast<const ConstantNode<double>* >( newP );
    }*/
}
