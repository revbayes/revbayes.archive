#import "DistributionCauchyGui.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionCauchyGui

- (DistributionGui*)clone {

    return [[DistributionCauchyGui alloc] initWithDist:self];
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
		[self setDistributionName:@"Cauchy"];
		[self setDistributionType:DISTRIBUTION_CAUCHY];
        [self setDistributionDomain:REAL_NUMBER];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* locParm = [[Parameter alloc] initWithDist:self];
		unichar uc = 0x03BC; // location parameter
		NSString* pStr1 = [NSString stringWithFormat:@"%C:", uc];
		[locParm setParameterName:pStr1];
		[locParm setType:REAL_NUMBER];
		[locParm setRefParm:nil];

		Parameter* scaleParm = [[Parameter alloc] initWithDist:self];
		uc = 0x03B1; // scale parameter
		NSString* pStr2 = [NSString stringWithFormat:@"%C:", uc];
		[scaleParm setParameterName:pStr2];
		[scaleParm setType:POSITIVE_REAL_NUMBER];
		[scaleParm setRefParm:nil];
		
		[parameters addObject:locParm];
		[parameters addObject:scaleParm];
		}
    return self;
}

@end
