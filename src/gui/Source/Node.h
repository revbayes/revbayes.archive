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
}

@property (readwrite)        int       index;
@property (readwrite)        BOOL      isLeaf;
@property (readwrite)        BOOL      isRoot;
@property (readwrite)        double    branchLength;
@property (readwrite,retain) NSString* name;
@property (readwrite)        float     x;
@property (readwrite)        float     y;
@property (readwrite)        int       depthFromTip;

- (void)addDescendant:(Node*)des;
- (int)numberOfDescendants;
- (Node*)ancestor;
- (Node*)descendantIndexed:(int)idx;
- (void)print;
- (void)removeDescendant:(Node*)des;
- (void)setAncestor:(Node*)anc;

@end
