#import "DistributionPoissonGui.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionPoissonGui

- (DistributionGui*)clone {

    return [[DistributionPoissonGui alloc] initWithDist:self];
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
		[self setDistributionName:@"Poisson"];
		[self setDistributionType:DISTRIBUTION_POISSON];
        [self setDistributionDomain:POSITIVE_INTEGER];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* lambdaParm = [[Parameter alloc] initWithDist:self];
		unichar uc = 0x03BB;
		NSString* pStr = [NSString stringWithFormat:@"%C:", uc];
		[lambdaParm setParameterName:pStr];
		[lambdaParm setType:POSITIVE_REAL_NUMBER];
		[lambdaParm setRefParm:nil];
		
		[parameters addObject:lambdaParm];
		}
    return self;
}

@end
