#import "DistributionExponential.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionExponential

- (Distribution*)clone {

    return [[DistributionExponential alloc] initWithDist:self];
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
		[self setDistributionName:@"Exponential"];
		[self setDistributionType:DISTRIBUTION_EXPONENTIAL];
        [self setDistributionDomain:POSITIVE_REAL_NUMBER];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* lambdaParm = [[Parameter alloc] initWithDist:self];
		unichar uc = 0x03BB;
		NSString* pStr1 = [NSString stringWithFormat:@"%C:", uc];
		[lambdaParm setParameterName:pStr1];
		[lambdaParm setType:POSITIVE_REAL_NUMBER];
		[lambdaParm setRefParm:nil];
		
		[parameters addObject:lambdaParm];
		}
    return self;
}

@end
