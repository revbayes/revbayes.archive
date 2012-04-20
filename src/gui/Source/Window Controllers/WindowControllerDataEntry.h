#import <Cocoa/Cocoa.h>
#include <vector>
@class ToolDataEntry;
class CharacterData;



@interface WindowControllerDataEntry : NSWindowController <NSTabViewDelegate> {

	IBOutlet NSSplitView*              windowSplitView;
	IBOutlet NSScrollView*             dataScrollView;
	IBOutlet NSScrollView*             namesScrollView;

	NSFont*                            dataNamesFont;
	NSFont*                            headerFont;
	NSColor*                           headerBackground;
	NSColor*                           namesBackground;
	NSColor*                           dataTextColor;
	NSColor*                           namesTextColor;
	NSColor*                           headerTextColor;
	NSDictionary*                      standardColorsDict;

    ToolDataEntry*                     myTool;
	IBOutlet NSButton*                 okButton;
	float                              cellWidth;
	float                              cellHeight;
	NSMutableArray*                    dataMatrices;
	NSMutableArray*                    taxaMatrices;
    int                                oldCellValue;
}

- (IBAction)displayMatrix:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (BOOL)initDataMatrixWithMatrix:(RbData*)matrix;
- (id)initWithTool:(ToolDataEntry*)t;
- (void)initStateColorDefaults;
- (float)lengthOfLongestName:(NSMatrix*)matrix;
- (IBAction)okButtonAction:(id)sender;
- (void)setStandardColorsDictKeys:(NSArray*)keysArray forValues:(NSArray*)valueArray;
- (void)splitViewDidResizeSubviews:(NSNotification*)aNotification;
- (NSDictionary*)standardColorsDict;

@end
