// __________________________
// ¦¦¯¦¯¦¦¯¯¯¦¯¯¦¯¦  ___ __  ¦   ___                __      
// ¦  _ ¦_ ¯¦¦_ ¯_¦ ¦¦   ¦¦  ¦  ¯¦_  _¯¦¦ _¦_¦ ¦¦¯_ ¦¦  _¦¦¦
// ¦  ¦ ¦¯¯ _¦  ¦ ¦ ¯¦_¦ ¦¦__¦  __¦¯ ¯_¦¦ ¦¦ ¦ ¦¦¯  ¯¦_ ¯¦__
// ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯----------+                 ¯¯
//  Simple scroller using 16x16 metatiles
//-----------------------------------------------------------------------------

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Data table
const unsigned char s_tiles[] =
{
    // brick

	0b00000001,
	0b00000001,
	0b00000001,
	0b11111111,
	0b00010000,
	0b00010000,
	0b00010000,
	0b11111111,

    //block 1
    
	0b01111111,
	0b10111111,
	0b11011111,
	0b11101010,
	0b11110101,
	0b11101010,
	0b11110101,
	0b11101010,

	0b11111110,
	0b11111100,
	0b11111000,
	0b10101000,
	0b01010000,
	0b10101000,
	0b01010000,
	0b10101000,

	0b11110101,
	0b11101010,
	0b11110101,
	0b11101010,
	0b11110101,
	0b11100000,
	0b11000000,
	0b10000000,

	0b01010000,
	0b10101000,
	0b01010000,
	0b10101000,
	0b01010000,
	0b00000100,
	0b00000010,
	0b00000001,    
    
    //block 2
    
	0b11111111,
    0b11010101,
    0b10101010,
    0b11010101,
    0b10101010,
    0b11010101,
    0b10101010,
    0b11010101,
    
	0b10111110,
    0b00110100,
    0b10101010,
    0b00110100,
    0b10101010,
    0b00000001,
    0b10111110,
    0b00110100,
    
	0b10101010,
    0b11010101,
    0b00101010,
    0b11000101,
    0b10110000,
    0b11011110,
    0b10101010,
    0b00000001,
    
	0b10101010,
    0b00110100,
    0b01101010,
    0b11010100,
    0b10101010,
    0b11010100,
    0b10101000,
    0b10000000,    
};

// 16x16 metatiles are made of 4 8x8 tiles

const unsigned char g_metatile[] = { 
    0,0,0,0,  // brick
    1,2,3,4,  // block 1
    5,6,7,8   // block 2
 };

// list of metatiles to scroll
const unsigned char s_level[] = {0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,0,0,0,0,1,1,1,0,0,1,1,1,0,0,2,0,0,0,1,1,1,1,0,0,0,1,1};

struct SEQ 
{ 
    unsigned char a, b; 
};

#define PATTERN_SIZE 32
struct SEQ data[PATTERN_SIZE];
unsigned char g_patterns[8*PATTERN_SIZE];
unsigned char g_table_names_size = 0;
unsigned char g_table_names[32*4]; // 4 rows

inline void scroll(unsigned char p1, unsigned char p2, unsigned char offset, unsigned char  dest)
{
    unsigned char offset2 = 8-offset;    
    unsigned char *pchar1 =  s_tiles + p1*8;    
    unsigned char *pchar2 =  s_tiles + p2*8; 
    unsigned char *dptr   =  g_patterns + dest*8;   
 
    *dptr++ = (*pchar1++ << offset) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset) | (*pchar2++ >> offset2);
}

void begin_frame()
{
    g_table_names_size = 0;
}

void scroll_level_ab(unsigned char a, unsigned char b, unsigned char o, unsigned char offset_l)
{
    // check if we have alerady computed that pattern 
    for(unsigned char i=0;i<g_table_names_size;i++)
    {
        if ((a==data[i].a) && (b==data[i].b))
        {             
            g_table_names[o]=i+1;
            return;
        }
    }

    data[g_table_names_size].a=a;
    data[g_table_names_size].b=b;

    scroll(a, b, offset_l, g_table_names_size);
    
    g_table_names_size++;
    
    g_table_names[o]=g_table_names_size;                
}

////////////////////////////// 16x16 tiles

inline unsigned char get_data(const unsigned char * level, unsigned char x, unsigned char y) 
{
    return g_metatile[4*level[x/2] + (x&1) + y];
}

inline void get_meta(unsigned char * level, unsigned char x, unsigned char y, unsigned char * a, unsigned char * b)
{
    *a = get_data(level, x, y);
    *b = get_data(level, x + 1, y);    
}

void scroll_level_meta(const unsigned char * level, unsigned int offset, unsigned char dest)
{
    unsigned char  offset_h = offset / 8;
    unsigned char  offset_l = offset & 7;

    for(unsigned char y=0;y<2;y++)
    {
        for(unsigned char o=0;o<32;o++)
        {
            unsigned char a,b;
            get_meta(level, o + offset_h, y*2, &a, &b);
            scroll_level_ab(a, b, o+y*32, offset_l);
        }
    }    
    
    // write names
    VDP_WriteVRAM_16K( g_table_names, g_ScreenLayoutLow + dest, 32*2);     
}

void end_frame()
{
    // write patterns
    VDP_WriteVRAM_16K( g_patterns, g_ScreenPatternLow + 8, g_table_names_size * 8);      
}

//=============================================================================
// MAIN LOOP
//=============================================================================
#include "font\font_mgl_std0.h"

//-----------------------------------------------------------------------------
/// Program entry point
void main()
{
    VDP_SetMode(VDP_MODE_GRAPHIC2);
    
    VDP_SetColor(0xF0);
	VDP_FillVRAM(0x00, 0x0000, 0, 256*64); // Clear 16KB VRAM
    VDP_EnableVBlank(true);

    Print_SetTextFont(g_Font_MGL_Std0, 1);
        
    unsigned char offset = 0;
    while(1)
    {
        Halt();
        
        if(Keyboard_IsKeyPressed(KEY_RIGHT))
            offset+=1;
        if(Keyboard_IsKeyPressed(KEY_LEFT) && offset>0)
            offset-=1;
                
        begin_frame();
        scroll_level_meta(s_level, offset, 0);
        end_frame();        
        
        Print_SetPosition(0, 10);        Print_DrawHex8(offset); 
        Print_SetPosition(4, 10);        Print_DrawHex8(g_table_names_size);
   }
	Bios_ClearHook(H_TIMI);
	Bios_Exit(0);
}
