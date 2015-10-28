#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_simTree.h"
#include "Natural.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "Tree.h"
#include "TypeSpec.h"

#include <math.h>       /* log2 */

using namespace RevLanguage;

/** Default constructor */
Func_simTree::Func_simTree( void ) : Procedure()
{
    
}


/** Clone object */
Func_simTree* Func_simTree::clone( void ) const
{
    
    return new Func_simTree( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_simTree::execute( void )
{
    
    int numTaxa             = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
    const std::string& type = static_cast<const RlString &>( args[1].getVariable()->getRevObject() ).getValue();
    
    // the time tree object (topology + times)
    RevBayesCore::Tree *psi = new RevBayesCore::Tree();
    
    // internally we treat unrooted topologies the same as rooted
    psi->setRooted( true );
    
    RevBayesCore::TopologyNode* root = new RevBayesCore::TopologyNode();
    std::vector<RevBayesCore::TopologyNode* > nodes;
    nodes.push_back(root);
    
    if ( type == "balanced" )
    {
        simulateBalancedTree(numTaxa, nodes);
    }
    else if ( type == "caterpillar" )
    {
        simulateCaterpillarTree(numTaxa, root);
    }
    
    // initialize the topology by setting the root
    psi->setRoot(root);
    
    // set the ages recursively
    setAges(psi, *root);
    
    return new RevVariable( new TimeTree( psi ) );
}


/** Get argument rules */
const ArgumentRules& Func_simTree::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "numTaxa", Natural::getClassTypeSpec(), "How many taxa this tree has.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        std::vector<std::string> optionsCondition;
        optionsCondition.push_back( "balanced" );
        optionsCondition.push_back( "caterpillar" );
//        optionsCondition.push_back( "random" );
        argumentRules.push_back( new OptionRule( "type"    , new RlString("balanced"), optionsCondition, "The type of the shape of the topology." ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_simTree::getClassType(void)
{
    
    static std::string revType = "Func_simTree";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_simTree::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_simTree::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "simTree";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_simTree::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_simTree::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = TimeTree::getClassTypeSpec();
    
    return returnTypeSpec;
}


void Func_simTree::setAges(RevBayesCore::Tree *t, RevBayesCore::TopologyNode &n)
{
    
    if ( n.isTip() )
    {
        t->getNode( n.getIndex() ).setAge( 0.0 );
    }
    else
    {
        RevBayesCore::TopologyNode &left = n.getChild( 0 );
        setAges(t, left);
        
        RevBayesCore::TopologyNode &right = n.getChild( 1 );
        setAges(t, right);
        
        double a = t->getNode(left.getIndex()).getAge();
        double b = t->getNode(right.getIndex()).getAge();
        double max = (a > b ? a : b);
        
        t->getNode(n.getIndex()).setAge(max + 1.0);
    }
    
}


void Func_simTree::simulateBalancedTree( size_t n, std::vector<RevBayesCore::TopologyNode*> &nodes )
{
    
    // check if the number of taxa is divideable by two
    size_t half = n / 2;
    if ( (half+half) != n )
    {
        throw RbException("Bad number of taxa.");
    }
    
    std::vector<RevBayesCore::TopologyNode*> children;
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        RevBayesCore::TopologyNode *parent = nodes[i];
        
        // add a left child
        RevBayesCore::TopologyNode* leftChild = new RevBayesCore::TopologyNode(0);
        parent->addChild(leftChild);
        leftChild->setParent(parent);
        children.push_back(leftChild);
        
        // add a right child
        RevBayesCore::TopologyNode* rightChild = new RevBayesCore::TopologyNode(0);
        parent->addChild(rightChild);
        rightChild->setParent(parent);
        children.push_back(rightChild);
        
    }
    
    if ( half == 1 )
    {
        // we are done with the recursion
        for (size_t i = 0; i < children.size(); ++i)
        {
            RevBayesCore::TopologyNode *node = children[i];
            std::string name = "Taxon_" + StringUtilities::to_string(i+1);
            node->setName(name);
        }
        
    }
    else
    {
        simulateBalancedTree(half, children);
    }
    
}




void Func_simTree::simulateCaterpillarTree( size_t n, RevBayesCore::TopologyNode* node )
{
    
    
    // add a left child
    RevBayesCore::TopologyNode* leftChild = new RevBayesCore::TopologyNode(0);
    node->addChild(leftChild);
    leftChild->setParent(node);
    
    // add a right child
    RevBayesCore::TopologyNode* rightChild = new RevBayesCore::TopologyNode(0);
    node->addChild(rightChild);
    rightChild->setParent(node);
    
    std::string name = "Taxon_" + StringUtilities::to_string(n);
    rightChild->setName(name);
    
    if ( n > 2 )
    {
        simulateCaterpillarTree(n-1, leftChild);
    }
    else
    {
        std::string name = "Taxon_" + StringUtilities::to_string(n-1);
        leftChild->setName(name);
    }
    
}

