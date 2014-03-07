//
//  Simplex.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Simplex.h"


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

Simplex::Simplex(void) : TypedContainer<std::vector<double> >( Probability::getClassTypeSpec() ) {
    
}



Simplex::Simplex(const std::vector<double> &v) : TypedContainer<std::vector<double> >( Probability::getClassTypeSpec(), v ) {
    
}



Simplex::Simplex(RevBayesCore::TypedDagNode<std::vector<double> > *v) : TypedContainer<std::vector<double> >( Probability::getClassTypeSpec(), v ) {
    
}



Simplex::Simplex(const Simplex &v) : TypedContainer<std::vector<double> >( v ) {
    
}



Simplex::~Simplex() {
    
}


Simplex& Simplex::operator=(const Simplex &v) {
    
    if ( this != &v ) {
        // delegate to base class
        TypedContainer<std::vector<double> >::operator=( v );
    }
    
    return *this;
}


/** Get iterator to the beginning of the Container. */
Simplex::iterator Simplex::begin( void ) {
    
    return value->getValue().begin();
}


/** Get const-iterator to the beginning of the Container. */
Simplex::const_iterator Simplex::begin( void ) const {
    
    return value->getValue().begin();
}


/** Clear the container. */
void Simplex::clear( void ) {
    
    return value->getValue().clear();
}


Simplex* Simplex::clone( void ) const {
    
    return new Simplex( *this );
}


/** Clear the container. */
RbLanguageObject* Simplex::convertTo(const TypeSpec &type ) const {
    
    return Container::convertTo( type );
}


/** Get iterator to the end of the Vector. */
Simplex::iterator Simplex::end( void ) {
    
    return value->getValue().end();
}


/** Get const-iterator to the end of the Vector. */
Simplex::const_iterator Simplex::end( void ) const {
    
    return value->getValue().end();
}


/* Map calls to member methods */
RbLanguageObject* Simplex::executeMethod(std::string const &name, const std::vector<Argument> &args) {
        
    if ( name == "[]") 
    {
        // get the member with give index
        const Natural &index = static_cast<const Natural &>( args[0].getVariable()->getValue() );

        if (size() < (size_t)(index.getValue()) || index.getValue() < 1 )
        {
            throw RbException("Index out of bounds in []");
        }
        
        RbLanguageObject* element = getElement(index.getValue() - 1);
        RealPos * v = static_cast<RealPos*>( element );
        return v;

//        RevBayesCore::VectorIndexOperator<double>* f = new RevBayesCore::VectorIndexOperator<double>( this->value, index.getValueNode() );
//        RevBayesCore::DeterministicNode<double> *detNode = new RevBayesCore::DeterministicNode<double>("", f);
            
//        RealPos* v = new RealPos( detNode );
        
//        return v;
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
int Simplex::findIndex(const RbLanguageObject& x) const {
    
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
        throw RbException("Missing implementation of findIndex() in Simplex.");
    }
    
    return -1;
}


/* Get class name of object */
const std::string& Simplex::getClassName(void) { 
    
    static std::string rbClassName = "Simplex";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Simplex::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ), new TypeSpec( RealPos::getClassTypeSpec() ) );
    
	return rbClass; 
}


RbLanguageObject* Simplex::getElement(size_t index) {
    
    return new RealPos( value->getValue()[index] );
}


/* Get method specifications */
const MethodTable&  Simplex::getMethods(void) const {
    
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
const TypeSpec& Simplex::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}



const std::vector<double>& Simplex::getValue( void ) const {
    
    return value->getValue();
}



RevBayesCore::TypedDagNode<std::vector<double> >* Simplex::getValueNode( void ) const {
    
    return value;
}


bool Simplex::isConstant( void ) const {
    return value->isConstant();
}


void Simplex::makeConstantValue( void ) {
    
    if ( value == NULL ) {
        throw RbException("Cannot convert a variable without value to a constant value.");
    } else {
        // @todo: we might check if this variable is already constant. Now we construct a new value anyways.
        RevBayesCore::ConstantNode<std::vector<double> >* newVal = new RevBayesCore::ConstantNode<std::vector<double> >(value->getName(), new std::vector<double>(value->getValue()) );
        value->replace(newVal);
        value->decrementReferenceCount();       // We will not be referencing the value any longer
        delete value;
        value = newVal;
    }
}


void Simplex::setName(std::string const &n) {
    
    if ( value == NULL ) {
        throw RbException("Null-pointer-exception: Cannot set name of value.");
    } else {
        value->setName( n );
    }
}




/** Print value for user */
void Simplex::printValue(std::ostream &o) const {
    
    o << "[ ";
    value->printValue(o,", ");
    o << " ]";
}


void Simplex::replaceVariable(RbLanguageObject *newVar) {
    
    RevBayesCore::DagNode* newParent = newVar->getValueNode();
    
    while ( value->getNumberOfChildren() > 0 ) {
        value->getFirstChild()->swapParent(value, newParent);
    }
    
}


/** Get the size of the vector */
size_t Simplex::size( void ) const {
    
    return value->getValue().size();
    
}


void Simplex::sort( void ) {
    throw RbException("A simplex should not be sorted!");
}


void Simplex::unique( void ) {
    throw RbException("A simplex should cannot be made unique!");
}
