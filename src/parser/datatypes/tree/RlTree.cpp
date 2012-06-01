/**
 * @file
 * This file contains the implementation of Sequence, an
 * abstract base class for different types of character vectors.
 *
 * @brief Implementation of VectorCharacterObservations
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends Vector
 *
 * $Id$
 */


#include "CharacterState.h"
#include "ConstArgumentRule.h"
#include "Ellipsis.h"
#include "MethodTable.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "RlTree.h"
#include "SimpleMemberFunction.h"


/** Constructor with element type, used to properly construct vectors */
RlTree::RlTree(const TreePlate &t) : MemberObject(), tree( t ) {
    
}


RlTree* RlTree::clone(void) const {
    
    return new RlTree(*this);
}


/* Map calls to member methods */
RbPtr<RbLanguageObject> RlTree::executeSimpleMethod(std::string const &name, const std::vector<const RbObject *> &args) {
    
//    if (name == "size") {
//        
//        return RbPtr<RbLanguageObject>( new Natural( taxon.getNumberOfCharacters() ) );
//    } else if ( name == "[]") {
//        // get the member with give index
//        const Natural& index = static_cast<const Natural&>( *args[0] );
//        
//        if ( taxon.getNumberOfCharacters() < (size_t)(index.getValue()) ) {
//            throw RbException("Index out of bounds in [] of RlTree.");
//        }
//        
//        // we have to internally reduce the index by 1
//        CharacterState& element = taxon[index.getValue() - 1];
//        return RbPtr<RbLanguageObject>( element.clone() );
//    } 
    
    return MemberObject::executeSimpleMethod( name, args );
}


/** Get class name of object */
const std::string& RlTree::getClassName(void) { 
    
    static std::string rbClassName = "Tree";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlTree::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


const MemberRules& RlTree::getMemberRules(void) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) {
        
        memberRules.push_back( new ArgumentRule( "topology" , true, Topology::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/* Get method specifications */
const MethodTable& RlTree::getMethods(void) const {
    
    static MethodTable      methods = MethodTable();
    static bool             methodsSet = false;
    
    if ( methodsSet == false ) {
//        ArgumentRules* sizeArgRules = new ArgumentRules();
//        methods.addFunction("size", new SimpleMemberFunction( Natural::getClassTypeSpec(), sizeArgRules) );
//        
//        // add method for call "x[]" as a function
//        ArgumentRules* squareBracketArgRules = new ArgumentRules();
//        squareBracketArgRules->push_back( new ConstArgumentRule( "index" , Natural::getClassTypeSpec() ) );
//        methods.addFunction("[]",  new SimpleMemberFunction( RbObject::getClassTypeSpec(), squareBracketArgRules) );
//        
//        // necessary call for proper inheritance
//        methods.setParentTable( &MemberObject::getMethods() );
//        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
const TypeSpec& RlTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const TreePlate& RlTree::getValue( void ) const {
    return tree;
}


void RlTree::printValue(std::ostream &o) const {
    
    o << tree;
}


/** Set a member variable */
void RlTree::setSimpleMemberValue(const std::string& name, const RbPtr<const RbLanguageObject> &var) {
    
    if ( name == "topology" ) {
        throw RbException("Missing implementation of setting a topology in RlTree");
//        const Topology *t = static_cast<const Topology*>( (const RbLanguageObject *) var );
//        tree.setTopology( t );
    }
    else {
        MemberObject::setSimpleMemberValue(name, var);
    }
}


