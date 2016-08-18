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
}

@property (nonatomic,strong) NSMutableArray*     myTrees;

- (void)addTreeToSet:(GuiTree*)t;
- (void)closeControlPanel;
- (void)closeInspectorPanel;
- (void)encodeWithCoder:(NSCoder *)aCoder;
- (GuiTree*)getTreeIndexed:(int)idx;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (int)numberOfInlets;
- (BOOL)readTreesFile;
- (NSMutableAttributedString*)sendTip;
- (int)numberOfTreesInSet;
- (IBAction)okButtonAction:(id)sender;
- (void)removeAllTreesFromSet;
- (void)setNumberOfInlets:(int)x;
- (BOOL)writeTreesFile;

@end

