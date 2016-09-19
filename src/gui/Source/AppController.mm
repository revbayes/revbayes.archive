#import "AnalysisDocument.h"
#import "AppController.h"
#import "ServerComm.h"
#import "WindowControllerPreferences.h"
#import "WindowControllerRegistration.h"

#define TURN_OFF_SERVER_COMMUNICATION




@implementation AppController

- (void)awakeFromNib {

    NSMenu* mainMenu = [NSApp mainMenu];
    NSMenuItem* treeMenu = [mainMenu itemWithTitle:@"Tree"];
    if (treeMenu != nil)
        {
        [treeMenu setHidden:YES];
        }
		
#   ifndef TURN_OFF_SERVER_COMMUNICATION
    // check the RevBayes server
    ServerComm* server = [[ServerComm alloc] init];
    [server connectToServer];                                 // establish a connection with the server
    BOOL newVersionAvailable = [server checkForNewVersion];   // check to see if a new version of the program is available
    [server checkForNewModels];                               // check for new curated models
    [server disconnectFromServer];                            // disconnect from the server
    [server release];
    
    if ( newVersionAvailable == YES)
        {
        long choice = NSRunAlertPanel(@"New RevBayes Version Available", @"A newer version of RevBayes is available. Do you wish to download the new version?", @"Yes", @"Not now", nil);
        if (choice == NSAlertDefaultReturn)
            {
            [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://fisher.berkeley.edu/cteg/"]];
            }
        }
#   endif

    NSError* error;
    [[NSDocumentController sharedDocumentController] openUntitledDocumentAndDisplay:YES error:&error];
}

- (void)checkExistenceOfSupportFolder {

    // check for the existence of the curated models folder
    NSString* folder1 = @"~/Library/Application Support/RevBayes/Curated Models/";
    NSFileManager* fileManager = [NSFileManager defaultManager];
    folder1 = [folder1 stringByExpandingTildeInPath];
    if ([fileManager fileExistsAtPath:folder1] == NO)
        [fileManager createDirectoryAtPath:folder1 withIntermediateDirectories:YES attributes:nil error:NULL];

    // check for the existence of the user models folder
    NSString* folder2 = @"~/Library/Application Support/RevBayes/User Models/";
    folder2 = [folder2 stringByExpandingTildeInPath];
    if ([fileManager fileExistsAtPath:folder2] == NO)
        [fileManager createDirectoryAtPath:folder2 withIntermediateDirectories:YES attributes:nil error:NULL];
}

- (id)init {

    if ( (self = [super init]) ) 
		{
        // create the preferences window
		preferencesController = [[WindowControllerPreferences alloc] init];
		
		// create the registration panel
		registrationController = [[WindowControllerRegistration alloc] init];
        
        // check that the application-support folder exists at "~/Library/Application Support/RevBayes/"
        [self checkExistenceOfSupportFolder];
        }
    return self;
}

+ (void)initialize {

	// create the dictionary
	NSMutableDictionary* defaultValues = [NSMutableDictionary dictionary];
	
	// archive the color and font objects
	NSData* analysisBgrndColorAsData = [NSKeyedArchiver archivedDataWithRootObject:[NSColor whiteColor]];
	NSData* analysisGridColorAsData  = [NSKeyedArchiver archivedDataWithRootObject:[NSColor blueColor]];
	NSData* modelBgrndColorAsData    = [NSKeyedArchiver archivedDataWithRootObject:[NSColor greenColor]];
	NSData* modelGridColorAsData     = [NSKeyedArchiver archivedDataWithRootObject:[NSColor whiteColor]];
    NSData* notebookFontAsData       = [NSKeyedArchiver archivedDataWithRootObject:[NSFont fontWithName:@"Chalkboard" size:14.0]];
	
	// put the defaults into the dictionary
	[defaultValues setObject:analysisBgrndColorAsData     forKey:RB_AnalysisBgrndColorKey];   // background color for analysis window
	[defaultValues setObject:analysisGridColorAsData      forKey:RB_AnalysisGridColorKey];    // grid color for analysis window
	[defaultValues setObject:modelBgrndColorAsData        forKey:RB_ModelBgrndColorKey];      // background color for graphical model window
	[defaultValues setObject:modelGridColorAsData         forKey:RB_ModelGridColorKey];       // grid color for graphical model window
	[defaultValues setObject:[NSNumber numberWithBool:NO] forKey:RB_UserHasRegisteredKey];    // has the user registered (YES) or not (NO)
	[defaultValues setObject:@""                          forKey:RB_UserFirstNameKey];        // the user's first name
	[defaultValues setObject:@""                          forKey:RB_UserLastNameKey];         // the user's last name
	[defaultValues setObject:@""                          forKey:RB_UserEmailKey];            // the user's e-mail address
	[defaultValues setObject:@""                          forKey:RB_UserInstitutionKey];      // the user's institution
	[defaultValues setObject:@"United States"             forKey:RB_UserCountryKey];          // the user's country
	[defaultValues setObject:notebookFontAsData           forKey:RB_NotebookFontKey];         // the default font for the notebook
    
	// register the dictionary defaults
	[[NSUserDefaults standardUserDefaults] registerDefaults:defaultValues];
}

- (IBAction)showPreferencesPanel:(id)sender {

    [preferencesController openWindow];
}

- (IBAction)showRegistrationPanel:(id)sender {

	[registrationController openWindow];
}

- (BOOL)validateMenuItem:(NSMenuItem*)item {
    
	SEL act = [item action];
	if ( act == @selector(showRegistrationPanel:) )
        {
        if ( [[[NSUserDefaults standardUserDefaults] objectForKey:RB_UserHasRegisteredKey] boolValue] == YES )
            return NO;
        else 
            return YES;
        }
	else
		return YES;
}

@end
