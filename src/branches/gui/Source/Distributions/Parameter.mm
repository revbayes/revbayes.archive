#import "Parameter.h"



@implementation Parameter

@synthesize parameterName;
@synthesize parameterDescription;
@synthesize myDist;
@synthesize refParm;
@synthesize type;

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[aCoder encodeObject:parameterName        forKey:@"parameterName"];
	[aCoder encodeObject:parameterDescription forKey:@"parameterDescription"];
	[aCoder encodeObject:myDist               forKey:@"myDist"];
	[aCoder encodeObject:refParm              forKey:@"refParm"];
	[aCoder encodeInt:type                    forKey:@"type"];
}

- (id)init {

	[self initWithDist:nil];
	return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
        parameterName        = [aDecoder decodeObjectForKey:@"parameterName"];
        parameterDescription = [aDecoder decodeObjectForKey:@"parameterDescription"];
		myDist               = [aDecoder decodeObjectForKey:@"myDist"];
        refParm              = [aDecoder decodeObjectForKey:@"refParm"];
		type                 = [aDecoder decodeIntForKey:@"type"];
		[parameterName retain];
		[parameterDescription retain];
		}
	return self;
}

- (id)initWithDist:(Distribution*)d {

    if ( (self = [super init]) ) 
		{
		myDist               = d;
        parameterName        = [[NSString alloc] initWithString:@""];
        parameterDescription = [[NSString alloc] initWithString:@""];
		refParm              = nil;
		type                 = 0;
		}
    return self;
}

- (id)initWithParameter:(Parameter*)p {

    if ( (self = [super init]) ) 
		{
        parameterName        = [[NSString alloc] initWithString:@""];
        parameterDescription = [[NSString alloc] initWithString:@""];
        
        [self setParameterName:[p parameterName]];
        [self setParameterDescription:[p parameterDescription]];
        [self setRefParm:[p refParm]];
        [self setType:[p type]];
        [self setMyDist:[p myDist]];
		}
    return self;
}

@end
