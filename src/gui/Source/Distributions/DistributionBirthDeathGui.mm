#import "DistributionBirthDeathGui.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionBirthDeathGui

- (DistributionGui*)clone {

    return [[DistributionBirthDeathGui alloc] initWithDist:self];
}

/* JPHARC - (void)dealloc {

	[super dealloc];
} */

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
		[self setDistributionName:@"Birth-Death"];
		[self setDistributionType:DISTRIBUTION_BIRTH_DEATH];
        [self setDistributionDomain:POSITIVE_REAL_NUMBER];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:YES];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        

		Parameter* speciationRateParm = [[Parameter alloc] initWithDist:self];
		unichar uc = 0x03BB;
		NSString* pStr1 = [NSString stringWithFormat:@"%C:", uc];
		[speciationRateParm setParameterName:pStr1];
		[speciationRateParm setType:POSITIVE_REAL_NUMBER];
		[speciationRateParm setRefParm:nil];

		Parameter* extinctionRateParm = [[Parameter alloc] initWithDist:self];
		uc = 0x03BC;
		NSString* pStr2 = [NSString stringWithFormat:@"%C:", uc];
		[extinctionRateParm setParameterName:pStr2];
		[extinctionRateParm setType:POSITIVE_REAL_NUMBER];
		[extinctionRateParm setRefParm:nil];

		Parameter* speciesSamplingFraction = [[Parameter alloc] initWithDist:self];
		uc = 0x03C1;
		NSString* pStr3 = [NSString stringWithFormat:@"%C:", uc];
		[speciesSamplingFraction setParameterName:pStr3];
		[speciesSamplingFraction setType:POSITIVE_REAL_NUMBER];
		[speciesSamplingFraction setRefParm:nil];
		
		[parameters addObject:speciationRateParm];
		[parameters addObject:extinctionRateParm];
        [parameters addObject:speciesSamplingFraction];
		}
    return self;
}

@end
