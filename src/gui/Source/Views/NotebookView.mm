#import "AnalysisTools.h"
#import "NoteBookView.h"
#import "RbAccessoryViewController.h"
#import "WindowControllerNoteBook.h"



@implementation NotebookView

-(BOOL)acceptsFirstResponder {

    return YES;
}

- (void)awakeFromNib {

	//[self setUsesFontPanel:YES];
}

- (IBAction)print:(id)sender {

	NSPrintInfo *tempPrintInfo = [NSPrintInfo sharedPrintInfo];
	
	// Default margins
	[tempPrintInfo setTopMargin: 72];
	[tempPrintInfo setBottomMargin: 72];
	[tempPrintInfo setLeftMargin: 72];
	[tempPrintInfo setRightMargin: 72];
	
	[tempPrintInfo setVerticallyCentered: NO];
	[tempPrintInfo setHorizontallyCentered: NO];
<<<<<<< HEAD
	[tempPrintInfo setHorizontalPagination: NSFitPagination];
=======
	[tempPrintInfo setHorizontalPagination: NSAutoPagination];
>>>>>>> 61480579c58f84b39cbc487a4a6a95bbd1fa0697
	
	NSPrintOperation *op = [NSPrintOperation printOperationWithView:self printInfo:tempPrintInfo];		
	
	NSPrintPanel *printPanel = [op printPanel];
	[printPanel addAccessoryController:[RbAccessoryViewController sharedController]];
	[printPanel setOptions:([printPanel options] | NSPrintPanelShowsPaperSize | NSPrintPanelShowsOrientation | NSPrintPanelShowsPreview)];
	
	[op runOperation];	
}

- (IBAction)selectAll:(id)sender {

}

@end
