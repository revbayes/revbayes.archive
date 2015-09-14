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
TimeTree::TimeTree(void) : ModelObject<RevBayesCore::TimeTree>()
{
    
    ArgumentRules* isRootArgRules = new ArgumentRules();
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("isRoot", new MemberProcedure(RlBoolean::getClassTypeSpec(), isRootArgRules ) );
    
    ArgumentRules* isInternalArgRules = new ArgumentRules();
    isInternalArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("isInternal", new MemberProcedure(RlBoolean::getClassTypeSpec(), isInternalArgRules ) );
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(), nnodesArgRules ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction("ntips", new MemberProcedure(Natural::getClassTypeSpec(), ntipsArgRules ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules ) );
    
    ArgumentRules* nodeNameArgRules = new ArgumentRules();
    nodeNameArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("nodeName", new MemberProcedure(RlString::getClassTypeSpec(),  nodeNameArgRules ) );

    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("rescale", new MemberProcedure(RlUtils::Void, rescaleArgRules ) );

    
    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("rootAge", new MemberFunction<TimeTree,RealPos>(this, heightArgRules   ) );
    
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("parent", new MemberFunction<TimeTree, Natural>(this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("branchLength", new MemberFunction<TimeTree, RealPos>(this, branchLengthArgRules   ) );
    
    ArgumentRules* nodeAgeArgRules = new ArgumentRules();
    nodeAgeArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("nodeAge", new MemberFunction<TimeTree, RealPos>(this, nodeAgeArgRules   ) );
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TimeTree *t) : ModelObject<RevBayesCore::TimeTree>( t )
{
    
    ArgumentRules* isRootArgRules = new ArgumentRules();
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("isRoot", new MemberProcedure(RlBoolean::getClassTypeSpec(), isRootArgRules ) );
    
    ArgumentRules* isInternalArgRules = new ArgumentRules();
    isInternalArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("isInternal", new MemberProcedure(RlBoolean::getClassTypeSpec(), isInternalArgRules ) );
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),          nnodesArgRules   ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction("ntips", new MemberProcedure(Natural::getClassTypeSpec(),          ntipsArgRules   ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules    ) );
    
    ArgumentRules* nodeNameArgRules = new ArgumentRules();
    nodeNameArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("nodeName", new MemberProcedure(RlString::getClassTypeSpec(),  nodeNameArgRules    ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("rescale", new MemberProcedure(RlUtils::Void,                       rescaleArgRules  ) );
    
    
    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("rootAge", new MemberFunction<TimeTree,RealPos>(this, heightArgRules   ) );
    
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("parent", new MemberFunction<TimeTree, Natural>(this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("branchLength", new MemberFunction<TimeTree, RealPos>(this, branchLengthArgRules   ) );
    
    ArgumentRules* nodeAgeArgRules = new ArgumentRules();
    nodeAgeArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("nodeAge", new MemberFunction<TimeTree, RealPos>(this, nodeAgeArgRules   ) );
}

/** Construct from bool */
TimeTree::TimeTree(const RevBayesCore::TimeTree &t) : ModelObject<RevBayesCore::TimeTree>( new RevBayesCore::TimeTree( t ) )
{
    
    ArgumentRules* isRootArgRules = new ArgumentRules();
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("isRoot", new MemberProcedure(RlBoolean::getClassTypeSpec(), isRootArgRules ) );
    
    ArgumentRules* isInternalArgRules = new ArgumentRules();
    isInternalArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("isInternal", new MemberProcedure(RlBoolean::getClassTypeSpec(), isInternalArgRules ) );
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(), nnodesArgRules   ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction("ntips", new MemberProcedure(Natural::getClassTypeSpec(), ntipsArgRules   ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules    ) );
    
    ArgumentRules* nodeNameArgRules = new ArgumentRules();
    nodeNameArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("nodeName", new MemberProcedure(RlString::getClassTypeSpec(),  nodeNameArgRules    ) );

    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("rescale", new MemberProcedure(RlUtils::Void, rescaleArgRules  ) );
    
    
    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("rootAge", new MemberFunction<TimeTree,RealPos>(this, heightArgRules   ) );
    
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("parent", new MemberFunction<TimeTree, Natural>(this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("branchLength", new MemberFunction<TimeTree, RealPos>(this, branchLengthArgRules   ) );
    
    ArgumentRules* nodeAgeArgRules = new ArgumentRules();
    nodeAgeArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("nodeAge", new MemberFunction<TimeTree, RealPos>(this, nodeAgeArgRules   ) );
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *n) : ModelObject<RevBayesCore::TimeTree>( n )
{
    
    ArgumentRules* isRootArgRules = new ArgumentRules();
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("isRoot", new MemberProcedure(RlBoolean::getClassTypeSpec(), isRootArgRules ) );
    
    ArgumentRules* isInternalArgRules = new ArgumentRules();
    isInternalArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("isInternal", new MemberProcedure(RlBoolean::getClassTypeSpec(), isInternalArgRules ) );
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(), nnodesArgRules   ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction("ntips", new MemberProcedure(Natural::getClassTypeSpec(), ntipsArgRules   ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(), namesArgRules    ) );
    
    ArgumentRules* nodeNameArgRules = new ArgumentRules();
    nodeNameArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("nodeName", new MemberProcedure(RlString::getClassTypeSpec(),  nodeNameArgRules    ) );

    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("rescale", new MemberProcedure(RlUtils::Void, rescaleArgRules  ) );
    
    
    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("rootAge", new MemberFunction<TimeTree,RealPos>(this, heightArgRules   ) );
    
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("parent", new MemberFunction<TimeTree, Natural>(this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("branchLength", new MemberFunction<TimeTree, RealPos>(this, branchLengthArgRules   ) );
    
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
    else if (name == "isInternal")
    {
        found = true;
        
        int index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() ).getValue() - 1;
        
        bool tf = this->dagNode->getValue().getNode((size_t)index).isInternal();
        return new RevVariable( new RlBoolean( tf ) );
    }
    else if (name == "nnodes")
    {
        found = true;
        
        size_t n = this->dagNode->getValue().getNumberOfNodes();
        return new RevVariable( new Natural( n ) );
    }
    else if (name == "ntips") 
    {
        found = true;
        
        size_t n = this->dagNode->getValue().getNumberOfTips();
        return new RevVariable( new Natural( n ) );
    } 
    else if (name == "names") 
    {
        found = true;
        
        const std::vector<std::string>& n = this->dagNode->getValue().getTipNames();
        return new RevVariable( new ModelVector<RlString>( n ) );
    }
    else if (name == "nodeName")
    {
        found = true;
        
        int index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() ).getValue() - 1;
        const std::string& n = this->dagNode->getValue().getNode((size_t)index).getName();
        return new RevVariable( new RlString( n ) );
    }
    else if (name == "rescale")
    {
        found = true;
        
        double f = static_cast<const RealPos&>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::TimeTree &tree = dagNode->getValue();
        RevBayesCore::TreeUtilities::rescaleTree(&tree, &tree.getRoot(), f);
        
        return NULL;
    }
    
    return ModelObject<RevBayesCore::TimeTree>::executeMethod( name, args, found );
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


