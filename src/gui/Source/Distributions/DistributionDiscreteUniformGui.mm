#import "DistributionDiscreteUniformGui.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionDiscreteUniformGui

- (DistributionGui*)clone {

    return [[DistributionDiscreteUniformGui alloc] initWithDist:self];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

	[super encodeWithCoder:aCoder];
	// more here
}

- (id)init {

	return [self initWithParm:nil];
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
		[self setDistributionName:@"Discrete Uniform"];
		[self setDistributionType:DISTRIBUTION_DISCRETEUNIFORM];
        [self setDistributionDomain:INTEGER];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* lowerParm = [[Parameter alloc] initWithDist:self];
		[lowerParm setParameterName:@"Lower:"];
		[lowerParm setType:INTEGER];
		[lowerParm setRefParm:nil];

		Parameter* upperParm = [[Parameter alloc] initWithDist:self];
		[upperParm setParameterName:@"Upper:"];
		[upperParm setType:INTEGER];
		[upperParm setRefParm:nil];
		
		[parameters addObject:lowerParm];
		[parameters addObject:upperParm];
		}
    return self;
}

@end
