#import "ValueSimplex.h"


@implementation ValueSimplex

@synthesize value;

- (Value*)clone {
    
    return [[ValueSimplex alloc] initWithValue:self];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
    [super encodeWithCoder:aCoder];
	[aCoder encodeObject:value forKey:@"value"];
}

- (id)init {
    
    NSArray* v = [NSArray array];
    
    self = [self initWithVector:v];
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {
    
    if ( (self = [super initWithCoder:aDecoder]) ) 
        {
        value = [aDecoder decodeObjectForKey:@"value"];
        }
	return self;
}

- (id)initWithVector:(NSArray*)v {
    
    if ( (self = [super init]) ) 
        {
        value = [[NSArray alloc] initWithArray:v];
        }
    return self;
}

@end
