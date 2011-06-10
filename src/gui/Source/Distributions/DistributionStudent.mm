#import "DistributionStudent.h"
#import "Parameter.h"
#import "RevBayes.h"


@implementation DistributionStudent

- (DistributionGui*)clone {

    return [[DistributionStudent alloc] initWithDist:self];
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
		[self setDistributionName:@"Student's-t"];
		[self setDistributionType:DISTRIBUTION_STUDENTT];
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

		Parameter* dfParm = [[Parameter alloc] initWithDist:self];
		uc = 0x03BD;
		NSString* pStr3 = [NSString stringWithFormat:@"%C:", uc];
		[dfParm setParameterName:pStr3];
		[dfParm setType:POSITIVE_REAL_NUMBER];
		[dfParm setRefParm:nil];
		
		[parameters addObject:muParm];
		[parameters addObject:sigmaParm];
		[parameters addObject:dfParm];
		}
    return self;
}

@end
