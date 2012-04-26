#import "Node.h"



@implementation Node

@synthesize index;
@synthesize isLeaf;
@synthesize isRoot;
@synthesize branchLength;
@synthesize name;
@synthesize x;
@synthesize y;
@synthesize depthFromTip;

- (void)addDescendant:(Node*)des {

    [descendants addObject:des];
}

- (int)numberOfDescendants {

    return (int)[descendants count];
}

- (Node*)ancestor {

    return ancestor;
}

- (Node*)descendantIndexed:(int)idx {

    if (idx < 0 || idx >= [descendants count])
        return nil;
    return [descendants objectAtIndex:idx];
}

- (void)dealloc {

	[name release];
    [descendants release];
	[super dealloc];
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
		index        = 0;
        isLeaf       = NO;
        isRoot       = NO;
		branchLength = 0.0;
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
		[name retain];
        [ancestor retain];
        [descendants retain];
		}
	return self;
}

- (void)print {

    NSString* s = [NSString stringWithFormat:@"%d (%x) %lf %x (", index, self, branchLength, ancestor];
    for (int i=0; i<[descendants count]; i++)
        {
        s = [s stringByAppendingFormat:@"%x", [descendants objectAtIndex:i]];
        if (i + 1 < [descendants count])
            s = [s stringByAppendingString:@" "];
        }
    s = [s stringByAppendingString:@")"];
    if (isLeaf == YES)
        s = [s stringByAppendingFormat:@" %@", name];
    NSLog(@"%@", s);
}

- (void)removeDescendant:(Node*)des {

    [descendants removeObject:des];
}

- (void)setAncestor:(Node*)anc {

    ancestor = anc;
}

@end
