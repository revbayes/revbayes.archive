/**
 * @file
 * This file contains the implementation of a TreePlate.
 *
 * @brief Implementation of TreePlate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "Boolean.h"
#include "ConstantNode.h"
#include "ContainerNode.h"
#include "List.h"
#include "MemberFunction.h"
#include "MemberSlot.h"
#include "Natural.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "ReferenceRule.h"
#include "Topology.h"
#include "TreePlate.h"
#include "ValueRule.h"
#include "VectorString.h"



/* Default constructor */
TreePlate::TreePlate(void) : MemberObject( getMemberRules() ) {

    orderingTopology = NULL;
    treePlateVariableTypeRule = new ValueRule( "", TypeSpec( List_name ) );
}

/* constructor */
TreePlate::TreePlate(Topology* top) : MemberObject( getMemberRules() ) {
    
    orderingTopology = top;
    setVariable("topology", top->wrapIntoVariable());
    treePlateVariableTypeRule = new ValueRule( "", TypeSpec( List_name ) );
}


/* Copy constructor */
TreePlate::TreePlate(const TreePlate& t) : MemberObject( t ) {

    if (t.orderingTopology != NULL)
        orderingTopology = t.orderingTopology->clone();
    
    treePlateVariableTypeRule = t.treePlateVariableTypeRule->clone();
}


/* Destructor */
TreePlate::~TreePlate(void) {

//    delete orderingTopology;
//    delete treePlateVariableTypeRule;
}


/* Clone function */
TreePlate* TreePlate::clone(void) const {

    return new TreePlate(*this);
}


/* Get class information */
const VectorString& TreePlate::getClass(void) const {
    
    static VectorString rbClass = VectorString(TreePlate_name) + MemberObject::getClass();
    return rbClass;
}


/* Map calls to member methods */
DAGNode* TreePlate::executeOperation(const std::string& name, ArgumentFrame& args) {
    
    // special handling for adding a variable
    if (name == "addVariable") {
        // get the name of the variable
        std::string varName = static_cast<const RbString*>( args[1].getValue() )->getValue();
        
        // test whether this variable already exists
        if (hasMember(name)) {
            throw RbException("Variable with name \"" + name + "\" already exists.");
        }
        
        // get the replication parameter
        bool repl = static_cast<const Boolean*>( args[3].getValue() )->getValue();
        
        if (repl) {
            if (orderingTopology == NULL) {
                // we don't have a topology and we initialize the member with an empty list
                throw RbException("Cannot add variable with name \"" + name + "\" replicated for each node if the topology was not set.");
            }
            else {
                // we have a tree
                size_t nNodes = orderingTopology->getNumberOfNodes();
                
                // prepare the variable and slot for the variable
                DAGNode* var = args[2].getReference();
                ValueRule* varTypeRule = new ValueRule( "", var->getValueType() );
                MemberSlot* slot = new MemberSlot( varTypeRule );
                for (int i=0; i<nNodes; i++) {
                    members.push_back( varName, slot->clone() );
                    members[ members.size() - 1 ].setVariable( var );
                }
                
                delete slot;
            }
        } else {
            DAGNode* var = args[2].getReference();
            ValueRule* varTypeRule = new ValueRule( "", var->getValueType() );
            MemberSlot* slot = new MemberSlot( varTypeRule );
            members.push_back( varName, slot );
            members[ members.size() - 1 ].setVariable( var );
        }
        
    }

    return MemberObject::executeOperation( name, args );
}


/* Get method specifications */
const MethodTable& TreePlate::getMethods(void) const {
    
    static MethodTable   methods;
    static ArgumentRules addvariableArgRules;
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
        // this must be here so the parser can distinguish between different instances of a tree plates
//        addvariableArgRules.push_back(  new ReferenceRule( "", TreePlate_name ) );
        addvariableArgRules.push_back(  new ReferenceRule( ""      , MemberObject_name ) );
            
        addvariableArgRules.push_back(  new ValueRule( "name"      , RbString_name  ) );
        addvariableArgRules.push_back(  new ReferenceRule( ""      , RbObject_name ) );
        addvariableArgRules.push_back(  new ValueRule( "replicate" , Boolean_name  ) );
        
        methods.addFunction("addVariable",  new MemberFunction(RbVoid_name, addvariableArgRules)  );
        
        // necessary call for proper inheritance
        methods.setParentTable( const_cast<MethodTable*>( &MemberObject::getMethods() ) );
        methodsSet = true;
        }

    return methods;
}


/* Get member rules */
const MemberRules& TreePlate::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
    {
        TypeSpec varType(Topology_name,0,true);
        memberRules.push_back( new ReferenceRule( "topology" , varType ) );
        
        rulesSet = true;
        }

    return memberRules;
}


/* Print the tree */
void TreePlate::printValue(std::ostream& o) const {

    o << "Tree Plate:\n";
    orderingTopology->printValue(o);
    
    // TODO: print other member too
}


/* Get a lot of information about the TreePlate */
std::string TreePlate::richInfo(void) const {

    std::ostringstream o;
    o <<  "Tree Plate: ";
    printValue(o);
    return o.str();
}

/** Catch setting of the topology variable */
void TreePlate::setVariable(const std::string& name, DAGNode* var) {
    
    if ( name == "topology" )
        orderingTopology = (Topology*)(var->getValue());
    
    MemberObject::setVariable(name, var);
}


