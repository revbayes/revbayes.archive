#include "SumMatrixFunction.h"

using namespace RevBayesCore;

SumMatrixFunction::SumMatrixFunction(const TypedDagNode<RbVector<MatrixReal> > *v) : TypedFunction<MatrixReal>( new MatrixReal(v->getValue()[0].getNumberOfRows(),
                                                                                                                               v->getValue()[0].getNumberOfColumns()) ), vals( v )
{
    // add the parameters as parents
    this->addParameter( vals );
    
    update();
}


SumMatrixFunction::~SumMatrixFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}

void SumMatrixFunction::checkDims( void )
{
    
    const RbVector<MatrixReal> &v = vals->getValue();
    
    // if the vector is one long, the dimensions must match!
    size_t num_matrices = v.size();
    if ( num_matrices == 1 )
    {
        return;
    }
    
    // otherwise, make sure the number of dimensions
    // match for each pair of matrices
    for (size_t i = 1; i < num_matrices; ++i)
    {
        for (size_t j = 0; j < i; ++j)
        {
            if (v.at(i).getNumberOfRows() != v.at(j).getNumberOfRows() || v.at(i).getNumberOfColumns() != v.at(j).getNumberOfColumns())
            {
                throw RbException("Dimensions of matrices to sum do not match!");
            }
        }
    }
    
}

SumMatrixFunction* SumMatrixFunction::clone( void ) const
{
    return new SumMatrixFunction( *this );
}


void SumMatrixFunction::update( void )
{
    
    const RbVector<MatrixReal> &v = vals->getValue();
    size_t n_row = v.at(0).getNumberOfRows();
    size_t n_col = v.at(0).getNumberOfColumns();
    
    MatrixReal m = MatrixReal(n_row, n_col, 0.0);
    for ( RbConstIterator<MatrixReal> it = v.begin(); it != v.end(); ++it)
    {
        m += *it;
    }

    *this->value = m;
    
}



void SumMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == vals )
    {
        vals = static_cast<const TypedDagNode<RbVector<MatrixReal> >* >( newP );
    }
    
}

