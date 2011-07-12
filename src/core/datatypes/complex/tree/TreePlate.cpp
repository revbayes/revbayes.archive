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


/* Copy constructor */
TreePlate::TreePlate(const TreePlate& t) : MemberObject( getMemberRules() ) {

    treePlateVariableTypeRule = new ValueRule( "", TypeSpec( List_name ) );
}


/* Destructor */
TreePlate::~TreePlate(void) {

    
    delete treePlateVariableTypeRule;
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
    
    if (name == "addVariable") 
        {
        std::string varName = static_cast<const RbString*>( args[1].getValue() )->getValue();

        MemberSlot* slot = new MemberSlot( treePlateVariableTypeRule );
        members.push_back( varName, slot );
        List* lst = NULL;
        if (orderingTopology == NULL)
            {
            // we don't have a topology and we initialize the member with an empty list
            lst = new List;
            }
        else
            {
            // we have a tree
            size_t nNodes = orderingTopology->getNumberOfNodes();
            lst = new List(nNodes);
            }
        members[ members.size() - 1 ].setVariable( lst->wrapIntoVariable() );
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
        // this must be here so the parser can distinguish between different instances of a character matrix
        addvariableArgRules.push_back(  new ReferenceRule( "", TreePlate_name ) );
        addvariableArgRules.push_back(  new ValueRule(     "", RbString_name  ) );
        
        methods.addFunction("addVariable",  new MemberFunction(RbString_name, addvariableArgRules)  );
        
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
        rulesSet = true;
        }

    return memberRules;
}


/* Print the tree */
void TreePlate::printValue(std::ostream& o) const {

    o << "";
}


/* Get a lot of information about the TreePlate */
std::string TreePlate::richInfo(void) const {

    std::ostringstream o;
    o <<  "Tree Plate: ";
    printValue(o);
    return o.str();
}


