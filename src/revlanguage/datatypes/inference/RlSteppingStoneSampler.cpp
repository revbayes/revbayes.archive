#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "RevObject.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "Real.h"
#include "RlSteppingStoneSampler.h"
#include "RlString.h"
#include "TypeSpec.h"


using namespace RevLanguage;

SteppingStoneSampler::SteppingStoneSampler() : WorkspaceObject<RevBayesCore::SteppingStoneSampler>() {
    
}


/** Clone object */
SteppingStoneSampler* SteppingStoneSampler::clone(void) const {
    
	return new SteppingStoneSampler(*this);
}


void SteppingStoneSampler::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // get the parameter values
    const std::string&   fn      = static_cast<const RlString &>( filename->getValue() ).getValue();
    const std::string&   pn      = static_cast<const RlString &>( powerColumnName->getValue() ).getValue();
    const std::string&   ln      = static_cast<const RlString &>( likelihoodColumnName->getValue() ).getValue();
    const std::string&   del     = static_cast<const RlString &>( delimmiter->getValue() ).getValue();
    
    value = new RevBayesCore::SteppingStoneSampler(fn, pn, ln, del);
    
}


/* Map calls to member methods */
RevObject* SteppingStoneSampler::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "marginal")
    {
        
        double ml = value->marginalLikelihood();
        
        return new Real( ml );
    }
    
    return RevObject::executeMethod( name, args );
}


/** Get class name of object */
const std::string& SteppingStoneSampler::getClassName(void)
{
    
    static std::string rbClassName = "SteppingStoneSampler";
    
	return rbClassName;
}

/** Get class type spec describing type of object */
const TypeSpec& SteppingStoneSampler::getClassTypeSpec(void)
{
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( WorkspaceObject<RevBayesCore::SteppingStoneSampler>::getClassTypeSpec() ) );
    
	return rbClass;
}



/** Return member rules (no members) */
const MemberRules& SteppingStoneSampler::getMemberRules(void) const {
    
    static MemberRules samplerMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        samplerMemberRules.push_back( new ArgumentRule("filename", true, RlString::getClassTypeSpec() ) );
        samplerMemberRules.push_back( new ArgumentRule("powerColumnName", true, RlString::getClassTypeSpec() ) );
        samplerMemberRules.push_back( new ArgumentRule("likelihoodColumnName", true, RlString::getClassTypeSpec() ) );
        samplerMemberRules.push_back( new ArgumentRule("delimmiter", true, RlString::getClassTypeSpec(), new RlString( "\t" ) ) );
        
        rulesSet = true;
    }
    
    return samplerMemberRules;
}


/* Get method specifications */
const MethodTable& SteppingStoneSampler::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false )
    {
        ArgumentRules* marginalArgRules = new ArgumentRules();
        methods.addFunction("marginal", new MemberFunction( Real::getClassTypeSpec(), marginalArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
const TypeSpec& SteppingStoneSampler::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void SteppingStoneSampler::printValue(std::ostream &o) const {
    
    o << "SteppingStoneSampler";
}


/** Set a member variable */
void SteppingStoneSampler::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "likelihoodColumnName")
    {
        likelihoodColumnName = var;
    }
    else if ( name == "powerColumnName")
    {
        powerColumnName = var;
    }
    else if ( name == "filename")
    {
        filename = var;
    }
    else if ( name == "delimmiter")
    {
        delimmiter = var;
    }
    else
    {
        RevObject::setConstMemberVariable(name, var);
    }
}
