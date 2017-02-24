#include "CholeskyDecomposition.h"
#include "MatrixReal.h"

#include <assert.h>
#include <vector>

using namespace RevBayesCore;

/** Constructor from real matrix */
CholeskyDecomposition::CholeskyDecomposition( const MatrixReal* m )
{
    
    // check that the matrix, m, is square and return
    // an empty CholeskyDecomposition if it is not
    if ( m->getNumberOfRows() != m->getNumberOfColumns() )
    {
        throw RbException("Matrix must be square for Cholesky decomposition.");
    }
    
    // set the pointer to the matrix
    qPtr = m;
    
	// set the dimensions of the matrix
	n = m->getNumberOfRows();
    
    // initialize the decomposed and inverted matrices
    L = MatrixReal(n, n, 0.0);
    inverseMatrix = MatrixReal(n, n, 0.0);
    
    // update the decomposition
    update();
 
}

void CholeskyDecomposition::computeInverse( void )
{
    
    // first, invert the lower cholesky factor
    MatrixReal inverseLowerFactor = L.computeInverse();
    
    // now, transpose the inverse lower factor
    MatrixReal inverseLowerFactorTranspose(n, n, 0.0);
    for (size_t r = 0; r < n; ++r)
    {
        for (size_t c = 0; c <= r; ++c)
        {
            inverseLowerFactorTranspose[c][r] = inverseLowerFactor[r][c];
        }
    }
    
    // now, multiply the two matrices together
    inverseMatrix = inverseLowerFactorTranspose * inverseLowerFactor;
//    inverseMatrix.getLogDet(); // this is just for debugging purposes: I want to force the matrix to update
    
}

void CholeskyDecomposition::decomposeMatrix( void )
{
    
    // TODO: check sqrt(R+)
    // sometimes we might accidentally square root a small negative number
    
    L.clear();
    L.resize(n, n);

    for (size_t r = 0; r < n; ++r)
    {
        for (size_t c = 0; c <= r; ++c)
        {
            if (c == r)
            {
                double sum = 0.0;
                for (size_t j = 0; j < c; ++j)
                {
                    sum += L[c][j] * L[c][j];
                }
                L[c][c] = std::sqrt((*qPtr)[c][c] - sum);
            }
            else
            {
                double sum = 0.0;
                for (size_t j = 0; j < c; ++j)
                {
                    sum += L[r][j] * L[c][j];
                }
                L[r][c] = 1.0 / L[c][c] * ( (*qPtr)[r][c] - sum );
            }
        }
    }

}

void CholeskyDecomposition::update( void )
{
    
    decomposeMatrix();
    computeInverse();
    
}
