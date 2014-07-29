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

	return [self initWithDist:nil];
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
        parameterName        = [aDecoder decodeObjectForKey:@"parameterName"];
        parameterDescription = [aDecoder decodeObjectForKey:@"parameterDescription"];
		myDist               = [aDecoder decodeObjectForKey:@"myDist"];
        refParm              = [aDecoder decodeObjectForKey:@"refParm"];
		type                 = [aDecoder decodeIntForKey:@"type"];
		/* JPHARC [parameterName retain];
		[parameterDescription retain]; */
		}
	return self;
}

- (id)initWithDist:(DistributionGui*)d {

    if ( (self = [super init]) ) 
		{
		myDist               = d;
        parameterName        = @"";
        parameterDescription = @"";
		refParm              = nil;
		type                 = 0;
		}
    return self;
}

- (id)initWithParameter:(Parameter*)p {

    if ( (self = [super init]) ) 
		{
        parameterName        = @"";
        parameterDescription = @"";
        
        [self setParameterName:[p parameterName]];
        [self setParameterDescription:[p parameterDescription]];
        [self setRefParm:[p refParm]];
        [self setType:[p type]];
        [self setMyDist:[p myDist]];
		}
    return self;
}

@end
