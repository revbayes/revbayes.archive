#import "DistributionChiSquare.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionChiSquare

- (DistributionGui*)clone {

    return [[DistributionChiSquare alloc] initWithDist:self];
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
		[self setDistributionName:@"Chi Square"];
		[self setDistributionType:DISTRIBUTION_CHISQUARE];
        [self setDistributionDomain:POSITIVE_REAL_NUMBER];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* dfParm = [[Parameter alloc] initWithDist:self];
		unichar uc = 0x03BD;
		NSString* pStr1 = [NSString stringWithFormat:@"%C:", uc];
		[dfParm setParameterName:pStr1];
		[dfParm setType:POSITIVE_REAL_NUMBER];
		[dfParm setRefParm:nil];
		
		[parameters addObject:dfParm];
		}
    return self;
}

@end
