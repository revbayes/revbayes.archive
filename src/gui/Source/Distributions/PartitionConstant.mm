#import "PartitionConstant.h"
#import "TaxonBipartition.h"
#import "Value.h"



@implementation PartitionConstant

@synthesize partition;
@synthesize constantParm;
@synthesize value;

- (PartitionConstant*)clone {
    
    return [[PartitionConstant alloc] initWithPartitionConstant:self];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[aCoder encodeObject:value        forKey:@"value"];
	[aCoder encodeObject:partition    forKey:@"partition"];
	[aCoder encodeObject:constantParm forKey:@"constantParm"];
}

- (id)init {
    
	return [self initWithParm:nil];
}

- (id)initWithCoder:(NSCoder*)aDecoder {
    
    if ( (self = [super init]) ) 
		{
        value        = [aDecoder decodeObjectForKey:@"value"];
        partition    = [aDecoder decodeObjectForKey:@"partition"];
        constantParm = [aDecoder decodeObjectForKey:@"constantParm"];
		}
	return self;
}

- (id)initWithParm:(Parm*)p {
    
    if ( (self = [super init]) ) 
		{
        value        = nil;
        partition    = nil;
        constantParm = p;
		}
    return self;
}

- (id)initWithPartitionConstant:(PartitionConstant*)pc {
    
    if ( (self = [super init]) ) 
		{
        value        = [[pc value] clone];
        partition    = [[pc partition] clone];
        constantParm = [pc constantParm];
		}
    return self;
}

@end
