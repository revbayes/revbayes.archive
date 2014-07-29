#import "Value.h"



@implementation Value

- (Value*)clone {

    return nil;
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

    NSString* strRep = @"";
    return strRep;
}

@end
