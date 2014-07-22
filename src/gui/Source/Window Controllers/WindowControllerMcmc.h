#import <Cocoa/Cocoa.h>
@class SettingsMcmc;
@class ToolMcmc;



@interface WindowControllerMcmc : NSWindowController {

	IBOutlet NSButton*         okButton;
    ToolMcmc*                  myTool;
    SettingsMcmc*              settings;
    int                        mcmcType;
    int                        numRuns;
    int                        numChains;
    int                        numCycles;
    int                        statusFreq;
    int                        sampleFreq;
    double                     temperature;
    long int                   initSeed;
}

@property (nonatomic) int      mcmcType;
@property (nonatomic) int      numRuns;
@property (nonatomic) int      numChains;
@property (nonatomic) int      numCycles;
@property (nonatomic) int      statusFreq;
@property (nonatomic) int      sampleFreq;
@property (nonatomic) double   temperature;
@property (nonatomic) long int initSeed;

- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolMcmc*)t andSettings:(SettingsMcmc*)s;
- (IBAction)okButtonAction:(id)sender;

@end
