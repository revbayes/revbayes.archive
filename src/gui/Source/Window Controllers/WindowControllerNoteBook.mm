#import "AnalysisTools.h"
#import "RbAccessoryViewController.h"
#import "WindowControllerNoteBook.h"
#import "WindowControllerPreferences.h"



@implementation WindowControllerNoteBook

-(BOOL)acceptsFirstResponder {

    [[self window] makeFirstResponder:self];
    return YES;
}

- (void)awakeFromNib {

	[textView setImportsGraphics:YES];
	[textView setAllowsImageEditing:YES];
	[[textView layoutManager] setDefaultAttachmentScaling:NSImageScaleProportionallyDown];
	
	if ( [mString length] == 0 )
		{
		NSData* fontAsData = [[NSUserDefaults standardUserDefaults] objectForKey:RB_NotebookFontKey];
		NSFont* df = [NSKeyedUnarchiver unarchiveObjectWithData:fontAsData];

		NSMutableDictionary* attributes = [NSMutableDictionary dictionaryWithCapacity:1];
		[attributes setObject:df forKey:NSFontAttributeName];
		[textView setTypingAttributes:attributes];
		}
	else 
		{
		NSDictionary* attributes = [mString attributesAtIndex:([mString length]-1) effectiveRange:nil];
		NSLog(@"attributes = %@", attributes);
		NSFont* df = [attributes objectForKey:NSFontAttributeName];
		NSLog(@"df = %@", df);
		[textView setTypingAttributes:attributes];
		}
	
	NSString* nbTitle = [NSString stringWithString:@"RevBayes Notebook for \""];
	nbTitle = [nbTitle stringByAppendingString:[myAnalysis analysisName]];
	nbTitle = [nbTitle stringByAppendingString:@"\""];
	[[self window] setTitle:nbTitle];
}

- (NSData*)getData:(NSError**)outError {

    [self setString:[textView textStorage]];
    NSMutableDictionary* dict = [NSDictionary dictionaryWithObject:NSRTFTextDocumentType forKey:NSDocumentTypeDocumentAttribute];
    [textView breakUndoCoalescing];
    NSData* data = [[self string] dataFromRange:NSMakeRange(0, [[self string] length]) documentAttributes:dict error:outError];
    return data;
}

- (id)initWithAnalysis:(AnalysisTools*)a {

	if ( (self = [super initWithWindowNibName:@"NoteBook"]) )
		{
		if (mString == nil)
			mString = [[NSMutableAttributedString alloc] initWithString:@""];
		myAnalysis = a;
		}
	return self;
}

- (BOOL)readFromData:(NSData*)data error:(NSError**)outError {

    BOOL readSuccess = NO;
    NSMutableAttributedString* fileContents = [[NSMutableAttributedString alloc] initWithData:data options:NULL documentAttributes:NULL error:outError];
    if (fileContents) 
		{
        readSuccess = YES;
        [self setString:fileContents];
        [fileContents release];
		}
    return readSuccess;
}

- (void)setString:(NSMutableAttributedString*)newValue {

	if (mString != newValue) 
		{
		if (mString) 
			[mString release];
		mString = [newValue copy];
		}
}

- (NSMutableAttributedString*)string { 

	return [[mString retain] autorelease]; 
}
 
- (void)textDidChange:(NSNotification*)notification {

    [self setString:[textView textStorage]];
}

- (void)windowDidLoad {

	if ([self string] != nil)
		[[textView textStorage] setAttributedString:[self string]];
}

- (void)windowWillClose:(NSNotification*)notification {

	[myAnalysis setNotebookString:[self string]];
}

@end
