// /**
//  * @file
//  * This file contains the declaration of SBNParameters, which is
//  * class that holds the parameters for an SBN.
//  *
//  * @brief Implementation of SBNParameters
//  *
//  * (c) Copyright 2009- under GPL version 3
//  * @date Last modified: $Date$
//  * @author The RevBayes Development Core Team
//  * @license GPL version 3
//  *
//  * $Id$
//  */
//
// #include "RbException.h"
// #include "SBNParameters.h"
//
// #include <cmath>
// #include <iomanip>
//
//
// using namespace RevBayesCore;
//
//
// /** Construct rate matrix with n states */
// SBNParameters::SBNParameters(size_t n) : nElements( n*n )
// {
//
//     theMatrix = new double[ nElements ];
//     for ( size_t i = 0; i < nElements; ++i)
//     {
//         theMatrix[i] = 0.0;
//     }
//
//     num_states = n;
// }
//
// /** Construct rate matrix with n states */
// SBNParameters::SBNParameters( const SBNParameters &tpm ) :
//     num_states( tpm.num_states ),
//     nElements( tpm.nElements )
// {
//
//     theMatrix = new double[ nElements ];
//     for ( size_t i = 0; i < nElements; ++i)
//     {
//         theMatrix[i] = tpm.theMatrix[i];
//     }
//
// }
//
//
// SBNParameters::~SBNParameters()
// {
//
//     delete [] theMatrix;
//
// }
//
//
// /** Construct rate matrix with n states */
// SBNParameters& SBNParameters::operator=( const SBNParameters &tpm ) {
//
//     if ( this != &tpm )
//     {
//         nElements = tpm.nElements;
//         num_states = tpm.num_states;
//
//         delete [] theMatrix;
//         theMatrix = new double[ nElements ];
//         for ( size_t i = 0; i < nElements; ++i)
//         {
//             theMatrix[i] = tpm.theMatrix[i];
//         }
//     }
//
//     return *this;
// }
//
//
// /** Index operator (const) */
// const double* SBNParameters::operator[]( const size_t i ) const {
//
//     return theMatrix + i*num_states;
// }
//
//
// /** Index operator */
// double* SBNParameters::operator[]( const size_t i ) {
//
//     return theMatrix + i*num_states;
// }
//
// SBNParameters& SBNParameters::operator*=(const SBNParameters& B) {
//
//     SBNParameters C(num_states);
//     for (size_t i=0; i<num_states; i++)
//     {
//         for (size_t j=0; j<num_states; j++)
//         {
//             double sum = 0.0;
//             for (size_t k=0; k<num_states; k++)
//                 sum += (*this)[i][k] * B[k][j];
//             C[i][j] = sum;
//         }
//     }
//
//     for (size_t i=0; i<num_states*num_states; i++)
//         theMatrix[i] = C.theMatrix[i];
//
// 	return *this;
// }
//
// double SBNParameters::getElement(size_t i, size_t j) const {
//
//     return *(theMatrix + num_states*i + j);
// }
//
//
// double& SBNParameters::getElement(size_t i, size_t j) {
//
//     return *(theMatrix + num_states*i + j);
// }
//
//
// const double* SBNParameters::getElements( void ) const {
//
//     return theMatrix;
// }
//
//
// double* SBNParameters::getElements( void ) {
//
//     return theMatrix;
// }
//
//
// size_t SBNParameters::getNumberOfStates( void ) const {
//
//     return num_states;
// }
//
//
// size_t SBNParameters::size(void) const {
//
//     return nElements;
// }
//
//
// std::ostream& RevBayesCore::operator<<(std::ostream& o, const SBNParameters& x) {
//
//     std::streamsize previousPrecision = o.precision();
//     std::ios_base::fmtflags previousFlags = o.flags();
//
//     o << "[ ";
//     o << std::fixed;
//     o << std::setprecision(4);
//
//     // print the RbMatrix with each column of equal width and each column centered on the decimal
//     for (size_t i=0; i < x.getNumberOfStates(); i++)
//     {
//         if (i == 0)
//             o << "[ ";
//         else
//             o << "  ";
//
//         for (size_t j = 0; j < x.getNumberOfStates(); ++j)
//         {
//             if (j != 0)
//                 o << ", ";
//             o << x[i][j];
//         }
//         o <<  " ]";
//
//         if (i == x.size()-1)
//             o << " ]";
//         else
//             o << " ,\n";
//
//     }
//
//     o.setf(previousFlags);
//     o.precision(previousPrecision);
//
//     return o;
// }
