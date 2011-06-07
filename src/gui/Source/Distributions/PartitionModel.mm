#import "PartitionModel.h"



@implementation PartitionModel

@synthesize partition;
@synthesize randomVariable;

- (Distribution*)distribution {

    return distribution;
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

	[aCoder encodeObject:distribution   forKey:@"distribution"];
	[aCoder encodeObject:partition      forKey:@"partition"];
	[aCoder encodeObject:randomVariable forKey:@"randomVariable"];
}

- (id)init {

	[self initWithParm:nil];
	return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super init]) ) 
		{
        distribution   = [aDecoder decodeObjectForKey:@"distribution"];
        partition      = [aDecoder decodeObjectForKey:@"partition"];
        randomVariable = [aDecoder decodeObjectForKey:@"randomVariable"];
		[distribution retain];
		}
	return self;
}

- (id)initWithParm:(Parm*)p {

    if ( (self = [super init]) ) 
		{
        distribution   = nil;
        partition      = nil;
        randomVariable = p;
		}
    return self;
}

- (void)setDistribution:(Distribution *)d {

    if (distribution == d)
        return;
    Distribution* oldValue = distribution;
    distribution = [d retain];
    [oldValue release];
}

@end

