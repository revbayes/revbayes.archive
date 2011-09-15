#import <Foundation/Foundation.h>
@class DistributionGui;
@class Parm;



@interface DistributionListGui : NSObject {

    NSMutableArray*          distributionList;
}

- (DistributionGui*)getDistributionWithIndex:(int)idx;
- (void)initializeDistributionList;
- (DistributionGui*)newDistributionWithName:(NSString*)distName andParm:(Parm*)prm;
- (int)numDistributions;

@end
