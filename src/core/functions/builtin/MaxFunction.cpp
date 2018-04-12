#include "MaxFunction.h"
#include "RbConstants.h"

using namespace RevBayesCore;

MaxFunction::MaxFunction(const TypedDagNode< RbVector<double> > *v) : TypedFunction<double>( new double(0.0) ),
    matrix(false), vals( v )
{
    // add the parameters as parents
    this->addParameter( vals );
    
    update();
}

MaxFunction::MaxFunction(const TypedDagNode< MatrixReal > *v) : TypedFunction<double>( new double(0.0) ),
    matrix(true), vals( v )
{
    // add the parameters as parents
    this->addParameter( vals );

    update();
}


MaxFunction::~MaxFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



MaxFunction* MaxFunction::clone( void ) const {
    return new MaxFunction( *this );
}


void MaxFunction::update( void ) {
    
    double m;
    if( matrix )
    {
        const MatrixReal &v = dynamic_cast<const TypedDagNode< MatrixReal >* >(vals)->getValue();
        m = RbConstants::Double::neginf;

        for ( size_t row = 0; row < v.size(); row++) {
            for ( size_t col = 0; col < v[row].size(); col++) {
                if( v[row][col] > m )
                    m = v[row][col];
            }
        }
    }
    else
    {
        const std::vector<double> &v = dynamic_cast<const TypedDagNode< RbVector<double> >* >(vals)->getValue();
        m = *(v.begin());
        if (v.size() > 1)
        {
            for ( std::vector<double>::const_iterator it = v.begin()+1; it != v.end(); ++it) {
                if (  *it > m) {
                    m = *it;
                }
            }
        }
    }

    *this->value = m ;
    
}



void MaxFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == vals ) {
        vals = newP;
    }
    
}

