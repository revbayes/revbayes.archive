#import <Cocoa/Cocoa.h>
#import "Tool.h"

class Tree;

@interface ToolTreeSet : Tool <NSCoding> {
	
    IBOutlet NSTextField*                   mcmcTextField;
    
	IBOutlet NSButton*                      okButton;
	IBOutlet NSTabView*                     sourceTypeTab;
    
	IBOutlet NSPopUpButton*                 importFileTypeButton;
	IBOutlet NSPopUpButton*                 exportFileTypeButton;
    
    NSString*                               source;
	int                                     fileType;

	NSMutableArray*                         trees;
}

- (void)encodeWithCoder:(NSCoder *)aCoder;
- (void)execute;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (BOOL)readTreesFile;
- (NSMutableAttributedString*)sendTip;
- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem;
- (IBAction)okButtonAction:(id)sender;
- (BOOL)writeTreesFile;

@end

/*
   TAH notes:
   This should probably look like Sebastian's ToolNumericalMCMCOutput class
   With options to get trees from MCMC, import tree(s) and export
   
   This tool can also have an outlet that can go to the model tool if the 
   user wants to import a starting tree or distribution of trees
   
*/