#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class GuiTree;
@class WindowControllerTreeSet;
@class WindowControllerTreeViewer;




@interface ToolTreeSet : Tool <NSCoding> {
	    
    WindowControllerTreeSet*        controlWindow;
    WindowControllerTreeViewer*     treeInspector;
	NSMutableArray*                 myTrees;
    int                             numberOfInlets;
    int                             outgroupIdx;
}

@property (nonatomic,strong) NSMutableArray*    myTrees;
@property (nonatomic)        int                outgroupIdx;

- (void)addTreeToSet:(GuiTree*)t;
- (void)closeControlPanel;
- (void)closeInspectorPanel;
- (void)encodeWithCoder:(NSCoder *)aCoder;
- (NSString*)getOutgroupName;
- (GuiTree*)getTreeIndexed:(int)idx;
- (int)indexOfTaxon:(NSString*)name;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (int)numberOfInlets;
- (BOOL)readTreesFile;
- (NSMutableAttributedString*)sendTip;
- (int)numberOfTreesInSet;
- (IBAction)okButtonAction:(id)sender;
- (void)removeAllTreesFromSet;
- (void)rerootOnTaxonIndexed:(int)newRootIdx;
- (void)setNumberOfInlets:(int)x;
- (BOOL)writeTreesFile;

@end

