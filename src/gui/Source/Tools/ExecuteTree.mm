#import "AnalysisView.h"
#import "ExecuteNode.h"
#import "ExecuteTree.h"
#import "Tool.h"
#include <iostream>



@implementation ExecuteTree

@synthesize root;

- (void)addLoop:(NSArray*)loopTools  repeated:(int)nTimes {

    NSMutableSet* tipSet = [NSMutableSet set];
    for (Tool* t in loopTools)
        {
        ExecuteNode* n = [self findNodeWithTool:t];
        [tipSet addObject:n];
        }
    
    for (ExecuteNode* n in downPassSequence)
        {
        if ([n isLeaf] == NO)
            {
            NSSet* nodeSet = [n getNodeSet];
            if ( [tipSet isSubsetOfSet:nodeSet] == YES )
                {
                ExecuteNode* newNode = [self addNode];
                [newNode setParent:n];
                [newNode setNumRepeats:nTimes];
                NSArray* children = [n getChildren];
                NSMutableArray* childrenToRemove = [NSMutableArray array];
                for (ExecuteNode* child in children)
                    {
                    if ( [[child getNodeSet] intersectsSet:tipSet] == YES )
                        {
                        [child setParent:newNode];
                        [newNode addChild:child];
                        [childrenToRemove addObject:child];
                        }
                    }
                for (ExecuteNode* child in tipSet)
                    [newNode addToTipSet:child];
                    
                for (int i=0; i<[childrenToRemove count]; i++)
                    {
                    ExecuteNode* c = [childrenToRemove objectAtIndex:i];
                    if (i == 0)
                        [n replaceChild:c withChild:newNode];
                    else
                        [n removeChild:c];
                    }
                break;
                }
            }
        }
    
    [self initializeDownPassSequence];
}

- (ExecuteNode*)addNode {

    ExecuteNode* newNode = [[ExecuteNode alloc] init];
    [nodes addObject:newNode];
    return newNode;
}

- (Tool*)executeTree {

    badTool = nil;
    [self executeTreeFromNode:root];
    return badTool;
}

- (void)executeTreeFromNode:(ExecuteNode*)p {

    if (p != nil && badTool == nil)
        {
        for (int rep=0; rep<[p numRepeats]; rep++)
            {
            NSArray* children = [p getChildren];
            for (ExecuteNode* child in children)
                [self executeTreeFromNode:child];
                
            if ([p tool] != nil)
                {
                [[p tool] setIsCurrentlyExecuting:YES];
                [[[p tool] analysisView] setNeedsDisplay:YES];
                BOOL isSuccessful = [[p tool] execute];
                if (isSuccessful == NO)
                    badTool = [p tool];
                [[[p tool] analysisView] setNeedsDisplay:YES];
                [[p tool] setIsCurrentlyExecuting:NO];
                [[[p tool] analysisView] setNeedsDisplay:YES];
                }
            }
        }
}

- (ExecuteNode*)findNodeWithTool:(Tool*)t {

    for (ExecuteNode* n in nodes)
        {
        if ([n tool] == t)
            return n;
        }
    return nil;
}

- (id)init {

    if ( (self = [super init]) ) 
        {
        nodes            = [[NSMutableArray alloc] init];
        downPassSequence = [[NSMutableArray alloc] init];
        root             = nil;
        }
    return self;
}

- (id)initWithTools:(NSArray*)toolArray {

    if ( (self = [super init]) ) 
        {
        nodes            = [[NSMutableArray alloc] init];
        downPassSequence = [[NSMutableArray alloc] init];
        root             = [self addNode];
        for (Tool* t in toolArray)
            {
            ExecuteNode* newNode = [self addNode];
            [newNode addToTipSet:newNode];
            [newNode setTool:t];
            [root addChild:newNode];
            [root addToTipSet:newNode];
            [newNode setParent:root];
            }
        [self initializeDownPassSequence];
        }
    return self;

}

- (void)initializeDownPassSequence {

    [downPassSequence removeAllObjects];
    [self passDown:root];
}

- (void)listNodes {

    NSLog(@"List of nodes:");
    for (ExecuteNode* p in nodes)
        {
        if ([p tool] != nil)
            std::cout << [[p tool] executeOrder] << " ";
        else
            std::cout << "N ";
            
        if ([p parent] != nil)
            std::cout << [[[p parent] tool] executeOrder] << " ( ";
        else
            std::cout << "- ( ";
        
        NSArray* children = [p getChildren];
        for (ExecuteNode* child in children)
            {
            if ([child tool] != nil)
                std::cout << [[child tool] executeOrder] << " ";
            else
                std::cout << "N ";
            }
        
        std::cout << ")" << std::endl;
        }
}

- (void)passDown:(ExecuteNode*)p {

    if (p != nil)
        {
        NSArray* children = [p getChildren];
        for (ExecuteNode* child in children)
            [self passDown:child];
        [downPassSequence addObject:p];
        }
}

- (void)print {

    [self printNode:root withIndent:3];
}

- (void)printNode:(ExecuteNode*)p withIndent:(int)indent {

    if (p != nil)
        {
        for (int i=0; i<indent; i++)
            std::cout << " ";
            
        int idx = (int)[nodes indexOfObject:p];
        std::cout << idx << " -- ";
            
        if ([p parent] != nil)
            std::cout << (int)[nodes indexOfObject:[p parent]] << " ( ";
        else
            std::cout << "- ( ";
        NSArray* children = [p getChildren];
        for (ExecuteNode* child in children)
            {
            std::cout << (int)[nodes indexOfObject:child] << " ";
            }
        std::cout << ") ";
        if ([p tool] != nil)
            std::cout << "-> " << [[p tool] executeOrder];
        else
            std::cout << "-> (Repeats " << [p numRepeats] << ")";
        std::cout << std::endl;

        for (ExecuteNode* child in children)
            [self printNode:child withIndent:(indent+3)];
        }
}

@end
