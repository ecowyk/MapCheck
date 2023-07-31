#ifndef __MapCheckSuites_H__
#define __MapCheckSuites_H__

#include "IllustratorSDK.h"
#include "Suites.hpp"

#include "AIPanel.h"
#include "AIDocumentView.h"
// AI suite headers

// Suite externs
extern "C"  SPBlocksSuite *sSPBlocks;
extern "C"  AIUnicodeStringSuite*	sAIUnicodeString;
extern "C"	AIPathSuite*			sAIPath;
extern "C"  AIMenuSuite*            sAIMenu;
extern "C" AILayerListSuite*        sAILayerList;
extern "C" AILayerSuite*            sAILayer;
extern "C" AIArtSuite*              sAIArt;
extern "C" AIPathStyleSuite*        sAIPathStyle;
extern "C" AIPanelSuite*            sAIPanel;
extern "C" AIDocumentViewSuite*     sAIDocumentView;
#endif
