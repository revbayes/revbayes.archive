#import "DistributionNormalGui.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionNormalGui

- (DistributionGui*)clone {

    return [[DistributionNormalGui alloc] initWithDist:self];
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
		[self setDistributionName:@"Normal"];
		[self setDistributionType:DISTRIBUTION_NORMAL];
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

		Parameter* sigmaParm = [[Parameter alloc] initWithDist:self];
		uc = 0x03C3;
		NSString* pStr2 = [NSString stringWithFormat:@"%C:", uc];
		[sigmaParm setParameterName:pStr2];
		[sigmaParm setType:POSITIVE_REAL_NUMBER];
		[sigmaParm setRefParm:nil];
		
		[parameters addObject:muParm];
		[parameters addObject:sigmaParm];
		}
    return self;
}

@end
