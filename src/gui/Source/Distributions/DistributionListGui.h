#import <Cocoa/Cocoa.h>
@class DistributionGui;
@class Parm;



@interface DistributionListGui : NSObject {

    NSMutableArray*          distributionList;
}

- (DistributionGui*)getDistributionWithIndex:(size_t)idx;
- (void)initializeDistributionList;
- (DistributionGui*)newDistributionWithName:(NSString*)distName andParm:(Parm*)prm;
- (int)numDistributions;

@end
