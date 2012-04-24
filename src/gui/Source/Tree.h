#import <Cocoa/Cocoa.h>
@class Node;



@interface Tree : NSObject <NSCoding> {

	NSMutableArray*      nodes;
    NSMutableArray*      downPassSequence;
    Node*                root;
}

- (Node*)downPassNodeIndexed:(int)idx;
- (void)initializeDownPassSequence;
- (int)numberOfNodes;
- (void)passDown:(Node*)p;
- (void)setNodesToArray:(NSMutableArray*)n;
- (void)setRoot:(Node*)r;

@end
