#import "DistributionNegativeBinomial.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionNegativeBinomial

- (DistributionGui*)clone {

    return [[DistributionNegativeBinomial alloc] initWithDist:self];
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
		[self setDistributionName:@"Negative Binomial"];
		[self setDistributionType:DISTRIBUTION_NEGATIVEBINOMIAL];
        [self setDistributionDomain:POSITIVE_INTEGER];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* alphaParm = [[Parameter alloc] initWithDist:self];
		unichar uc = 0x03B1;
		NSString* pStr1 = [NSString stringWithFormat:@"%C:", uc];
		[alphaParm setParameterName:pStr1];
		[alphaParm setType:POSITIVE_REAL_NUMBER];
		[alphaParm setRefParm:nil];

		Parameter* betaParm = [[Parameter alloc] initWithDist:self];
		uc = 0x03B2;
		NSString* pStr2 = [NSString stringWithFormat:@"%C:", uc];
		[betaParm setParameterName:pStr2];
		[betaParm setType:POSITIVE_REAL_NUMBER];
		[betaParm setRefParm:nil];
		
		[parameters addObject:alphaParm];
		[parameters addObject:betaParm];
		}
    return self;
}

@end
