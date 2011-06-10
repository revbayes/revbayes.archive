#import "DistributionMultinomial.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionMultinomial

- (DistributionGui*)clone {
    
    return [[DistributionMultinomial alloc] initWithDist:self];
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
		[self setDistributionName:@"Multinomial"];
		[self setDistributionType:DISTRIBUTION_MULTINOMIAL];
        [self setDistributionDomain:VECTOR_POSITIVE_INTEGER];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        
        
		Parameter* pParm = [[Parameter alloc] initWithDist:self];
		unichar uc = 0x0070;
		NSString* pStr1 = [NSString stringWithFormat:@"%C:", uc];
		[pParm setParameterName:pStr1];
		[pParm setType:SIMPLEX];
		[pParm setRefParm:nil];
        
		[parameters addObject:pParm];
		}
    return self;
}

@end
