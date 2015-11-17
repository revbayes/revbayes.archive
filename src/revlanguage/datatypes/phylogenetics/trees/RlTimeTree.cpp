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
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "isRoot", RlBoolean::getClassTypeSpec(), isRootArgRules ) );
    
    
    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction( new MemberFunction<TimeTree,RealPos>( "rootAge", this, heightArgRules   ) );
    
    ArgumentRules* nodeAgeArgRules = new ArgumentRules();
    nodeAgeArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<TimeTree, RealPos>( "nodeAge", this, nodeAgeArgRules   ) );
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::Tree *t) : Tree( t )
{
    
    ArgumentRules* isRootArgRules = new ArgumentRules();
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "isRoot", RlBoolean::getClassTypeSpec(), isRootArgRules ) );
    
    
    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction( new MemberFunction<TimeTree,RealPos>( "rootAge", this, heightArgRules   ) );
    
    ArgumentRules* nodeAgeArgRules = new ArgumentRules();
    nodeAgeArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<TimeTree, RealPos>( "nodeAge", this, nodeAgeArgRules   ) );

}

/** Construct from bool */
TimeTree::TimeTree(const RevBayesCore::Tree &t) : Tree( t )
{
    
    
    ArgumentRules* isRootArgRules = new ArgumentRules();
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "isRoot", RlBoolean::getClassTypeSpec(), isRootArgRules ) );
    
    
    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction( new MemberFunction<TimeTree,RealPos>( "rootAge", this, heightArgRules   ) );
    
    ArgumentRules* nodeAgeArgRules = new ArgumentRules();
    nodeAgeArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<TimeTree, RealPos>( "nodeAge", this, nodeAgeArgRules   ) );

}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TypedDagNode<RevBayesCore::Tree> *n) : Tree( n )
{
    
    
    ArgumentRules* isRootArgRules = new ArgumentRules();
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "isRoot", RlBoolean::getClassTypeSpec(), isRootArgRules ) );
    
    
    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction( new MemberFunction<TimeTree,RealPos>( "rootAge", this, heightArgRules   ) );
    
    ArgumentRules* nodeAgeArgRules = new ArgumentRules();
    nodeAgeArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<TimeTree, RealPos>( "nodeAge", this, nodeAgeArgRules   ) );

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
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Tree::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& TimeTree::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


