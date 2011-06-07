#import <Cocoa/Cocoa.h>
@class Parameter;
@class Parm;



@interface Distribution : NSObject <NSCoding> {

	NSString*                distributionName;
	NSString*                distributionDescription;
	int                      distributionType;
    int                      distributionDomain;
    BOOL                     appliesExclusivelyToNodesOfUnrootedTrees;
    BOOL                     appliesExclusivelyToBranchesOfUnrootedTrees;
    BOOL                     appliesExclusivelyToNodesOfRootedTrees;
    BOOL                     appliesExclusivelyToBranchsOfRootedTrees;
	NSMutableArray*          parameters;
	Parm*                    myParm;
}

@property (retain) NSString* distributionName;
@property (retain) NSString* distributionDescription;
@property (readwrite) int distributionDomain;
@property (readwrite) int distributionType;
@property (readwrite,assign) Parm* myParm;
@property (readwrite) BOOL appliesExclusivelyToNodesOfUnrootedTrees;
@property (readwrite) BOOL appliesExclusivelyToBranchesOfUnrootedTrees;
@property (readwrite) BOOL appliesExclusivelyToNodesOfRootedTrees;
@property (readwrite) BOOL appliesExclusivelyToBranchsOfRootedTrees;

- (Distribution*)clone;
- (Parameter*)getParameterIndexed:(int)idx;
- (id)initWithDist:(Distribution*)d;
- (id)initWithParm:(Parm*)p;
- (int)numParameters;

@end
