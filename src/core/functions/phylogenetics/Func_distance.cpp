/**
 * @file
 * This file contains the implementation of Func_distance, a class
 * used to construct a distance matrix.
 *
 * @brief Implementation of Func_distance
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

#include "CharacterData.h"
#include "DistanceMatrix.h"
#include "DnaState.h"
#include "Func_distance.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "Real.h"
#include "RnaState.h"
#include "StringUtilities.h"
#include "TaxonData.h"
#include "UserInterface.h"
#include "ValueRule.h"
#include "VectorReal.h"
#include "VectorString.h"
#include <cmath>
#include <sstream>
#include <vector>


// Definition of the static type spec member
const TypeSpec Func_distance::typeSpec(Func_distance_name);
const TypeSpec Func_distance::returnTypeSpec(DistanceMatrix_name);

/** Clone object */
Func_distance* Func_distance::clone(void) const {

    return new Func_distance( *this );
}


/** Execute function */
RbLanguageObject* Func_distance::executeFunction(void) {

    // get the information from the arguments for reading the file
    CharacterData* m      = static_cast<CharacterData*>( (*args)[0]->getValue() );
    RbString*      dName  = static_cast<RbString*>     ( (*args)[1]->getValue() );
    RbString*      freqs  = static_cast<RbString*>     ( (*args)[2]->getValue() );
    RbString*      asrv   = static_cast<RbString*>     ( (*args)[3]->getValue() );
    Real*          shape  = static_cast<Real*>         ( (*args)[4]->getValue() );
    Real*          pinvar = static_cast<Real*>         ( (*args)[5]->getValue() );
        
    // check that the data matrix is aligned
    if ( m->getIsHomologyEstablished() == false )
        throw( RbException("Data is not aligned") );

    // check that we have DNA or RNA sequences
    if  ( !(m->getDataType() == DnaState_name || m->getDataType() == RnaState_name) )
        throw( RbException("Data must be DNA or RNA") );
        
    // determine the distance model
    enum distanceModel { p_dist, jc69, f81, tn93, k2p, hky85, k3p, gtr, logdet };
    distanceModel myModel;
    std::string distanceStr = dName->getValue();
    StringUtilities::toLower(distanceStr);
    if ( distanceStr == "p" )
        myModel = p_dist;
    else if ( distanceStr == "jc69" )
        myModel = jc69;
    else if ( distanceStr == "f81" )
        myModel = f81;
    else if ( distanceStr == "tn93" )
        myModel = tn93;
    else if ( distanceStr == "k2p" )
        myModel = k2p;
    else if ( distanceStr == "hky85" )
        myModel = hky85;
    else if ( distanceStr == "k3p" )
        myModel = k3p;
    else if ( distanceStr == "gtr" )
        myModel = gtr;
    else if ( distanceStr == "logdet" )
        myModel = logdet;
    else
        throw( RbException("Unknown distance model \"" + distanceStr + "\"") );
    
    // get the dimensions of the distance matrix
    int n = (int)m->getNumberOfTaxa();

    // allocate the distance matrix
    DistanceMatrix* d = new DistanceMatrix(n);

    // fill in the distance matrix
    for (int i=0; i<n; i++)
        {
        const TaxonData* td_i = m->getTaxonData(i);
        for (int j=i+1; j<n; j++)
            {
            const TaxonData* td_j = m->getTaxonData(j);
            
            double dist = 0.0;
            if (myModel == p_dist)
                dist = distanceP(td_i, td_j);
            else if (myModel == jc69)
                dist = distanceJC69(td_i, td_j);
            else if (myModel == f81)
                dist = distanceJC69(td_i, td_j);
            else if (myModel == tn93)
                dist = distanceJC69(td_i, td_j);
            else if (myModel == k2p)
                dist = distanceJC69(td_i, td_j);
            else if (myModel == hky85)
                dist = distanceJC69(td_i, td_j);
            else if (myModel == k3p)
                dist = distanceJC69(td_i, td_j);
            else if (myModel == gtr)
                dist = distanceJC69(td_i, td_j);
            else if (myModel == logdet)
                dist = distanceJC69(td_i, td_j);
            (*(*d)[i])[j] = dist;
            (*(*d)[j])[i] = dist;
            }
        }
        
    // fill in the taxa
    for (int i=0; i<n; i++)
        {
        std::string tName = m->getTaxonNameWithIndex(i);
        d->addTaxonWithName(tName);
        }

    return d;
}


double Func_distance::distanceJC69(const TaxonData* td1, const TaxonData* td2) {

    int numDiff = 0;
    for (int c=0; c<td1->getNumberOfCharacters(); c++)
        {
        const Character* c1 = td1->getCharacter(c);
        const Character* c2 = td2->getCharacter(c);
        unsigned a = c1->getUnsignedValue();
        unsigned b = c2->getUnsignedValue();
        if (a != b)
            numDiff++;
        }
    double p = (double)numDiff / td1->getNumberOfCharacters();
    if (p >= 0.75)
        return 10.0;
    return -0.75 * log(1.0 - (4.0/3.0)*p);
}


double Func_distance::distanceP(const TaxonData* td1, const TaxonData* td2) {

    int numDiff = 0;
    for (int c=0; c<td1->getNumberOfCharacters(); c++)
        {
        const Character* c1 = td1->getCharacter(c);
        const Character* c2 = td2->getCharacter(c);
        unsigned a = c1->getUnsignedValue();
        unsigned b = c2->getUnsignedValue();
        if (a != b)
            numDiff++;
        }
    double p = (double)numDiff / td1->getNumberOfCharacters();
    return p;
}

/** Get argument rules */
const ArgumentRules* Func_distance::getArgumentRules(void) const {

    static ArgumentRules* argumentRules = new ArgumentRules();
    static bool rulesSet = false;

    if (!rulesSet)
        {
        argumentRules->push_back( new ValueRule( "data",   CharacterData_name ) );
        argumentRules->push_back( new ValueRule( "model",  RbString_name      ) );
        argumentRules->push_back( new ValueRule( "freqs",  RbString_name      ) );
        argumentRules->push_back( new ValueRule( "asrv",   RbString_name      ) );
        argumentRules->push_back( new ValueRule( "shape",  Real_name          ) );
        argumentRules->push_back( new ValueRule( "pinvar", Real_name          ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_distance::getClass(void) const {

    static VectorString rbClass = VectorString( Func_distance_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_distance::getReturnType(void) const {

    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_distance::getTypeSpec(void) const {

    return typeSpec;
}

