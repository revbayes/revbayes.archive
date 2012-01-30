#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class WindowControllerDistanceMatrix;
@class WindowControllerDistanceViewer;

#define P_DISTANCE      0
#define JC69            1
#define F81             2
#define TN93            3
#define K2P             4
#define HKY85           5
#define K3P             6
#define GTR             7
#define LOGDET          8

#define EQUAL_RATES     0
#define GAMMA_RATES     1

#define EQUAL_FREQS     0
#define EMPIRICAL_FREQS 1



@interface ToolDistanceMatrix : Tool <NSCoding> {

    int                                distanceType;
    BOOL                               gammaRateVariation;
    int                                baseFreqTreatment;
    double                             proportionInvariableSites;
    double                             gammaShape;

    WindowControllerDistanceMatrix*    controlWindow;
    WindowControllerDistanceViewer*    distanceViewer;
}

@property (readwrite) int    distanceType;
@property (readwrite) BOOL   gammaRateVariation;
@property (readwrite) int    baseFreqTreatment;
@property (readwrite) double proportionInvariableSites;
@property (readwrite) double gammaShape;

- (void)calculateDistances;
- (void)closeControlPanel;
- (void)execute;
- (void)showControlPanel;

@end
