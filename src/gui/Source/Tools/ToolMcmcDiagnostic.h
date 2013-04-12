//
//  ToolMcmcDiagnostic.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 3/28/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import "Tool.h"
#import "Trace.h"

#import <vector>

@class WindowControllerMcmcDiagnostic;

#define FIXED_BURNIN 0
#define OPTIMAL_BURNIN 1


@interface ToolMcmcDiagnostic : Tool <NSCoding> {
    
    // the table showing the results per parameter per chain
    IBOutlet NSTableView*               table;
    
	IBOutlet NSButton*                  okButton;
	IBOutlet NSTabView*                 viewTab;
    
    // the panel to show the progress while analysing the traces
    IBOutlet NSPanel*                   progressPanel;
    IBOutlet NSTextField*               progressTextField;
    IBOutlet NSProgressIndicator*       progressBar;
    
    // within chain convergence gui objects
    IBOutlet NSTextField*               pGewekeTextField;
    IBOutlet NSTextField*               pGewekeLabel;
    IBOutlet NSTextField*               kEssTextField;
    IBOutlet NSTextField*               kEssLabel;
    IBOutlet NSTextField*               kSemTextField;
    IBOutlet NSTextField*               kSemLabel;
    IBOutlet NSTextField*               pStationarityTextField;
    IBOutlet NSTextField*               pStationarityLabel;
    
    // between chain convergence gui objects
    IBOutlet NSTextField*               pIidBetweenTextField;
    IBOutlet NSTextField*               pIidBetweenLabel;
    IBOutlet NSTextField*               rGelmanTextField;
    IBOutlet NSTextField*               rGelmanLabel;
    
    // burnin gui objects
    IBOutlet NSTextField*               fixPercentageTextField;
    IBOutlet NSTextField*               fixPercentageLabel;
    IBOutlet NSPopUpButton*             optimalBurninPuButton;
    
    NSString*                           tab;
    
    std::vector<RevBayesCore::Trace* >* data;
    NSMutableArray*                     included;
    
    
    
    // within chain convergence gui objects
    double                              pGeweke;
    double                              kEss;
    double                              kSem;
    double                              pStationarity;
    
    // between chain convergence gui objects
    double                              pIidBetween;
    double                              rGelman;
    
    // burnin gui objects
    double                              fixPercentage;
    
    // within chain convergence settings
    BOOL                                useGeweke;
    BOOL                                useHeidelbergerWelch;
    BOOL                                useRafteryLewis;
    BOOL                                useEssThreshold;
    BOOL                                useSemThreshold;
    BOOL                                useStationarity;
    
    // between chain convergence settings
    BOOL                                useIidBetweenChains;
    BOOL                                useGelmanRubin;
    
    // burnin estimation settings
    BOOL                                useFixPercentage;
    BOOL                                useOptimalBurninEstimation;
    int                                 burninSelection;
    
}

@property (readwrite,assign) std::vector<RevBayesCore::Trace* >* data;
@property (readwrite,assign) NSMutableArray* included;

@property (readwrite,assign) BOOL useGeweke;
@property (readwrite,assign) BOOL useHeidelbergerWelch;
@property (readwrite,assign) BOOL useRafteryLewis;
@property (readwrite,assign) BOOL useEssThreshold;
@property (readwrite,assign) BOOL useSemThreshold;
@property (readwrite,assign) BOOL useStationarity;
@property (readwrite,assign) BOOL useIidBetweenChains;
@property (readwrite,assign) BOOL useGelmanRubin;
@property (readwrite,assign) BOOL useFixPercentage;
@property (readwrite,assign) BOOL useOptimalBurninEstimation;

@property (readwrite,assign) double pGeweke;
@property (readwrite,assign) double kEss;
@property (readwrite,assign) double kSem;
@property (readwrite,assign) double pStationarity;
@property (readwrite,assign) double pIidBetween;
@property (readwrite,assign) double rGelman;
@property (readwrite,assign) double fixPercentage;

- (NSString*)checkString:(int)index cell:(NSTextFieldCell*)aCell;
- (IBAction)closeAction:(id)sender;
- (void)closeControlPanel;
- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem;
- (void)encodeWithCoder:(NSCoder *)aCoder;
- (void)execute;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (int)numberOfRowsInTableView:(NSTableView *)tableView;
- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row;
- (void)removeAllTraces;
- (IBAction)runButtonAction:(id)sender;
- (NSMutableAttributedString*)sendTip;
- (void)setControlsEnabledState;
- (void)setControlWindowSize;
- (void)setTableColumns;
//- (void)setToolValues;
- (void)setInletsAndOutlets;
- (IBAction)settingChangedAction:(id)sender;
- (void)showControlPanel;
- (void)showInspectorPanel;
- (void)updateForChangeInState;


@end
