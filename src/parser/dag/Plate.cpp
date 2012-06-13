/**
 * @file
 * This file contains the implementation of a Plate.
 *
 * @brief Implementation of Plate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-23 19:18:46 +0100 (Fri, 23 Mar 2012) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends MemberObject
 *
 * $Id: Plate.cpp 1360 2012-03-23 18:18:46Z hoehna $
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "Ellipsis.h"
#include "OptionRule.h"
#include "RlDistribution.h"
#include "Plate.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "Workspace.h"


/* Default constructor */
Plate::Plate(void) : MemberObject( getMemberRules() ) {
    
}


/* Clone function */
Plate* Plate::clone(void) const {
    
    return new Plate(*this);
}


/** Get class name of object */
const std::string& Plate::getClassName(void) { 
    
    static std::string rbClassName = "Plate";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Plate::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Plate::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/* Map calls to member methods */
RlValue<RbLanguageObject> Plate::executeMethod(const std::string& name, const std::vector<RbPtr<Argument> >& args) {
    
    // special handling for adding a variable
    if (name == "add") {
        
        // get the argument
        RbPtr<Argument> theArg = args[0];
        
        // get the Variable
        const RbPtr<Variable> &theVar = theArg->getReferenceVariable();
        
        // get the DAG node
        const RbPtr<DAGNode>& theNode = theVar->getDagNode();
        
        // expand the DAG node
        theNode->setPlate( this );
        theNode->expandAll();
        theNode->touch();
//        theNode->keep();
        
        return RlValue<RbLanguageObject>( NULL );
    }
    else {
        return MemberObject::executeMethod( name, args );
    }
}

/* Get method specifications */
const MethodTable& Plate::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static ArgumentRules* addArgRules = new ArgumentRules();
    
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        
        // add the 'addVariable()' method
        addArgRules->push_back( new ArgumentRule( "var", false, RbObject::getClassTypeSpec() ) );
        
        methods.addFunction("add", new MemberFunction(RbVoid_name, addArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/* Get member rules */
const MemberRules& Plate::getMemberRules(void) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        memberRules.push_back( new ArgumentRule( "size"  , true,  Natural::getClassTypeSpec() ) );
        memberRules.push_back( new ArgumentRule( "parent", false, Plate::getClassTypeSpec(), NULL ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


size_t Plate::getLength( void ) const {
    return size;
}


const RbPtr<const Plate>& Plate::getParentPlate( void ) const {
    return parent;
}


std::vector<size_t> Plate::getPlateLengths( void ) const {
    
    if ( parent == NULL ) {
        std::vector<size_t> plateLengths;
        plateLengths.push_back( size );
        
        return plateLengths;
    }
    else  {
        std::vector<size_t> plateLengths = parent->getPlateLengths();
        plateLengths.push_back( size );
        
        return plateLengths;
    }
}



/* Print the tree */
void Plate::printValue(std::ostream& o) const {
    
    o << "Plate(i in 1:" << size << ")";
    
}



/** Catch setting of the size variable */
void Plate::setSimpleMemberValue(const std::string& name, const RbPtr<const RbLanguageObject> &var) {
    
    if ( name == "size" ) {
        size = static_cast<const Natural *>( (const RbLanguageObject *)var )->getValue();
    }
    else if ( name == "parent" ) {
        parent = static_cast<const Plate *>( (const RbLanguageObject *)var );
    }
    else {
        MemberObject::setSimpleMemberValue(name, var);
    }
}