#include "MatrixInverse.h"
#include "RbException.h"

using namespace RevBayesCore;

MatrixInverse::MatrixInverse(const TypedDagNode< MatrixReal > *m) : TypedFunction< MatrixReal >( new MatrixReal(m->getValue().getDim()) ),
    matrix( m )
{

    // check dimensions here
    addParameter( matrix );
    update();
    
}


MatrixInverse::~MatrixInverse( void ) {

}


MatrixInverse* MatrixInverse::clone( void ) const {

    return new MatrixInverse( *this );
}


void MatrixInverse::update(void) {

    matrix->getValue().setCholesky(true);
    *value = matrix->getValue().computeInverse();
    
//    const MatrixReal R = matrix->getValue();
//    size_t nrows = R.getDim();
//
//    value->clear();
//    value->resize( nrows * (nrows - 1) / 2 );
//    
//    size_t k = 0;
//    for (size_t i = 0; i < nrows; ++i)
//    {
//        for (size_t j = i + 1; j < nrows; ++j)
//        {
//            (*value)[k++] = R[i][j];
//        }
//    }

}


void MatrixInverse::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {

    // check dimensions here
    if (oldP == matrix)
    {
        matrix = static_cast<const TypedDagNode< MatrixReal >* >( newP );
    }
}


