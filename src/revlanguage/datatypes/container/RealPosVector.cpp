//
//  RealPosVector.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "RealPosVector.h"


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "Natural.h"
#include "Probability.h"
#include "RbException.h"
#include "RlUtils.h"
#include "StochasticNode.h"
#include "VectorIndexOperator.h"

using namespace RevLanguage;

RealPosVector::RealPosVector(void) : TypedContainer<std::vector<double> >( Probability::getClassTypeSpec() ) {
    
}



RealPosVector::RealPosVector(const std::vector<double> &v) : TypedContainer<std::vector<double> >( Probability::getClassTypeSpec(), v ) {
    
}



RealPosVector::RealPosVector(RevBayesCore::TypedDagNode<std::vector<double> > *v) : TypedContainer<std::vector<double> >( Probability::getClassTypeSpec(), v ) {
    
}



RealPosVector::RealPosVector(const RealPosVector &v) : TypedContainer<std::vector<double> >( v ) {
    
}



RealPosVector::~RealPosVector() {
    
}


RealPosVector& RealPosVector::operator=(const RealPosVector &v) {
    
    if ( this != &v ) {
        // delegate to base class
        TypedContainer<std::vector<double> >::operator=( v );
    }
    
    return *this;
}


/** Get iterator to the beginning of the Container. */
RealPosVector::iterator RealPosVector::begin( void ) {
    
    return value->getValue().begin();
}


/** Get const-iterator to the beginning of the Container. */
RealPosVector::const_iterator RealPosVector::begin( void ) const {
    
    return value->getValue().begin();
}


/** Clear the container. */
void RealPosVector::clear( void ) {
    
    return value->getValue().clear();
}


RealPosVector* RealPosVector::clone( void ) const {
    
    return new RealPosVector( *this );
}


/** Clear the container. */
RbLanguageObject* RealPosVector::convertTo(const TypeSpec &type ) const {
    
    return Container::convertTo( type );
}


/** Get iterator to the end of the Vector. */
RealPosVector::iterator RealPosVector::end( void ) {
    
    return value->getValue().end();
}


/** Get const-iterator to the end of the Vector. */
RealPosVector::const_iterator RealPosVector::end( void ) const {
    
    return value->getValue().end();
}


/* Map calls to member methods */
RbLanguageObject* RealPosVector::executeMethod(std::string const &name, const std::vector<Argument> &args) {
        
    if ( name == "[]") 
    {
        // get the member with give index
        const Natural &index = static_cast<const Natural &>( args[0].getVariable()->getValue() );

        if (size() < (size_t)(index.getValue()) || index.getValue() < 1 )
        {
            throw RbException("Index out of bounds in []");
        }

        RevBayesCore::VectorIndexOperator<double>* f = new RevBayesCore::VectorIndexOperator<double>( this->value, index.getValueNode() );
        RevBayesCore::DeterministicNode<double> *detNode = new RevBayesCore::DeterministicNode<double>("", f);
            
        RealPos* v = new RealPos( detNode );
        
        return v;
    } 
    
    return TypedContainer<std::vector<double> >::executeMethod( name, args );
}



/**
 * Find the index of the given element.
 * We rely on overloaded operator== in the element classes to check for matches.
 * 
 * \param x the element we are looking for. 
 * \return The index or -1 if we didn't find it.
 */
int RealPosVector::findIndex(const RbLanguageObject& x) const {
    
    // get the iterator to the first element
    const_iterator i;
    
    // initialize the index
    int index = 0;
    for ( i = value->getValue().begin(); i != value->getValue().end(); i++, index++) 
    {
        // test if the object matches
        // note that we rely on the implemented operator==
        //if ( *i == x.getValue() ) {
        //    return index;
        //        }
        throw RbException("Missing implementation of findIndex() in RealPosVector.");
    }
    
    return -1;
}


/* Get class name of object */
const std::string& RealPosVector::getClassName(void) { 
    
    static std::string rbClassName = "RealPosVector";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& RealPosVector::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ) );
    
	return rbClass; 
}


RbLanguageObject* RealPosVector::getElement(size_t index) {
    
    return new RealPos( value->getValue()[index] );
}


/* Get method specifications */
const MethodTable&  RealPosVector::getMethods(void) const {
    
    static MethodTable methods      = MethodTable();
    static bool        methodsSet   = false;
    
    if ( methodsSet == false ) 
    {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( RealPos::getClassTypeSpec(), squareBracketArgRules) );
        
        ArgumentRules* clampArgRules = new ArgumentRules();
        clampArgRules->push_back( new ArgumentRule("x", true, getTypeSpec() ) );
        methods.addFunction("clamp", new MemberFunction( RlUtils::Void, clampArgRules) );
        
        ArgumentRules* setValueArgRules = new ArgumentRules();
        setValueArgRules->push_back( new ArgumentRule("x", true, getTypeSpec() ) );
        methods.addFunction("setValue", new MemberFunction( RlUtils::Void, setValueArgRules) );
                
        // necessary call for proper inheritance
        methods.setParentTable( &TypedContainer<std::vector<double> >::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RealPosVector::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}



const std::vector<double>& RealPosVector::getValue( void ) const {
    
    return value->getValue();
}



RevBayesCore::TypedDagNode<std::vector<double> >* RealPosVector::getValueNode( void ) const {
    
    return value;
}


bool RealPosVector::isConstant( void ) const {
    return value->isConstant();
}


void RealPosVector::makeConstantValue( void ) {
    
    if ( value == NULL ) {
        throw RbException("Cannot convert a variable without value to a constant value.");
    } else {
        // @todo: we might check if this variable is already constant. Now we construct a new value anyways.
        RevBayesCore::ConstantNode<std::vector<double> >* newVal = new RevBayesCore::ConstantNode<std::vector<double> >(value->getName(), new std::vector<double>(value->getValue()) );
        value->replace(newVal);
        // We will not be referencing the value any longer
        if ( value->decrementReferenceCount() == 0) 
        {
            delete value;
        }
        value = newVal;
        value->incrementReferenceCount();
    }
}


void RealPosVector::setName(std::string const &n) {
    
    if ( value == NULL ) {
        throw RbException("Null-pointer-exception: Cannot set name of value.");
    } else {
        value->setName( n );
    }
}




/** Print value for user */
void RealPosVector::printValue(std::ostream &o) const {
    
    o << "[ ";
    value->printValue(o,", ");
    o << " ]";
}


void RealPosVector::replaceVariable(RbLanguageObject *newVar) {
    
    RevBayesCore::DagNode* newParent = newVar->getValueNode();
    
    while ( value->getNumberOfChildren() > 0 ) {
        value->getFirstChild()->swapParent(value, newParent);
    }
    
}


/** Get the size of the vector */
size_t RealPosVector::size( void ) const {
    
    return value->getValue().size();
    
}


void RealPosVector::sort( void ) {
    throw RbException("A RealPosVector should not be sorted!");
}


void RealPosVector::unique( void ) {
    throw RbException("A RealPosVector should cannot be made unique!");
}
