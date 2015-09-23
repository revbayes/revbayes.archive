#include "ModelVector.h"
#include "Natural.h"
#include "RbUtil.h"
#include "RlBoolean.h"
#include "RlTimeTree.h"
#include "RlMemberFunction.h"
#include "RlString.h"
#include "RealPos.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
TimeTree::TimeTree(void) : Tree()
{
    
    ArgumentRules* isRootArgRules = new ArgumentRules();
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("isRoot", new MemberProcedure(RlBoolean::getClassTypeSpec(), isRootArgRules ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("rescale", new MemberProcedure(RlUtils::Void, rescaleArgRules ) );

    
    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("rootAge", new MemberFunction<TimeTree,RealPos>(this, heightArgRules   ) );
    
    ArgumentRules* nodeAgeArgRules = new ArgumentRules();
    nodeAgeArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("nodeAge", new MemberFunction<TimeTree, RealPos>(this, nodeAgeArgRules   ) );
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::Tree *t) : Tree( t )
{
    
    ArgumentRules* isRootArgRules = new ArgumentRules();
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("isRoot", new MemberProcedure(RlBoolean::getClassTypeSpec(), isRootArgRules ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("rescale", new MemberProcedure(RlUtils::Void,                       rescaleArgRules  ) );
    
    
    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("rootAge", new MemberFunction<TimeTree,RealPos>(this, heightArgRules   ) );
    
    ArgumentRules* nodeAgeArgRules = new ArgumentRules();
    nodeAgeArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("nodeAge", new MemberFunction<TimeTree, RealPos>(this, nodeAgeArgRules   ) );
}

/** Construct from bool */
TimeTree::TimeTree(const RevBayesCore::Tree &t) : Tree( t )
{
    
    ArgumentRules* isRootArgRules = new ArgumentRules();
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("isRoot", new MemberProcedure(RlBoolean::getClassTypeSpec(), isRootArgRules ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("rescale", new MemberProcedure(RlUtils::Void, rescaleArgRules  ) );
    
    
    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("rootAge", new MemberFunction<TimeTree,RealPos>(this, heightArgRules   ) );
    
    ArgumentRules* nodeAgeArgRules = new ArgumentRules();
    nodeAgeArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("nodeAge", new MemberFunction<TimeTree, RealPos>(this, nodeAgeArgRules   ) );
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TypedDagNode<RevBayesCore::Tree> *n) : Tree( n )
{
    
    ArgumentRules* isRootArgRules = new ArgumentRules();
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("isRoot", new MemberProcedure(RlBoolean::getClassTypeSpec(), isRootArgRules ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("rescale", new MemberProcedure(RlUtils::Void, rescaleArgRules  ) );
    
    
    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("rootAge", new MemberFunction<TimeTree,RealPos>(this, heightArgRules   ) );
    
    ArgumentRules* nodeAgeArgRules = new ArgumentRules();
    nodeAgeArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("nodeAge", new MemberFunction<TimeTree, RealPos>(this, nodeAgeArgRules   ) );
}


/** Clone object */
TimeTree* TimeTree::clone(void) const
{
    
	return new TimeTree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::RevVariable> TimeTree::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "isRoot")
    {
        found = true;
        
        int index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() ).getValue() - 1;
        
        bool tf = this->dagNode->getValue().getNode((size_t)index).isRoot();
        return new RevVariable( new RlBoolean( tf ) );
    }
    else if (name == "rescale")
    {
        found = true;
        
        double f = static_cast<const RealPos&>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::Tree &tree = dagNode->getValue();
        RevBayesCore::TreeUtilities::rescaleTree(&tree, &tree.getRoot(), f);
        
        return NULL;
    }
    
    return Tree::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& TimeTree::getClassType(void)
{
    
    static std::string revType = "TimeTree";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& TimeTree::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& TimeTree::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


