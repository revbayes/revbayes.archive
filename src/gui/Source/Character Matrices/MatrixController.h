#import <Foundation/Foundation.h>



@interface MatrixController : NSObject <NSTableViewDataSource> {

    IBOutlet NSTableView*       tableView;
    NSMutableArray*             rowData;
    int                         numCharacters;
}

@property (readwrite) int numCharacters;

- (IBAction)addRow:(id)sender;
- (IBAction)deleteRow:(id)sender;
- (IBAction)addColumn:(id)sender;
- (IBAction)deleteColumn:(id)sender;
- (void)keyDown:(NSEvent*)event;
- (int)numberOfColumns;

@end
