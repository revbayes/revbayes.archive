
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Ellipsis.h"
#include "ExtendedNewickTreeMonitor.h"
#include "Mntr_ExtendedNewickFile.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RevObject.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;



Mntr_ExtendedNewickFile::Mntr_ExtendedNewickFile(void) : Monitor()
{
    
}


/** Clone object */
Mntr_ExtendedNewickFile* Mntr_ExtendedNewickFile::clone(void) const
{
    
	return new Mntr_ExtendedNewickFile(*this);
}


void Mntr_ExtendedNewickFile::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const std::string& fn = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string& sep = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int g = static_cast<const Natural &>( printgen->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::Tree> *t = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    vars.erase( unique( vars.begin(), vars.end() ), vars.end() );
    sort( vars.begin(), vars.end(), compareVarNames );
    std::vector<RevBayesCore::DagNode *> n;
    for (std::vector<RevPtr<const RevVariable> >::iterator i = vars.begin(); i != vars.end(); ++i)
    {
        RevBayesCore::DagNode* node = (*i)->getRevObject().getDagNode();
        n.push_back( node );
    }
//    
//    
//    std::set<RevBayesCore::DagNode*> n;
//    for (std::set<RevPtr<const RevVariable> >::iterator i = vars.begin(); i != vars.end(); ++i)
//    {
//        RevBayesCore::DagNode* node = (*i)->getRevObject().getDagNode();
//        n.insert( node );
//    }
    bool np = static_cast<const RlBoolean &>( isNodeParameter->getRevObject() ).getValue();
    bool pp = static_cast<const RlBoolean &>( posterior->getRevObject() ).getValue();
    bool l = static_cast<const RlBoolean &>( likelihood->getRevObject() ).getValue();
    bool pr = static_cast<const RlBoolean &>( prior->getRevObject() ).getValue();
    value = new RevBayesCore::ExtendedNewickTreeMonitor(t, n, np, size_t(g), fn, sep, pp, l, pr);
}


/** Get Rev type of object */
const std::string& Mntr_ExtendedNewickFile::getClassType(void)
{
    
    static std::string revType = "Mntr_ExtendedNewickFile";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Mntr_ExtendedNewickFile::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Mntr_ExtendedNewickFile::getMonitorName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "ExtNewick";
    
    return c_name;
}


/** Return member rules (no members) */
const MemberRules& Mntr_ExtendedNewickFile::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
    
        memberRules.push_back( new ArgumentRule("filename", RlString::getClassTypeSpec(), "The name of the file.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule("tree"    , TimeTree::getClassTypeSpec(), "The tree variable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new Ellipsis( "Variables at nodes or branches.", RevObject::getClassTypeSpec() ) );
        memberRules.push_back( new ArgumentRule("isNodeParameter" , RlBoolean::getClassTypeSpec(), "Is this a node or branch parameter?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        memberRules.push_back( new ArgumentRule("printgen"  , Natural::getClassTypeSpec()  , "How frequently do we print.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        memberRules.push_back( new ArgumentRule("separator" , RlString::getClassTypeSpec() , "The separator between variables.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString(" ") ) );
        memberRules.push_back( new ArgumentRule("posterior" , RlBoolean::getClassTypeSpec(), "Should we print the posterior probability as well.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        memberRules.push_back( new ArgumentRule("likelihood", RlBoolean::getClassTypeSpec(), "Should we print the likelihood as well?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        memberRules.push_back( new ArgumentRule("prior"     , RlBoolean::getClassTypeSpec(), "Should we print the prior probability as well?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& Mntr_ExtendedNewickFile::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mntr_ExtendedNewickFile::printValue(std::ostream &o) const
{
    
    o << "Mntr_ExtendedNewickFile";
}


/** Set a member variable */
void Mntr_ExtendedNewickFile::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "" )
    {
        vars.push_back( var );
    }
    else if ( name == "filename" )
    {
        filename = var;
    }
    else if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "isNodeParameter" )
    {
        isNodeParameter = var;
    }
    else if ( name == "separator" )
    {
        separator = var;
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
