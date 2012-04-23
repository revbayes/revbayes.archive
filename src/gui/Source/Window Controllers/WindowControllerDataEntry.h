#import <Cocoa/Cocoa.h>
#include <vector>
@class RbData;
@class ToolDataEntry;



@interface WindowControllerDataEntry : NSWindowController <NSTableViewDataSource> {

	IBOutlet NSButton*                  okButton;
    IBOutlet NSPopUpButton*             changeDimensionsButton;
    ToolDataEntry*                      myTool;
    IBOutlet NSTableView*               tableView;
    NSMutableArray*                     rowData;
    int                                 numCharacters;
    int                                 numTaxa;
}

@property (readwrite) int numCharacters;
@property (readwrite) int numTaxa;

- (IBAction)addRow:(id)sender;
- (IBAction)addColumn:(id)sender;
- (ToolDataEntry*)dataEntryTool;
- (IBAction)deleteColumn:(id)sender;
- (IBAction)deleteRow:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolDataEntry*)t;
- (int)numberOfColumns;
- (IBAction)okButtonAction:(id)sender;
- (void)saveMatrixToTool;

@end
