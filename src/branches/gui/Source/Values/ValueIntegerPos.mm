#import "ValueIntegerPos.h"



@implementation ValueIntegerPos

- (Value*)clone {
    
    return [[ValueIntegerPos alloc] initWithValue:self];
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

    self = [self initWithNumber:0];
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

- (id)initWithNumber:(int)v {

    if ( (self = [super init]) ) 
		{
        value = [[NSNumber alloc] initWithInt:v];
		}
    return self;
}

- (id)initWithValue:(Value*)v {
    
    if ( (self = [super init]) ) 
		{
        int x = [(ValueIntegerPos*)v value];
        value = [[NSNumber alloc] initWithInt:x];
		}
    return self;
}

- (void)setValue:(int)x {

    NSNumber* tempNumber = [NSNumber numberWithInt:x];
    [value release];
    value = tempNumber;
    [value retain];
}

- (NSString*)stringRepresentation {
    
    NSString* strRep = [NSString stringWithFormat:@"%d", [value intValue]];
    return strRep;
}

- (int)value {

    return [value intValue];
}

@end
