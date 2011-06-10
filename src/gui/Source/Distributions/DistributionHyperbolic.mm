#import "DistributionHyperbolic.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionHyperbolic

- (DistributionGui*)clone {

    return [[DistributionHyperbolic alloc] initWithDist:self];
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
		[self setDistributionName:@"Hyperbolic"];
		[self setDistributionType:DISTRIBUTION_HYPERBOLIC];
        [self setDistributionDomain:REAL_NUMBER];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* muParm = [[Parameter alloc] initWithDist:self];
		unichar uc = 0x03BC;
		NSString* pStr1 = [NSString stringWithFormat:@"%C:", uc];
		[muParm setParameterName:pStr1];
		[muParm setType:REAL_NUMBER];
		[muParm setRefParm:nil];

		Parameter* alphaParm = [[Parameter alloc] initWithDist:self];
		uc = 0x03B1;
		NSString* pStr2 = [NSString stringWithFormat:@"%C:", uc];
		[alphaParm setParameterName:pStr2];
		[alphaParm setType:REAL_NUMBER];
		[alphaParm setRefParm:nil];

		Parameter* betaParm = [[Parameter alloc] initWithDist:self];
		uc = 0x03B2;
		NSString* pStr3 = [NSString stringWithFormat:@"%C:", uc];
		[betaParm setParameterName:pStr3];
		[betaParm setType:REAL_NUMBER];
		[betaParm setRefParm:nil];

		Parameter* scaleParm = [[Parameter alloc] initWithDist:self];
		uc = 0x03B4;
		NSString* pStr4 = [NSString stringWithFormat:@"%C:", uc];
		[scaleParm setParameterName:pStr4];
		[scaleParm setType:REAL_NUMBER];
		[scaleParm setRefParm:nil];
		
		[parameters addObject:muParm];
		[parameters addObject:alphaParm];
		[parameters addObject:betaParm];
		[parameters addObject:scaleParm];
		}
    return self;
}

@end
