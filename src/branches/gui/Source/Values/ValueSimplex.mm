#import "ValueSimplex.h"


@implementation ValueSimplex

- (Value*)clone {
    
    return [[ValueSimplex alloc] initWithValue:self];
}

- (void)dealloc {
    
    [value release];
	[super dealloc];
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
        [value retain];
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

- (void)setValue:(NSArray*)x {

    value = x;
    [value retain];
}

- (NSArray*)value {
    
    return value;
}

@end
