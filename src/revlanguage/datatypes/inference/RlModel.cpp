
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "Model.h"
#include "RbLanguageObject.h"
#include "RbException.h"
#include "RlModel.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

Model::Model() : RlControlVariableWrapper<RevBayesCore::Model>() {
    
}


/** Clone object */
Model* Model::clone(void) const {
    
	return new Model(*this);
}


void Model::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    std::set<const RevBayesCore::DagNode*> s;
    for (std::set<RbPtr<const Variable> >::iterator it = sources.begin(); it != sources.end(); ++it) {
        RevBayesCore::DagNode* n = (*it)->getValue().getValueNode();
        s.insert( n );
    }
    value = new RevBayesCore::Model( s );
}


/** Get class name of object */
const std::string& Model::getClassName(void) { 
    
    static std::string rbClassName = "Model";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Model::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RlControlVariableWrapper<RevBayesCore::Model>::getClassTypeSpec() ) );
    
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& Model::getMemberRules(void) const {
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        modelMemberRules.push_back( new ArgumentRule("x", true, RbLanguageObject::getClassTypeSpec() ) );
        modelMemberRules.push_back( new Ellipsis( RbLanguageObject::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return modelMemberRules;
}

/** Get type spec */
const TypeSpec& Model::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Model::printValue(std::ostream &o) const {
    
    o << "Model";
}


/** Set a member variable */
void Model::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "" || name == "x") {
        sources.insert( var );
    }
    else {
        RbLanguageObject::setConstMemberVariable(name, var);
    }
}
