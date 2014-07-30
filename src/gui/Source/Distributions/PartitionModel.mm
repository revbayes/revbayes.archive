#import "PartitionModel.h"



@implementation PartitionModel

@synthesize partition;
@synthesize randomVariable;
@synthesize distribution;

- (void)encodeWithCoder:(NSCoder*)aCoder {

	[aCoder encodeObject:distribution   forKey:@"distribution"];
	[aCoder encodeObject:partition      forKey:@"partition"];
	[aCoder encodeObject:randomVariable forKey:@"randomVariable"];
}

- (id)init {

	return [self initWithParm:nil];
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super init]) ) 
		{
        distribution   = [aDecoder decodeObjectForKey:@"distribution"];
        partition      = [aDecoder decodeObjectForKey:@"partition"];
        randomVariable = [aDecoder decodeObjectForKey:@"randomVariable"];
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

@end

