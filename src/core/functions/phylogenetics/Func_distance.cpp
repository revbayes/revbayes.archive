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

#include "ArgumentRule.h"
#include "CharacterData.h"
#include "DistanceMatrix.h"
#include "DnaState.h"
#include "Func_distance.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "Real.h"
#include "RlCharacterData.h"
#include "RlDnaState.h"
#include "RnaState.h"
#include "StringUtilities.h"
#include "TaxonData.h"
#include "UserInterface.h"
#include <cmath>
#include <sstream>
#include <vector>


Func_distance::Func_distance(void) : RbFunction() {
    
}


/** Clone object */
Func_distance* Func_distance::clone(void) const {

    return new Func_distance( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_distance::executeFunction( const std::vector<const RbObject*> &args ) {

    // get the information from the arguments for reading the file
    const CharacterData& m      = static_cast<const RlCharacterData&>( *args[0] ).getValue();
    const RbString&      dName  = static_cast<const RbString&>     ( *args[1] );
    const RbString&      f      = static_cast<const RbString&>     ( *args[2] );
    const RbString&      a      = static_cast<const RbString&>     ( *args[3] );
    const Real&          s      = static_cast<const Real&>         ( *args[4] );
    const Real&          pi     = static_cast<const Real&>         ( *args[5] );
        
    // check that the data matrix is aligned
    if ( m.getIsHomologyEstablished() == false )
        throw( RbException("Data is not aligned") );

    // check that we have DNA or RNA sequences
//    if  ( !(m.getDataType() == RlDnaState::getClassName() || m.getDataType() == RnaState::getClassName() ) )
//        throw( RbException("Data must be DNA or RNA") );
        
    // determine the distance model
    enum distanceModel { p_dist, jc69, f81, tn93, k2p, hky85, k3p, gtr, logdet };
    distanceModel myModel;
    std::string distanceStr = dName.getValue();
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
    int n = (int)m.getNumberOfTaxa();

    // allocate the distance matrix
    DistanceMatrix *matrix = new DistanceMatrix(n);

    // fill in the distance matrix
    for (int i=0; i<n; i++)
        {
        const TaxonData& td_i = m.getTaxonData(i);
        for (int j=i+1; j<n; j++)
            {
            const TaxonData& td_j = m.getTaxonData(j);
            
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
            (*matrix)[i][j] = dist;
            (*matrix)[j][i] = dist;
            }
        }
        
    // fill in the taxa
    for (int i=0; i<n; i++)
        {
        std::string tName = m.getTaxonNameWithIndex(i);
        matrix->addTaxonWithName(tName);
        }

    return RbPtr<RbLanguageObject>( matrix );
}


double Func_distance::distanceJC69(const TaxonData& td1, const TaxonData& td2) {

    int numDiff = 0;
    for (int c=0; c<td1.getNumberOfCharacters(); c++)
        {
        const CharacterState& c1 = td1.getCharacter(c);
        const CharacterState& c2 = td2.getCharacter(c);
        if (c1 != c2)
            numDiff++;
        }
    double p = (double)numDiff / td1.getNumberOfCharacters();
    if (p >= 0.75)
        return 10.0;
    return -0.75 * log(1.0 - (4.0/3.0)*p);
}


double Func_distance::distanceP(const TaxonData& td1, const TaxonData& td2) {

    int numDiff = 0;
    for (int c=0; c<td1.getNumberOfCharacters(); c++)
        {
        const CharacterState& c1 = td1.getCharacter(c);
        const CharacterState& c2 = td2.getCharacter(c);
        if (c1 != c2)
            numDiff++;
        }
    double p = (double)numDiff / td1.getNumberOfCharacters();
    return p;
}

/** Get argument rules */
const ArgumentRules& Func_distance::getArgumentRules(void) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;

    if (!rulesSet)
        {
        argumentRules.push_back( new ArgumentRule( "data",   true, RlCharacterData::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "model",  true, RbString::getClassTypeSpec()      ) );
        argumentRules.push_back( new ArgumentRule( "freqs",  true, RbString::getClassTypeSpec()      ) );
        argumentRules.push_back( new ArgumentRule( "asrv",   true, RbString::getClassTypeSpec()      ) );
        argumentRules.push_back( new ArgumentRule( "shape",  true, Real::getClassTypeSpec()          ) );
        argumentRules.push_back( new ArgumentRule( "pinvar", true, Real::getClassTypeSpec()          ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class name of object */
const std::string& Func_distance::getClassName(void) { 
    
    static std::string rbClassName = "Distance matrix function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_distance::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_distance::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_distance::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = DistanceMatrix::getClassTypeSpec();
    return returnTypeSpec;
}

