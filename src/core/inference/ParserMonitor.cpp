//
//  ParserMonitor.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 5/2/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "InferenceMonitor.h"
#include "ParserMonitor.h"

ParserMonitor::ParserMonitor( InferenceMonitor *m, const std::string &n, const MemberRules &mr, const std::set<std::string> &a) : MemberObject( mr ), monitor( m ), memberRules( mr ), name( n ), typeSpec( getClassName() + " (" + n + ")", new TypeSpec( MemberObject::getClassTypeSpec() ) ), attributeNames( a ) {
    
}

ParserMonitor::ParserMonitor( const ParserMonitor &m ) : MemberObject( m ), monitor( m.monitor->clone() ), memberRules( m.memberRules ), name( m.name ), typeSpec( m.typeSpec ), attributeNames( m.attributeNames ), nodes( m.nodes ) {
    
}

ParserMonitor::~ParserMonitor( void ) {
    delete monitor;
}


ParserMonitor& ParserMonitor::operator=(const ParserMonitor &m) {
    
    // check for self-assignment
    if ( &m != this ) {
        // call the base class assignment operator
        MemberObject::operator=( m );
        
        // delete my monitor
        delete monitor;
        
        // create a clone of the monitor
        monitor         = m.monitor->clone();
        
        // make a shallow copy of all attributes
        attributeNames  = m.attributeNames;
        nodes           = m.nodes;
        memberRules     = m.memberRules;
        name            = m.name;
        typeSpec        = m.typeSpec;
    }
    
    return *this;
}


ParserMonitor* ParserMonitor::clone( void ) const {
    return new ParserMonitor( *this );
}


/** Get class name of object */
const std::string& ParserMonitor::getClassName(void) { 
    
    static std::string rbClassName = "ParserMonitor";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ParserMonitor::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


const InferenceMonitor* ParserMonitor::getLeanMonitor( void ) const {
    return monitor;
}


const std::vector<const DAGNode *>& ParserMonitor::getMonitorArgumgents( void ) const {
    return nodes;
}

/** Get type spec */
const TypeSpec& ParserMonitor::getTypeSpec( void ) const {
    
    return typeSpec;
}

/** Return member rules */
const MemberRules& ParserMonitor::getMemberRules( void ) const {
    
    return memberRules;
}


void ParserMonitor::setMemberVariable(std::string const &name, const RbPtr<Variable> &var) {
    
    if ( attributeNames.find( name ) != attributeNames.end() ) {
        RbValue<void*> lValue = var->getValue().getLeanValue();
        monitor->setAttribute( name, lValue );
    }
    else {
        nodes.push_back( var->getDagNode() );
    }
}



/** Print value for user */
void ParserMonitor::printValue(std::ostream& o) const {
    
    o << typeSpec;
}
