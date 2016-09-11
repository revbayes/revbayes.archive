#import "Node.h"
#import "GuiTree.h"
#import "ToolTreeConsensus.h"
#import "TreeTaxonBipartitions.h"
#include <iostream>
#include <set>
#include "NodeVal.h"



@implementation TreeTaxonBipartitions

@synthesize numSamples;

- (void)addPartitionsForTree:(GuiTree*)t withWeight:(float)w {

    if (numSamples < 0.00001)
        {
        for (int i=0; i<[t numberOfTaxa]; i++)
            [names addObject:@"name"];
        }
    if (numTaxa == 0)
        numTaxa = [t numberOfTaxa];

    // get the partitions for this tree
    std::vector<boost::dynamic_bitset<> > currentParts;
    for (int i=0; i<[t numbeOfDownPassNodes]; i++)
        currentParts.push_back( boost::dynamic_bitset<>(numTaxa, 0) );
    for (int n=0; n<[t numbeOfDownPassNodes]; n++)
        {
        Node* p = [t downPassNodeIndexed:n];
        if ( p != [t root] )
            {
            if ( [p isLeaf] == YES )
                {
                currentParts[ [p index] ].flip([p index]);
                if (numSamples == 0)
                    {
                    NSString* taxonName = [p name];
                    [names replaceObjectAtIndex:[p index] withObject:taxonName];
                    }
                }
            else
                {
                for (int i=0; i<[p numberOfDescendants]; i++)
                    {
                    Node* child = [p descendantIndexed:i];
                    currentParts[ [p index] ] |= currentParts[ [child index] ];
                    }
                }
            }
        }
    
    // add the partitions to the map containing all of the partitions
    for (std::vector<boost::dynamic_bitset<> >::iterator it = currentParts.begin(); it != currentParts.end(); it++)
        {
        if ((*it)[0] == true)
            (*it).flip();
        size_t numOn = (*it).count();
        if (numOn != 0 && numOn != (*it).size())
            {
            NodeVals* vals = [self isKeyPresent:(*it)];
            if (vals == NULL)
                {
                NodeVals* newVal = new NodeVals( (*it) );
                NodeKey newKey( (*it) );
                parts.insert( std::make_pair(newKey, newVal) );
                newVal->addSample(w);
                }
            else
                {
                vals->addSample(w);
                }
            }
        }

    numSamples += w;
}

- (GuiTree*)consensusTree {
    
    double freq = 0.0;
    if ([myTool showAllCompatiblePartitions] == YES)
        freq = 0.0;
    else
        freq = [myTool partitionFrequencies];
    
    size_t partSize = [names count];
    
    // sort the partitions
    std::set<NodeVals*,comp> sortedParts;
    for (std::map<NodeKey,NodeVals*>::iterator it = parts.begin(); it != parts.end(); it++)
        {
        if ( (it->second->getNumSamples() / numSamples) > freq )
            {
            if (it->second->isSingleton() == false)
                {
                it->second->normalizeBitVector();
                sortedParts.insert(it->second);
                }
            }
        }
    
    // find the set of compatible partitions
    std::vector<boost::dynamic_bitset<> > conParts;
    std::vector<float> support;
    for (std::set<NodeVals*,comp>::iterator it = sortedParts.begin(); it != sortedParts.end(); it++)
        {
        boost::dynamic_bitset<> p1 = (*it)->getPartition();
        BOOL isCompat = YES;
        for (int i=0; i<conParts.size(); i++)
            {
            if (conParts[i].count() < p1.count())
                {
                if ([self isPart:conParts[i] compatibleWithPart:p1] == NO)
                    {
                    isCompat = NO;
                    break;
                    }
                }
            else
                {
                if ([self isPart:p1 compatibleWithPart:conParts[i]] == NO)
                    {
                    isCompat = NO;
                    break;
                    }
                }
            }
        if (isCompat == YES)
            {
            boost::dynamic_bitset<> bv = boost::dynamic_bitset<>(partSize,0);
            bv = (*it)->getPartition();
            conParts.push_back(bv);
            support.push_back( ((*it)->getNumSamples() / numSamples) );
            }
        }
    
    // build up the consensus tree
    int intNodeIdx = (int)[names count];
    GuiTree* conTree = [[GuiTree alloc] init];
    [conTree setNumberOfTaxa:(int)[names count]];
    Node* conRoot = [[Node alloc] init];
    [conTree setRoot:conRoot];
    [conRoot setIsRoot:YES];
    [conRoot setIndex:intNodeIdx++];
    for (int i=0; i<[names count]; i++)
        {
        Node* newNode = [conTree addNode];
        [newNode setName:[names objectAtIndex:i]];
        [newNode setIndex:i];
        [newNode setIsLeaf:YES];
        [newNode setAncestor:conRoot];
        [newNode setSupport:1.0];
        [conRoot addDescendant:newNode];
        }
    [conTree addNodeToTree:conRoot];
    [conTree setInitializedDownPass:NO];
    [conTree initializeDownPassSequence];
    
    for (int i=0; i<conParts.size(); i++)
        {
        // get the ancestor and descendants of the portion of the tree where we need to insert a node
        std::vector<Node*> descendants;
        Node* anc = [self findAncestorAndDescendants:descendants forPartition:conParts[i] inTree:conTree];
        if (anc == nil)
            {
            NSLog(@"ERROR");
            exit(1);
            }
        
        // add the new node to the tree
        Node* newNode = [conTree addNode];
        [newNode setSupport:support[i]];
        [newNode setIndex:intNodeIdx++];
        [newNode setAncestor:anc];
        [anc addDescendant:newNode];
        for (std::vector<Node*>::iterator it = descendants.begin(); it != descendants.end(); it++)
            {
            [anc removeDescendant:(*it)];
            [newNode addDescendant:(*it)];
            [(*it) setAncestor:newNode];
            }

        // initialize the traversal sequence
        [conTree setInitializedDownPass:NO];
        [conTree initializeDownPassSequence];
        
        //[conTree print];
        }

    // relabel indices of interior nodes
    int intIdx = [conTree numberOfTaxa];
    for (int n=0; n<[conTree numbeOfDownPassNodes]; n++)
        {
        Node* p = [conTree downPassNodeIndexed:n];
        if ( [p isLeaf] == NO )
            [p setIndex:intIdx++];
        }
    
    //[conTree print];
    
    return conTree;
}

- (Node*)findAncestorAndDescendants:(std::vector<Node*>&)des forPartition:(boost::dynamic_bitset<>&)bv inTree:(GuiTree*)t {

    // mark a path from each tip node to the root
    [t setAllFlagsTo:NO];
    for (int i=0; i<bv.size(); i++)
        {
        if (bv[i] == true)
            {
            Node* p = [t nodeWithOffset:i];
            while (p != nil)
                {
                [p setFlag:YES];
                p = [p ancestor];
                }
            }
        }
    
    for (int i=[t numbeOfDownPassNodes]-1; i>=0; i--)
        {
        Node* p = [t downPassNodeIndexed:i];
        if ( [p flag] == YES && [p isLeaf] == NO )
            {
            if ([p numberOfFlaggedChildren] <= 1)
                {
                [p setFlag:NO];
                }
            else
                {
                for (int j=0; j<[p numberOfDescendants]; j++)
                    {
                    Node* d = [p descendantIndexed:j];
                    if ([d flag] == YES)
                        des.push_back(d);
                    }
                return p;
                }
            }
        }
    return nil;
}

- (BOOL)isPart:(boost::dynamic_bitset<>&)smaller nestedWithPart:(boost::dynamic_bitset<>&)larger {

    if ( (smaller | larger) != larger )
        return NO;
    return YES;
}

- (BOOL)isPart:(boost::dynamic_bitset<>&)smaller compatibleWithPart:(boost::dynamic_bitset<>&)larger {

    if ( (smaller & larger).count() == 0 )
        {
        return YES;
        }
    else
        {
        if ( (smaller | larger) == larger )
            return YES;
        }
    return NO;
}

- (void)dealloc {

    for (std::map<NodeKey,NodeVals*>::iterator it = parts.begin(); it != parts.end(); it++)
        {
        delete it->second;
        }
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolTreeConsensus*)t {

    if ( (self = [super init]) ) 
		{
        myTool = t;
        numTaxa = 0;
        numSamples = 0.0;
        names = [[NSMutableArray alloc] init];
		}
    return self;
}

- (NodeVals*)isKeyPresent:(boost::dynamic_bitset<>&)b {

	NodeKey queryKey(b);
	std::map<NodeKey, NodeVals*>::iterator it = parts.find( queryKey );
	if ( it != parts.end() )
		return it->second;
	return NULL;
}

- (int)numberOfPartitions {

    return (int)parts.size();
}

- (void)print {

    for (int i=0; i<[names count]; i++)
        NSLog(@"%d -- %@", i, [names objectAtIndex:i]);
        
    int i = 0;
    for (std::map<NodeKey,NodeVals*>::iterator it = parts.begin(); it != parts.end(); it++)
        {
        std::cout << i++ << " -- ";
        std::cout << (it->first).getPartition() << " ";
        std::cout << it->second->getNumSamples();
        std::cout << std::endl;
        }
}

- (void)removePartitions {

    parts.clear();
    numSamples = 0;
    [names removeAllObjects];
}

- (NSString*)taxonIndexed:(int)idx {

    if (idx < [names count])
        return [names objectAtIndex:idx];
    return nil;
}

@end
