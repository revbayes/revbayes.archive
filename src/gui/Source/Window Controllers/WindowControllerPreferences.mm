#import "RevBayes.h"
#import "WindowControllerPreferences.h"

NSString* const RB_AnalysisBgrndColorKey = @"AnalysisBackgroundColor";
NSString* const RB_AnalysisGridColorKey  = @"AnalysisGridColor";
NSString* const RB_ModelBgrndColorKey    = @"ModelBackgroundColor";
NSString* const RB_ModelGridColorKey     = @"ModelGridColor";
NSString* const RB_UserHasRegisteredKey  = @"UserHasRegistered";
NSString* const RB_UserFirstNameKey      = @"UserFirstName";
NSString* const RB_UserLastNameKey       = @"UserLastName";
NSString* const RB_UserEmailKey          = @"UserEmail";
NSString* const RB_UserCountryKey        = @"UserCountry";
NSString* const RB_UserInstitutionKey    = @"UserInstitution";
NSString* const RB_NotebookFontKey       = @"NotebookFont";


@implementation WindowControllerPreferences

@synthesize notebookFont;

-(BOOL)acceptsFirstResponder {

    [[self window] makeFirstResponder:self];
    return YES;
}

- (NSColor*)analysisBkgrndColor {

    return currentAnalysisBkgndColor;
}

- (NSColor*)analysisGridColor {

    return currentAnalysisGridColor;
}

- (void)awakeFromNib {

    theFontPanel = [[NSFontManager sharedFontManager] fontPanel:YES];
    [[NSFontManager sharedFontManager] setDelegate:self];

    // get the background color from the defaults
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	
    NSData *colorAsData = [defaults objectForKey:RB_AnalysisBgrndColorKey];
    NSColor* abc = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData]; 
    [analysisBkgrndColor setColor:abc];

    colorAsData = [defaults objectForKey:RB_AnalysisGridColorKey];
    NSColor* agc = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData]; 
    [analysisGridColor setColor:agc];

    colorAsData = [defaults objectForKey:RB_ModelBgrndColorKey];
    NSColor* mbc = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData]; 
    [modelBkgrndColor setColor:mbc];
	
    colorAsData = [defaults objectForKey:RB_ModelGridColorKey];
    NSColor* mgc = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData]; 
    [modelGridColor setColor:mgc];
    
    NSData* fontAsData = [defaults objectForKey:RB_NotebookFontKey];
    NSFont* df = [NSKeyedUnarchiver unarchiveObjectWithData:fontAsData];
	NSLog(@"df name = %@", [df fontName]);
	[defaultFontTextField setStringValue:[df fontName]];
}

- (IBAction)cancelButtonAction:(id)sender {

	// set the values for the objects to their values when the window was opened
    [analysisBkgrndColor setColor:startingAnalysisBkgndColor];
    [analysisGridColor   setColor:startingAnalysisGridColor];
    [modelBkgrndColor    setColor:startingModelBkgndColor];
    [modelGridColor      setColor:startingModelGridColor];
    currentAnalysisBkgndColor = startingAnalysisBkgndColor;
    currentAnalysisGridColor  = startingAnalysisGridColor;
    currentModelBkgndColor    = startingModelBkgndColor;
    currentModelGridColor     = startingModelGridColor;
    [[NSNotificationCenter defaultCenter] postNotificationName:@"UpdatedAnalysisBkgrndColor" object:self];
    [self close];
}

- (IBAction)changeAnalysisBkgrndColor:(id)sender {

    currentAnalysisBkgndColor = [analysisBkgrndColor color];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"UpdatedAnalysisBkgrndColor" object:self];
}

- (IBAction)changeAnalysisGridColor:(id)sender {

    currentAnalysisGridColor = [analysisGridColor color];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"UpdatedAnalysisBkgrndColor" object:self];
}

- (void)changeFont:(id)sender {

    NSFont* oldFont = [self notebookFont];
    NSFont* newFont = [sender convertFont:oldFont];
    [self setNotebookFont:newFont];
	[defaultFontTextField setStringValue:[notebookFont fontName]];
}

- (IBAction)changeModelBkgrndColor:(id)sender {

    currentModelBkgndColor = [modelBkgrndColor color];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"UpdatedModelBkgrndColor" object:self];
}

- (IBAction)changeModelGridColor:(id)sender {

    currentModelGridColor = [modelGridColor color];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"UpdatedModelBkgrndColor" object:self];
}

- (void)getStartingState {

	// get the user defaults
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];

	// unarchive the analysis background color
	NSData *colorAsData = [defaults objectForKey:RB_AnalysisBgrndColorKey];
	currentAnalysisBkgndColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData]; 

	// unarchive the analysis grid color
	colorAsData = [defaults objectForKey:RB_AnalysisGridColorKey];
	currentAnalysisGridColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData]; 
	
	// unarchive the model background color
	colorAsData = [defaults objectForKey:RB_ModelBgrndColorKey];
	currentModelBkgndColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData]; 

	// unarchive the model grid color
	colorAsData = [defaults objectForKey:RB_ModelGridColorKey];
	currentModelGridColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData]; 
	
	// unarchive the notebook font
	NSData* fontAsData = [defaults objectForKey:RB_NotebookFontKey];
	notebookFont = [NSKeyedUnarchiver unarchiveObjectWithData:fontAsData];
}

- (id)init {

	if ( (self = [super initWithWindowNibName:@"PreferencesPanel"]) )
        {
		// get the user defaults
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
		
		// unarchive the analysis background color
        NSData *colorAsData = [defaults objectForKey:RB_AnalysisBgrndColorKey];
        currentAnalysisBkgndColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData]; 

		// unarchive the analysis grid color
        colorAsData = [defaults objectForKey:RB_AnalysisGridColorKey];
        currentAnalysisGridColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData]; 
		
		// unarchive the model background color
        colorAsData = [defaults objectForKey:RB_ModelBgrndColorKey];
        currentModelBkgndColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData]; 

		// unarchive the model grid color
        colorAsData = [defaults objectForKey:RB_ModelGridColorKey];
        currentModelGridColor = [NSKeyedUnarchiver unarchiveObjectWithData:colorAsData]; 
		
		// unarchive the notebook font
        NSData* fontAsData = [defaults objectForKey:RB_NotebookFontKey];
        notebookFont = [NSKeyedUnarchiver unarchiveObjectWithData:fontAsData];
		
		// set the starting values for the background colors
		startingAnalysisBkgndColor = currentAnalysisBkgndColor;
		startingAnalysisGridColor  = currentAnalysisGridColor;
		startingModelBkgndColor    = currentModelBkgndColor;
		startingModelGridColor     = currentModelGridColor;
        }
	return self;
}

- (NSColor*)modelBkgrndColor {

    return currentModelBkgndColor;
}

- (NSColor*)modelGridColor {

    return currentModelGridColor;
}

- (IBAction)okButtonAction:(id)sender {

    // get a pointer to the defaults
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];

    // register default values for analysis window background color
	NSColor *color = [analysisBkgrndColor color];
	NSData *colorAsData = [NSKeyedArchiver archivedDataWithRootObject:color];
	[defaults setObject:colorAsData forKey:RB_AnalysisBgrndColorKey];

    // register default values for analysis grid color
	color = [analysisGridColor color];
	colorAsData = [NSKeyedArchiver archivedDataWithRootObject:color];
	[defaults setObject:colorAsData forKey:RB_AnalysisGridColorKey];

    // register default values for model window background color
	color = [modelBkgrndColor color];
	colorAsData = [NSKeyedArchiver archivedDataWithRootObject:color];
	[defaults setObject:colorAsData forKey:RB_ModelBgrndColorKey];

    // register default values for model grid color
	color = [modelGridColor color];
	colorAsData = [NSKeyedArchiver archivedDataWithRootObject:color];
	[defaults setObject:colorAsData forKey:RB_ModelGridColorKey];
    
    // register default values for the notebook font
    NSData* fontAsData = [NSKeyedArchiver archivedDataWithRootObject:notebookFont];
    [defaults setObject:fontAsData forKey:RB_NotebookFontKey];

    // close the window
    [self close];
}

- (IBAction)openFontSelector:(id)sender {

    [theFontPanel orderFront:sender];
}

- (void)openWindow {

	[self getStartingState];
	startingAnalysisBkgndColor = currentAnalysisBkgndColor;
	startingAnalysisGridColor  = currentAnalysisGridColor;
	startingModelBkgndColor    = currentModelBkgndColor;
	startingModelGridColor     = currentModelGridColor;
	NSData* fontAsData = [[NSUserDefaults standardUserDefaults] objectForKey:RB_NotebookFontKey];
	notebookFont = [NSKeyedUnarchiver unarchiveObjectWithData:fontAsData];
	[defaultFontTextField setStringValue:[notebookFont fontName]];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"UpdatedAnalysisBkgrndColor" object:self];
    [[self window] center];
	[self showWindow:self];
}

- (IBAction)restoreButtonAction:(id)sender {

	currentAnalysisBkgndColor = [NSColor blueColor];
	currentAnalysisGridColor  = [NSColor whiteColor];
	currentModelBkgndColor    = [NSColor greenColor];
	currentModelGridColor     = [NSColor whiteColor];
	[self setNotebookFont:[NSFont fontWithName:@"Chalkboard" size:14.0]];
	
    [analysisBkgrndColor setColor:currentAnalysisBkgndColor];
    [analysisGridColor setColor:currentAnalysisGridColor];
    [modelBkgrndColor setColor:currentModelBkgndColor];
    [modelGridColor setColor:currentModelGridColor];
	[defaultFontTextField setStringValue:[notebookFont fontName]];

    [[NSNotificationCenter defaultCenter] postNotificationName:@"UpdatedAnalysisBkgrndColor" object:self];
    
    [self restoreRegistration];  // for debugging purposes only
}

- (void)restoreRegistration {

	NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults removeObjectForKey:RB_UserHasRegisteredKey];
    [defaults removeObjectForKey:RB_UserFirstNameKey];
    [defaults removeObjectForKey:RB_UserLastNameKey];
    [defaults removeObjectForKey:RB_UserEmailKey];
    [defaults removeObjectForKey:RB_UserInstitutionKey];
    [defaults removeObjectForKey:RB_UserCountryKey];
    [defaults removeObjectForKey:RB_NotebookFontKey];
}

@end
