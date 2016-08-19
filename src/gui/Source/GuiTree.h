#import <Cocoa/Cocoa.h>
@class Node;



@interface GuiTree : NSObject <NSCoding> {

	NSMutableArray*     nodes;
    NSMutableArray*     downPassSequence;
    BOOL                initializedDownPass;
    Node*               root;
    int                 numberOfTaxa;
	NSString*           info;
    int                 outgroupIdx;
}

@property (nonatomic)        BOOL               initializedDownPass;
@property (nonatomic,strong) NSString*          info;
@property (nonatomic,strong) Node*              root;
@property (nonatomic)        int                numberOfTaxa;
@property (nonatomic,strong) NSMutableArray*    nodes;
@property (nonatomic)        int                outgroupIdx;

- (Node*)addNode;
- (void)addTaxonToRandomBranch;
- (void)buildRandomTreeWithSize:(int)n;
- (void)deroot;
- (void)deselectAllNodes;
- (Node*)downPassNodeIndexed:(int)idx;
- (Node*)getRoot;
- (id)initWithTipSize:(int)n;
- (void)initializeDownPassSequence;
- (BOOL)isRoot:(Node*)p;
- (float)maxNameHeightForFont:(NSFont*)f;
- (float)maxNameWidthForFont:(NSFont*)f;
- (void)moveFromBranch:(Node*)fromBranch toBranch:(Node*)toBranch forTreeYCoordinates:(float)c;
- (Node*)nodeWithIndex:(int)idx;
- (Node*)nodeWithName:(NSString*)str;
- (int)numberOfNodes;
- (int)getNumberOfTaxa;
- (void)passDown:(Node*)p;
- (void)print;
- (void)removeSubtreeAboveNode:(Node*)p;
- (void)rootTreeOnNode:(Node*)p;
- (void)setCoordinates:(BOOL)drawMonophyleticWrOutgroup;
- (void)setAllFlagsTo:(BOOL)tf;
- (void)setOutgroupIdx:(int)idx;
- (void)setXCoordinates;
- (void)setNodesToArray:(NSMutableArray*)n;
- (NSMutableArray*)taxaNames;

@end
