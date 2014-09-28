#include "MinFunction.h"

using namespace RevBayesCore;

MinFunction::MinFunction(const TypedDagNode< RbVector<double> > *v) : TypedFunction<double>( new double(0.0) ),
    vals( v )
{
    // add the parameters as parents
    this->addParameter( vals );
    
    update();
}


MinFunction::~MinFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



MinFunction* MinFunction::clone( void ) const {
    return new MinFunction( *this );
}


void MinFunction::update( void ) {
    
    const std::vector<double> &v = vals->getValue();
    double m = *(v.begin());
    if (v.size() > 1)
    {
        for ( std::vector<double>::const_iterator it = v.begin()+1; it != v.end(); ++it) {
            if (  *it < m) {
                m = *it;
            }
        }
    }
    *this->value = m ;
    
}



void MinFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == vals ) {
        vals = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
}

