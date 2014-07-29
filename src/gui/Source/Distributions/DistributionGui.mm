#import "DistributionGui.h"
#import "Parameter.h"



@implementation DistributionGui

@synthesize distributionName;
@synthesize distributionDescription;
@synthesize distributionDomain;
@synthesize distributionType;
@synthesize myParm;
@synthesize appliesExclusivelyToNodesOfUnrootedTrees;
@synthesize appliesExclusivelyToBranchesOfUnrootedTrees;
@synthesize appliesExclusivelyToNodesOfRootedTrees;
@synthesize appliesExclusivelyToBranchsOfRootedTrees;

- (DistributionGui*)clone {

    return nil;
}

/* JPHARC - (void)dealloc {

    NSLog(@"distribution with name %@ is dealloced", [self className]);
	[distributionName release];
	[parameters release];
	[super dealloc];
}*/

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[aCoder encodeObject:distributionName                          forKey:@"distributionName"];
	[aCoder encodeObject:distributionDescription                   forKey:@"distributionDescription"];
    [aCoder encodeInt:distributionDomain                           forKey:@"distributionDomain"];
	[aCoder encodeInt:distributionType                             forKey:@"distributionType"];
	[aCoder encodeObject:parameters                                forKey:@"parameters"];
	[aCoder encodeObject:myParm                                    forKey:@"myParm"];
    [aCoder encodeBool:appliesExclusivelyToNodesOfUnrootedTrees    forKey:@"appliesExclusivelyToNodesOfUnrootedTrees"];
    [aCoder encodeBool:appliesExclusivelyToBranchesOfUnrootedTrees forKey:@"appliesExclusivelyToBranchesOfUnrootedTrees"];
    [aCoder encodeBool:appliesExclusivelyToNodesOfRootedTrees      forKey:@"appliesExclusivelyToNodesOfRootedTrees"];
    [aCoder encodeBool:appliesExclusivelyToBranchsOfRootedTrees    forKey:@"appliesExclusivelyToBranchsOfRootedTrees"];
}

- (Parameter*)getParameterIndexed:(int)idx {

	return [parameters objectAtIndex:idx];
}

- (id)init {

	return [self initWithParm:nil];
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
        distributionName                            = [aDecoder decodeObjectForKey:@"distributionName"];
		distributionDescription                     = [aDecoder decodeObjectForKey:@"distributionDescription"];
        distributionDomain                          = [aDecoder decodeIntForKey:@"distributionDomain"];
		distributionType                            = [aDecoder decodeIntForKey:@"distributionType"];
        parameters                                  = [aDecoder decodeObjectForKey:@"parameters"];
		myParm                                      = [aDecoder decodeObjectForKey:@"myParm"];
        appliesExclusivelyToNodesOfUnrootedTrees    = [aDecoder decodeBoolForKey:@"appliesExclusivelyToNodesOfUnrootedTrees"];
        appliesExclusivelyToBranchesOfUnrootedTrees = [aDecoder decodeBoolForKey:@"appliesExclusivelyToBranchesOfUnrootedTrees"];
        appliesExclusivelyToNodesOfRootedTrees      = [aDecoder decodeBoolForKey:@"appliesExclusivelyToNodesOfRootedTrees"];
        appliesExclusivelyToBranchsOfRootedTrees    = [aDecoder decodeBoolForKey:@"appliesExclusivelyToBranchsOfRootedTrees"];
		/* JPHARC [distributionName retain];
		[distributionDescription retain];
		[parameters retain]; */
		}
	return self;
}

- (id)initWithDist:(DistributionGui*)d {

    if ( (self = [super init]) ) 
		{
        distributionName        = [[NSString alloc] initWithString:@""];
		distributionDescription = [[NSString alloc] initWithString:@""];
		parameters              = [[NSMutableArray alloc] init];
        
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:[d appliesExclusivelyToNodesOfUnrootedTrees]];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:[d appliesExclusivelyToBranchesOfUnrootedTrees]];
        [self setAppliesExclusivelyToNodesOfRootedTrees:[d appliesExclusivelyToNodesOfRootedTrees]];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:[d appliesExclusivelyToBranchsOfRootedTrees]];        
        [self setMyParm:[d myParm]];
        [self setDistributionName:[d distributionName]];
        [self setDistributionDescription:[d distributionDescription]];
        [self setDistributionType:[d distributionType]];
        for (int i=0; i<[d numParameters]; i++)
            {
            Parameter* p = [d getParameterIndexed:i];
            Parameter* newParm = [[Parameter alloc] initWithParameter:p];
            [parameters addObject:newParm];
            }
		}
	return self;
}

- (id)initWithParm:(Parm*)p {

    if ( (self = [super init]) ) 
		{
		myParm                                      = p;
        distributionName                            = [[NSString alloc] initWithString:@""];
		distributionDescription                     = [[NSString alloc] initWithString:@""];
		parameters                                  = [[NSMutableArray alloc] init];
		distributionType                            = 0;
        appliesExclusivelyToNodesOfUnrootedTrees    = NO;
        appliesExclusivelyToBranchesOfUnrootedTrees = NO;
        appliesExclusivelyToNodesOfRootedTrees      = NO;
        appliesExclusivelyToBranchsOfRootedTrees    = NO;
		}
    return self;
}

- (int)numParameters {

	return (int)[parameters count];
}

@end
