//
//  RateMatrix_Pomo.cpp
//
//  Created by Bastien Boussau on 4/4/14.
//  Copyright (c) 2014 Bastien Boussau. All rights reserved.
//

#include "RateMatrix_Pomo.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_Pomo::RateMatrix_Pomo(size_t n) : AbstractRateMatrix( n ){
    
    updateMatrix();
}


/** Copy constructor */
RateMatrix_Pomo::RateMatrix_Pomo(const RateMatrix_Pomo& m) : AbstractRateMatrix( m ) {
    
}


/** Destructor */
RateMatrix_Pomo::~RateMatrix_Pomo(void) {
    
}


RateMatrix_Pomo& RateMatrix_Pomo::operator=(const RateMatrix_Pomo &r) {
    
    if (this != &r)
    {
        RateMatrix::operator=( r );
    }
    
    return *this;
}

double RateMatrix_Pomo::averageRate(void) const
{
    return 1.0;
}

void RateMatrix_Pomo::buildRateMatrix(void) 
{
    
    // compute auxilliary variables
    double N2 = (double) (N*N);
    int Nminus1 = (int)N-1;
    for (size_t i = 0 ; i < 4; i++) 
    {
        mu[i][i] = 0.0;
    }
    
    // calculate the transition probabilities
    for (size_t i=0; i< 4; i++) 
    {
    //The first 4 states are the monomorphic states; we can't directly change from one into another one
        for (size_t j=0; j< 4; j++) 
        {
        (*theRateMatrix)[i][j] = 0.0;
        }
    }
    //Change from a monomorphic into a polymorphic state
    //(i.e. the first 4 lines in the matrix )
    //The 4..4+Nminus1 states are the AC matrix
    //Only 2 entries can differ from 0, (N-1)A and (N-1)C
    //(N-1)A can only come from monomorphic state A, i.e. i=0
    //(N-1)A is at the end of the submatrix, j=4+N-1
    (*theRateMatrix)[0][4 + Nminus1 - 1] = N2 * mu[0][1];
    //(N-1)C can only come from monomorphic state C, i.e. i=1
    //(N-1)C is at the begining of the submatrix, j=4+N
    (*theRateMatrix)[1][4] = N2 * mu[1][0];
    
    //The 4+Nminus1..4+2Nminus1 states are the AG matrix
    //Only 2 entries can differ from 0, (N-1)A and (N-1)G
    //(N-1)A can only come from monomorphic state A, i.e. i=0
    //(N-1)A is at the end of the submatrix, j=4+2*Nminus1
    (*theRateMatrix)[0][4 + 2*Nminus1 -1] = N2 * mu[0][2];
    //(N-1)G can only come from monomorphic state G, i.e. i=1
    //(N-1)G is at the begining of the submatrix, j=4+N
    (*theRateMatrix)[2][4 + Nminus1] = N2 * mu[2][0];

    //The 4+2Nminus1..4+3Nminus1 states are the AT matrix
    //Only 2 entries can differ from 0, (N-1)A and (N-1)T
    //(N-1)A can only come from monomorphic state A, i.e. i=0
    //(N-1)A is at the end of the submatrix, j=4+3*Nminus1
    (*theRateMatrix)[0][4 + 3*Nminus1 -1] = N2 * mu[0][3];
    //(N-1)T can only come from monomorphic state T, i.e. i=1
    //(N-1)T is at the begining of the submatrix, j=4+N
    (*theRateMatrix)[3][4 + 2*Nminus1] = N2 * mu[3][0];

    //The 4+3Nminus1..4+4Nminus1 states are the CG matrix
    //Only 2 entries can differ from 0, (N-1)C and (N-1)G
    //(N-1)C can only come from monomorphic state C, i.e. i=0
    //(N-1)C is at the end of the submatrix, j=4+4*Nminus1
    (*theRateMatrix)[1][4 + 4*Nminus1 -1] = N2 * mu[1][2];
    //(N-1)G can only come from monomorphic state G, i.e. i=1
    //(N-1)G is at the begining of the submatrix, j=4+N
    (*theRateMatrix)[2][4 + 3*Nminus1] = N2 * mu[2][1];

    //The 4+4Nminus1..4+5Nminus1 states are the CT matrix
    //Only 2 entries can differ from 0, (N-1)C and (N-1)T
    //(N-1)C can only come from monomorphic state C, i.e. i=0
    //(N-1)C is at the end of the submatrix, j=4+5*Nminus1
    (*theRateMatrix)[1][4 + 5*Nminus1 -1] = N2 * mu[1][3];
    //(N-1)T can only come from monomorphic state T, i.e. i=1
    //(N-1)T is at the begining of the submatrix, j=4+N
    (*theRateMatrix)[3][4 + 4*Nminus1] = N2 * mu[3][1];

    //The 4+5Nminus1..4+6Nminus1 states are the GT matrix
    //Only 2 entries can differ from 0, (N-1)G and (N-1)T
    //(N-1)G can only come from monomorphic state G, i.e. i=0
    //(N-1)G is at the end of the submatrix, j=4+5*Nminus1
    (*theRateMatrix)[2][4 + 6*Nminus1 -1] = N2 * mu[2][3];
    //(N-1)T can only come from monomorphic state T, i.e. i=1
    //(N-1)T is at the begining of the submatrix, j=4+N
    (*theRateMatrix)[3][4 + 5*Nminus1] = N2 * mu[3][2];


    //Now we move from a polymorphic state to a monomorphic state
    //(i.e. the first four columns in the matrix)
    //The 4..4+Nminus1 states are the AC matrix
    //Only 2 entries can differ from 0, (N-1)A going to mono A and (N-1)C going to mono C
    //(N-1)A can only go to monomorphic state A, i.e. j=0
    //(N-1)A is at the end of the submatrix, i=4+N-1
    double temp = (N-1)*(1+s[0]-s[1]);
    (*theRateMatrix)[4 + Nminus1 - 1][0] = temp / ( temp + 1) * (1) / N;
    //(N-1)C can only go to monomorphic state C, i.e. j=1
    //(N-1)C is at the begining of the submatrix, i=4
    temp = (N-1)*(1+s[1]-s[0]);
    (*theRateMatrix)[4 ][0] = temp / ( temp + 1) * (1) / N;

    //The 4+Nminus1..4+2Nminus1 states are the AG matrix
    //Only 2 entries can differ from 0, (N-1)A going to mono A and (N-1)G going to mono G
    //(N-1)A can only go to monomorphic state A, i.e. j=0
    //(N-1)A is at the end of the submatrix, i=4+2*(Nminus1)
    temp = (N-1)*(1+s[0]-s[2]);
    (*theRateMatrix)[4 + 2*Nminus1 - 1][0] = temp / ( temp + 1) * (1) / N;
    //(N-1)G can only go to monomorphic state G, i.e. j=1
    //(N-1)G is at the begining of the submatrix, i=4
    temp = (N-1)*(1+s[2]-s[0]);
    (*theRateMatrix)[4 + Nminus1 ][0] = temp / ( temp + 1) * (1) / N;

    //The 4+2Nminus1..4+3Nminus1 states are the AT matrix
    //Only 2 entries can differ from 0, (N-1)A going to mono A and (N-1)T going to mono T
    //(N-1)A can only go to monomorphic state A, i.e. j=0
    //(N-1)A is at the end of the submatrix, i=4+3*(Nminus1)
    temp = (N-1)*(1+s[0]-s[3]);
    (*theRateMatrix)[4 + 3*Nminus1 - 1][0] = temp / ( temp + 1) * (1) / N;
    //(N-1)T can only go to monomorphic state T, i.e. j=3
    //(N-1)T is at the begining of the submatrix, i=4+ 2*Nminus1
    temp = (N-1)*(1+s[3]-s[0]);
    (*theRateMatrix)[4 + 2*Nminus1 ][3] = temp / ( temp + 1) * (1) / N;

    //The 4+3Nminus1..4+4Nminus1 states are the CG matrix
    //Only 2 entries can differ from 0, (N-1)C going to mono C and (N-1)G going to mono G
    //(N-1)C can only go to monomorphic state C, i.e. j=1
    //(N-1)C is at the end of the submatrix, i=4+4*(Nminus1)
    temp = (N-1)*(1+s[1]-s[2]);
    (*theRateMatrix)[4 + 4*Nminus1 - 1][1] = temp / ( temp + 1) * (1) / N;
    //(N-1)G can only go to monomorphic state G, i.e. j=2
    //(N-1)G is at the begining of the submatrix, i=4
    temp = (N-1)*(1+s[2]-s[1]);
    (*theRateMatrix)[4 + 3*Nminus1 ][2] = temp / ( temp + 1) * (1) / N;
    
    //The 4+4Nminus1..4+5Nminus1 states are the CT matrix
    //Only 2 entries can differ from 0, (N-1)C going to mono C and (N-1)T going to mono T
    //(N-1)C can only go to monomorphic state C, i.e. j=1
    //(N-1)C is at the end of the submatrix, i=4+5*(Nminus1)
    temp = (N-1)*(1+s[1]-s[3]);
    (*theRateMatrix)[4 + 5*Nminus1 - 1][1] = temp / ( temp + 1) * (1) / N;
    //(N-1)T can only go to monomorphic state T, i.e. j=3
    //(N-1)T is at the begining of the submatrix, i=4
    temp = (N-1)*(1+s[3]-s[1]);
    (*theRateMatrix)[4 + 4*Nminus1 ][3] = temp / ( temp + 1) * (1) / N;

    //The 4+5Nminus1..4+6Nminus1 states are the GT matrix
    //Only 2 entries can differ from 0, (N-1)G going to mono G and (N-1)T going to mono T
    //(N-1)G can only go to monomorphic state G, i.e. j=2
    //(N-1)G is at the end of the submatrix, i=4+6*(Nminus1)
    temp = (N-1)*(1+s[2]-s[3]);
    (*theRateMatrix)[4 + 6*Nminus1 - 1][2] = temp / ( temp + 1) * (1) / N;
    //(N-1)T can only go to monomorphic state T, i.e. j=3
    //(N-1)T is at the begining of the submatrix, i=4
    temp = (N-1)*(1+s[3]-s[2]);
    (*theRateMatrix)[4 + 5*Nminus1 ][3] = temp / ( temp + 1) * (1) / N;

    
    //Now we need to fill the rest of the matrix, i.e. the B matrices along the diagonal.
    //In these B matrices, again most cells = 0.
    //The diagonal is such that it's 1 - (sum of the cells in the line)
    double cell1, cell2;
    //First, the B^N_AC matrix
    //The 4..4+Nminus1 states are the AC matrix
    //Cell 4,4 is its first cell.
    size_t firstCell = 4;
    //case i = 0: moving from 1A to 2As
    temp = 1+s[0]-s[1];
    (*theRateMatrix)[firstCell][firstCell+1] = temp / (temp + N - 1) * (N-1) / N;
        for (size_t i=1; i< N; i++) 
    {
        temp = i*(1+s[0]-s[1]);
        //Loosing one A, from iA to (i-1)A
        cell1 = (N - i) /  (temp + N - i) * i / N;
        //Gaining one A, from iA to (i+1)A
        cell2 = temp / (temp + N - i) * (N-i) / N;
        (*theRateMatrix)[firstCell+i][firstCell+i-1] = cell1;
        (*theRateMatrix)[firstCell+i][firstCell+i+1] = cell2;
        (*theRateMatrix)[firstCell+i][firstCell+i] = 1 - cell1 - cell2 - (*theRateMatrix)[firstCell+i][0] - (*theRateMatrix)[firstCell+i][1] - (*theRateMatrix)[firstCell+i][2] - (*theRateMatrix)[firstCell+i][3];
    }

    //Then, the B^N_AG matrix
    //The 4+Nminus1..4+2Nminus1 states are the AG matrix
    //Cell 4+N,4+N is its first cell.
    firstCell = 4+Nminus1;
    //case i = 0: moving from 1A to 2As
    temp = 1+s[0]-s[2];
    (*theRateMatrix)[firstCell][firstCell+1] = temp / (temp + N - 1) * (N-1) / N;
        for (size_t i=1; i< N; i++) 
    {
        temp = i*(1+s[0]-s[2]);
        //Loosing one A, from iA to (i-1)A
        cell1 = (N - i) /  (temp + N - i) * i / N;
        //Gaining one A, from iA to (i+1)A
        cell2 = temp / (temp + N - i) * (N-i) / N;
        (*theRateMatrix)[firstCell+i][firstCell+i-1] = cell1;
        (*theRateMatrix)[firstCell+i][firstCell+i+1] = cell2;
        (*theRateMatrix)[firstCell+i][firstCell+i] = 1 - cell1 - cell2 - (*theRateMatrix)[firstCell+i][0] - (*theRateMatrix)[firstCell+i][1] - (*theRateMatrix)[firstCell+i][2] - (*theRateMatrix)[firstCell+i][3];
    }

    //Then, the B^N_AT matrix
    //The 4+2Nminus1..4+3Nminus1 states are the AT matrix
    //Cell 4+N,4+N is its first cell.
    firstCell = 4+2*Nminus1;
    //case i = 0: moving from 1A to 2As
    temp = 1+s[0]-s[3];
    (*theRateMatrix)[firstCell][firstCell+1] = temp / (temp + N - 1) * (N-1) / N;
        for (size_t i=1; i< N; i++) 
    {
        temp = i*(1+s[0]-s[3]);
        //Loosing one A, from iA to (i-1)A
        cell1 = (N - i) /  (temp + N - i) * i / N;
        //Gaining one A, from iA to (i+1)A
        cell2 = temp / (temp + N - i) * (N-i) / N;
        (*theRateMatrix)[firstCell+i][firstCell+i-1] = cell1;
        (*theRateMatrix)[firstCell+i][firstCell+i+1] = cell2;
        (*theRateMatrix)[firstCell+i][firstCell+i] = 1 - cell1 - cell2 - (*theRateMatrix)[firstCell+i][0] - (*theRateMatrix)[firstCell+i][1] - (*theRateMatrix)[firstCell+i][2] - (*theRateMatrix)[firstCell+i][3];
    }

    //Then, the B^N_CG matrix
    //The 4+3Nminus1..4+4Nminus1 states are the CG matrix
    //Cell 4+N,4+N is its first cell.
    firstCell = 4+3*Nminus1;
    //case i = 0: moving from 1C to 2Cs
    temp = 1+s[1]-s[2];
    (*theRateMatrix)[firstCell][firstCell+1] = temp / (temp + N - 1) * (N-1) / N;
        for (size_t i=1; i< N; i++) 
    {
        temp = i*(1+s[1]-s[2]);
        //Loosing one C, from iC to (i-1)C
        cell1 = (N - i) /  (temp + N - i) * i / N;
        //Gaining one C, from iC to (i+1)C
        cell2 = temp / (temp + N - i) * (N-i) / N;
        (*theRateMatrix)[firstCell+i][firstCell+i-1] = cell1;
        (*theRateMatrix)[firstCell+i][firstCell+i+1] = cell2;
        (*theRateMatrix)[firstCell+i][firstCell+i] = 1 - cell1 - cell2 - (*theRateMatrix)[firstCell+i][0] - (*theRateMatrix)[firstCell+i][1] - (*theRateMatrix)[firstCell+i][2] - (*theRateMatrix)[firstCell+i][3];
    }

    //Then, the B^N_CT matrix
    //The 4+4Nminus1..4+5Nminus1 states are the CT matrix
    //Cell 4+N,4+N is its first cell.
    firstCell = 4+4*Nminus1;
    //case i = 0: moving from 1C to 2Cs
    temp = 1+s[1]-s[3];
    (*theRateMatrix)[firstCell][firstCell+1] = temp / (temp + N - 1) * (N-1) / N;
        for (size_t i=1; i< N; i++) 
    {
        temp = i*(1+s[1]-s[3]);
        //Loosing one C, from iC to (i-1)C
        cell1 = (N - i) /  (temp + N - i) * i / N;
        //Gaining one C, from iC to (i+1)C
        cell2 = temp / (temp + N - i) * (N-i) / N;
        (*theRateMatrix)[firstCell+i][firstCell+i-1] = cell1;
        (*theRateMatrix)[firstCell+i][firstCell+i+1] = cell2;
        (*theRateMatrix)[firstCell+i][firstCell+i] = 1 - cell1 - cell2 - (*theRateMatrix)[firstCell+i][0] - (*theRateMatrix)[firstCell+i][1] - (*theRateMatrix)[firstCell+i][2] - (*theRateMatrix)[firstCell+i][3];
    }

    //Then, the B^N_GT matrix
    //The 4+5Nminus1..4+6Nminus1 states are the GT matrix
    //Cell 4+N,4+N is its first cell.
    firstCell = 4+5*Nminus1;
    //case i = 0: moving from 1G to 2Gs
    temp = 1+s[2]-s[3];
    (*theRateMatrix)[firstCell][firstCell+1] = temp / (temp + N - 1) * (N-1) / N;
        for (size_t i=1; i< N; i++) 
    {
        temp = i*(1+s[2]-s[3]);
        //Loosing one G, from iG to (i-1)G
        cell1 = (N - i) /  (temp + N - i) * i / N;
        //Gaining one G, from iG to (i+1)G
        cell2 = temp / (temp + N - i) * (N-i) / N;
        (*theRateMatrix)[firstCell+i][firstCell+i-1] = cell1;
        (*theRateMatrix)[firstCell+i][firstCell+i+1] = cell2;
        (*theRateMatrix)[firstCell+i][firstCell+i] = 1 - cell1 - cell2 - (*theRateMatrix)[firstCell+i][0] - (*theRateMatrix)[firstCell+i][1] - (*theRateMatrix)[firstCell+i][2] - (*theRateMatrix)[firstCell+i][3];
    }
}

/** Calculate the transition probabilities */
void RateMatrix_Pomo::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const {
     
    
    //Now the instantaneous rate matrix has been filled up entirely.
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    computeExponentialMatrixByRepeatedSquaring(t, P);
    
    return;
}

void RateMatrix_Pomo::computeExponentialMatrixByRepeatedSquaring(double t,  TransitionProbabilityMatrix& P ) const {
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    //Ideally one should dynamically decide how many squarings are necessary. 
    //For the moment, we arbitrarily do 10 such squarings, as it seems to perform well in practice (N. Lartillot, personal communication).
    //first, multiply the matrix by the right scalar
    //2^10 = 1024
    double tOver2s = t/(1024);
    for ( size_t i = 0; i < matrixSize; i++ ) {
        for ( size_t j = 0; j < matrixSize; j++ ) {
            P[i][j] = (*theRateMatrix)[i][j] * tOver2s; 
        }
    }
    //Add the identity matrix:
     for ( size_t i = 0; i < matrixSize; i++ ) {
         P[i][i] += 1;
     }
     //Now we can do the multiplications
     TransitionProbabilityMatrix P2 (matrixSize);
     squareMatrix (P, P2); //P2 at power 2
     squareMatrix (P2, P); //P at power 4
     squareMatrix (P, P2); //P2 at power 8
     squareMatrix (P2, P); //P at power 16
     squareMatrix (P, P2); //P2 at power 32
     squareMatrix (P2, P); //P at power 64
     squareMatrix (P, P2); //P2 at power 128
     squareMatrix (P2, P); //P at power 256
     squareMatrix (P, P2); //P2 at power 512
     squareMatrix (P2, P); //P at power 1024

     return;
}

inline void RateMatrix_Pomo::squareMatrix( TransitionProbabilityMatrix& P,  TransitionProbabilityMatrix& P2) const {
    //Could probably use boost::ublas here, for the moment we do it ourselves.
    for ( size_t i = 0; i < matrixSize; i++ ) {
        for ( size_t j = 0; j < matrixSize; j++ ) {
            P2.getElement ( i, j ) = 0;
            for ( size_t k = 0; k < matrixSize; k++ ) {
                P2.getElement ( i, j ) += P.getElement ( i, k ) * P.getElement ( k, j );
                }
            }
        }
    }



RateMatrix_Pomo* RateMatrix_Pomo::clone( void ) const {
    return new RateMatrix_Pomo( *this );
}

const std::vector<double>& RateMatrix_Pomo::getStationaryFrequencies( void ) const {
    
    return stationaryFreqs;
}


void RateMatrix_Pomo::updateMatrix( void ) {
    
    if ( needsUpdate )
    {
        buildRateMatrix();
        // clean flags
        needsUpdate = false;
    }
}


void RateMatrix_Pomo::setMutationRates(const std::vector<double>& mr) {
    mu[0][1] = mr[0];
    mu[0][2] = mr[1];
    mu[0][3] = mr[2];
    mu[1][0] = mr[3];
    mu[1][2] = mr[4];
    mu[1][3] = mr[5];
    mu[2][0] = mr[6];
    mu[2][1] = mr[7];
    mu[2][3] = mr[8];
    mu[3][0] = mr[9];
    mu[3][1] = mr[10];
    mu[3][2] = mr[11];
}


void RateMatrix_Pomo::setSelectionCoefficients(const std::vector<double>& sc){
    s = sc;

}
