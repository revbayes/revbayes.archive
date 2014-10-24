#include "ModelVector.h"
#include "Natural.h"
#include "RbUtil.h"
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

    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),          nnodesArgRules   ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction("ntips", new MemberProcedure(Natural::getClassTypeSpec(),          ntipsArgRules   ) );
    
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("rootAge", new MemberProcedure(RealPos::getClassTypeSpec(),          heightArgRules   ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules    ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("rescale", new MemberProcedure(RlUtils::Void,                       rescaleArgRules  ) );
    
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TimeTree *t) : ModelObject<RevBayesCore::TimeTree>( t )
{

    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),          nnodesArgRules   ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction("ntips", new MemberProcedure(Natural::getClassTypeSpec(),          ntipsArgRules   ) );
    
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("rootAge", new MemberProcedure(RealPos::getClassTypeSpec(),          heightArgRules   ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules    ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("rescale", new MemberProcedure(RlUtils::Void,                       rescaleArgRules  ) );
    
}

/** Construct from bool */
TimeTree::TimeTree(const RevBayesCore::TimeTree &t) : ModelObject<RevBayesCore::TimeTree>( new RevBayesCore::TimeTree( t ) )
{

    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),          nnodesArgRules   ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction("ntips", new MemberProcedure(Natural::getClassTypeSpec(),          ntipsArgRules   ) );
    
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("rootAge", new MemberProcedure(RealPos::getClassTypeSpec(),          heightArgRules   ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules    ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("rescale", new MemberProcedure(RlUtils::Void,                       rescaleArgRules  ) );
    
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *n) : ModelObject<RevBayesCore::TimeTree>( n )
{

    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),          nnodesArgRules   ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction("ntips", new MemberProcedure(Natural::getClassTypeSpec(),          ntipsArgRules   ) );
    
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction("rootAge", new MemberProcedure(RealPos::getClassTypeSpec(),          heightArgRules   ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules    ) );
    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("rescale", new MemberProcedure(RlUtils::Void,                       rescaleArgRules  ) );
    
}


/** Clone object */
TimeTree* TimeTree::clone(void) const {
    
	return new TimeTree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::Variable> TimeTree::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "nnodes") 
    {
        found = true;
        
        size_t n = this->dagNode->getValue().getNumberOfNodes();
        return new Variable( new Natural( n ) );
    }
    else if (name == "ntips") 
    {
        found = true;
        
        size_t n = this->dagNode->getValue().getNumberOfTips();
        return new Variable( new Natural( n ) );
    } 
    else if (name == "names") 
    {
        found = true;
        
        const std::vector<std::string>& n = this->dagNode->getValue().getTipNames();
        return new Variable( new ModelVector<RlString>( n ) );
    } 
    else if (name == "rescale")
    {
        found = true;
        
        double f = static_cast<const RealPos&>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::TimeTree &tree = dagNode->getValue();
        RevBayesCore::TreeUtilities::rescaleTree(&tree, &tree.getRoot(), f);
        
        return NULL;
    }
    else if (name == "rootAge")
    {
        found = true;
        
        double a = this->dagNode->getValue().getRoot().getAge();
        return new Variable( new RealPos( a ) );
    }
    
    return ModelObject<RevBayesCore::TimeTree>::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& TimeTree::getClassType(void) { 
    
    static std::string revType = "TimeTree";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& TimeTree::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& TimeTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


