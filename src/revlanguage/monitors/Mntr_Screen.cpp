
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Ellipsis.h"
#include "ModelVector.h"
#include "Mntr_Screen.h"
#include "Natural.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlString.h"
#include "ScreenMonitor.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Mntr_Screen::Mntr_Screen(void) : Monitor() {
    
}


/** Clone object */
Mntr_Screen* Mntr_Screen::clone(void) const {
    
	return new Mntr_Screen(*this);
}


void Mntr_Screen::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate space for a new Mntr_Screen object
    int g = static_cast<const Natural &>( printgen->getRevObject() ).getValue();
    
    std::set<RevBayesCore::DagNode *> n;
    for (std::set<RevPtr<const Variable> >::iterator i = vars.begin(); i != vars.end(); ++i)
    {
        RevBayesCore::DagNode* node = (*i)->getRevObject().getDagNode();
        n.insert( node );
    }
    
    bool pp = static_cast<const RlBoolean &>( posterior->getRevObject() ).getValue();
    bool l = static_cast<const RlBoolean &>( likelihood->getRevObject() ).getValue();
    bool pr = static_cast<const RlBoolean &>( prior->getRevObject() ).getValue();
    value = new RevBayesCore::ScreenMonitor(n, g, pp, l, pr);
}


/** Get Rev type of object */
const std::string& Mntr_Screen::getClassType(void)
{
    
    static std::string revType = "Mntr_Screen";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Mntr_Screen::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Mntr_Screen::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new Ellipsis( RevObject::getClassTypeSpec() ) );
        memberRules.push_back( new ArgumentRule("printgen"  , Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        memberRules.push_back( new ArgumentRule("posterior" , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        memberRules.push_back( new ArgumentRule("likelihood", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        memberRules.push_back( new ArgumentRule("prior"     , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& Mntr_Screen::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mntr_Screen::printValue(std::ostream &o) const
{
    
    o << "Mntr_Screen";
}


/** Set a member variable */
void Mntr_Screen::setConstParameter(const std::string& name, const RevPtr<const Variable> &var)
{
    
    if ( name == "" )
    {
        vars.insert( var );
    }
    else if ( name == "printgen" )
    {
        printgen = var;
    }
    else if ( name == "prior" )
    {
        prior = var;
    }
    else if ( name == "posterior" )
    {
        posterior = var;
    }
    else if ( name == "likelihood" )
    {
        likelihood = var;
    }
    else
    {
        RevObject::setConstParameter(name, var);
    }
}
