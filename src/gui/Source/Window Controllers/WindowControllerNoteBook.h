#import <Cocoa/Cocoa.h>
@class AnalysisTools;
@class EditorBackgroundView;
@class NotebookView;



@interface WindowControllerNoteBook : NSWindowController {

    IBOutlet NotebookView*       textView;
    NSMutableAttributedString*   mString;
	AnalysisTools*               myAnalysis;
}

- (id)initWithAnalysis:(AnalysisTools*)a;
- (void)setString:(NSMutableAttributedString*)newValue;
- (NSMutableAttributedString*)string;

@end
