#import "Value.h"



@implementation Value

- (Value*)clone {

    return nil;
}

- (void)dealloc {

    // dealloc here
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

}

- (id)init {

    if ( (self = [super init]) ) 
		{
		}
	return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super init]) ) 
		{
		}
	return self;
}

- (id)initWithValue:(Value*)v {

    return nil;
}

- (NSString*)stringRepresentation {

    NSString* strRep = [NSString stringWithString:@""];
    return strRep;
}

@end
