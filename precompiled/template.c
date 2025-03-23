// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
//  Simple scroller using 16x16 metatiles, nothing is precalulated
//  
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "asm.h"
#include "smb.h"

// 98h
#define P_VDP_0			0x98			///< Primary MSX port for VDP port #0
#define P_VDP_DATA		P_VDP_0			///< VRAM data port (read/write)

// 99h
#define P_VDP_1			0x99			///< Primary MSX port for VDP port #1
#define P_VDP_REG		P_VDP_1			///< Register setup port (write) (bit 7=1 in second write)
#define P_VDP_ADDR		P_VDP_1			///< VRAM address port (write) (bit 7=0 in second write, bit 6: read/write access (0=read, 1=write))
#define F_VDP_WRIT	    0x40

void VDP_set_vram_dest_16K(u16 dest) __sdcccall(1)
{
	dest;
 
	__asm                
		// Setup address register 
		ld		a, l
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0xFF)
		ld		a, h
		and		a, #0x3F
		add		a, #F_VDP_WRIT
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT
	__endasm;
}


void VDP_write_16K(u8 count, const u8* src) __sdcccall(1)
{
	src, count;
	__asm
		//exit if count is 0
        or a
        jr z, vdp_write_wrt16_exit_loop 
        ld		b, a				// count
        
		ld		c, #P_VDP_DATA	      
        ex      de, hl
       
        // Fast loop	        
	write_wrt16_loop_start:
		outi							// out(c) ; hl++ ; b--
		jp		nz, write_wrt16_loop_start
        
vdp_write_wrt16_exit_loop:

	__endasm;
}

struct Sprite
{
    u8 y,x,pattern, color;
};

enum State {
    air = 0,
    ground = 1
}; 

#define OFFSET_X 10
#define OFFSET_Y 8
#define DEBUG
#define PROFILER
//=============================================================================
// MAIN LOOPy
//=============================================================================
//-----------------------------------------------------------------------------
/// Program entry point
void main()
{
    VDP_SetMode(VDP_MODE_GRAPHIC1);
    
    VDP_SetColor(0xF0);
	VDP_FillVRAM(0x00, 0x0000, 0, 256*64); // Clear 16KB VRAM
    VDP_EnableVBlank(true);
    VDP_RegWrite(0x01,0xe2); //big srpites
    
    Print_SetTextFont(PRINT_DEFAULT_FONT, 1);
    VDP_FillVRAM_16K(0xf1, g_ScreenColorLow, 32*24); // black & white
        
    i16 offset = 0;
    signed char offset8 = 0;
    enum State state = air;

    VDP_WriteVRAM_16K(mario1, g_SpritePatternLow, 10*32);

    struct Sprite sp[5];
    sp[0].pattern = 0; //shoes 
    sp[0].color = 0xe;  


    VDP_WriteVRAM_16K((u8*)sp, g_SpriteAttributeLow, 4*1);        

    i16 velX=0, velY=0;
    i32 posX=(OFFSET_X*8)<<8;
    i16 posY=(0*8)<<8;
    u8 jump=0;
    i16 seq=0;
    for(;;)
    {
        // scroll or move ///////////////////////

        offset  = (posX >> 8)-OFFSET_X*8;
        if (offset<0) // move mario
        {
            offset=0;
            sp[0].x = (posX >> 8);
        }
        else //scroll world
        {
            sp[0].x = OFFSET_X*8;
        }
        sp[0].y = (posY >> 8) + (8*OFFSET_Y);
        
        // set sprite sequence //////////////////

        seq += velX;

        if (velX==0) // idle
        {
            sp[0].pattern = 4*(4);
            seq=0;
        }
        else if (velX<0) // left
        {
            if (state==air) //jumping
            { 
                sp[0].pattern = 4*8; 
            }
            else if(Keyboard_IsKeyPressed(KEY_RIGHT))
            {
                sp[0].pattern = 4*(0); //squid looking right
                seq=0;
            }
            else // running sequence
            {
                i8 sq = seq>>10;
                if (sq<-2)
                {                
                    seq = 0;
                    sq = 0;
                }
                sp[0].pattern = 4*(5-sq);
            }
        }
        else if (velX>0) // right
        {
            if (state==air) // jumping
                sp[0].pattern = 4*3;
            else if(Keyboard_IsKeyPressed(KEY_LEFT))
            {
                sp[0].pattern = 4*(5); //squid looking left
                seq=0;
            }
            else // running
            {
                i8 sq = seq>>10;
                if (sq>2)
                {
                    seq = 0;
                    sq = 0;
                }
                sp[0].pattern = 4*sq;
            }
        }

        #ifdef PROFILER
        VDP_SetColor(0x00); // transparent
        #endif

        Halt();

        #ifdef PROFILER
        VDP_SetColor(0x04); //dark blue
        #endif

        // update VRAM //////////////////////////////
        VDP_WriteVRAM_16K(pPatterns[offset&0x7], g_ScreenPatternLow + 8, 8*PATTERN_COUNT);

        #ifdef PROFILER
        VDP_SetColor(0x06); // dark red
        #endif    

        VDP_set_vram_dest_16K(g_ScreenLayoutLow + (OFFSET_Y*32));
        for(u8 y=0;y<LEVEL_HEIGHT;y++)
        {
            VDP_write_16K(32, &level[y][(offset)>>3]);
        }        

        #ifdef PROFILER
        VDP_SetColor(0x07); // light blue
        #endif

        VDP_WriteVRAM_16K((u8*)sp, g_SpriteAttributeLow, 4);        

        #ifdef PROFILER
        VDP_SetColor(0x12); // dark green
        #endif


#ifdef DEBUG
    if(Keyboard_IsKeyPressed(KEY_RIGHT))
        posX+=256;
    if(Keyboard_IsKeyPressed(KEY_LEFT))
        posX-=256;
#else
        // phyiscs ///////////////////////////////

        // left/right forces + friction
        if (state!=air)
        {
            if(Keyboard_IsKeyPressed(KEY_LEFT))
            {
                if (velX>(-256)) 
                    velX-=16;
            }
            else if(Keyboard_IsKeyPressed(KEY_RIGHT))
            {
                if (velX<256) 
                    velX+=16;
            } 
            else
            {            
                //friction
                if (velX>0)
                {   
                    velX-=4;
                }
                else if (velX<0)
                {
                    velX+=4;
                }
            }
        }

        // apply gravity force
        velY += 14;

        // jump force
        if(Keyboard_IsKeyPressed(KEY_UP))
        {
            if (state!=air)
            {
                velY=-600;
                jump++;
            }

            // check longjump
            if (jump>0)
            {
                jump++;

                if (jump==8) // long jump if pressed 8*0.16ms 
                    velY=-620;
            }
        }
        else
        {
            jump=0;
        }

        posX += velX;
        posY += velY;

        // collision detection and response ///////////////////////////

        // feet above the level
        if (((posY>>8)>>3)+2<0)
            continue;

        // feet hitting ground?
        u8 *pDown = level[((posY>>8)>>3)+2];
        state = (
                    solid[pDown[(((posX + ( 4<<8))>>8)>>3)]] ||
                    solid[pDown[(((posX + (12<<8))>>8)>>3)]]
                )
                ? ground : air;

        if (velY>0)
        {
            if (state==ground)
            {
                posY &= 0xf800;
                velY = 0;
            }
        }    

        // head and sides above the level
        if (((posY>>8)>>3)<=0)
            continue;

        if (velY<0) // hitting ceiling
        {
            u8 *pTop = level[((posY>>8)>>3)];
            if (
                    solid[pTop[(((posX + ( 4<<8))>>8)>>3)]] ||
                    solid[pTop[(((posX + (12<<8))>>8)>>3)]] 
                )
            {
                posY |= 0x07ff;
                velY = 0;
            }
        }

        u8 *pLevel = level[((posY>>8)>>3)+1];
        
        // right side
        {
            u8 r = solid[pLevel[((posX>>8)>>3)+2]];
            if (r>0)
            {
                posX &= 0xfffffe00; 
                if (Keyboard_IsKeyPressed(KEY_RIGHT))
                    velX=3<<4;
                else
                    velX=0;
            }
        }
        
        // left side
        {
            u8 r = solid[pLevel[((posX>>8)>>3)]];
            if (r>0)
            {
                posX |= 0x0000007f;
                if (Keyboard_IsKeyPressed(KEY_LEFT))
                    velX=-3<<4;
                else
                    velX=0;
            }
        }

#endif        
   }
}