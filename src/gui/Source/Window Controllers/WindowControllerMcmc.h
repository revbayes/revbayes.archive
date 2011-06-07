#import <Cocoa/Cocoa.h>
@class SettingsMcmc;
@class ToolMcmc;



@interface WindowControllerMcmc : NSWindowController {

	IBOutlet NSButton*    okButton;
    ToolMcmc*             myTool;
    SettingsMcmc*         settings;
    int                   mcmcType;
    int                   numRuns;
    int                   numChains;
    int                   numCycles;
    int                   statusFreq;
    int                   sampleFreq;
    double                temperature;
    long int              initSeed;
}

@property (readwrite) int mcmcType;
@property (readwrite) int numRuns;
@property (readwrite) int numChains;
@property (readwrite) int numCycles;
@property (readwrite) int statusFreq;
@property (readwrite) int sampleFreq;
@property (readwrite) double temperature;
@property (readwrite) long int initSeed;

- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolMcmc*)t andSettings:(SettingsMcmc*)s;
- (IBAction)okButtonAction:(id)sender;

@end
