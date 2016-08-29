#import "GuiTree.h"
#import "Node.h"
#import "RbGuiHelper.h"
#include "NclReader.h"
#include "NewickTreeReader.h"
#include "TopologyNode.h"
#include "Tree.h"
#include <string>
#include <vector>

/*#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readTreeTrace.h"
#include "ModelVector.h"
#include "NclReader.h"
#include "NewickConverter.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlTraceTree.h"
#include "RlUserInterface.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TraceTree.h"
#include "TreeUtilities.h"*/


@implementation RbGuiHelper

- (GuiTree*)convertCoreTreeToGuiTree:(RevBayesCore::Tree*)coreTree usingTranslationTable:(std::map<int,std::string>*) tb {

    // allocate the new tree
    GuiTree* guiTree = [[GuiTree alloc] init];
    
    // set information for this tree
    Node* guiRoot = [[Node alloc] init];
    [guiRoot setIsRoot:YES];
    [guiTree setRoot:guiRoot];
    [guiTree setNumberOfTaxa:(int)coreTree->getNumberOfTips()];
    std::map<int,std::string>::iterator it = (*tb).find(0);
    if (it == (*tb).end())
        NSLog(@"Error: Problem finding outgroup");
    std::string oName = it->second;
    NSString* oNameStr = [NSString stringWithCString:oName.c_str() encoding:NSASCIIStringEncoding];
    [guiTree setOutgroupName:oNameStr];

    // recrusively
    RevBayesCore::TopologyNode coreRoot = coreTree->getRoot();
    [self constructBranchLengthTreefromNclRecursively:guiRoot andTree:guiTree fromCoreNode:&coreRoot andTranslationTable:tb];
    
    // set indices for internal nodes
    int intIdx = [guiTree getNumberOfTaxa];
    [guiTree initializeDownPassSequence];
    for (unsigned i=0; i<[guiTree numbeOfDownPassNodes]; i++)
        {
        Node* p = [guiTree downPassNodeIndexed:i];
        if ([p isLeaf] != YES)
            [p setIndex:intIdx++];
        }

    // reroot the tree on the outgroup
    [guiTree rootTreeOnNodeNamed:oNameStr];
    
    return guiTree;
}

- (NSMutableArray*)readTreesFromFile:(NSString*)fn {

    // read the trees
    RevBayesCore::NclReader nclReader = RevBayesCore::NclReader();
    std::string fileName = [fn cStringUsingEncoding:NSASCIIStringEncoding];
    std::vector<std::map<int,std::string> > translationTables;
    std::vector<RevBayesCore::Tree*>* coreTrees = nclReader.readBranchLengthTrees(fileName, translationTables);
    if (translationTables.size()==0)
        {
        NSLog(@"Error: No translation table found");
        }
    for (int n=0; n<translationTables.size(); n++)
        {
        for (std::map<int,std::string>::iterator it=translationTables[n].begin(); it != translationTables[n].end(); it++)
            std::cout << it->first << " -> " << it->second << std::endl;
        }
    
    // convert the trees from the core to the GUI
    NSMutableArray* trees = [[NSMutableArray alloc] init];
    
    for (size_t i=0; i<coreTrees->size(); i++)
        {
        RevBayesCore::Tree* coreTree = (*coreTrees)[i];
        
        // add the GUI tree to an array
        //GuiTree* guiTree = [[GuiTree alloc] init];
        GuiTree* guiTree = [self convertCoreTreeToGuiTree:(RevBayesCore::Tree*)coreTree usingTranslationTable:&translationTables[0]];
        [trees addObject:guiTree];
        
        
#       if 0
        
        
        
        
        [guiTree setNumberOfTaxa:(int)coreTree->getNumberOfTips()];
        [guiTree setOutgroupName:@""];
        
        // convert the core tree to the gui tree
        //[self convertCoreTreeToGuiTree:(RevBayesCore::Tree*)coreTree];
        
        // add the nodes to the GUI tree and index each, with the index being the offset from the beginning of the array
        const std::vector<RevBayesCore::TopologyNode*> nodes = coreTree->getNodes();
        size_t nNodes = nodes.size();
        for (size_t n=0; n<nNodes; n++)
            {
            Node* p = [guiTree addNode];
            [p setIndex:(int)n];
            }
            
        // set the pointers for the tree, using the index as the offset
        // (We assume the tree indices go from 0, 1, ..., N - 1.)
        for (size_t n=0; n<nNodes; n++)
            {
            // set the descendants
            RevBayesCore::TopologyNode* pCore = nodes[n];
            Node* pGui = [guiTree nodeWithIndex:(int)n];
            
            // set the ancestor
            const std::vector<RevBayesCore::TopologyNode*> descendants = pCore->getChildren();
            for (size_t d=0; d<descendants.size(); d++)
                {
                [pGui addDescendant:[guiTree nodeWithIndex:(int)(descendants[d]->getIndex())]];
                }
            if (pCore->isRoot() == false)
                {
                [pGui setAncestor:[guiTree nodeWithIndex:(int)(pCore->getParent().getIndex())]];
                }
            else
                {
                [pGui setIsRoot:YES];
                [guiTree setRoot:pGui];
                }
                
            // set the tip information
            if (pCore->isTip() == true)
                {
                [pGui setIsLeaf:YES];
                std::string coreName = pCore->getSpeciesName();
                NSString* guiName = [NSString stringWithCString:coreName.c_str() encoding:NSASCIIStringEncoding];
                [pGui setName:guiName];
                }
            }
            
        // initialize the downpass sequence for the tree
        [guiTree initializeDownPassSequence];
        //[guiTree setOutgroupIdx:0]; // TEMP FIX
#       endif
        }
    
    return trees;
}

/** Constructs a tree from NCL */
-(void) constructBranchLengthTreefromNclRecursively:(Node*)tn andTree:(GuiTree*)t fromCoreNode:(RevBayesCore::TopologyNode*)tnCore andTranslationTable:(std::map<int,std::string>*) tb {
    
    // add the current node to the vector of nodes
    [t addNodeToTree:tn];
    
    // get the children
    const std::vector<RevBayesCore::TopologyNode*>& coreChildren = tnCore->getChildren();
    
    // iterate over all children
    for (std::vector<RevBayesCore::TopologyNode*>::const_iterator it = coreChildren.begin(); it!=coreChildren.end(); it++)
        {
        // create a new tree node with given name
        std::string coreName = (*it)->getName();

        // find the index of the taxon name, if *it is a taxon
        int guiIdx = -1;
        for (std::map<int,std::string>::iterator tbit = (*tb).begin(); tbit != (*tb).end(); tbit++)
            {
            if (tbit->second == coreName)
                {
                guiIdx = tbit->first;
                break;
                }
            }

        // allocate the child node for the gui tree and set some information for it
        Node* guiChild = [[Node alloc] init];
        NSString* guiName = [NSString stringWithCString:coreName.c_str() encoding:NSASCIIStringEncoding];
        [guiChild setName:guiName];
        if (guiIdx != -1)
            {
            [guiChild setIndex:guiIdx];
            [guiChild setIsLeaf:YES];
            }
        
        // add new node as child
        [tn addDescendant:guiChild];
        
        // add parent to new node
        [guiChild setAncestor:tn];
        
        // recursive call for the child to parse the tree
        [self constructBranchLengthTreefromNclRecursively:guiChild andTree:t fromCoreNode:(*it) andTranslationTable:tb];
        //constructBranchLengthTreefromNclRecursively(child, nodes, brlens, *it, tb);
    }
}


@end
