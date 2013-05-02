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

- (DistributionGui*)getDistributionWithIndex:(size_t)idx {

    if ( (idx + 1) > [distributionList count] )
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
    
    [distributionList addObject:[[DistributionBetaBinomialGui       alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionBinomialGui           alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionBirthDeathGui         alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionCauchyGui             alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionChiSquareGui          alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionDirichletGui          alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionDiscreteUniformGui    alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionDiscreteUniformPosGui alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionExponentialGui        alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionGammaGui              alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionHyperbolicGui         alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionLogisticGui           alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionLogNormalGui          alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionMultinomialGui        alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionNegGui                alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionNegativeBinomialGui   alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionNormalGui             alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionPoissonGui            alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionSkewNormalGui         alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionStudentGui            alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionUniformGui            alloc] initWithParm:nil]];
    [distributionList addObject:[[DistributionUniformPosGui         alloc] initWithParm:nil]];
}

- (DistributionGui*)newDistributionWithName:(NSString*)distName andParm:(Parm*)prm {
    
    for (size_t i=0; i<[distributionList count]; i++)
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
