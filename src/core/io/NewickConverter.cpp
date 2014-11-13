#include "AdmixtureTree.h"
#include "BranchLengthTree.h"
#include "NewickConverter.h"
#include "RbException.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "Tree.h"
#include <boost/lexical_cast.hpp>

#include <sstream>

using namespace RevBayesCore;

NewickConverter::NewickConverter() {

}


NewickConverter::~NewickConverter() {
    
}



BranchLengthTree* NewickConverter::convertFromNewick(std::string const &n) {
    
    // create and allocate the tree object
    BranchLengthTree *t = new BranchLengthTree();
    
    Topology *tau = new Topology();
    
    std::vector<TopologyNode*> nodes;
    std::vector<double> brlens;
    
    // create a string-stream and throw the string into it
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    ss << n;
    
    // ignore white spaces
    std::string trimmed = "";
    char c;
    while ( ss.good() )
    {
        c = char( ss.get() );
        if ( c != ' ')
            trimmed += c;
    }
	
	// construct the tree starting from the root
    TopologyNode *root = createNode( trimmed, nodes, brlens );
    
	// remember the current order of nodes - we need these to properly set branch lengths
	std::vector<TopologyNode*> old_nodes = nodes;
	
    // set up the tree (nodes are reordered by index)
	tau->setRootNoReIndexing( root );
	
    // connect the topology to the tree
    t->setTopology( tau, true );
    
    // correctly set the branch lengths using the old order of nodes
    for (size_t i = 0; i < nodes.size(); ++i) {
		t->setBranchLength(old_nodes[i]->getIndex(), brlens[i]);
    }
	
	// now reorder nodes by tree traversal
	tau->setRoot( root );
	
    // return the tree, the caller is responsible for destruction
    return t;
}



// used for reading in tree with existing node indexes we need to keep
BranchLengthTree* NewickConverter::convertFromNewickNoReIndexing(std::string const &n) {
    
    // create and allocate the tree object
    BranchLengthTree *t = new BranchLengthTree();
    
    Topology *tau = new Topology();
    
    std::vector<TopologyNode*> nodes;
    std::vector<double> brlens;
    
    // create a string-stream and throw the string into it
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    ss << n;
    
    // ignore white spaces
    std::string trimmed = "";
    char c;
    while ( ss.good() )
    {
        c = char( ss.get() );
        if ( c != ' ')
            trimmed += c;
    }
	
	// construct the tree starting from the root
    TopologyNode *root = createNode( trimmed, nodes, brlens );
    
	// remember the current order of nodes - we need these to properly set branch lengths
	std::vector<TopologyNode*> old_nodes = nodes;
	
    // set up the tree (nodes are reordered)
	tau->setRootNoReIndexing( root );
	
    // connect the topology to the tree
    t->setTopology( tau, true );
    
    // correctly set the branch lengths using the old order of nodes
    for (size_t i = 0; i < nodes.size(); ++i) {
		t->setBranchLength(old_nodes[i]->getIndex(), brlens[i]);
    }
	
    // return the tree, the caller is responsible for destruction
    return t;
}



TopologyNode* NewickConverter::createNode(const std::string &n, std::vector<TopologyNode*> &nodes, std::vector<double> &brlens) {
    
    // create a string-stream and throw the string into it
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    ss << n;
    
    char c;
    ss.get(c);
    
    // the initial character has to be '('
    if ( c != '(')
    {
        throw RbException("Error while converting Newick tree. We expected an opening parenthesis, but didn't get one.");
    }
    
    TopologyNode *node = new TopologyNode(); 
    while ( ss.good() && ss.peek() != ')' )
    {
        
        TopologyNode *childNode;
        if (ss.peek() == '(' )
        {
            // we received an internal node
            int depth = 0;
            std::string child = "";
            do
            {
                ss.get(c);
                child += c;
                if ( c == '(' )
                {
                    depth++;
                }
                else if ( c == ')' )
                {
                    depth--;
                }
                
            } while ( ss.good() && depth > 0 );
        
            // construct the child node
            childNode = createNode( child, nodes, brlens );
        }
        else
        {
            // construct the node
            childNode = new TopologyNode();
        }
        
        // set the parent child relationship
        node->addChild( childNode );
        childNode->setParent( node );
        
        // read the optional label
        std::string lbl = "";
        while ( ss.good() && (c = char( ss.peek() ) ) != ':' && c != '[' && c != ';' && c != ',' && c != ')')
        {
            lbl += char( ss.get() );
        }
        childNode->setName( lbl );
        
        // read the optional node parameters
        if ( ss.peek() == '[' )
        {
            
            do
            {
                
                ss.ignore();
                // ignore the '&' before parameter name
                if ( ss.peek() == '&') 
                {
                    ss.ignore();
                }
                
                // read the parameter name
                std::string paramName = "";
                while ( ss.good() && (c = char( ss.peek() ) ) != '=' && c != ',')
                {
                    paramName += char( ss.get() );
                }
                
                // ignore the equal sign between parameter name and value
                if ( ss.peek() == '=') 
                {
                    ss.ignore();
                }
                
                // read the parameter name
                std::string paramValue = "";
                while ( ss.good() && (c = char( ss.peek() ) ) != ']' && c != ',')
                {
                    paramValue += char( ss.get() );
                }
                
				if (paramName=="index") {
					
                    childNode->setIndex( boost::lexical_cast<std::size_t>(paramValue) );
					
                } else if (paramName=="species") {
					
					// \todo: Needs implementation
                    childNode->setSpeciesName(paramValue);
                
				} else {
					
                    childNode->addNodeParameter(paramName, paramValue);
                }
				
            } while ( (c = char( ss.peek() ) ) == ',' );
            
            // ignore the final ']'
            if ( (c = char( ss.peek( ) ) ) == ']' )
            {
                ss.ignore();
            }
            
        }
        
        // read the optional branch length
        if ( ss.peek() == ':' )
        {
            ss.ignore();
            std::string time = "";
            while ( ss.good() && (c = char( ss.peek( ) ) ) != ';' && c != ','  && c != ')')
            {
                time += char( ss.get() );
            }
            
            std::istringstream stm;
            stm.str(time);
            double d;
            stm >> d;
            nodes.push_back( childNode );
            brlens.push_back( d );
        }
        else
        {
            nodes.push_back( childNode );
            brlens.push_back( 0.0 );
        }
        
        // skip comma
        if ( char( ss.peek() ) == ',' )
        {
            ss.ignore();
        }
        
    }
    
    // remove closing parenthesis
    ss.ignore();
    
    // read the optional label
    std::string lbl = "";
    while ( ss.good() && (c = char( ss.peek() )) != ':' && c != ';' && c != ',' && c != '[')
    {
        lbl += char( ss.get() );
    }
    node->setName( lbl );
    
    // read the optional node parameters
    if ( char( ss.peek() ) == '[' )
    {
        
        do
        {
            
            ss.ignore();

            // ignore the '&' before parameter name
            if ( char( ss.peek() ) == '&') 
            {
                ss.ignore();
            }
            
            // read the parameter name
            std::string paramName = "";
            while ( ss.good() && (c = char( ss.peek() )) != '=' && c != ',') 
            {
                paramName += char( ss.get() );
            }
            
            // ignore the equal sign between parameter name and value
            if ( char( ss.peek() ) == '=') 
            {
                ss.ignore();
            }
            
            // read the parameter name
            std::string paramValue = "";
            while ( ss.good() && (c = char( ss.peek() )) != ']' && c != ',') 
            {
                paramValue += char( ss.get() );
            }
			
			if (paramName=="index") {
				
				node->setIndex( boost::lexical_cast<std::size_t>(paramValue) );
				
			} else if (paramName=="species") {
				
				// \todo: Needs implementation
                node->setSpeciesName(paramValue);
				
            } else {
				
                node->addNodeParameter(paramName, paramValue);
            }

            
        } while ( (c = char( ss.peek() )) == ',' );
        
        // ignore the final ']'
        if ( (c = char( ss.peek() )) == ']' )
        {
            ss.ignore();
        }
        
    }
    
    // read the optinal  branch length
    if ( char( ss.peek() ) == ':' )
    {
        ss.ignore();
        std::string time = "";
        while ( ss.good() && (c = char( ss.peek() )) != ';' && c != ',')
        {
            time += char( ss.get() );
        }
        
        std::istringstream stm;
        stm.str(time);
        double d;
        stm >> d;
        nodes.push_back( node );
        brlens.push_back( d );
    }
    else
    {
        nodes.push_back( node );
        brlens.push_back( 0.0 );
    }
    
    
    return node;
}


AdmixtureTree* NewickConverter::getAdmixtureTreeFromNewick(std::string const &n)
{
    
    // create and allocate the tree object
    AdmixtureTree *t = new AdmixtureTree();
    
    std::vector<TopologyNode*> nodes;
    std::vector<double> brlens;
    
    // construct the tree starting from the root
    //TopologyNode *root = createNode( n, nodes, brlens );
    
    // convert to AdmixtureNode*
    std::vector<AdmixtureNode*> adm_nodes;
    for (size_t i = 0; i < nodes.size(); i++)
        adm_nodes.push_back(static_cast<AdmixtureNode*>(nodes[i]));
    
    // set up the tree
    t->setRoot(adm_nodes[adm_nodes.size()-1]);
    
    // set the branch lengths
    //for (size_t i = 0; i < nodes.size(); ++i) {
    t->setAgesFromBrlens(brlens);
    ;//t->setBranchLength(nodes[i]->getIndex(), brlens[i]);
    // }
    
    // return the tree, the caller is responsible for destruction
    return t;
}

