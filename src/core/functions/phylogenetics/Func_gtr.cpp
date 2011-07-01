/**
 * @file
 * This file contains the implementation of Func_gtr.
 *
 * @brief Implementation of Func_readAlingment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id:$
 */

#include "Func_gtr.h"
#include "List.h"
#include "RateMatrix.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "Simplex.h"
#include "UserInterface.h"
#include "ValueContainer.h"
#include "ValueRule.h"
#include "VectorReal.h"
#include "VectorString.h"
#include <sstream>
#include <vector>



/** Clone object */
Func_gtr* Func_gtr::clone( void ) const {
    
    return new Func_gtr( *this );
}


/** Execute function */
DAGNode* Func_gtr::execute( void ) {

    // get the information from the arguments for reading the file
    //const Simplex* r = static_cast<const Simplex*>( args[0].getValue() );
    //const Simplex* f = static_cast<const Simplex*>( args[1].getValue() );
    
    // TEMP: Initialize two simplices that mimic the simplices that are passed
    // into the function
    std::vector<double> vF, vR;
    for (size_t i=0; i<4; i++)
        vF.push_back( (double)(i+1) );
    for (size_t i=0; i<6; i++)
        vR.push_back( 1.0/6.0 );
    Simplex f(vF);
    Simplex r(vR);
    
    // initialize the number of states
    const size_t nStates = 4;
    
    // check the sizes of the simplices, to make certain that they are consistent
    // with a model with nStates states
    if ( f.getElementsSize() != nStates )
        {
        std::stringstream o;
        o << "The simplex containing the state frequencies is not of size ";
        o << nStates;
        throw( RbException(o.str()) );
        }
    if (r.getElementsSize() != nStates*(nStates-1)/2)
        {
        std::stringstream o;
        o << "The simplex containing the rates is not of size ";
        o << nStates;
        o << " choose 2 = ";
        o << nStates*(nStates-1)/2;
        throw( RbException(o.str()) );
        }
    
    // construct a rate matrix of the correct dimensions
    RateMatrix* m = new RateMatrix(nStates);
   
    // set the off-diagonal portions of the rate matrix
    for (size_t i=0, k=0; i<nStates; i++)
        {
        for (size_t j=i+1; j<nStates; j++)
            {
            (*m)[i][j] = r[k] * f[j];
            (*m)[j][i] = r[k] * f[i];
            k++;
            }
        }
        
    // set the diagonal elements of the rate matrix
    m->setDiagonal();
    
    // set the stationary frequencies for the rate matrix
    m->calculateStationaryFrequencies();
    
    std::cout << m->richInfo() << std::endl;

    // wrap up the rate matrix object and send it on its way
    return m->wrapIntoVariable();
}


/** Get argument rules */
const ArgumentRules& Func_gtr::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules;
    static bool          rulesSet = false;
    
    if (!rulesSet) 
        {
        //argumentRules.push_back( new ValueRule( "rates", Simplex_name ) );
        //argumentRules.push_back( new ValueRule( "freqs", Simplex_name ) );
        rulesSet = true;
        }
            
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_gtr::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_gtr_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_gtr::getReturnType( void ) const {
    
    return TypeSpec( RbNULL_name );
}


