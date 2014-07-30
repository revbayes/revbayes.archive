#import "ValueInteger.h"



@implementation ValueInteger

- (Value*)clone {
    
    return [[ValueInteger alloc] initWithValue:self];
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
        int x = [(ValueInteger*)v value];
        value = [[NSNumber alloc] initWithInt:x];
		}
    return self;
}

- (void)setValue:(int)x {

    value = [NSNumber numberWithInt:x];
}

- (NSString*)stringRepresentation {
    
    NSString* strRep = [NSString stringWithFormat:@"%d", [value intValue]];
    return strRep;
}

- (int)value {

    return [value intValue];
}

@end
