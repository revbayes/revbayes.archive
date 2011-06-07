#import "ValueVector.h"



@implementation ValueVector

- (Value*)clone {
    
    return nil;
}

- (void)dealloc {
    
    // dealloc here
    [vector release];
	[super dealloc];
}

- (int)dimensions {

    return (int)[vector count];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
    [super encodeWithCoder:aCoder];
	[aCoder encodeObject:vector forKey:@"vector"];
}

- (id)init {
    
    if ( (self = [super init]) ) 
		{
        vector = [[NSMutableArray alloc] init];
        [self setDimensions:2];
		}
	return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {
    
    if ( (self = [super init]) ) 
		{
        vector = [aDecoder decodeObjectForKey:@"vector"];
        [vector retain];
		}
	return self;
}

- (id)initWithDimensions:(int)dim {

    return self;
}

- (id)initWithValue:(Value*)v {
    
    return nil;
}

- (Value*)newValue {

    return nil;
}

- (void)setDimensions:(int)dim {

    if ([self dimensions] == dim)
        return;
    [vector removeAllObjects];
    for (int i=0; i<dim; i++)
        [vector addObject:[self newValue]];
}

- (NSString*)stringRepresentation {
    
    NSString* strRep = [NSString stringWithString:@""];
    return strRep;
}

- (Value*)valueIndexed:(int)idx {
    
    if ( idx >= [vector count] || idx < 0 ) 
        return nil;
    return [vector objectAtIndex:idx];
}

@end
