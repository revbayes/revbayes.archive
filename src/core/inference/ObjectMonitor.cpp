/**
 * @file
 * This file contains the implementation of FileMonitor, used to save
 * information to file about the monitoring of a variable DAG node.
 *
 * @brief Implementation of FileMonitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "ConstArgumentRule.h"
#include "DagNodeContainer.h"
#include "Ellipsis.h"
#include "Integer.h"
#include "MemberFunction.h"
#include "ObjectMonitor.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "VariableNode.h"


#include <sstream>
#include <map>


/** Constructor */
ObjectMonitor::ObjectMonitor(void) : Monitor(getMemberRules() ) {
    
}

/** Copy Constructor */
ObjectMonitor::ObjectMonitor(const ObjectMonitor &x) : Monitor(x) {
    
    // shallow copy
    values = x.values;
}

ObjectMonitor::~ObjectMonitor() {
    
}


/** Clone the object */
ObjectMonitor* ObjectMonitor::clone(void) const {
    
    return new ObjectMonitor(*this);
}


/** Map calls to member methods */
RbPtr<RbLanguageObject> ObjectMonitor::executeOperationSimple(const std::string& name, const std::vector<RbPtr<Argument> >& args) {
    
//    if (name == "getValues") {
//        const RbString& name = static_cast<const RbString&>( args[0].getVariable().getValue() );
//        
//        return getValues( name );
//    }
    
    return MemberObject::executeOperationSimple( name, args );
}


/** Get class name of object */
const std::string& ObjectMonitor::getClassName(void) { 
    
    static std::string rbClassName = "Object monitor";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ObjectMonitor::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ObjectMonitor::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

/** Return member rules */
const MemberRules& ObjectMonitor::getMemberRules( void ) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) 
    {

        const MemberRules& parentRules = Monitor::getMemberRules();
        memberRules.insert(memberRules.end(), parentRules.begin(), parentRules.end());
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get methods */
const MethodTable& ObjectMonitor::getMethods( void ) const {
    
    static MethodTable methods = MethodTable();
    static bool             methodsSet = false;
    
    if ( !methodsSet ) {
        
        ArgumentRules* getValues = new ArgumentRules();
        getValues->push_back( new ConstArgumentRule( "name", RbString::getClassTypeSpec() ) );
        
        methods.addFunction( "getValues", new MemberFunction( RbVector::getClassTypeSpec(), getValues ) );
        
        methods.setParentTable( &Monitor::getMethods() );
        
        methodsSet = true;
    }
    
    return methods;
}


/** Monitor value unconditionally */
void ObjectMonitor::monitor(void) {
    
//    for (std::set<DAGNode*>::const_iterator it=nodes.begin(); it!=nodes.end(); it++) {
//        
//        // save the value        
//        RbLanguageObject* temp = (*it)->getValue().clone();
//        values[(*it)->getName()].push_back( temp );
//    }
    
}
