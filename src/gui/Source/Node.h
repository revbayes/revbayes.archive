#import <Cocoa/Cocoa.h>



@interface Node : NSObject <NSCoding> {

    double               branchLength;
	NSString*            name;
	int                  index;
    BOOL                 isLeaf;
    BOOL                 isRoot;
    Node*                ancestor;
	NSMutableArray*      descendants;
    float                x;
    float                y;
    int                  depthFromTip;
    BOOL                 isNodeSelected;
    BOOL                 isBranchSelected;
    NSPoint              viewCoordinates;
    int                  state;
    BOOL                 flag;
}

@property (readwrite)        BOOL      flag;
@property (readwrite)        int       index;
@property (readwrite)        BOOL      isLeaf;
@property (readwrite)        BOOL      isRoot;
@property (readwrite)        BOOL      isNodeSelected;
@property (readwrite)        BOOL      isBranchSelected;
@property (readwrite)        double    branchLength;
@property (readwrite,retain) NSString* name;
@property (readwrite)        float     x;
@property (readwrite)        float     y;
@property (readwrite)        int       depthFromTip;
@property (readwrite)        NSPoint   viewCoordinates;
@property (readwrite)        int       state;

- (void)addDescendant:(Node*)des;
- (void)addDescendantAtLeftMostPosition:(Node*)des;
- (int)numberOfDescendants;
- (Node*)ancestor;
- (Node*)descendantIndexed:(size_t)idx;
- (void)print;
- (void)removeDescendant:(Node*)des;
- (void)removeAllDescendants;
- (void)setAncestor:(Node*)anc;

@end
