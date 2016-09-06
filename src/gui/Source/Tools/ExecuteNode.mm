#import "ExecuteNode.h"
#import "Tool.h"



@implementation ExecuteNode

@synthesize parent;
@synthesize tool;
@synthesize numRepeats;
@synthesize loopId;

- (void)addChild:(ExecuteNode*)n {

    [children addObject:n];
}

- (void)addToTipSet:(ExecuteNode*)n {

    [tipSet addObject:n];
}

- (void)execute {

    if (tool != nil)
        [tool execute];
}

- (NSArray*)getChildren {

    NSArray* c = [[NSArray alloc] initWithArray:children];
    return c;
}

- (NSMutableSet*)getNodeSet {

    return tipSet;
}

- (id)init {

    if ( (self = [super init]) ) 
        {
        children   = [[NSMutableArray alloc] init];
        tipSet     = [[NSMutableSet alloc] init];
        parent     = nil;
        tool       = nil;
        loopId     = nil;
        numRepeats = 1;
        }
    return self;
}

- (BOOL)isLeaf {

    if ([children count] == 0)
        return YES;
    return NO;
}

- (void)removeAllChildren {

    [children removeAllObjects];
}

- (void)removeChild:(ExecuteNode*)n {

    [children removeObject:n];
}

- (void)replaceChild:(ExecuteNode*)n withChild:(ExecuteNode*)c {

    int idx = (int)[children indexOfObject:n];
    [children replaceObjectAtIndex:idx withObject:c];
}

@end
