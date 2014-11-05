#include "MeanFunction.h"

using namespace RevBayesCore;

MeanFunction::MeanFunction(const TypedDagNode< RbVector<double> > *v) : TypedFunction<double>( new double(0.0) ),
    vals( v )
{
    // add the parameters as parents
    this->addParameter( vals );
    
    update();
}


MeanFunction::~MeanFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



MeanFunction* MeanFunction::clone( void ) const {
    return new MeanFunction( *this );
}


void MeanFunction::update( void ) {
    
    double m = 0;
    const std::vector<double> &v = vals->getValue();
    for ( std::vector<double>::const_iterator it = v.begin(); it != v.end(); ++it) {
        m += *it;
    }
    
    *this->value = m / v.size();
    
}



void MeanFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == vals ) {
        vals = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
}

