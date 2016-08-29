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
    NSString*                       outgroupName;
}

@property (nonatomic,strong) NSMutableArray*    myTrees;
@property (nonatomic,strong) NSString*          outgroupName;

- (void)addTreeToSet:(GuiTree*)t;
- (void)closeControlPanel;
- (void)closeInspectorPanel;
- (void)encodeWithCoder:(NSCoder *)aCoder;
- (void)exportTrees;
- (NSString*)getOutgroupName;
- (GuiTree*)getTreeIndexed:(int)idx;
- (void)importTrees;
- (void)importTaskWithFile:(NSString*)fileToOpen;
- (int)indexOfTaxon:(NSString*)name;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (int)numberOfInlets;
- (BOOL)readTreesFile;
- (NSMutableAttributedString*)sendTip;
- (int)numberOfTreesInSet;
- (IBAction)okButtonAction:(id)sender;
- (void)removeAllTreesFromSet;
- (void)rerootOnTaxonNamed:(NSString*)newOutgroupName;
- (void)setNumberOfInlets:(int)x;
- (BOOL)writeTreesFile;

@end

