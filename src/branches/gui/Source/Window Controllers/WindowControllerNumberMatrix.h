#import <Cocoa/Cocoa.h>
#import "ToolNumericalMcmcOutput.h"



@interface WindowControllerNumberMatrix : NSWindowController {
@private
    
    IBOutlet NSTableView*               table;
    IBOutlet NSButton*                  showInfoButton;
    
    IBOutlet NSTextField*               burninLabel;
    IBOutlet NSTextField*               convergenceLabel;
    IBOutlet NSTextField*               essLabel;
    IBOutlet NSTextField*               meanLabel;
    IBOutlet NSTextField*               medianLabel;
    IBOutlet NSTextField*               parameterNameLabel;
    IBOutlet NSTextField*               semLabel;
    
//    NSButton*                           close;
    ToolNumericalMcmcOutput*            myTool;
}

- (IBAction)closeAction:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolNumericalMcmcOutput*)t;
- (void)tableView:(NSTableView *)tableView mouseDownInHeaderOfTableColumn:(NSTableColumn *)tableColumn;
- (int)numberOfRowsInTableView:(NSTableView *)tableView;
//- (void)setControlsEnabledState;
- (void)setControlWindowSize;
- (void)setTableColumns;
//- (void)setToolValues;
- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row;

@end
