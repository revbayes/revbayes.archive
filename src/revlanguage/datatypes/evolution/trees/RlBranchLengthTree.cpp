#include "ModelVector.h"
#include "Natural.h"
#include "RbUtil.h"
#include "RlBranchLengthTree.h"
#include "RlMemberFunction.h"
#include "RlString.h"
#include "RealPos.h"
#include "TypeSpec.h"
#include "Topology.h"
#include "RlTopology.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
BranchLengthTree::BranchLengthTree(void) : ModelObject<RevBayesCore::BranchLengthTree>()
{

    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules       ) );
    
    ArgumentRules* rerootArgRules = new ArgumentRules();
    rerootArgRules->push_back( new ArgumentRule("leaf"    , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE) );
    
    methods.addFunction("reroot", new MemberProcedure(RlUtils::Void,  rerootArgRules       ) );

	ArgumentRules* topologyArgRules = new ArgumentRules();
    methods.addFunction("topology", new MemberProcedure(RevLanguage::Topology::getClassTypeSpec(),  topologyArgRules       ) );
    
    
    // member functions
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("parent", new MemberFunction<BranchLengthTree, Natural>(this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("branchLength", new MemberFunction<BranchLengthTree, RealPos>(this, branchLengthArgRules   ) );

}

/** Construct from bool */
BranchLengthTree::BranchLengthTree(RevBayesCore::BranchLengthTree *t) : ModelObject<RevBayesCore::BranchLengthTree>( t )
{

    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules       ) );
    
    ArgumentRules* rerootArgRules = new ArgumentRules();
    rerootArgRules->push_back( new ArgumentRule("leaf"    , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE) );
    
    methods.addFunction("reroot", new MemberProcedure(RlUtils::Void,  rerootArgRules       ) );

	ArgumentRules* topologyArgRules = new ArgumentRules();
    methods.addFunction("topology", new MemberProcedure(RevLanguage::Topology::getClassTypeSpec(),  topologyArgRules       ) );
    
    
    // member functions
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("parent", new MemberFunction<BranchLengthTree, Natural>(this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("branchLength", new MemberFunction<BranchLengthTree, RealPos>(this, branchLengthArgRules   ) );

}

/** Construct from bool */
BranchLengthTree::BranchLengthTree(const RevBayesCore::BranchLengthTree &t) : ModelObject<RevBayesCore::BranchLengthTree>( new RevBayesCore::BranchLengthTree( t ) )
{
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules       ) );
    
    ArgumentRules* rerootArgRules = new ArgumentRules();
    rerootArgRules->push_back( new ArgumentRule("leaf"    , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE) );
    
    methods.addFunction("reroot", new MemberProcedure(RlUtils::Void,  rerootArgRules       ) );
    
	ArgumentRules* topologyArgRules = new ArgumentRules();
    methods.addFunction("topology", new MemberProcedure(RevLanguage::Topology::getClassTypeSpec(),  topologyArgRules       ) );
    
    
    // member functions
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("parent", new MemberFunction<BranchLengthTree, Natural>(this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("branchLength", new MemberFunction<BranchLengthTree, RealPos>(this, branchLengthArgRules   ) );
    
}

/** Construct from bool */
BranchLengthTree::BranchLengthTree(RevBayesCore::TypedDagNode<RevBayesCore::BranchLengthTree> *n) : ModelObject<RevBayesCore::BranchLengthTree>( n )
{

    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),       nnodesArgRules              ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules       ) );
    
    ArgumentRules* rerootArgRules = new ArgumentRules();
    rerootArgRules->push_back( new ArgumentRule("leaf"    , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE) );
    
    methods.addFunction("reroot", new MemberProcedure(RlUtils::Void,  rerootArgRules       ) );

	ArgumentRules* topologyArgRules = new ArgumentRules();
    methods.addFunction("topology", new MemberProcedure(RevLanguage::Topology::getClassTypeSpec(),  topologyArgRules       ) );
    
    
    // member functions
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("parent", new MemberFunction<BranchLengthTree, Natural>(this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    methods.addFunction("branchLength", new MemberFunction<BranchLengthTree, RealPos>(this, branchLengthArgRules   ) );

}


/** Clone object */
BranchLengthTree* BranchLengthTree::clone(void) const {
    
	return new BranchLengthTree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::RevVariable> BranchLengthTree::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "nnodes")
    {
        found = true;
        
        size_t n = this->dagNode->getValue().getNumberOfNodes();
        return new RevVariable( new Natural( n ) );
    }
    else if (name == "names")
    {
        found = true;
        
        const std::vector<std::string>& n = this->dagNode->getValue().getTipNames();
        return new RevVariable( new ModelVector<RlString>( n ) );
    }
    else if (name == "reroot")
    {
        
        const RevObject& st = args[0].getVariable()->getRevObject();
        if ( st.isType( RlString::getClassTypeSpec() ) )
        {
            std::string n = std::string( static_cast<const RlString&>( st ).getValue() );
            this->dagNode->getValue().reroot(n);
        }
        return NULL;
        
    }
	else if (name == "topology") {
		found = true;
        const RevBayesCore::Topology& t = this->dagNode->getValue().getTopology();
        return new RevVariable( new RevLanguage::Topology( t ) );
    } 
    
    return ModelObject<RevBayesCore::BranchLengthTree>::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& BranchLengthTree::getClassType(void) { 
    
    static std::string revType = "BranchLengthTree";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& BranchLengthTree::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& BranchLengthTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

