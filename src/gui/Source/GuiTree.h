#import <Cocoa/Cocoa.h>
@class Node;



@interface GuiTree : NSObject <NSCoding> {

	NSMutableArray*      nodes;
    NSMutableArray*      downPassSequence;
    BOOL                 initializedDownPass;
    Node*                root;
    int                  numberOfTaxa;
	NSString*            info;

}

@property (readwrite)        BOOL      initializedDownPass;
@property (readwrite,retain) NSString* info;

- (void)addTaxonToRandomBranch;
- (void)buildRandomTreeWithSize:(int)n;
- (void)deselectAllNodes;
- (Node*)downPassNodeIndexed:(int)idx;
- (id)initWithTipSize:(int)n;
- (void)initializeDownPassSequence;
- (BOOL)isRoot:(Node*)p;
- (void)moveFromBranch:(Node*)fromBranch toBranch:(Node*)toBranch forTreeYCoordinates:(float)c;
- (int)numberOfNodes;
- (int)getNumberOfTaxa;
- (void)passDown:(Node*)p;
- (void)print;
- (void)removeSubtreeAboveNode:(Node*)p;
- (void)rootTreeOnNode:(Node*)p;
- (void)setCoordinates;
- (void)setAllFlagsTo:(BOOL)tf;
- (void)setXCoordinates;
- (void)setNodesToArray:(NSMutableArray*)n;
- (void)setRoot:(Node*)r;

@end
