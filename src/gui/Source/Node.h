#import <Cocoa/Cocoa.h>



@interface Node : NSObject <NSCoding> {

    double            branchLength;
	NSString*         name;
	int               index;
    BOOL              isLeaf;
    BOOL              isRoot;
    Node*             ancestor;
	NSMutableArray*   descendants;
    float             x;
    float             y;
    int               depthFromTip;
    BOOL              isNodeSelected;
    BOOL              isBranchSelected;
    NSPoint           viewCoordinates;
    int               state;
    BOOL              flag;
    float             support;
}

@property (nonatomic)        BOOL      flag;
@property (nonatomic)        int       index;
@property (nonatomic)        BOOL      isLeaf;
@property (nonatomic)        BOOL      isRoot;
@property (nonatomic)        BOOL      isNodeSelected;
@property (nonatomic)        BOOL      isBranchSelected;
@property (nonatomic)        double    branchLength;
@property (nonatomic,strong) NSString* name;
@property (nonatomic)        float     x;
@property (nonatomic)        float     y;
@property (nonatomic)        int       depthFromTip;
@property (nonatomic)        NSPoint   viewCoordinates;
@property (nonatomic)        int       state;
@property (nonatomic,strong) Node*     ancestor;
@property (nonatomic)        float     support;

- (void)addDescendant:(Node*)des;
- (void)addDescendantAtLeftMostPosition:(Node*)des;
- (int)numberOfDescendants;
- (Node*)descendantIndexed:(size_t)idx;
- (int)numberOfFlaggedChildren;
- (void)print;
- (void)removeDescendant:(Node*)des;
- (void)removeAllDescendants;

@end
