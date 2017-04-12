#ifndef CholeskyDecomposition_H
#define CholeskyDecomposition_H

#include "MatrixComplex.h"
#include "MatrixReal.h"

#include <complex>


namespace RevBayesCore {

    class CholeskyDecomposition {

    public:
                                                CholeskyDecomposition(const MatrixReal* m);

        void                                    update(void);
        const MatrixReal                        getInverse(void) const { return inverseMatrix; }
        const MatrixReal                        getMatrix(void) const { return L; }

    private:

        void                                    computeInverse(void);
        void                                    decomposeMatrix(void);
        
        size_t                                  n;                                              //!< Row and column dimension (square matrix)
        const MatrixReal*                       qPtr;                                           //!< A pointer to the matrix for this cholesky decomposition
        MatrixReal                              L;
        MatrixReal                              inverseMatrix;
        bool                                    isSemiPositiveDefinite;

    };
    
}

#endif
