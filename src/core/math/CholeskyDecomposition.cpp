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
    L.setCholesky(false);
    
    
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
    // TODO:: clear out inverse matrix just to be safe
    inverseMatrix = inverseLowerFactorTranspose * inverseLowerFactor;
    
}

void CholeskyDecomposition::decomposeMatrix( void )
{
    
    // TODO: clear and remake the L matrix here, just to be safe.
    // TODO: check sqrt(R+)
    
    for (size_t r = 0; r < n; ++r)
    {
        for (size_t c = 0; c <= r; ++c)
        {
//            std::cout << r << " -- " << c;
            if (c == r)
            {
                double sum = 0.0;
                for (size_t j = 0; j < c; ++j)
                {
                    sum += L[c][j] * L[c][j];
                }
                L[c][c] = std::sqrt((*qPtr)[c][c] - sum);
//                std::cout << " -- diagonal -- " << (*qPtr)[c][c] << " -- " << L[c][c] << " -- " << sum << std::endl;
            }
            else
            {
                double sum = 0.0;
                for (size_t j = 0; j < c; ++j)
                {
                    sum += L[r][j] * L[c][j];
                }
                L[r][c] = 1.0 / L[c][c] * ( (*qPtr)[r][c] - sum );
//                std::cout << " -- off-diagonal -- " << (*qPtr)[r][c] << " -- " << L[r][c] << " -- " << sum << std::endl;
            }
        }
    }
    
//    double[,] ret = new double[n, n];
//    for (int r = 0; r < n; r++)
//        for (int c = 0; c <= r; c++)
//        {
//            if (c == r)
//            {
//                double sum = 0;
//                for (int j = 0; j < c; j++)
//                {
//                    sum += ret[c, j] * ret[c, j];
//                }
//                ret[c, c] = Math.Sqrt(a[c, c] - sum);
//            }
//            else
//            {
//                double sum = 0;
//                for (int j = 0; j < c; j++)
//                    sum += ret[r, j] * ret[c, j];
//                ret[r, c] = 1.0 / ret[c, c] * (a[r, c] - sum);
//            }
//        }
//    
//    return ret;
    
//    for (size_t i = 0; i < n; ++i)
//    {
//        std::vector<double> thisRowI = L[i];
//        double d = 0.0;
//
//        for (size_t j = 0; j < n; ++j)
//        {
//            std::vector<double> thisRowJ = L[j];
//            double s = 0.0;
//            for (size_t k = 0; k < j; ++k)
//            {
//                s += thisRowJ[k] * thisRowI[k];
//            }
//            s = ((*qPtr)[i][j] - s) / L[j][j];
//            thisRowI[j] = s;
//            d = d + s * s;
//            isSemiPositiveDefinite = isSemiPositiveDefinite & ( (*qPtr)[i][j] == (*qPtr)[j][i] );
//        }
//        
//        d = (*qPtr)[i][i] - d;
//        isSemiPositiveDefinite = isSemiPositiveDefinite & (d > 0.0);
//        
//        L[i][i] = d > 0.0 ? sqrt(d) : 0.0;
//        
//    }

}

void CholeskyDecomposition::update( void )
{
    
    decomposeMatrix();
    computeInverse();
    
}
