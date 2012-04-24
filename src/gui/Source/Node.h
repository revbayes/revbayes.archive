#import <Cocoa/Cocoa.h>



@interface Node : NSObject <NSCoding> {

    double               branchLength;
	NSString*            name;
	int                  index;
    BOOL                 isLeaf;
    Node*                ancestor;
	NSMutableArray*      descendants;
}

@property (readwrite)        int       index;
@property (readwrite)        BOOL      isLeaf;
@property (readwrite)        double    branchLength;
@property (readwrite,retain) NSString* name;

- (void)addDescendant:(Node*)des;
- (int)numberOfDescendants;
- (Node*)ancestor;
- (Node*)descendantIndexed:(int)idx;
- (void)print;
- (void)removeDescendant:(Node*)des;
- (void)setAncestor:(Node*)anc;

@end
