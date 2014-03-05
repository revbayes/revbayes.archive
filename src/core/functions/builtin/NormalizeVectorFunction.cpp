#include "NormalizeVectorFunction.h"

using namespace RevBayesCore;

NormalizeVectorFunction::NormalizeVectorFunction(const TypedDagNode<std::vector<double> > *v) : TypedFunction< std::vector<double> >( new std::vector<double>(v->getValue().size(), 1.0) ), vals( v ) {
    // add the parameters as parents
    this->addParameter( vals );
    
    update();
}


NormalizeVectorFunction* NormalizeVectorFunction::clone( void ) const {
    
    return new NormalizeVectorFunction( *this );
}


void NormalizeVectorFunction::update( void ) {
    
    double m = 0;
    const std::vector<double> &v = vals->getValue();
    for ( std::vector<double>::const_iterator it = v.begin(); it != v.end(); ++it) 
    {
        m += *it;
    }
    
    m /= v.size();
    
    
    for ( size_t i = 0; i < v.size(); ++i ) 
    {
        (*value)[i] = v[i] / m;
    }
    
    
}



void NormalizeVectorFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == vals ) 
    {
        vals = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
    
}

