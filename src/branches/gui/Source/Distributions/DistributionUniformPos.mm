#import "DistributionUniformPos.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionUniformPos

- (Distribution*)clone {

    return [[DistributionUniformPos alloc] initWithDist:self];
}

- (void)dealloc {

	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

	[super encodeWithCoder:aCoder];
	// more here
}

- (id)init {

	[self initWithParm:nil];
	return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{

		}
	return self;
}

- (id)initWithParm:(Parm*)p {

    if ( (self = [super initWithParm:p]) ) 
		{
		[self setDistributionName:@"Uniform"];
		[self setDistributionType:DISTRIBUTION_UNIFORMPOS];
        [self setDistributionDomain:POSITIVE_REAL_NUMBER];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* lowerParm = [[Parameter alloc] initWithDist:self];
		[lowerParm setParameterName:@"Lower:"];
		[lowerParm setType:POSITIVE_REAL_NUMBER];
		[lowerParm setRefParm:nil];

		Parameter* upperParm = [[Parameter alloc] initWithDist:self];
		[upperParm setParameterName:@"Upper:"];
		[upperParm setType:POSITIVE_REAL_NUMBER];
		[upperParm setRefParm:nil];
		
		[parameters addObject:lowerParm];
		[parameters addObject:upperParm];
		}
    return self;
}

@end
