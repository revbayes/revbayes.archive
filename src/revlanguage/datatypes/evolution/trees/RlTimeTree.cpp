#include "Natural.h"
#include "RbUtil.h"
#include "RlTimeTree.h"
#include "RlMemberFunction.h"
#include "RlString.h"
#include "RealPos.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"
#include "TypeSpec.h"
#include "Vector.h"

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
RevLanguage::RevObject* TimeTree::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "nnodes") 
    {
        size_t n = this->dagNode->getValue().getNumberOfNodes();
        return new Natural( n );
    }
    else if (name == "height") 
    {
        const RevBayesCore::TopologyNode& r = this->dagNode->getValue().getTipNode( 0 );
        return new RealPos( r.getTime() );
    } 
    else if (name == "names") 
    {
        const std::vector<std::string>& n = this->dagNode->getValue().getTipNames();
        return new Vector<RlString>( n );
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


/** Get class name of object */
const std::string& TimeTree::getClassName(void) { 
    
    static std::string rbClassName = "TimeTree";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& TimeTree::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass; 
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

        ArgumentRules* heightArgRules = new ArgumentRules();
        methods.addFunction("height", new MemberProcedure(Natural::getClassTypeSpec(),          heightArgRules   ) );

        ArgumentRules* namesArgRules = new ArgumentRules();
        methods.addFunction("names", new MemberProcedure(Vector<RlString>::getClassTypeSpec(),  namesArgRules    ) );

        ArgumentRules* rescaleArgRules = new ArgumentRules();
        rescaleArgRules->push_back( new ArgumentRule( "factor", true, RealPos::getClassTypeSpec() ) );
        methods.addFunction("rescale", new MemberProcedure(RlUtils::Void,                       rescaleArgRules  ) );
        
        ArgumentRules* rootAgeArgRules = new ArgumentRules();
        methods.addFunction("rootAge", new MemberFunction<TimeTree,RealPos>( this, rootAgeArgRules   ) );

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

