#import "DistributionList.h"
#import "DistributionBetaBinomial.h"
#import "DistributionBinomial.h"
#import "DistributionBirthDeath.h"
#import "DistributionCauchy.h"
#import "DistributionChiSquare.h"
#import "DistributionDirichlet.h"
#import "DistributionDiscreteUniform.h"
#import "DistributionDiscreteUniformPos.h"
#import "DistributionExponential.h"
#import "DistributionGamma.h"
#import "DistributionHyperbolic.h"
#import "DistributionLogistic.h"
#import "DistributionLogNormal.h"
#import "DistributionMultinomial.h"
#import "DistributionNeg.h"
#import "DistributionNegativeBinomial.h"
#import "DistributionNormal.h"
#import "DistributionPoisson.h"
#import "DistributionSkewNormal.h"
#import "DistributionStudent.h"
#import "DistributionUniform.h"
#import "DistributionUniformPos.h"



@implementation DistributionList

- (void)dealloc {
    
    [distributionList release];
    [super dealloc];
}

- (Distribution*)getDistributionWithIndex:(int)idx {

    if ( (idx + 1) > [distributionList count] || idx < 0 )
        return nil;
    return [distributionList objectAtIndex:idx];
}

- (id)init {
    
    if ( (self = [super init]) ) 
        {
        distributionList = [[NSMutableArray alloc] init];
        [self initializeDistributionList];
        }
    return self;
}

- (void)initializeDistributionList {
    
    [distributionList addObject:[[DistributionBetaBinomial       alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionBinomial           alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionBirthDeath         alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionCauchy             alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionChiSquare          alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionDirichlet          alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionDiscreteUniform    alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionDiscreteUniformPos alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionExponential        alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionGamma              alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionHyperbolic         alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionLogistic           alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionLogNormal          alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionMultinomial        alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionNeg                alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionNegativeBinomial   alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionNormal             alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionPoisson            alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionSkewNormal         alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionStudent            alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionUniform            alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionUniformPos         alloc] initWithParm:nil]];
}

- (Distribution*)newDistributionWithName:(NSString*)distName andParm:(Parm*)prm {
    
    for (int i=0; i<[distributionList count]; i++)
        {
        Distribution* d = [distributionList objectAtIndex:i];
        if ( [[d distributionName] isEqualToString:distName] == YES )
            {
            Distribution* newDist = [d clone];
            [newDist setMyParm:prm];
            [newDist autorelease];
            return newDist;
            }
        }
    return nil;
}

- (int)numDistributions {

    return (int)[distributionList count];
}

@end
