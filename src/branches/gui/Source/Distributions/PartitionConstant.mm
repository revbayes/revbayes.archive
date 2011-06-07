#import "PartitionConstant.h"
#import "TaxonBipartition.h"
#import "Value.h"



@implementation PartitionConstant

@synthesize partition;
@synthesize constantParm;

- (PartitionConstant*)clone {
    
    return [[PartitionConstant alloc] initWithPartitionConstant:self];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[aCoder encodeObject:value        forKey:@"value"];
	[aCoder encodeObject:partition    forKey:@"partition"];
	[aCoder encodeObject:constantParm forKey:@"constantParm"];
}

- (id)init {
    
	[self initWithParm:nil];
	return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {
    
    if ( (self = [super init]) ) 
		{
        value        = [aDecoder decodeObjectForKey:@"value"];
        partition    = [aDecoder decodeObjectForKey:@"partition"];
        constantParm = [aDecoder decodeObjectForKey:@"constantParm"];
		[value retain];
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

- (void)setValue:(Value*)v {
    
    if (value == v)
        return;
    if (value != nil)
        [value release];
    value = v;
    [v retain];
}

- (Value*)value {
    
    return value;
}

@end
