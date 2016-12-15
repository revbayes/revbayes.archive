#include "ModelVector.h"
#include "Natural.h"
#include "RbUtil.h"
#include "RlBoolean.h"
#include "RlClade.h"
#include "RlTree.h"
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
Tree::Tree(void) : ModelObject<RevBayesCore::Tree>()
{
    
    initMethods();
    
}

/** Construct from core pointer */
Tree::Tree(RevBayesCore::Tree *t) : ModelObject<RevBayesCore::Tree>( t )
{
  
    initMethods();
    
}

/** Construct from core reference */
Tree::Tree(const RevBayesCore::Tree &t) : ModelObject<RevBayesCore::Tree>( new RevBayesCore::Tree( t ) )
{

    initMethods();
    
}

/** Construct from core DAG node */
Tree::Tree(RevBayesCore::TypedDagNode<RevBayesCore::Tree> *n) : ModelObject<RevBayesCore::Tree>( n )
{
    
    initMethods();
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Tree* Tree::clone(void) const
{
    
    return new Tree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::RevVariable> Tree::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "dropTip")
    {
        found = true;
        
        const RevObject &taxon = args[0].getVariable()->getRevObject();
        std::string taxon_name = "";
        if ( taxon.isType( RlString::getClassTypeSpec() ) )
        {
            taxon_name = static_cast<const RlString&>( taxon ).getValue();
        }
        else
        {
            taxon_name = static_cast<const Taxon&>( taxon ).getValue().getSpeciesName();
        }

        this->dagNode->getValue().dropTipNodeWithName( taxon_name );
        
        return NULL;
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
    else if (name == "names" || name == "taxa")
    {
        found = true;
        
        std::vector<RevBayesCore::Taxon> t = this->dagNode->getValue().getTaxa();
        return new RevVariable( new ModelVector<Taxon>( t ) );
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
        RevBayesCore::Tree &tree = dagNode->getValue();
        RevBayesCore::TreeUtilities::rescaleTree(&tree, &tree.getRoot(), f);
        
        return NULL;
    }
    
    return ModelObject<RevBayesCore::Tree>::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& Tree::getClassType(void)
{
    
    static std::string rev_type = "Tree";
    
    return rev_type;
}

/** Get class type spec describing type of object */
const TypeSpec& Tree::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/** Get type spec */
const TypeSpec& Tree::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/**
 * Initialize the member methods.
 */
void Tree::initMethods( void )
{
    
    ArgumentRules* isInternalArgRules = new ArgumentRules();
    isInternalArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "isInternal", RlBoolean::getClassTypeSpec(), isInternalArgRules ) );
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "nnodes", Natural::getClassTypeSpec(), nnodesArgRules ) );
    
    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "ntips", Natural::getClassTypeSpec(), ntipsArgRules ) );
    
    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "names", ModelVector<RlString>::getClassTypeSpec(), namesArgRules ) );
    
    ArgumentRules* taxaArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "taxa", ModelVector<Taxon>::getClassTypeSpec(), taxaArgRules ) );
    
    ArgumentRules* nodeNameArgRules = new ArgumentRules();
    nodeNameArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "nodeName", RlString::getClassTypeSpec(),  nodeNameArgRules ) );

    ArgumentRules* drop_tip_arg_rules = new ArgumentRules();
    std::vector<TypeSpec> tip_types;
    tip_types.push_back( RlString::getClassTypeSpec() );
    tip_types.push_back( Taxon::getClassTypeSpec() );
    drop_tip_arg_rules->push_back( new ArgumentRule( "node", tip_types, "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "dropTip", RlUtils::Void,  drop_tip_arg_rules ) );

    
    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), "The scaling factor.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "rescale", RlUtils::Void, rescaleArgRules ) );
    
    
    // member functions
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, Natural>( "parent", this, parentArgRules   ) );
    
    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RealPos>( "branchLength", this, branchLengthArgRules   ) );
    
    ArgumentRules* containedInCaldeArgRules = new ArgumentRules();
    containedInCaldeArgRules->push_back( new ArgumentRule( "node" , Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    containedInCaldeArgRules->push_back( new ArgumentRule( "clade", Clade::getClassTypeSpec()  , "The embracing clade.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RlBoolean>( "isContainedInClade", this, containedInCaldeArgRules ) );
    
    ArgumentRules* treeLengthArgRules = new ArgumentRules();
    methods.addFunction( new MemberFunction<Tree, RealPos>( "treeLength", this, treeLengthArgRules   ) );

    
}


