#import <Cocoa/Cocoa.h>
@class AnalysisSplitView;
@class AnalysisSplitViewDelegate;
@class AnalysisTools;
@class AnalysisView;



@interface AnalysisDocument : NSDocument
{

    IBOutlet NSButton*                    helpButton;            // a pointer to the help button
	IBOutlet AnalysisView*                analysisViewPtr;       // a pointer to the view for the window
	IBOutlet NSScrollView*                toolScrollView;        // a pointer to the scroll view containing the tools
    IBOutlet NSPopUpButton*               analysisActionButton;
	NSMutableArray*                       tools;                 // an array holding the pointers to the tools
	IBOutlet NSArrayController*           analysesController;
    AnalysisTools*                        selectedAnalysis;
    NSMutableArray*                       analyses;
	IBOutlet NSSplitView*                 splitView;
	AnalysisSplitViewDelegate*            splitViewDelegate;
	BOOL                                  snapToGrid;
	BOOL                                  showGrid;
	NSTimer*                              rbTimer;
	BOOL                                  isRbTimerActive;
}

@property (readonly) AnalysisView* analysisViewPtr;
@property (readwrite) BOOL isRbTimerActive;

- (IBAction)addAnalysis:(id)sender;
- (NSMutableArray*)checkAnalysis;
- (IBAction)copyAnalysis:(id)sender;
- (IBAction)executeButton:(id)sender;
- (NSString*)findUniqueNameForAnalysis;
- (IBAction)helpButton:(id)sender;
- (void)invalidateTimer;
- (BOOL)isAnaysisNameUnique:(NSString*)str;
- (IBAction)notebookButton:(id)sender;
- (int)numTools;
- (IBAction)removeAnalysis:(id)sender;
- (void)removeFlags;
- (void)scaleToolsByScale:(float)s andFactor:(float)f;
- (void)selectAllTools;
- (void)setShowGrid:(BOOL)x;
- (void)setSnapToGrid:(BOOL)x;
- (BOOL)showGrid;
- (BOOL)snapToGrid;
- (void)toolSourceChanged:(NSNotification*)notification;

@end
