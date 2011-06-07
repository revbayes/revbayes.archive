#import "ValueRealPos.h"



@implementation ValueRealPos

- (Value*)clone {
    
    return [[ValueRealPos alloc] initWithValue:self];
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

    self = [self initWithNumber:0.0];
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

- (id)initWithNumber:(float)v {

    if ( (self = [super init]) ) 
		{
        value = [[NSNumber alloc] initWithFloat:v];
		}
    return self;
}

- (id)initWithValue:(Value*)v {
    
    if ( (self = [super init]) ) 
		{
        float x = [(ValueRealPos*)v value];
        value = [[NSNumber alloc] initWithFloat:x];
		}
    return self;
}

- (void)setValue:(float)x {

    NSNumber* tempNumber = [NSNumber numberWithFloat:x];
    [value release];
    value = tempNumber;
    [value retain];
}

- (NSString*)stringRepresentation {
    
    NSString* strRep = [NSString stringWithFormat:@"%f", [value floatValue]];
    return strRep;
}

- (float)value {

    return [value floatValue];
}

@end
