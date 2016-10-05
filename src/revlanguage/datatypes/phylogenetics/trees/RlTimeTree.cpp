#include "ModelVector.h"
#include "Natural.h"
#include "RbUtil.h"
#include "RlBoolean.h"
#include "RlTimeTree.h"
#include "RlMemberFunction.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RealPos.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
TimeTree::TimeTree(void) : Tree()
{

    // initialize the member methods
    initMethods();
    
}

/** Construct from core object */
TimeTree::TimeTree(RevBayesCore::Tree *t) : Tree( t )
{
    
    // initialize the member methods
    initMethods();

}

/** Construct from bool */
TimeTree::TimeTree(const RevBayesCore::Tree &t) : Tree( t )
{
    
    
    // initialize the member methods
    initMethods();

}

/** Construct from bool */
TimeTree::TimeTree(RevBayesCore::TypedDagNode<RevBayesCore::Tree> *n) : Tree( n )
{
    
    
    // initialize the member methods
    initMethods();

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
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
    else if (name == "getFossils")
    {
        found = true;
        
        std::vector<RevBayesCore::Taxon> t = this->dagNode->getValue().getFossilTaxa();
        return new RevVariable( new ModelVector<Taxon>( t ) );
    }
    else if (name == "nSampledAncestors")
    {
        found = true;

        size_t n = this->dagNode->getValue().getNumberOfTips();

        size_t num = 0;
        for(size_t i=0; i<n; i++){
            RevBayesCore::TopologyNode &node = this->dagNode->getValue().getNode(i);
            num += node.isSampledAncestor();
        }
        return new RevVariable( new Natural( num ) );
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
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Tree::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/** Get type spec */
const TypeSpec& TimeTree::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


void TimeTree::initMethods( void )
{
    
    ArgumentRules* isRootArgRules = new ArgumentRules();
    isRootArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "isRoot", RlBoolean::getClassTypeSpec(), isRootArgRules ) );

    ArgumentRules* getFossilsArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "getFossils", ModelVector<Taxon>::getClassTypeSpec(), getFossilsArgRules ) );

    ArgumentRules* nSampledAncestorsArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "nSampledAncestors", Natural::getClassTypeSpec(), nSampledAncestorsArgRules ) );

    // member functions
    ArgumentRules* heightArgRules = new ArgumentRules();
    methods.addFunction( new MemberFunction<TimeTree,RealPos>( "rootAge", this, heightArgRules   ) );
    
    ArgumentRules* nodeAgeArgRules = new ArgumentRules();
    nodeAgeArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<TimeTree, RealPos>( "nodeAge", this, nodeAgeArgRules   ) );

}
