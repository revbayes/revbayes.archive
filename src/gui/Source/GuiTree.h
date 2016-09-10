#import <Cocoa/Cocoa.h>
@class Node;



@interface GuiTree : NSObject <NSCoding> {

	NSMutableArray*     nodes;
    NSMutableArray*     downPassSequence;
    BOOL                initializedDownPass;
    Node*               root;
    int                 numberOfTaxa;
	NSString*           info;
    NSString*           outgroupName;
    float               weight;
}

@property (nonatomic)        BOOL               initializedDownPass;
@property (nonatomic,strong) NSString*          info;
@property (nonatomic,strong) Node*              root;
@property (nonatomic)        int                numberOfTaxa;
@property (nonatomic,strong) NSMutableArray*    nodes;
@property (nonatomic,strong) NSString*          outgroupName;
@property (nonatomic)        float              weight;

- (Node*)addNode;
- (void)addNodeToTree:(Node*)n;
- (void)addTaxonToRandomBranch;
- (void)buildRandomTreeWithSize:(int)n;
- (void)deroot;
- (void)deselectAllNodes;
- (Node*)downPassNodeIndexed:(int)idx;
- (Node*)getRoot;
- (id)initWithTipSize:(int)n;
- (id)initWithTree:(GuiTree*)t;
- (void)initializeDownPassSequence;
- (BOOL)isRoot:(Node*)p;
- (float)maxNameHeightForFont:(NSFont*)f;
- (float)maxNameWidthForFont:(NSFont*)f;
- (void)moveFromBranch:(Node*)fromBranch toBranch:(Node*)toBranch forTreeYCoordinates:(float)c;
- (NSString*)newickString;
- (Node*)nodeWithIndex:(int)idx;
- (Node*)nodeWithName:(NSString*)str;
- (Node*)nodeWithOffset:(int)idx;
- (int)numbeOfDownPassNodes;
- (int)numberOfNodes;
- (int)getNumberOfTaxa;
- (void)passDown:(Node*)p;
- (void)print;
- (void)removeSubtreeAboveNode:(Node*)p;
- (void)rootTreeOnNode:(Node*)p;
- (void)rootTreeOnNodeNamed:(NSString*)name;
- (void)setCoordinates:(BOOL)drawMonophyleticWrOutgroup;
- (void)setAllFlagsTo:(BOOL)tf;
- (void)setOutgroupName:(NSString*)oName;
- (void)setXCoordinates;
- (void)setNodesToArray:(NSMutableArray*)n;
- (NSMutableArray*)taxaNames;
- (void)traverseNewick:(Node*)p andString:(NSMutableString*)s;

@end
