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
TimeTree::TimeTree(void) : ModelObject<RevBayesCore::TimeTree>() {
    
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TimeTree *t) : ModelObject<RevBayesCore::TimeTree>( t ) {
    
}

/** Construct from bool */
TimeTree::TimeTree(const RevBayesCore::TimeTree &t) : ModelObject<RevBayesCore::TimeTree>( new RevBayesCore::TimeTree( t ) ) {
    
}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *n) : ModelObject<RevBayesCore::TimeTree>( n ) {
    
}



/** Construct from bool */
TimeTree::TimeTree(const TimeTree &t) : ModelObject<RevBayesCore::TimeTree>( t ) {
    
}


/** Clone object */
TimeTree* TimeTree::clone(void) const {
    
	return new TimeTree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::Variable> TimeTree::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "nnodes") 
    {
        size_t n = this->dagNode->getValue().getNumberOfNodes();
        return new Variable( new Natural( n ) );
    }
    else if (name == "ntips") 
    {
        size_t n = this->dagNode->getValue().getNumberOfTips();
        return new Variable( new Natural( n ) );
    } 
    else if (name == "names") 
    {
        const std::vector<std::string>& n = this->dagNode->getValue().getTipNames();
        return new Variable( new ModelVector<RlString>( n ) );
    } 
    else if (name == "rescale")
    {
        double f = static_cast<const RealPos&>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::TimeTree &tree = dagNode->getValue();
        RevBayesCore::TreeUtilities::rescaleTree(&tree, &tree.getRoot(), f);
        
        return NULL;
    }
    
    return ModelObject<RevBayesCore::TimeTree>::executeMethod( name, args );
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


/* Get method specifications */
const RevLanguage::MethodTable& TimeTree::getMethods(void) const 
{
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) 
    {
        
        ArgumentRules* nnodesArgRules = new ArgumentRules();
        methods.addFunction("nnodes", new MemberProcedure(Natural::getClassTypeSpec(),          nnodesArgRules   ) );

        ArgumentRules* ntipsArgRules = new ArgumentRules();
        methods.addFunction("ntips", new MemberProcedure(Natural::getClassTypeSpec(),          ntipsArgRules   ) );

        ArgumentRules* heightArgRules = new ArgumentRules();
        methods.addFunction("rootAge", new MemberFunction<TimeTree, RealPos>(this,          heightArgRules   ) );

        ArgumentRules* namesArgRules = new ArgumentRules();
        methods.addFunction("names", new MemberProcedure(ModelVector<RlString>::getClassTypeSpec(),  namesArgRules    ) );

        ArgumentRules* rescaleArgRules = new ArgumentRules();
        rescaleArgRules->push_back( new ArgumentRule( "factor", true, RealPos::getClassTypeSpec() ) );
        methods.addFunction("rescale", new MemberProcedure(RlUtils::Void,                       rescaleArgRules  ) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &ModelObject<RevBayesCore::TimeTree>::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}


/** Get type spec */
const TypeSpec& TimeTree::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

