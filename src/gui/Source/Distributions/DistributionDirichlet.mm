#import "DistributionDirichlet.h"
#import "Parameter.h"
#import "RevBayes.h"



@implementation DistributionDirichlet

- (DistributionGui*)clone {
    
    return [[DistributionDirichlet alloc] initWithDist:self];
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
		[self setDistributionName:@"Dirichlet"];
		[self setDistributionType:DISTRIBUTION_DIRICHLET];
        [self setDistributionDomain:SIMPLEX];
		[self setDistributionDescription:@""];
        [self setAppliesExclusivelyToNodesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToBranchesOfUnrootedTrees:NO];
        [self setAppliesExclusivelyToNodesOfRootedTrees:NO];
        [self setAppliesExclusivelyToBranchsOfRootedTrees:NO];        
        
		Parameter* alphaParm = [[Parameter alloc] initWithDist:self];
		unichar uc = 0x03B1;
		NSString* pStr1 = [NSString stringWithFormat:@"%C:", uc];
		[alphaParm setParameterName:pStr1];
		[alphaParm setType:VECTOR_POSITIVE_REAL_NUMBER];
		[alphaParm setRefParm:nil];
        		
		[parameters addObject:alphaParm];
		}
    return self;
}

@end
