#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class Tree;
@class WindowControllerTreeSet;




@interface ToolTreeSet : Tool <NSCoding> {
	    
    WindowControllerTreeSet*        controlWindow;
	NSMutableArray*                 myTrees;
}

- (void)addTreeToSet:(Tree*)t;
- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder *)aCoder;
- (void)execute;
- (Tree*)getTreeIndexed:(int)idx;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (BOOL)readTreesFile;
- (NSMutableAttributedString*)sendTip;
- (int)numberOfTreesInSet;
- (IBAction)okButtonAction:(id)sender;
- (void)removeAllTreesFromSet;
- (BOOL)writeTreesFile;

@end

