#import "DistributionListGui.h"
#import "DistributionBetaBinomialGui.h"
#import "DistributionBinomialGui.h"
#import "DistributionBirthDeathGui.h"
#import "DistributionCauchyGui.h"
#import "DistributionChiSquareGui.h"
#import "DistributionDirichletGui.h"
#import "DistributionDiscreteUniformGui.h"
#import "DistributionDiscreteUniformPosGui.h"
#import "DistributionExponentialGui.h"
#import "DistributionGammaGui.h"
#import "DistributionHyperbolicGui.h"
#import "DistributionLogisticGui.h"
#import "DistributionLogNormalGui.h"
#import "DistributionMultinomialGui.h"
#import "DistributionNegGui.h"
#import "DistributionNegativeBinomialGui.h"
#import "DistributionNormalGui.h"
#import "DistributionPoissonGui.h"
#import "DistributionSkewNormalGui.h"
#import "DistributionStudentGui.h"
#import "DistributionUniformGui.h"
#import "DistributionUniformPosGui.h"



@implementation DistributionListGui

- (void)dealloc {
    
    [distributionList release];
    [super dealloc];
}

- (DistributionGui*)getDistributionWithIndex:(int)idx {

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

- (DistributionGui*)newDistributionWithName:(NSString*)distName andParm:(Parm*)prm {
    
    for (int i=0; i<[distributionList count]; i++)
        {
        DistributionGui* d = [distributionList objectAtIndex:i];
        if ( [[d distributionName] isEqualToString:distName] == YES )
            {
            DistributionGui* newDist = [d clone];
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
