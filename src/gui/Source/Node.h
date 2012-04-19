#import <Cocoa/Cocoa.h>



@interface Node : NSObject <NSCoding> {

    double               branchLength;
	NSString*            name;
	int                  index;
    Node*                ancestor;
	NSMutableArray*      descendants;
}

@property (readwrite)        int       index;
@property (readwrite)        double    branchLength;
@property (readwrite,retain) NSString* name;

- (void)addDescendant:(Node*)des;
- (int)numberOfDescendants;
- (Node*)ancestor;
- (Node*)descendantIndexed:(int)idx;
- (void)removeDescendant:(Node*)des;
- (void)setAncestor:(Node*)anc;

@end
