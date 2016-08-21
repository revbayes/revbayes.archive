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

- (NSMutableArray*)readTreesFromFile:(NSString*)fn {

    // read the trees
    RevBayesCore::NclReader nclReader = RevBayesCore::NclReader();
    std::string fileName = [fn cStringUsingEncoding:NSASCIIStringEncoding];
    std::vector<RevBayesCore::Tree*>* coreTrees = nclReader.readBranchLengthTrees(fileName);
    
    // convert the trees from the core to the GUI
    NSMutableArray* trees = [[NSMutableArray alloc] init];
    for (size_t i=0; i<coreTrees->size(); i++)
        {
        RevBayesCore::Tree* coreTree = (*coreTrees)[i];
        
        // add the GUI tree to an array
        GuiTree* guiTree = [[GuiTree alloc] init];
        [trees addObject:guiTree];
        [guiTree setNumberOfTaxa:(int)coreTree->getNumberOfTips()];
        [guiTree setOutgroupIdx:0];
        
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
        [guiTree setOutgroupIdx:0];
        }
    
    return trees;
}



@end
