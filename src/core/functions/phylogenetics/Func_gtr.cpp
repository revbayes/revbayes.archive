/**
 * @file
 * This file contains the implementation of Func_gtr, a class
 * used to construct a GTR model of DNA substitution.
 *
 * @brief Implementation of Func_gtr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "Func_gtr.h"
#include "RateMatrix.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "Simplex.h"
#include "UserInterface.h"
#include "ValueRule.h"
#include "VectorReal.h"
#include "VectorString.h"
#include <sstream>
#include <vector>


// Definition of the static type spec member
const TypeSpec Func_gtr::typeSpec(Func_gtr_name);
const TypeSpec Func_gtr::returnTypeSpec(RateMatrix_name);

/** Clone object */
Func_gtr* Func_gtr::clone(void) const {

    return new Func_gtr( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_gtr::executeFunction(void) {

    // get the information from the arguments for reading the file
    RbPtr<Simplex> r( static_cast<Simplex*>( (RbObject*)(*args)[0]->getValue() ) );
    RbPtr<Simplex> f( static_cast<Simplex*>( (RbObject*)(*args)[1]->getValue() ) );

    // initialize the number of states
    const size_t nStates = 4;

    // check the sizes of the simplices, to make certain that they are consistent
    // with a model with nStates states
    if ( f->size() != nStates )
        {
        std::stringstream o;
        o << "The simplex containing the state frequencies is not of size ";
        o << nStates;
        throw( RbException(o.str()) );
        }
    if (r->size() != nStates*(nStates-1)/2)
        {
        std::stringstream o;
        o << "The simplex containing the rates is not of size (";
        o << nStates;
        o << " choose 2) = ";
        o << nStates*(nStates-1)/2;
        throw( RbException(o.str()) );
        }

    // construct a rate matrix of the correct dimensions
    RbPtr<RateMatrix> m( new RateMatrix(nStates) );

    // set the off-diagonal portions of the rate matrix
    for (size_t i=0, k=0; i<nStates; i++)
        {
        for (size_t j=i+1; j<nStates; j++)
            {
            (*(*m)[i])[j] = (*r)[k] * (*f)[j];
            (*(*m)[j])[i] = (*r)[k] * (*f)[i];
            k++;
            }
        }

    // set the diagonal elements of the rate matrix
    m->setDiagonal();

    // Set the stationary frequencies for the rate matrix. Note that we
    // can do this in two ways. First, we can call calculateStationaryFrequencies
    // on the RateMatrix object. This function call calculates the stationary
    // frequencies using only knowledge of the rate matrix. Second, we can set
    // the stationary frequencies directly. This is what we do here, because the
    // stationary frequencies have been build directly into the rate matrix.
    std::vector<double> tempFreqs = f->getValue();
    m->setStationaryFrequencies(tempFreqs);

    // rescale the rate matrix such that the average rate is 1.0
    m->rescaleToAverageRate(1.0);

    // we know that the GTR model is time reversible (just look at the name of the
    // model!), so we might as well set its reversibility flag directly
    m->setIsTimeReversible(true);

    // Now that we have set the rate matrix, we should update its eigen system
    m->updateEigenSystem();

    // wrap up the rate matrix object and send it on its way to parser-ville
    return RbPtr<RbLanguageObject>( m );
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_gtr::getArgumentRules(void) const {

    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;

    if (!rulesSet)
        {
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "rates", Simplex_name ) ) );
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "freqs", Simplex_name ) ) );
        rulesSet = true;
        }

    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
const VectorString& Func_gtr::getClass(void) const {

    static VectorString rbClass = VectorString( Func_gtr_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_gtr::getReturnType(void) const {

    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_gtr::getTypeSpec(void) const {
    return typeSpec;
}

