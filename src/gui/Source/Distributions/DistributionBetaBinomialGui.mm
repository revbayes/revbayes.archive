#import "DistributionBetaBinomialGui.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionBetaBinomialGui

- (DistributionGui*)clone {

    return [[DistributionBetaBinomialGui alloc] initWithDist:self];
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
		[self setDistributionName:@"Beta Binomial"];
		[self setDistributionType:DISTRIBUTION_BETABINOMIAL];
        [self setDistributionDomain:POSITIVE_INTEGER];
		[self setDistributionDescription:@"Specifies the \"Beta-binomial distribution\" which has three parameters."];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* nParm = [[Parameter alloc] initWithDist:self];
		[nParm setParameterName:@"N:"];
		[nParm setType:POSITIVE_INTEGER];
		[nParm setRefParm:nil];

		Parameter* alphaParm = [[Parameter alloc] initWithDist:self];
		unichar uc = 0x03B1;
		NSString* pStr1 = [NSString stringWithFormat:@"%C:", uc];
		[alphaParm setParameterName:pStr1];
		[alphaParm setType:POSITIVE_REAL_NUMBER];
		[alphaParm setRefParm:nil];
		[alphaParm setParameterDescription:@""];

		Parameter* betaParm = [[Parameter alloc] initWithDist:self];
		uc = 0x03B2;
		NSString* pStr2 = [NSString stringWithFormat:@"%C:", uc];
		[betaParm setParameterName:pStr2];
		[betaParm setType:POSITIVE_REAL_NUMBER];
		[betaParm setRefParm:nil];
		[betaParm setParameterDescription:@""];
		
		[parameters addObject:nParm];
		[parameters addObject:alphaParm];
		[parameters addObject:betaParm];

		// set tips
		NSString* tStr1 = @"N > 0";
		NSString* tStr2 = [NSString stringWithFormat:@"%C %C 0", 0x03B1, 0x2265];
		NSString* tStr3 = [NSString stringWithFormat:@"%C %C 0", 0x03B2, 0x2265];
		[nParm     setParameterDescription:tStr1];
		[alphaParm setParameterDescription:tStr2];
		[betaParm  setParameterDescription:tStr3];
		}
    return self;
}

@end
