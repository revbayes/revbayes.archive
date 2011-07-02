/**
 * @file
 * This file contains the implementation of EigenSystem, which is
 * used to contain the eigen values and eigen vectors of a real matrix.
 *
 * @brief Implementation of EigenSystem
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id:$
 */

#include "EigenSystem.h"



/** Constructor from real matrix */
EigenSystem::EigenSystem(const MatrixReal& m) {

	// check that the matrix, m, is square and return
	// an empty eigensystem if it is not
	if ( m.getNumRows() != m.getNumCols() )
		return;
	
	// set the dimensions of the matrix
	n = m.getNumRows();

	// allocate and initialize components of eigensystem
	// assuming it is going to be real and not complex
	eigenvectors         = MatrixReal(n, n, 0.0);
	inverseEigenvectors  = MatrixReal(n, n, 0.0);
	realEigenvalues      = VectorReal(n, 0.0);
	imaginaryEigenvalues = VectorComplex(n, 0.0);

	// calculate eigenvalues and eigenvectors for a real matrix
	//update (m);
}


/** Destructor */
EigenSystem::~EigenSystem(void) {

}


/** Return determinant */
double EigenSystem::getDeterminant(void) {

}

