#include "IllustratorSDK.h"
#include "MapCheckSuites.h"

// Suite externs
extern "C"
{
	SPBlocksSuite*			sSPBlocks = NULL;
	AIUnicodeStringSuite*	sAIUnicodeString = NULL;
	AIPathSuite*			sAIPath = NULL;
	AIMenuSuite*            sAIMenu = NULL;
	AILayerListSuite*		sAILayerList = NULL;
	AILayerSuite*			sAILayer = NULL;
	AIArtSuite*				sAIArt = NULL;
	AIPathStyleSuite*		sAIPathStyle = NULL;
}

// Import suites
ImportSuite gImportSuites[] = 
{
	kSPBlocksSuite, kSPBlocksSuiteVersion, &sSPBlocks,
	kAIUnicodeStringSuite, kAIUnicodeStringVersion, &sAIUnicodeString,
	kAIPathSuite, kAIPathSuiteVersion,	&sAIPath,
	kAIMenuSuite, kAIMenuSuiteVersion,  &sAIMenu,
	kAILayerListSuite, kAILayerListSuiteVersion, &sAILayerList,
	kAILayerSuite, kAILayerVersion, &sAILayer,
	kAIArtSuite, kAIArtSuiteVersion, &sAIArt,
	kAIPathStyleSuite, kAIPathStyleSuiteVersion, &sAIPathStyle,
	nil, 0, nil
};
