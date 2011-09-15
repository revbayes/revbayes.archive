#import "DistributionGammaGui.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionGammaGui

- (DistributionGui*)clone {

    return [[DistributionGamma alloc] initWithDist:self];
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
		[self setDistributionName:@"Gamma"];
		[self setDistributionType:DISTRIBUTION_GAMMA];
        [self setDistributionDomain:POSITIVE_REAL_NUMBER];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* shapeParm = [[Parameter alloc] initWithDist:self];
		unichar uc = 0x03B1;
		NSString* pStr1 = [NSString stringWithFormat:@"%C:", uc];
		[shapeParm setParameterName:pStr1];
		[shapeParm setType:POSITIVE_REAL_NUMBER];
		[shapeParm setRefParm:nil];

		Parameter* scaleParm = [[Parameter alloc] initWithDist:self];
		uc = 0x03B2;
		NSString* pStr2 = [NSString stringWithFormat:@"%C:", uc];
		[scaleParm setParameterName:pStr2];
		[scaleParm setType:POSITIVE_REAL_NUMBER];
		[scaleParm setRefParm:nil];
		
		[parameters addObject:shapeParm];
		[parameters addObject:scaleParm];
		}
    return self;
}

@end
