#import "Node.h"



@implementation Node

@synthesize index;
@synthesize branchLength;
@synthesize name;

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
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

    [aCoder encodeInt:index           forKey:@"index"];
    [aCoder encodeDouble:branchLength forKey:@"branchLength"];
	[aCoder encodeObject:name         forKey:@"name"];
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
		branchLength = 0.0;
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super init]) ) 
		{
        index        = [aDecoder decodeIntForKey:@"index"];
        branchLength = [aDecoder decodeDoubleForKey:@"branchLength"];
		name         = [aDecoder decodeObjectForKey:@"name"];
		[name retain];
		}
	return self;
}

- (void)removeDescendant:(Node*)des {

    [descendants removeObject:des];
}

- (void)setAncestor:(Node*)anc {

    ancestor = anc;
}

@end
