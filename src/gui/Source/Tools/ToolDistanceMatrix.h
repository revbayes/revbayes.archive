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
    NSMutableArray*                    distances;
    int                                numTaxa;

    WindowControllerDistanceMatrix*    controlWindow;
    WindowControllerDistanceViewer*    distanceViewer;
}

@property (nonatomic) int              distanceType;
@property (nonatomic) BOOL             gammaRateVariation;
@property (nonatomic) int              baseFreqTreatment;
@property (nonatomic) double           proportionInvariableSites;
@property (nonatomic) double           gammaShape;

- (void)calculateDistances;
- (void)closeControlPanel;
- (BOOL)execute;
- (void)showControlPanel;

@end
