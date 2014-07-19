
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Model.h"
#include "ModelVector.h"
#include "RevObject.h"
#include "RbException.h"
#include "RlModel.h"
#include "TypeSpec.h"

#include <vector>

using namespace RevLanguage;

Model::Model() : WorkspaceObject<RevBayesCore::Model>() {
    
}


/** Clone object */
Model* Model::clone(void) const {
    
	return new Model(*this);
}


void Model::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a model
    std::set<const RevBayesCore::DagNode*> s;
    for (std::set<RevPtr<const Variable> >::iterator it = sources.begin(); it != sources.end(); ++it) {
        RevBayesCore::DagNode* n = (*it)->getRevObject().getDagNode();
        s.insert( n );
    }
    value = new RevBayesCore::Model( s );
}


/** Get Rev type of object */
const std::string& Model::getClassType(void) { 
    
    static std::string revType = "Model";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Model::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceObject<RevBayesCore::Model>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Model::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        modelMemberRules.push_back( new ArgumentRule("x", true, RevObject::getClassTypeSpec() ) );
        modelMemberRules.push_back( new Ellipsis( RevObject::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return modelMemberRules;
}

/** Get type spec */
const TypeSpec& Model::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print a simplified representation of the model for the user. */
void Model::printValue(std::ostream &o) const {
    
    const std::vector<RevBayesCore::DagNode*>& theNodes = value->getDagNodes();
    std::vector<RevBayesCore::DagNode*>::const_iterator it;

    o << "\nModel with " << theNodes.size() << " nodes\n\n";
    
    o << "List of nodes:\n";
    o << "==============\n\n";

    for ( it=theNodes.begin(); it!=theNodes.end(); ++it )
    {
        if ( (*it)->getName() != "" )
            o << (*it)->getName() <<  " <" << (*it) << "> :" << std::endl;
        else
            o << "<" << (*it) << "> :" << std::endl;
        
        o << "_value        = ";
        std::ostringstream o1;
        (*it)->printValue( o1, ", " );
        o << StringUtilities::oneLiner( o1.str(), 54 ) << std::endl;

        (*it)->printStructureInfo( o );

        o << std::endl;
    }
}


/** Set a member variable */
void Model::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {

    if ( name == "" || name == "x") {
        sources.insert( var );
    }
    else {
        RevObject::setConstMemberVariable(name, var);
    }
}
