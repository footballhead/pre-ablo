#include "stdlib.h"

// LoadFileInMem	0000000000482790	
// LoadFileWithMem	000000000048283B	
// get_alloc_file_4828D6	00000000004828D6	
// get_seek_file_48297B	000000000048297B	
// get_file_size_482A2A	0000000000482A2A	
// FileAddLoadPrefix	0000000000482A9D	
// DecodeFullCel	0000000000482AD3	
// CelDraw	0000000000482B53	
// engine_cel_482BB2	0000000000482BB2	
// DrawSlabCel	0000000000482BFB	
// CelDecodeHdrOnly	0000000000482CBC	
// DecodeFullCelL	0000000000482D5F	
// DrawFullCelL_xbytes	0000000000482DDC	
// CelDecDatLightTrans	0000000000482E56	
// engine_cel_482F54	0000000000482F54	
// CelDrawLight	0000000000483046	
// engine_cel_48312C	000000000048312C	
// CelDecodeHdrLightTrans	00000000004831F4	
// CelDrawLightTbl	00000000004832E1	
// CDecodeFullCel	0000000000483477	
// CDrawSlabCel	000000000048350C	
// CDrawSlabCelP	00000000004835CD	
// Cel2DecDatLightOnly	0000000000483670	
// Cel2DecDatLightEntry	0000000000483702	
// Cel2DecDatLightTrans	000000000048377C	
// CDrawSlabCelL	000000000048388F	
// engine_cel_483975	0000000000483975	
// Cel2DecodeLightTrans	0000000000483A3D	
// CDrawSlabCelI	0000000000483B2A	
// CelDecodeRect	0000000000483CD5	
// OutlineSlabCel	0000000000483D7E	
// CelDrawHdrClrHL	0000000000483E99	
// TranslateCels	0000000000483FFB	
// ENG_set_pixel	000000000048407B	
// engine_draw_pixel	00000000004840F9	
// DrawLine	00000000004841F1	
// GetDirection	00000000004848A1	

// .text:004849E2
int random_(int v) {
    if (v <= 0) {
        return 0;
    }
    return rand() % v;
}
