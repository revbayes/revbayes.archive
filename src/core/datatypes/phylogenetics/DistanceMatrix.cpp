/**
 * @file
 * This file contains the declaration of Alignment, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Implementation of Alignment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package datatypes
 *
 * $Id$
 */

#include "AbstractVector.h"
#include "DistanceMatrix.h"
#include "ConstantNode.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbString.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "Vector.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>


/** Constructor requires character type; passes member rules to base class */
DistanceMatrix::DistanceMatrix(const size_t nTaxa) : MatrixReal(nTaxa, nTaxa), typeSpec(DistanceMatrix_name) {

}


/** Copy constructor */
DistanceMatrix::DistanceMatrix(const DistanceMatrix& x) : MatrixReal(x), typeSpec(DistanceMatrix_name) {

    deletedTaxa       = x.deletedTaxa;
    sequenceNames     = x.sequenceNames;
}


/** Destructor */
DistanceMatrix::~DistanceMatrix(void) {

}


/** Assignment operator */
DistanceMatrix& DistanceMatrix::operator=(const DistanceMatrix& x) {

    if ( this != &x ) 
        {
        MemberObject::operator=( x );
        deletedTaxa = x.deletedTaxa;
        }
    return (*this);
}


/** clear the oblect */
void DistanceMatrix::clear(void) {
    
    sequenceNames.clear();
    MatrixReal::clear();
}


/** Clone object */
DistanceMatrix* DistanceMatrix::clone(void) const {

    return new DistanceMatrix(*this);
}


/** Exclude a taxon */
void DistanceMatrix::excludeTaxon(size_t i) {

    if (i >= sequenceNames.size())
        throw RbException( "Only " + RbString(int(sequenceNames.size())) + " taxa in matrix" );
    deletedTaxa.insert( i );
}


/** Exclude a taxon */
void DistanceMatrix::excludeTaxon(std::string& s) {

    for (size_t i = 0; i < size(); i++) 
        {
        if (s == sequenceNames[i]) 
            {
            deletedTaxa.insert( i );
            break;
            }
        }
}


/** Map calls to member methods */
const RbLanguageObject& DistanceMatrix::executeOperationSimple(const std::string& name, Environment& args) {

    if (name == "names") 
        {
        return sequenceNames;
        }
    else if (name == "ntaxa") 
        {
        int n = (int)getNumberOfTaxa();
        numTaxa.setValue( n );
        return numTaxa;
        }
    else if (name == "nexcludedtaxa")
        {
        int n = (int)deletedTaxa.size();
        numExcludedTaxa.setValue( n );
        return numExcludedTaxa;
        }
    else if (name == "nincludedtaxa")
        {
        int n = (int)(getNumberOfTaxa() - deletedTaxa.size());
        numIncludedTaxa.setValue( n );
        return numIncludedTaxa;
        }
    else if (name == "excludedtaxa")
        {
        std::vector<std::string> et;
        for (std::set<size_t>::iterator it = deletedTaxa.begin(); it != deletedTaxa.end(); it++)
            {
            std::string tn = getTaxonNameWithIndex(*it);
            et.push_back( tn );
            }
        excludedTaxa.setValue( et );
        return excludedTaxa;
        }
    else if (name == "includedtaxa")
        {
        std::vector<std::string> it;
        for (size_t i=0; i<getNumberOfTaxa(); i++)
            {
            if ( isTaxonExcluded(i) == false )
                it.push_back( getTaxonNameWithIndex(i) );
            }
            includedTaxa.setValue( it );
        return includedTaxa;
        }
    else if (name == "show")
        {
        std::vector<std::vector<double> > m = getValue();
        size_t n = getNumberOfTaxa();
        size_t lenOfLongestName = 0;
        for (int i=0; i<n; i++)
            {
            size_t x = getTaxonNameWithIndex(i).size();
            if (x > lenOfLongestName)
                lenOfLongestName = x;
            }
        for (int i=0; i<n; i++)
            {
            if ( isTaxonExcluded(i) == false )
                {
                std::cout << getTaxonNameWithIndex(i) << "   ";
                size_t myNameLen = getTaxonNameWithIndex(i).size();
                for (int j=0; j<lenOfLongestName-myNameLen; j++)
                    std::cout << " ";
                for (int j=0; j<n; j++)
                    {
                    if ( isTaxonExcluded(j) == false )
                        std::cout << std::fixed << std::setprecision(4) << m[i][j] << " ";
                    }
                std::cout << std::endl;
                }
            }
        return RbNullObject::getInstance();
        }
    else if (name == "excludetaxa")
        {
        RbLanguageObject& argument = args[1].getValue();
        if ( argument.isTypeSpec( TypeSpec(Natural_name) ) ) 
            {
            std::cout << "excluded b" << std::endl;
            int n = static_cast<const Natural&>( argument ).getValue();
            std::cout << "n = " << n << std::endl;
            deletedTaxa.insert( n-1 );
            std::cout << "excluded e" << std::endl;
            }
        else if ( argument.isTypeSpec( TypeSpec(VectorNatural_name) ) ) 
            {
            std::vector<unsigned int> x = static_cast<const VectorNatural&>( argument ).getValue();
            for ( size_t i=0; i<x.size(); i++ )
                deletedTaxa.insert( x[i]-1 );
            }
        else if ( argument.isTypeSpec( TypeSpec(RbString_name) ) ) 
            {
            std::string x = static_cast<const RbString&>( argument ).getValue();
            size_t idx = indexOfTaxonWithName(x);
            deletedTaxa.insert(idx);
            }
        else if ( argument.isTypeSpec( TypeSpec(VectorString_name) ) ) 
            {
            std::vector<std::string> x = static_cast<const VectorString&>( argument ).getValue();
            for (std::vector<std::string>::iterator it = x.begin(); it != x.end(); it++)
                {
                size_t idx = indexOfTaxonWithName(*it);
                deletedTaxa.insert(idx);
                }
            }
        return RbNullObject::getInstance();
        }

    return ConstantMemberObject::executeOperationSimple( name, args );
}


/** Get class vector describing type of object */
const VectorString& DistanceMatrix::getClass(void) const {

    static VectorString rbClass = VectorString(DistanceMatrix_name) + AbstractVector::getClass();
    return rbClass;
}


const RbObject& DistanceMatrix::getElement(size_t row, size_t col) const {

    //RealPos* x(value[row][col]);
    //const TaxonData* sequence = dynamic_cast<const TaxonData*>( (RbLanguageObject*)elements[row] );
    //return sequence->getElement(col);
}


RbObject& DistanceMatrix::getElement(size_t row, size_t col) {

    //TaxonData* sequence( dynamic_cast<TaxonData*>( (RbLanguageObject*)elements[row]) );
    //return (sequence->getElement(col));
}


/** Get member rules */
const MemberRules& DistanceMatrix::getMemberRules(void) const {

    static MemberRules memberRules = MemberRules();
    
    return memberRules;
}


/** Get methods */
const MethodTable& DistanceMatrix::getMethods(void) const {

    static MethodTable    methods               = MethodTable();
    static ArgumentRules* namesArgRules         = new ArgumentRules();
    static ArgumentRules* ntaxaArgRules         = new ArgumentRules();
    static ArgumentRules* nexcludedtaxaArgRules = new ArgumentRules();    
    static ArgumentRules* nincludedtaxaArgRules = new ArgumentRules();    
    static ArgumentRules* excludedtaxaArgRules  = new ArgumentRules();    
    static ArgumentRules* includedtaxaArgRules  = new ArgumentRules();    
    static ArgumentRules* showdataArgRules      = new ArgumentRules();
    static ArgumentRules* excludetaxaArgRules   = new ArgumentRules();
    static ArgumentRules* excludetaxaArgRules2  = new ArgumentRules();
    static ArgumentRules* excludetaxaArgRules3  = new ArgumentRules();
    static ArgumentRules* excludetaxaArgRules4  = new ArgumentRules();
    static bool           methodsSet            = false;

    if ( methodsSet == false ) 
        {
        excludetaxaArgRules->push_back(        new ValueRule(     "", Natural_name       ) );
        excludetaxaArgRules2->push_back(       new ValueRule(     "", VectorNatural_name ) );
        excludetaxaArgRules3->push_back(       new ValueRule(     "", RbString_name      ) );
        excludetaxaArgRules4->push_back(       new ValueRule(     "", VectorString_name  ) );

        methods.addFunction("names",         new MemberFunction(VectorString_name,  namesArgRules              ) );
        methods.addFunction("ntaxa",         new MemberFunction(Natural_name,       ntaxaArgRules              ) );
        methods.addFunction("nexcludedtaxa", new MemberFunction(Natural_name,       nexcludedtaxaArgRules      ) );
        methods.addFunction("nincludedtaxa", new MemberFunction(Natural_name,       nincludedtaxaArgRules      ) );
        methods.addFunction("excludedtaxa",  new MemberFunction(VectorNatural_name, excludedtaxaArgRules       ) );
        methods.addFunction("includedtaxa",  new MemberFunction(VectorNatural_name, includedtaxaArgRules       ) );
        methods.addFunction("show",          new MemberFunction(RbVoid_name,        showdataArgRules           ) );
        methods.addFunction("excludetaxa",   new MemberFunction(RbVoid_name,        excludetaxaArgRules        ) );
        methods.addFunction("excludetaxa",   new MemberFunction(RbVoid_name,        excludetaxaArgRules2       ) );
        methods.addFunction("excludetaxa",   new MemberFunction(RbVoid_name,        excludetaxaArgRules3       ) );
        methods.addFunction("excludetaxa",   new MemberFunction(RbVoid_name,        excludetaxaArgRules4       ) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
        }

    return methods;
}


size_t DistanceMatrix::getNumberOfTaxa(void) const {

    return sequenceNames.size();
}


/** Get taxon with index idx */
const std::string& DistanceMatrix::getTaxonNameWithIndex( size_t idx ) const {

    return sequenceNames[idx];
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& DistanceMatrix::getTypeSpec(void) const {

    return typeSpec;
}


/** Return the index of the element ( the index of the taxon with name elemName ) */
size_t DistanceMatrix::indexOfTaxonWithName( std::string& s ) const {
    
    // search through all names
    for (size_t i=0; i<sequenceNames.size(); i++) 
        {
        if (s == sequenceNames[ i ]) 
            {
            return i;
            }
        }
    return -1;
}


/** Is the taxon excluded */
bool DistanceMatrix::isTaxonExcluded(size_t i) const {

	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Is the taxon excluded */
bool DistanceMatrix::isTaxonExcluded(std::string& s) const {

    size_t i = indexOfTaxonWithName(s);
	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Print value for user */
void DistanceMatrix::printValue(std::ostream& o) const {

    o << "Number of taxa:       " << getNumberOfTaxa() << std::endl;
}


void DistanceMatrix::resize(size_t nRows, size_t nCols) {
    
    throw RbException("Not implemented method Alignment::resize(rows,cols)");
}


/** Restore a taxon */
void DistanceMatrix::restoreTaxon(size_t i) {

    if ( i >= getNumberOfTaxa() )
        return;
    deletedTaxa.erase( i );
}


/** Restore a taxon */
void DistanceMatrix::restoreTaxon(std::string& s) {

    size_t i = indexOfTaxonWithName( s );
    deletedTaxa.erase( i );
}


/** Complete info */
std::string DistanceMatrix::richInfo(void) const {

	std::ostringstream o;
    printValue( o );
    return o.str();
}

/** Overloaded container setElement method */
void DistanceMatrix::setElement( const size_t index, RbLanguageObject* var ) {
    
    /*if (var->isTypeSpec(TypeSpec(TaxonData_name))) {
        TaxonData* seq = static_cast<TaxonData*>( var );
        
        sequenceNames.erase(sequenceNames.begin() + index);
        sequenceNames.insert(sequenceNames.begin() + index,seq->getTaxonName());
        elements.insert( elements.begin() + index, var );
        
        // add the sequence also as a member so that we can access it by name
        Variable* variable = new Variable( new ConstantNode(var ) );
        members->addVariable(seq->getTaxonName(), variable );
    }*/
}

/** Overloaded container setElement method */
void DistanceMatrix::setElement( size_t row, size_t col, RbLanguageObject* var ) {
    
    throw RbException("Not implemented method Alignment::setElement()");
}

void DistanceMatrix::showData(void) {

}

/** transpose the matrix */
void DistanceMatrix::transpose(void) {

    throw RbException("There is no point in transposing a symmetric distance matrix.");
}




