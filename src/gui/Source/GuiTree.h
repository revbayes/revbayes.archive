#import <Cocoa/Cocoa.h>
@class Node;



@interface Tree : NSObject <NSCoding> {

	NSMutableArray*      nodes;
    NSMutableArray*      downPassSequence;
    BOOL                 initializedDownPass;
    Node*                root;
}

- (void)buildRandomTreeWithSize:(int)n;
- (Node*)downPassNodeIndexed:(int)idx;
- (id)initWithTipSize:(int)n;
- (void)initializeDownPassSequence;
- (int)numberOfNodes;
- (void)passDown:(Node*)p;
- (void)print;
- (void)setCoordinates;
- (void)setNodesToArray:(NSMutableArray*)n;
- (void)setRoot:(Node*)r;

@end
