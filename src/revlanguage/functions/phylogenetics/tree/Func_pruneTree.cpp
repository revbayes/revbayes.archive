#include "Func_pruneTree.h"
#include "ModelVector.h"
#include "PruneTreeFunction.h"
#include "RlBoolean.h"
#include "Real.h"
#include "RealPos.h"
#include "RlTree.h"
#include "RlDeterministicNode.h"
#include "RlTaxon.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_pruneTree::Func_pruneTree( void ) : TypedFunction<Tree>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_pruneTree* Func_pruneTree::clone( void ) const
{
    
    return new Func_pruneTree( *this );
}


RevBayesCore::TypedFunction<RevBayesCore::Tree>* Func_pruneTree::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const Tree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    ModelVector<Taxon> rtv = static_cast<const ModelVector<Taxon>&>( this->args[1].getVariable()->getRevObject() ).getValue();
    std::set<RevBayesCore::Taxon> rts;
    for (size_t i = 0; i < rtv.size(); i++)
        rts.insert(rtv[i]);
    ModelVector<Taxon> ptv = static_cast<const ModelVector<Taxon>&>( this->args[2].getVariable()->getRevObject() ).getValue();
    std::set<RevBayesCore::Taxon> pts;
    for (size_t i = 0; i < ptv.size(); i++)
        pts.insert(ptv[i]);
    bool pf = static_cast<const RlBoolean &>( this->args[3].getVariable()->getRevObject() ).getValue();
    
    RevBayesCore::PruneTreeFunction* f = new RevBayesCore::PruneTreeFunction( tau, rts, pts, pf );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_pruneTree::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "tree", Tree::getClassTypeSpec(), "The tree variable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "retainTaxa" , ModelVector<Taxon>::getClassTypeSpec() , "Taxon set to retain from tree.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<Taxon>() ) );

        argumentRules.push_back( new ArgumentRule( "pruneTaxa" , ModelVector<Taxon>::getClassTypeSpec() , "Taxon set to prune from tree.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<Taxon>() ) );
        argumentRules.push_back( new ArgumentRule( "pruneFossils" , RlBoolean::getClassTypeSpec() , "Prune fossils from tree?", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RlBoolean(false) ) );
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_pruneTree::getClassType(void)
{
    
    static std::string rev_type = "Func_pruneTree";
    
    return rev_type;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_pruneTree::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}



/**
 * Get the primary Rev name for this function.
 */
std::string Func_pruneTree::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnPruneTree";
    
    return f_name;
}


const TypeSpec& Func_pruneTree::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
