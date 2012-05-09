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

#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "MethodTable.h"
#include "Ellipsis.h"
#include "OptionRule.h"
#include "ParserDistribution.h"
#include "Plate.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "Workspace.h"


/* Default constructor */
Plate::Plate(void) : MemberObject( getMemberRules() ) {
    
}


/* Copy constructor */
Plate::Plate(const Plate& p) : MemberObject( p ) {
    
    size = p.size;
    parent = p.parent;
}


/* Destructor */
Plate::~Plate(void) {
    
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
const RbLanguageObject& Plate::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {
    
    // special handling for adding a variable
    if (name == "add") {
        
        // get the argument
        const Argument& theArg = args[0];
        
        // get the DAG node
        const DAGNode *theNode = theArg.getVariable().getDagNode();
        
        // expand the DAG node
        // \TODO: We shouldn't use const-casts.
        const_cast<DAGNode *>( theNode )->setPlate( this );
        const_cast<DAGNode *>( theNode )->expand();
        const_cast<DAGNode *>( theNode )->touch();
        
        return RbNullObject::getInstance();
    }
    else {
        return MemberObject::executeOperationSimple( name, args );
    }
}

/* Get method specifications */
const MethodTable& Plate::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static ArgumentRules* addArgRules = new ArgumentRules();
    
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        
        // add the 'addVariable()' method
        addArgRules->push_back( new ValueRule( "var"  , RbObject::getClassTypeSpec() )     );
        
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
        memberRules.push_back( new ValueRule( "size" , Natural::getClassTypeSpec() ) );
        memberRules.push_back( new ValueRule( "parent" , Plate::getClassTypeSpec(), true ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


size_t Plate::getLength( void ) const {
    return static_cast<const Natural&>( size->getValue() );
}


const Plate* Plate::getParentPlate( void ) const {
    if ( parent == NULL ) {
        return NULL;
    }
    else {
        return static_cast<const Plate*>( &parent->getValue() );
    }
}


std::vector<size_t> Plate::getPlateLengths( void ) const {
    
    if ( parent == NULL ) {
        std::vector<size_t> plateLengths;
        plateLengths.push_back( size_t(static_cast<const Natural&>( size->getValue() ).getValue() ) );
        
        return plateLengths;
    }
    else  {
        std::vector<size_t> plateLengths = static_cast<const Plate&>( parent->getValue() ).getPlateLengths();
        plateLengths.push_back( size_t(static_cast<const Natural&>( size->getValue() ).getValue() ) );
        
        return plateLengths;
    }
}



/* Print the tree */
void Plate::printValue(std::ostream& o) const {
    
    o << "Plate(i in 1:" << static_cast<const Natural&>( size->getValue() ) << ")";
    
}



/** Catch setting of the topology variable */
void Plate::setMemberVariable(const std::string& name, const Variable* var) {
    
    if ( name == "size" ) {
        size = var;
    }
    else if ( name == "parent" ) {
        parent = var;
    }
    else {
        MemberObject::setMemberVariable(name, var);
    }
}