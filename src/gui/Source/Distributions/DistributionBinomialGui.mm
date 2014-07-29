#import "DistributionBinomialGui.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionBinomialGui

- (DistributionGui*)clone {

    return [[DistributionBinomialGui alloc] initWithDist:self];
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
		[self setDistributionName:@"Binomial"];
		[self setDistributionType:DISTRIBUTION_BINOMIAL];
        [self setDistributionDomain:POSITIVE_INTEGER];
		[self setDistributionDescription:@"Specifies the \"Binomial distribution\" with two parameters."];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* nParm = [[Parameter alloc] initWithDist:self];
		[nParm setParameterName:@"N:"];
		[nParm setType:POSITIVE_INTEGER];
		[nParm setRefParm:nil];
		[nParm setParameterDescription:@"Number of trials"];

		Parameter* thetaParm = [[Parameter alloc] initWithDist:self];
		unichar uc = 0x03B8;
		NSString* pStr1 = [NSString stringWithFormat:@"%C:", uc];
		[thetaParm setParameterName:pStr1];
		[thetaParm setType:POSITIVE_REAL_NUMBER];
		[thetaParm setRefParm:nil];
		[thetaParm setParameterDescription:@"Probability of success on a single trial"];
		
		[parameters addObject:nParm];
		[parameters addObject:thetaParm];
		}
    return self;
}

@end
