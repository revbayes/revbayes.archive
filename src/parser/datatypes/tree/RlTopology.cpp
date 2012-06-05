//
//  RlTopology.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 6/4/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRules.h"
#include "MethodTable.h"
#include "RlTopology.h"
#include "SimpleMemberFunction.h"
#include "Topology.h"
#include "TopologyNode.h"




RlTopology::RlTopology( void ) : MemberObject( getMemberRules() ) {
    
}



RlTopology::RlTopology( const Topology &top) : MemberObject( getMemberRules() ), topology( top ) {
    
}


RlTopology::~RlTopology() {
    
}


RlTopology* RlTopology::clone( void ) const {
    return new RlTopology( *this );
}


/** Get class name of object */
const std::string& RlTopology::getClassName(void) { 
    
    static std::string rbClassName = "Tree Topology";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlTopology::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


RbValue<void*> RlTopology::getLeanValue( void ) const {
    RbValue<void*> tmp;
    tmp.value = static_cast<void*>( &const_cast<Topology&>( topology ) );
    return tmp;
}

/** Get type spec */
const TypeSpec& RlTopology::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/* Map calls to member methods */
RbPtr<RbLanguageObject> RlTopology::executeSimpleMethod(std::string const &name, const std::vector<const RbObject *> &args) {
    
    if (name == "nTips") {
        
        return RbPtr<RbLanguageObject>( new Natural( topology.getNumberOfTips() ) );
    }
    else if (name == "nnodes") {
        
        return RbPtr<RbLanguageObject>( new Natural( topology.getNumberOfNodes() ) );
    }
    
    return MemberObject::executeSimpleMethod( name, args );
}


/* Get member rules */
const MemberRules& RlTopology::getMemberRules(void) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {
        rulesSet = true;
    }
    
    return memberRules;
}


/* Get method specifications */
const MethodTable& RlTopology::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static ArgumentRules* ntipsArgRules = new ArgumentRules();
    static ArgumentRules* nnodesArgRules = new ArgumentRules();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        methods.addFunction("nTips",  new SimpleMemberFunction(Natural::getClassTypeSpec(), ntipsArgRules) );
        methods.addFunction("nnodes", new SimpleMemberFunction(Natural::getClassTypeSpec(), nnodesArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


const Topology& RlTopology::getValue( void ) const {
    return topology;
}


size_t RlTopology::memorySize( void ) const {
    return sizeof( Topology );
}


/* Print the tree */
void RlTopology::printValue(std::ostream& o) const {
    
    o << topology.getRoot().getName();
}