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
#include "Ellipsis.h"
#include "OptionRule.h"
#include "ParserDistribution.h"
#include "Plate.h"
#include "RbNullObject.h"
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
    if (name == "replicate") 
    {
        
        std::string varName = static_cast<const RbString&>( args[0].getVariable().getValue() ).getValue();
        std::string type    = static_cast<const RbString&>( args[1].getVariable().getValue() ).getValue();
        std::string fxtName = static_cast<const RbString&>( args[2].getVariable().getValue() ).getValue();
        
        if ( type == "Stochastic" ) {
            std::cerr << "Replicating a stochastic node with distribution " << fxtName << "!" << std::endl;
            
            std::vector<Argument> distributionArgs;
            for (size_t i = 3; i<args.size(); i++) {
                distributionArgs.push_back( args[i] );
            }
            RbFunction* fxt = Workspace::userWorkspace().getFunction( fxtName, distributionArgs ).clone();
            fxt->processArguments( distributionArgs );
            const ParserDistribution& d = static_cast<const ParserDistribution&>( fxt->execute() );
            
//            StochasticNode* stochNode = new StochasticNode( d.clone(), this );
//            stochNode->setName( varName );
//            
//            Workspace::userWorkspace().addVariable(varName, stochNode);
        }
        else {
            std::cerr << "Replicating a deterministic node with function " << fxtName << "!" << std::endl;
            
        }
        
        return RbNullObject::getInstance();
    }
    else {
        return MemberObject::executeOperationSimple( name, args );
    }
}

/* Get method specifications */
const MethodTable& Plate::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static ArgumentRules* replicateArgRules = new ArgumentRules();
    
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        
        // add the 'addVariable()' method
        replicateArgRules->push_back( new ValueRule( "varname"  , RbString::getClassTypeSpec() )     );
        RbVector<RbString> options;
        options.push_back(new RbString("Stochastic"));
        options.push_back(new RbString("Deterministic"));
        replicateArgRules->push_back( new OptionRule( "type"     , options )     );
        replicateArgRules->push_back( new ValueRule( "name"      , RbString::getClassTypeSpec() ) );
        replicateArgRules->push_back( new Ellipsis( RbLanguageObject::getClassTypeSpec() ) );
        
        methods.addFunction("replicate", new MemberFunction(RbVoid_name, replicateArgRules) );
        
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
