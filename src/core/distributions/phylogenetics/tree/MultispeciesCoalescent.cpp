#include "Clade.h"
#include "ConstantNode.h"
#include "MultispeciesCoalescent.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"
#include "Topology.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

MultispeciesCoalescent::MultispeciesCoalescent(const TypedDagNode<TimeTree> *sp,
                                               const std::vector<Taxon> &t) : TypedDistribution<TimeTree>( NULL ),
    taxa(t),
    speciesTree( sp ),
    Nes( NULL ),
    Ne( new ConstantNode<double>("Ne", new double(1.0) ) ),
    numTaxa( taxa.size() ),
    logTreeTopologyProb (0.0)
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( speciesTree );
    addParameter( Ne );
    
    std::set<std::string> speciesNames;
    for (std::vector<Taxon>::const_iterator it=taxa.begin(); it!=taxa.end(); ++it)
    {
        speciesNames.insert( it->getSpeciesName() );
    }
    
    if ( sp->getValue().getNumberOfTips() != speciesNames.size() )
    {
        //        throw RbException("Unequal number of species between species tree and gene tree.");
    }
    
    double lnFact = RbMath::lnFactorial((int)(numTaxa));
    
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - 2.0 * lnFact - std::log( numTaxa ) ;
    
    redrawValue();
    
    

}


MultispeciesCoalescent::~MultispeciesCoalescent()
{
    
}



void MultispeciesCoalescent::attachTimes(TimeTree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &times) {
    
    if (index < numTaxa-1)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t tip_index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(tip_index);
        psi->setAge( parent->getIndex(), times[numTaxa - index - 2] );
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+tip_index);
        
        // add a left child
        TopologyNode* leftChild = &parent->getChild(0);
        if ( !leftChild->isTip() )
        {
            tips.push_back(leftChild);
        }
        
        // add a right child
        TopologyNode* rightChild = &parent->getChild(1);
        if ( !rightChild->isTip() )
        {
            tips.push_back(rightChild);
        }
        
        // recursive call to this function
        attachTimes(psi, tips, index+1, times);
    }
}


void MultispeciesCoalescent::buildRandomBinaryTree(std::vector<TopologyNode*> &tips)
{
    
    if (tips.size() < numTaxa)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(index);
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+index);
        
        // add a left child
        TopologyNode* leftChild = new TopologyNode(0);
        parent->addChild(leftChild);
        leftChild->setParent(parent);
        tips.push_back(leftChild);
        
        // add a right child
        TopologyNode* rightChild = new TopologyNode(0);
        parent->addChild(rightChild);
        rightChild->setParent(parent);
        tips.push_back(rightChild);
        
        // recursive call to this function
        buildRandomBinaryTree(tips);
    }
}


MultispeciesCoalescent* MultispeciesCoalescent::clone( void ) const
{
    
    return new MultispeciesCoalescent( *this );
}


double MultispeciesCoalescent::computeLnProbability( void )
{
    
    // variable declarations and initialization
    double lnProbCoal = 0;
    
    const TimeTree &sp = speciesTree->getValue();
    //    std::cerr << sp << std::endl;
    
    
    const std::vector< TopologyNode* > &speciesTreeNodes = sp.getNodes();
    // first let's create a map from species names to the nodes of the species tree
    std::map<std::string, TopologyNode * > speciesNames2speciesNodes;
    for (std::vector< TopologyNode *>::const_iterator it = speciesTreeNodes.begin(); it != speciesTreeNodes.end(); ++it)
    {
        if ( (*it)->isTip() )
        {
            const std::string &name = (*it)->getName();
            speciesNames2speciesNodes[name] = *it;
        }
    }
    
    
    // create a map from individual name to the actual tip node for convenience
    std::map< std::string, TopologyNode*> individualNames2geneTreeTips;
    for ( size_t i = 0; i < numTaxa; ++i)
    {
        TopologyNode *tip = &value->getTipNode( i );
        individualNames2geneTreeTips[ tip->getName() ] = tip;
    }
    
    
    std::map< const TopologyNode *, std::set< TopologyNode* > > individualsPerBranch;
    
    for (std::vector< Taxon >::iterator it = taxa.begin(); it != taxa.end(); ++it)
    {
        const std::string &tipName = it->getName();
        TopologyNode *n = individualNames2geneTreeTips[tipName];
        const std::string &speciesName = it->getSpeciesName();
        TopologyNode *speciesNode = speciesNames2speciesNodes[speciesName];
        std::set< TopologyNode * > &nodesAtNode = individualsPerBranch[ speciesNode ];
        nodesAtNode.insert( n );
    }

    /*
    for (std::map<std::string,std::string>::iterator it = gene2species.begin(); it != gene2species.end(); ++it)
    {
        const std::string &tipName = it->first;
        TopologyNode *n = individualNames2geneTreeTips[tipName];
        const std::string &speciesName = it->second;
        TopologyNode *speciesNode = speciesNames2speciesNodes[speciesName];
        std::set< TopologyNode * > &nodesAtNode = individualsPerBranch[ speciesNode ];
        nodesAtNode.insert( n );
    }
    */
    std::map<double, TopologyNode*> speciesAge2speciesNodes;
    double i = -0.01;
    for (std::vector<TopologyNode *>::const_iterator it = speciesTreeNodes.begin(); it != speciesTreeNodes.end(); ++it)
    {
        if ( (*it)->isTip() )
        {
            speciesAge2speciesNodes[ i ] = (*it);
            i -= 0.01;
        }
        else
        {
            speciesAge2speciesNodes[ (*it)->getAge() ] = (*it);
        }
    }
    
    // we loop over the nodes of the species tree in phylogenetic traversal
    for (std::map<double, TopologyNode *>::const_iterator it = speciesAge2speciesNodes.begin(); it != speciesAge2speciesNodes.end(); ++it)
    {
        //        std::cerr << "Species Node:\t\t" << (it->second)->computeNewick() << std::endl;
        TopologyNode *spNode = it->second;
        const TopologyNode *spParentNode = NULL;
        double speciesAge = spNode->getAge();
        double parentSpeciesAge = RbConstants::Double::inf;
//        double branchLength = RbConstants::Double::inf;
        if ( !spNode->isRoot() )
        {
            spParentNode = &spNode->getParent();
            parentSpeciesAge = spParentNode->getAge();
//            branchLength = parentSpeciesAge - speciesAge;
        }
        
        //        std::cerr << "Species Age:\t\t" << speciesAge << std::endl;
        //        std::cerr << "Parent Age:\t\t" << parentSpeciesAge << std::endl;
        
        // create a local copy of the individuals per branch
        std::set<TopologyNode*> remainingIndividuals = individualsPerBranch[spNode];
        
        
        //        // Debug output
        //        std::cerr << "Coalescent times:\t\n";
        //        for (std::set<TopologyNode*>::iterator tmp = remainingIndividuals.begin(); tmp != remainingIndividuals.end(); ++tmp) {
        //            std::cerr << (*tmp)->getParent().getAge() << "\t" << (*tmp)->computeNewick() << std::endl;
        //            std::cerr << (*tmp)->getParent().computeNewick() << std::endl;
        //        }
        //        std::cerr << std::endl;
        
        // get all coalescent events among the individuals
        std::set<double> coalTimes;
        
        
        std::map<double, TopologyNode *> coalTimes2coalNodes;
        for ( std::set<TopologyNode*>::iterator remNode = remainingIndividuals.begin(); remNode != remainingIndividuals.end(); ++remNode)
        {
            TopologyNode *rn = (*remNode);
            if ( !rn->isRoot() )
            {
                TopologyNode &parent = rn->getParent();
                double parentAge = parent.getAge();
                coalTimes2coalNodes[ parentAge ] = &parent;
            }
        }
        
        double currentTime = speciesAge;
        while ( currentTime < parentSpeciesAge && coalTimes2coalNodes.size() > 0 )
        {
            
            TopologyNode *parent = coalTimes2coalNodes.begin()->second;
            double parentAge = parent->getAge();
			double branchNe = getNe(spNode->getIndex());
			double theta = 1.0 / branchNe;
            
            if ( parentAge < parentSpeciesAge )
            { //Coalescence in the species tree branch
                
                // get the left and right child of the parent
                TopologyNode *left = &parent->getChild( 0 );
                TopologyNode *right = &parent->getChild( 1 );
                if ( remainingIndividuals.find( left ) == remainingIndividuals.end() || remainingIndividuals.find( right ) == remainingIndividuals.end() )
                {
                    // one of the children does not belong to this species tree branch
                    return RbConstants::Double::neginf;
                }
                
				//We remove the coalescent event and the coalesced lineages
                coalTimes2coalNodes.erase( coalTimes2coalNodes.begin() );
                remainingIndividuals.erase( remainingIndividuals.find( left ) );
                remainingIndividuals.erase( remainingIndividuals.find( right ) );
                
				//We insert the parent in the vector of lineages in this branch
                remainingIndividuals.insert( parent );
                if ( !parent->isRoot() )
                {
                    TopologyNode *grandParent = &parent->getParent();
                    coalTimes2coalNodes[ grandParent->getAge() ] = grandParent;
                }
                
				//a is the time between the previous and the current coalescences
                double a = parentAge - currentTime;
                currentTime = parentAge;
                
                // now we do the computation
                
                // get the number j of individuals we had before the current coalescence
                size_t j = remainingIndividuals.size() + 1;
                // compute the number of pairs: pairs = C(j choose 2) = j * (j-1.0) / 2.0
                double nPairs = j * (j-1.0) / 2.0;
                double lambda = nPairs * theta;
                
                // add the density for this coalescent event
                //lnProbCoal += log( lambda ) - lambda * a;
				//Corrected version:
                lnProbCoal += log( theta ) - lambda * a;
				
                
                
            } //End if coalescence in the species tree branch
            else
            { //No more coalescences in this species tree branch
                
                // compute the probability of no coalescent event in the final part of the branch
                // only do this if the branch is not the root branch
                if ( spParentNode != NULL )
                {
                    double finalInterval = parentSpeciesAge - currentTime;
                    size_t k = remainingIndividuals.size();
                    lnProbCoal -= k*(k-1.0)/2.0 * finalInterval * theta;
                }
                
                // jump out of the while loop
//                currentTime = speciesAge;
                break;
            }
            
            
        } // end of while loop
        
        
        // merge the two sets of individuals that go into the next species
        if ( spParentNode != NULL )
        {
            std::set<TopologyNode *> &incomingLineages = individualsPerBranch[spParentNode];
            incomingLineages.insert( remainingIndividuals.begin(), remainingIndividuals.end());
        }
        
        
    } // end loop over all nodes in the species tree (in phylogenetic ordering)
    
    
    return lnProbCoal; // + logTreeTopologyProb;
    
}


//double MultispeciesCoalescent::computeLnProbability( void ) {
//
//    // variable declarations and initialization
//    double lnProbCoal = 0;
//
//    const TimeTree &sp = speciesTree->getValue();
//    std::cerr << sp << std::endl;
//
//
//    const std::vector< TopologyNode* > &speciesTreeNodes = sp.getNodes();
//    // first let's create a map from species names to the nodes of the species tree
//    std::map<std::string, TopologyNode * > speciesNames2speciesNodes;
//    for (std::vector< TopologyNode *>::const_iterator it = speciesTreeNodes.begin(); it != speciesTreeNodes.end(); ++it)
//    {
//        if ( (*it)->isTip() )
//        {
//            const std::string &name = (*it)->getName();
//            speciesNames2speciesNodes[name] = *it;
//        }
//    }
//
//
//    // create a map from individual name to the actual tip node for convenience
//    std::map< std::string, TopologyNode*> individualNames2geneTreeTips;
//    for ( size_t i = 0; i < numTaxa; ++i)
//    {
//        TopologyNode *tip = &value->getTipNode( i );
//        individualNames2geneTreeTips[ tip->getName() ] = tip;
//    }
//
//
//    std::map< const TopologyNode *, std::set< TopologyNode* > > individualsPerBranch;
//    for (std::map<std::string,std::string>::iterator it = gene2species.begin(); it != gene2species.end(); ++it)
//    {
//        const std::string &tipName = it->first;
//        TopologyNode *n = individualNames2geneTreeTips[tipName];
//        const std::string &speciesName = it->second;
//        TopologyNode *speciesNode = speciesNames2speciesNodes[speciesName];
//        std::set< TopologyNode * > &nodesAtNode = individualsPerBranch[ speciesNode ];
//        nodesAtNode.insert( n );
//    }
//
//    std::map<double, TopologyNode*> speciesAge2speciesNodes;
//    double i = -0.01;
//    for (std::vector<TopologyNode *>::const_iterator it = speciesTreeNodes.begin(); it != speciesTreeNodes.end(); ++it)
//    {
//       if ( (*it)->isTip() )
//        {
//            speciesAge2speciesNodes[ i ] = (*it);
//            i -= 0.01;
//        }
//        else
//        {
//            speciesAge2speciesNodes[ (*it)->getAge() ] = (*it);
//        }
//    }
//
//    // we loop over the nodes of the species tree in phylogenetic traversal
////    for (std::vector<TopologyNode *>::const_iterator it = speciesTreeNodes.begin(); it != speciesTreeNodes.end(); ++it)
//    for (std::map<double, TopologyNode *>::const_iterator it = speciesAge2speciesNodes.begin(); it != speciesAge2speciesNodes.end(); ++it)
//    {
//        std::cerr << "Species Node:\t\t" << (it->second)->computeNewick() << std::endl;
//        TopologyNode *spNode = it->second;
//        const TopologyNode *spParentNode = NULL;
//        double speciesAge = spNode->getAge();
//        double parentSpeciesAge = RbConstants::Double::inf;
//        double branchLength = RbConstants::Double::inf;
//        if ( !spNode->isRoot() )
//        {
//            spParentNode = &spNode->getParent();
//            parentSpeciesAge = spParentNode->getAge();
//            branchLength = parentSpeciesAge - speciesAge;
//        }
//
//        std::cerr << "Species Age:\t\t" << speciesAge << std::endl;
//        std::cerr << "Parent Age:\t\t" << parentSpeciesAge << std::endl;
//
//        std::set<TopologyNode*> remainingIndividuals = individualsPerBranch[spNode];
//
//        std::cerr << "Coalescent times:\t\n";
//        for (std::set<TopologyNode*>::iterator tmp = remainingIndividuals.begin(); tmp != remainingIndividuals.end(); ++tmp) {
//            std::cerr << (*tmp)->getParent().getAge() << "\t" << (*tmp)->computeNewick() << std::endl;
//            std::cerr << (*tmp)->getParent().computeNewick() << std::endl;
//        }
//        std::cerr << std::endl;
//
//        // get the number of individuals we start with at this branch
//        size_t j = remainingIndividuals.size();
//
//        // get all coalescent events among the individuals
//        std::set<double> coalTimes;
////        std::set<TopologyNode*> remainingIndividuals;
//        while ( remainingIndividuals.size() >= 2 )
//        {
//            // get the first individual and the age of its parent
//            std::set<TopologyNode *>::iterator nextNode = remainingIndividuals.begin();
//            TopologyNode *p = &(*nextNode)->getParent();
//            // find the first coalescent at the branch
//            // we need this because otherwise the sibling of the coalescent event might not have been added to the set of nodes at the branch
//            double minAge = p->getAge();
//            for (std::set<TopologyNode *>::iterator nn = remainingIndividuals.begin(); nn != remainingIndividuals.end(); ++nn)
//            {
//                TopologyNode *np = &(*nn)->getParent();
//                if ( np->getAge() < minAge ) {
//                    minAge = np->getAge();
//                    nextNode = nn;
//                }
//            }
//            TopologyNode *n = (*nextNode);
//            p = &n->getParent();
//            if ( p->getAge() <= parentSpeciesAge )
//            {
//                remainingIndividuals.erase( nextNode );
//                TopologyNode *brother = &p->getChild( 0 );
//                if ( brother == n )
//                {
//                    brother = &p->getChild( 1 );
//                }
//
//                // check that the brother is in the set too
//                std::set<TopologyNode*>::iterator itBrother = remainingIndividuals.find( brother );
//                if ( itBrother != remainingIndividuals.end() )
//                {
//
//                    // yes, the brother is in the set
//                    // erase the brother and add the coalescent time (time after the last speciation event)
//                    remainingIndividuals.erase( itBrother );
//
//                    // compute the coalescent age by subtracting the observed species-tree age from the gene-tree age
//                    double a = p->getAge() - speciesAge;
//                    if ( a <=0 ) {
//                        std::cerr << n->computeNewick() << std::endl;
//                        std::cerr << brother->computeNewick() << std::endl;
//                        std::cerr << "This should never happen!!!" << std::endl;
//                    }
//
//                    if ( coalTimes.find( a ) == coalTimes.end() )
//                    {
//                        coalTimes.insert( a );
//                        // insert the parent to the current set of nodes
//                        remainingIndividuals.insert( p );
//                    }
//                    else
//                    {
//                        std::cerr << "This should never happen !!! Hehe" << std::endl;
//                    }
//
//                }
//                else
//                {
//                    // the brother is not in the set
//                    // thus, there was some magic transfer event which is not allowed under this model
//                    // return -Inf as the probability for this tree
//                    return RbConstants::Double::neginf;
//                }
//            }
//            else
//            {
//                // this individual did not coalesce before the end of the branch
//                // add it to the set of remaining individuals which get promoted to the next branch
//                break;
////                remainingIndividuals.insert( n );
//            }
//        }
//        // we need to add the last individual to the set of remaining individuals
////        remainingIndividuals.insert( *initialIndividualsAtBranch.begin() );
//
//        if ( remainingIndividuals.size() == 1 )
//        {
//            double pa = (*remainingIndividuals.begin())->getParent().getAge();
//            if ( abs(0.0819692 - pa ) < 1E-6 )
//            {
//                std::cerr << "Gorrila-Chimp " << parentSpeciesAge << "\n";
//            }
//            if ( parentSpeciesAge < pa )
//            {
//                return RbConstants::Double::neginf;
//            }
//        }
//
//
//        // now we do the computation
//        double branchNe = Ne->getValue()[spNode->getIndex()];
//        double theta = 1.0 / branchNe;
//
//
//        // compute the probability of no coalescent event in the final part of the branch
//        // only do this if the branch is not the root branch
//        if ( spParentNode != NULL )
//        {
//            double lastCoalescentTime = 0.0;
//            if ( !coalTimes.empty() )
//            {
//                lastCoalescentTime = *coalTimes.rbegin();
//            }
//            double finalInterval = branchLength - lastCoalescentTime;
//            size_t k = remainingIndividuals.size();
//            lnProbCoal -= k*(k-1.0)/2.0 * finalInterval * theta;
//        }
//
//        // now
//        double lastCoalTime = 0.0;
//        while ( !coalTimes.empty() )
//        {
//            double deltaT = *coalTimes.begin() - lastCoalTime;
//            coalTimes.erase( coalTimes.begin() );
//            // compute the number of pairs: pairs = C(j choose 2)
//            double nPairs = j * (j-1.0) / 2.0;
//            double lambda = nPairs * theta;
//
//            // add the density for this coalescent event
//            lnProbCoal += log( lambda ) - lambda * deltaT;
//
//            j--;
//            lastCoalTime += deltaT;
//        }
//
//
//        // merge the two sets of individuals that go into the next species
//        if ( spParentNode != NULL )
//        {
//            std::set<TopologyNode *> &incomingLineages = individualsPerBranch[spParentNode];
//            incomingLineages.insert( remainingIndividuals.begin(), remainingIndividuals.end());
//        }
//
//
//    } // end loop over all nodes in the species tree (in phylogenetic ordering)
//
//
//    return lnProbCoal; // + logTreeTopologyProb;
//
//}


double  MultispeciesCoalescent::getNe(size_t index) const
{
    
    if (Ne != NULL)
    {
        return Ne->getValue();
    }
    else if (Nes != NULL)
    {
        return Nes->getValue()[index];
    }
    else
    {
        std::cerr << "Error: Null Pointers for Ne and Nes." << std::endl;
        exit(-1);
    }
}



void MultispeciesCoalescent::redrawValue( void )
{
    
    simulateTree();
    
}

void MultispeciesCoalescent::setNes(TypedDagNode< RbVector<double> >* inputNes)
{

    removeParameter( Nes );
    removeParameter( Ne );
    
    Nes = inputNes;
    Ne  = NULL;
    
    addParameter( Nes );

}


void MultispeciesCoalescent::setNe(TypedDagNode<double>* inputNe)
{

    removeParameter( Ne );
    removeParameter( Nes );
    
    Ne  = inputNe;
    Nes = NULL;
    
    addParameter( Ne );
}




void MultispeciesCoalescent::simulateTree( void )
{
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    const TimeTree &sp = speciesTree->getValue();
    const std::vector< TopologyNode* > &speciesTreeNodes = sp.getNodes();
    // first let's create a map from species names to the nodes of the species tree
    std::map<std::string, TopologyNode * > speciesNames2Nodes;
    for (std::vector< TopologyNode *>::const_iterator it = speciesTreeNodes.begin(); it != speciesTreeNodes.end(); ++it)
    {
        if ( (*it)->isTip() )
        {
            const std::string &name = (*it)->getName();
            speciesNames2Nodes[name] = *it;
        }
    }
    
    
    std::map< const TopologyNode *, std::vector< TopologyNode* > > individualsPerBranch;
    
    for (std::vector< Taxon >::iterator it = taxa.begin(); it != taxa.end(); ++it)
    {
        TopologyNode *n = new TopologyNode( *it );
        const std::string &speciesName = it->getSpeciesName();
        TopologyNode *speciesNode = speciesNames2Nodes[speciesName];
        std::vector< TopologyNode * > &nodesAtNode = individualsPerBranch[ speciesNode ];
        nodesAtNode.push_back( n );
    }

    
 /*   for (std::map<std::string,std::string>::iterator it = gene2species.begin(); it != gene2species.end(); ++it)
    {
        const std::string &tipName = it->first;
        TopologyNode *n = new TopologyNode( tipName );
        const std::string &speciesName = it->second;
        TopologyNode *speciesNode = speciesNames2Nodes[speciesName];
        std::vector< TopologyNode * > &nodesAtNode = individualsPerBranch[ speciesNode ];
        nodesAtNode.push_back( n );
    }
    */
    
    std::map<TopologyNode *, double> nodes2ages;
    TopologyNode *root = NULL;
    // we loop over the nodes of the species tree in phylogenetic traversal
    for (std::vector<TopologyNode *>::const_iterator it = speciesTreeNodes.begin(); it != speciesTreeNodes.end(); ++it)
    {
        TopologyNode *spNode = *it;
        const TopologyNode *spParentNode = NULL;
        double branchLength = RbConstants::Double::inf;
        if ( !spNode->isRoot() )
        {
            spParentNode = &spNode->getParent();
            branchLength = spParentNode->getAge() - spNode->getAge();
        }
        
        std::vector<TopologyNode*> initialIndividualsAtBranch = individualsPerBranch[spNode];
        double branchNe = getNe(spNode->getIndex() );

        double theta = 1.0 / branchNe;
        
        double prevCoalescentTime = 0.0;
        
        size_t j = initialIndividualsAtBranch.size();
        double nPairs = j * (j-1) / 2.0;
        double lambda = nPairs * theta;
        double u = RbStatistics::Exponential::rv( lambda, *rng);
        double nextCoalescentTime = prevCoalescentTime + u;
        
        while ( nextCoalescentTime < branchLength && j > 1 ) {
            // randomly coalesce two lineages
            size_t index = static_cast<size_t>( floor(rng->uniform01()*initialIndividualsAtBranch.size()) );
            TopologyNode *left = initialIndividualsAtBranch[index];
            initialIndividualsAtBranch.erase( initialIndividualsAtBranch.begin() + index);
            
            index = static_cast<size_t>( floor(rng->uniform01()*initialIndividualsAtBranch.size()) );
            TopologyNode *right = initialIndividualsAtBranch[index];
            initialIndividualsAtBranch.erase( initialIndividualsAtBranch.begin() + index);
            
            TopologyNode *newParent = new TopologyNode();
            newParent->addChild(left);
            left->setParent(newParent);
            newParent->addChild(right);
            right->setParent(newParent);
            
            root = newParent;
            
            initialIndividualsAtBranch.push_back( newParent );
            
            nodes2ages[newParent] = nextCoalescentTime + spNode->getAge();
            
            
            prevCoalescentTime = nextCoalescentTime;
            j--;
            nPairs = j * (j-1) / 2.0;
            lambda = nPairs * theta ;
            u = RbStatistics::Exponential::rv( lambda, *rng);
            nextCoalescentTime = prevCoalescentTime + u;
        }
        
        if ( spParentNode != NULL )
        {
            std::vector<TopologyNode *> &incomingLineages = individualsPerBranch[spParentNode];
            incomingLineages.insert(incomingLineages.end(), initialIndividualsAtBranch.begin(), initialIndividualsAtBranch.end());
        }
        
        
    }
    
    // the time tree object (topology + times)
    TimeTree *psi = new TimeTree();
    
    // Draw a random topology
    Topology *tau = new Topology();
    
    // internally we treat unrooted topologies the same as rooted
    tau->setRooted( true );
    
    // initialize the topology by setting the root
    tau->setRoot(root);
    
    // connect the tree with the topology
    psi->setTopology( tau, true );
    
    for ( std::map<TopologyNode*, double>::iterator it = nodes2ages.begin(); it != nodes2ages.end(); ++it) {
        size_t index = it->first->getIndex();
        psi->setAge(index, it->second);
    }
    
    // finally store the new value
    value = psi;
    
}


/** Swap a parameter of the distribution */
void MultispeciesCoalescent::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == Nes )
    {
        Nes = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
    if ( oldP == Ne )
    {
        Ne = static_cast<const TypedDagNode< double >* >( newP );
    }
    
    if ( oldP == speciesTree )
    {
        speciesTree = static_cast<const TypedDagNode< TimeTree >* >( newP );
    }
    
}
