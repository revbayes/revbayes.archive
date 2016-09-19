#import "Node.h"
#include <iomanip>
#include <iostream>


@implementation Node

@synthesize ancestor;
@synthesize flag;
@synthesize index;
@synthesize isLeaf;
@synthesize isRoot;
@synthesize isNodeSelected;
@synthesize isBranchSelected;
@synthesize branchLength;
@synthesize name;
@synthesize x;
@synthesize y;
@synthesize depthFromTip;
@synthesize viewCoordinates;
@synthesize state;

- (void)addDescendant:(Node*)des {

    [descendants addObject:des];
}

- (void)addDescendantAtLeftMostPosition:(Node*)des {

    [descendants insertObject:des atIndex:0];
}

- (int)numberOfDescendants {

    return (int)[descendants count];
}

- (Node*)descendantIndexed:(size_t)idx {

    if ( idx >= [descendants count] )
        return nil;
    return [descendants objectAtIndex:idx];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

    [aCoder encodeInt:index           forKey:@"index"];
    [aCoder encodeBool:isLeaf         forKey:@"isLeaf"];
    [aCoder encodeBool:isRoot         forKey:@"isRoot"];
    [aCoder encodeDouble:branchLength forKey:@"branchLength"];
	[aCoder encodeObject:name         forKey:@"name"];
    [aCoder encodeObject:ancestor     forKey:@"ancestor"];
    [aCoder encodeObject:descendants  forKey:@"descendants"];
}

- (id)init {

    if ( (self = [super init]) ) 
		{
		// allocate
		name        = [[NSString alloc] init];
        descendants = [[NSMutableArray alloc] init];
        ancestor    = nil;
        
        // initialize some variables
		index            = 0;
        isLeaf           = NO;
        isRoot           = NO;
        isBranchSelected = NO;
        isNodeSelected   = NO;
		branchLength     = 0.0;
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super init]) ) 
		{
        index        = [aDecoder decodeIntForKey:@"index"];
        isLeaf       = [aDecoder decodeBoolForKey:@"isLeaf"];
        isRoot       = [aDecoder decodeBoolForKey:@"isRoot"];
        branchLength = [aDecoder decodeDoubleForKey:@"branchLength"];
		name         = [aDecoder decodeObjectForKey:@"name"];
        ancestor     = [aDecoder decodeObjectForKey:@"ancestor"];
        descendants  = [aDecoder decodeObjectForKey:@"descendants"];

        isBranchSelected = NO;
        isNodeSelected   = NO;
		}
	return self;
}

- (void)print {

#   if 1
    std::cout << index << " ";
    if (ancestor != nil)
        std::cout << [ancestor index] << " (";
    else
        std::cout << "NIL (";
    for (size_t i=0; i<[descendants count]; i++)
        {
        std::cout << [(Node*)[descendants objectAtIndex:i] index];
        if (i + 1 < [descendants count])
            std::cout << " ";
        }
    std::cout << ") ";
    if (isLeaf == YES)
        {
        const char* myName = [name UTF8String];
        std::cout << myName;
        }
    if (isRoot == YES)
        std::cout << " <- Root";
    std::cout << std::endl;
#   else
    NSString* s = [NSString stringWithFormat:@"%d (%p) %lf %p (", index, self, branchLength, ancestor];
    for (int i=0; i<[descendants count]; i++)
        {
        s = [s stringByAppendingFormat:@"%p", [descendants objectAtIndex:i]];
        if (i + 1 < [descendants count])
            s = [s stringByAppendingString:@" "];
        }
    s = [s stringByAppendingString:@")"];
    if (isLeaf == YES)
        s = [s stringByAppendingFormat:@" %@", name];
    NSLog(@"%@", s);
#   endif
}

- (void)removeDescendant:(Node*)des {

    [descendants removeObject:des];
}

- (void)removeAllDescendants {

    [descendants removeAllObjects];
}

@end
