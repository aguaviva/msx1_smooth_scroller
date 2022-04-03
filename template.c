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

#define USE_ASM 1

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Data table
const unsigned char s_tiles[] =
{
    // nothing 0

	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,

    // brick  1

	0b11111111,
	0b00010000,
	0b00010000,
	0b00010000,
	0b11111111,
	0b00000001,
	0b00000001,
	0b00000001,

    //block 2
    
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
    
    //stones 6
    
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
    
    // question mark 10
	0b11010101,
	0b11111111,
	0b01011111,
	0b11111010,
	0b01110100,
	0b11111001,
	0b01110101,
	0b11111001,

	0b01010101,
	0b11111110,
	0b11111010,
	0b10111110,
	0b01011110,
	0b10101110,
	0b11001110,
	0b10101110,

	0b01111111,
	0b11111110,
	0b01111111,
	0b11111110,
	0b01111111,
	0b11011111,
	0b01111111,
	0b00000000,

	0b00001110,
	0b10111110,
	0b00111110,
	0b11111110,
	0b00111110,
	0b00111010,
	0b11111110,
	0b00000000,
    
    //castle door 14
	0b00001100,
	0b00110000,
	0b01000000,
	0b11000000,
	0b10000000,
	0b10000000,
	0b00000000,
	0b00000000,

	0b00110001,
	0b00001101,
	0b00000011,
	0b00000011,
	0b00000001,
	0b00000001,
	0b00000000,
	0b00000000,


    // turret 16

	0b11110000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b11111111,
	0b00000001,
	0b00000001,
	0b00000001,

    // brick 17

	0b00011111, 
	0b00010000,
	0b00010000,
	0b00010000,
	0b11111111,
	0b00000001,
	0b00000001,
	0b00000001,

    // stick 18

	0b00000011, 
	0b00000011,
	0b00000011,
	0b00000011,
	0b00000011,
	0b00000011,
	0b00000011,
	0b00000011,

    // flag 19

	0b10101010, 
	0b01010101,
	0b00101010,
	0b00010101,
	0b00001010,
	0b00000101,
	0b00000010,
	0b00000001,

	0b10101010,
	0b01111101,
	0b11010110,
	0b10010011,
	0b10111010,
	0b11101111,
	0b01111110,
	0b01010101,
    
    // coin 21
	0b00000000, 
	0b00000010,
	0b00000101,
	0b00000011,
	0b00000111,
	0b00001010,
	0b00000101,
	0b00001010,

	0b00000000,
	0b10000000,
	0b01000000,
	0b10100000,
	0b00010000,
	0b10100000,
	0b00010000,
	0b10100000,

	0b00000111,
	0b00001010,
	0b00000111,
	0b00001010,
	0b00000111,
	0b00000010,
	0b00000001,
	0b00000000,

	0b00010000,
	0b10100000,
	0b00010000,
	0b10100000,
	0b01000000,
	0b10100000,
	0b01000000,
	0b00000000,    
    
    // cloud top 25
    0x03,0x04,0x18,0x20,0x20,0x20,0x40,0x80,
    0xC0,0x20,0x10,0x14,0x0A,0x01,0x01,0x01,
    0x07,0x08,0x10,0x00,0x60,0x80,0x80,0x40,
    0x90,0xA8,0x48,0x0A,0x05,0x01,0x01,0x02    
};

// 16x16 metatiles are made of 4 8x8 tiles

const unsigned char g_metatile[] = { 
    0,0,0,0,     // none
    1,1,1,1,     // brick
    2,3,4,5,     // block 1
    6,7,8,9,     // block stone
    10,11,12,13, //question mark
    1,1,14,15,   // brick
    16,17,1,1,   // brick
    1,0,1,0,     // window 1      7   
    0,1,0,1,     // window 2      8
    0,18,0,18,   //stick         9
    19,20,0,19,  // flag
    21,22,23,24, // coin 11
    0,0,0,27, // cloud 1/3 12
    25,26,0,0, // cloud 2/3 13
    0,0,28,0, // cloud 3/3 14
 };

// list of metatiles to scroll
const unsigned char s_level0[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,0,0,0,0,0,0,0,0,0,0,11,11,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,9,0,0,0,0};
const unsigned char s_level1[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,9,0,0,0,0,0,6,6,6,0,0,0,0};
const unsigned char s_level2[] = {0,0,0,0,0,1,1,1,0,0,0,0,2,0,0,2,0,0,0,0,0,1,0,0,1,4,1,4,1,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,9,0,0,0,0,0,7,1,8,0,0,0,0};
const unsigned char s_level3[] = {0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,9,0,0,0,0,6,6,6,6,6,0,0,0};
const unsigned char s_level4[] = {0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,9,0,0,0,0,1,1,5,1,1,0,0,0};
const unsigned char s_level5[] = {0,12,13,14,0,0,0,0,0,0,2,0,2,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,2,0,0,0,0,1,1,0,1,1,0,0,0};
const unsigned char s_level6[] = {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};

const unsigned char *s_levels[] = {s_level0, s_level1, s_level2, s_level3, s_level4, s_level5, s_level6, s_level6 };

#define REALLOC_TILE_SLOTS
#define TRACK_TILE_REF_COUNT
#define CACHE_ROTATIONS

struct SEQ 
{ 
#ifdef TRACK_TILE_REF_COUNT
    unsigned int ref;
#endif    
    unsigned char a, b; 
};

#define PATTERN_SIZE 32   // can't make it bigger due 8bit math :/
#define NAME_ROWS 16
unsigned char g_tile_pairs_size = 0;
struct SEQ tile_pairs[PATTERN_SIZE];
unsigned char g_table_names[32*NAME_ROWS]; 
unsigned char *g_p_table_names = g_table_names;
unsigned char g_offset_l=0;
unsigned char g_offset_l_times_3=0;
unsigned int  g_offset_h=0;


#ifdef CACHE_ROTATIONS
unsigned char rotated_tiles_cache[8][8*PATTERN_SIZE];


void rotate_blocks(unsigned char *pchar1, unsigned char *pchar2, unsigned char  *dptr, u8 offset)
{
    unsigned char offset1 = offset;    
    unsigned char offset2 = 8-offset;    

    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);   
}


void cache_rotations_generate(u8 slot)
{    
    for(u8 i=0;i<8;i++)
    {
        unsigned char *pchar1 =  s_tiles + tile_pairs[slot].a*8;    
        unsigned char *pchar2 =  s_tiles + tile_pairs[slot].b*8; 
        unsigned char *dptr   =  &rotated_tiles_cache[i][slot*8];       
        rotate_blocks(pchar1, pchar2, dptr, i);
    }
}
#else
unsigned char g_patterns[8*PATTERN_SIZE];
#endif

#ifdef REALLOC_TILE_SLOTS
unsigned char tile_allocated_size=0;
unsigned char tile_available_size=0;
unsigned char tile_available[PATTERN_SIZE];

void tile_init_reallocs()
{
    for(u8 i=0;i<PATTERN_SIZE;i++)
    {
        tile_available[i] = (PATTERN_SIZE-1)-i;
    }
    
    tile_available_size = PATTERN_SIZE;
    tile_allocated_size = 0;
}

u8 tile_alloc()
{    
    tile_available_size--;
    tile_allocated_size++;
    return tile_available[tile_available_size];
}

void tile_free(u8 tile_id)
{
    tile_available[tile_available_size] = tile_id;
    tile_available_size++;
    tile_allocated_size--;        
}
#endif

void set_offset(unsigned int offset)
{
    g_offset_h = offset / 8;
    g_offset_l = (offset & 7);
#if USE_ASM == 1
    g_offset_l_times_3 = (offset & 7)*3;
#endif    
}


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


void VDP_write_block_16K(u8 count, const u8* src) __sdcccall(1)
{
	src, count;
	__asm

		//exit if count is 0
        or a
        jr z, vdp_exit_loop 
        
        ld		b, a				// count

		ld		c, #P_VDP_DATA	      
        ex      de, hl
        
        // Fast loop	       
	write_block_wrt16_loop_start:
        .rept 8            
		outi							// out(c) ; hl++ ; b--
        .endm
		jp		nz, write_block_wrt16_loop_start
        
vdp_exit_loop:

	__endasm;
}

void track_tile_pairs(unsigned char a, unsigned char b) __sdcccall(1)
{
#if USE_ASM == 1
    // check if we have already computed that pattern 
    unsigned char i=0;
    
#ifdef REALLOC_TILE_SLOTS    
    for(;i<g_tile_pairs_size;i++)
    {
        if (tile_pairs[i].ref == 0)
            continue;
            
        if ((a==tile_pairs[i].a) && (b==tile_pairs[i].b))
        {                     
        #ifdef TRACK_TILE_REF_COUNT
            tile_pairs[i].ref++;
        #endif    
            goto foundit;
        }
    }
#else
    for(;i<g_tile_pairs_size;i++)
    {
        if ((a==tile_pairs[i].a) && (b==tile_pairs[i].b))
        {                     
        #ifdef TRACK_TILE_REF_COUNT
            tile_pairs[i].ref++;
        #endif    
            goto foundit;
        }
    }
    //allocate new pair   
    g_tile_pairs_size++;       

#endif

#ifdef TRACK_TILE_REF_COUNT
    #ifdef REALLOC_TILE_SLOTS        
        i = tile_alloc();
        
        if (tile_allocated_size>g_tile_pairs_size)
            g_tile_pairs_size = tile_allocated_size;
        
     #endif    
    tile_pairs[i].ref=1;
#endif    

    tile_pairs[i].a=a;
    tile_pairs[i].b=b;
    
#ifdef CACHE_ROTATIONS    
    cache_rotations_generate(i);
#endif        
    
foundit:    

    *g_p_table_names=i+1;                
#else
    __asm
    ld d, a           ; a
    ld e, l           ; b
    ld hl, #_tile_pairs
    
    xor a
    ld c,a

    ld a, (#_g_tile_pairs_size)
    or a
    jr z,notfound 

    ld b,a    
loopa11:     
#ifdef TRACK_TILE_REF_COUNT
    inc hl           ; skip ref
#endif  

    ld a,(hl)
    inc hl
    cp d
    jr nz, nope1
    
    ld a,(hl)
    cp e
    jr z, foundit    

nope1:
    inc hl
    inc c
    djnz loopa11
;------------------------------------

    ; store a,b
notfound:    
    ld (hl),d
    inc hl
    ld (hl),e    
    
    inc c
    ld hl, #_g_tile_pairs_size
    ld (hl),c
    dec c
    
foundit:    

    ; g_table_names[o]=i+1;    
        
    ld hl, (#_g_p_table_names)

    inc c
    ld (hl),c    

    __endasm;  
#endif    
}

void track_tile_pairs_reffed(unsigned char a, unsigned char b) __sdcccall(1)
{   
#ifdef TRACK_TILE_REF_COUNT
    u8 slot = *g_p_table_names-1;
    tile_pairs[slot].ref--;
    
#ifdef REALLOC_TILE_SLOTS    
    if (tile_pairs[slot].ref==0)
    {
        tile_pairs[slot].a=0xff;
        tile_free(slot);
    }
#endif
        
#endif          
    
    track_tile_pairs(a, b);
}    

#ifdef CACHE_ROTATIONS
void scroll_patterns() {}
#else
//
// This code scrolls just one tile, this function is not used in this sample but I left it here as a reference as it might help understand more code below
//
void scroll_single_tile(unsigned char p1, unsigned char p2, unsigned char  dest)  __sdcccall(0)
{
#if USE_ASM == 0
    unsigned char offset1 = g_offset_l;    
    unsigned char offset2 = 8-g_offset_l;    
    unsigned char *pchar1 =  s_tiles + p1*8;    
    unsigned char *pchar2 =  s_tiles + p2*8; 
    unsigned char *dptr   =  g_patterns + dest*8;   
 
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);
    *dptr++ = (*pchar1++ << offset1) | (*pchar2++ >> offset2);   
#else
  __asm    
    .macro scroll_SHIFT amount
        .rept 8            
            ld a,(de)
            inc de

            push de
            
            .rept amount
            sla a
            .endm  
            
            ld d,a

            ld a,(bc)    
            inc bc
            
            .rept 8 - amount
            srl a
            .endm  
            
            or d
            
            pop de 
            
            ld (hl),a    
            inc hl
        .endm  
    .endm  
  
    push	ix
    ld		ix, #0
    add		ix, sp

    ;---------------------------------

    ; _s_tiles + p1*8  //TODO: Use 16 bit math as otherwise corruption happens when there are more than 32 scrolling tiles
    ld a, 4(IX) 
    sla a
    sla a    
    sla a

    ld de,#_s_tiles    

    add a,e
    ld e,a
    adc a,d
    sub e
    ld d,a    

    ;---------------------------------

    ; _s_tiles + p2*8  //TODO: same as above
    ld a, 5(IX) 
    sla a
    sla a    
    sla a

    ld bc,#_s_tiles    

    add a,c
    ld c,a
    adc a,b
    sub c
    ld b,a    

    
    ;---------------------------------
    
    ; g_patterns + dest*8    //TODO: same as above
    ld a, 6(IX) 
    sla a
    sla a    
    sla a

    ld hl,#_g_patterns

    add a,l
    ld l,a
    adc a,h
    sub l
    ld h,a   

    ; jump table ---------------------------
     
    push hl
    
    ld hl, #scroll_nexty
    
    ld a, (#_g_offset_l_times_3)
    
    add a,l
    ld l,a
    adc a,h
    sub l
    ld h,a       
    
    jp (hl)

scroll_nexty:    
    jp scroll_shift_0
    jp scroll_shift_1
    jp scroll_shift_2
    jp scroll_shift_3
    jp scroll_shift_4
    jp scroll_shift_5
    jp scroll_shift_6
    jp scroll_shift_7
scroll_shift_0:
    pop hl
    scroll_SHIFT 0
    jp scroll_done
scroll_shift_1:
    pop hl
    scroll_SHIFT 1
    jp scroll_done
scroll_shift_2:
    pop hl
    scroll_SHIFT 2
    jp scroll_done
scroll_shift_3:
    pop hl
    scroll_SHIFT 3
    jp scroll_done
scroll_shift_4:
    pop hl
    scroll_SHIFT 4
    jp scroll_done
scroll_shift_5:
    pop hl
    scroll_SHIFT 5
    jp scroll_done
scroll_shift_6:
    pop hl
    scroll_SHIFT 6
    jp scroll_done
scroll_shift_7:
    pop hl
    scroll_SHIFT 7
    jp scroll_done    
scroll_done:    
    pop	ix    
  __endasm;
#endif
}

void scroll_patterns()
{
#if USE_ASM == 0
    for(unsigned char i=0;i<g_tile_pairs_size;i++)
    {
        scroll_single_tile(tile_pairs[i].a, tile_pairs[i].b, i);
    }
#else
    __asm

    ;----------------------------------------------------
    ; scrolls horizontally a 8x8 tile by a certain amount
    ; tiles input: de, bc
    ; scrolled output:   hl
    ; amount: macro parameter
    ;----------------------------------------------------
    .macro SHIFT amount
        .rept 8            
            ld a,(de)
            inc de
            push de
                .rept amount
                sla a
                .endm  
                
                ld d,a

                ld a, (bc)    
                inc bc
                
                .rept 8 - amount
                srl a
                .endm  
                
                or d
            
                ld (hl), a    
                inc hl
            pop de
        .endm  
    .endm  

    ;----------------------------------------------------
    ; scrolls horizontally a 8x8 tile
    ; tiles input: de, bc
    ; scrolled output:   hl
    ; amount: _g_offset
    ;----------------------------------------------------
    .macro JUMP_TABLE
    push hl

    ld hl, #nexty
    
    ld a, (#_g_offset_l_times_3)

    add a,l
    ld l,a
    adc a,h
    sub l
    ld h,a       
    
    jp (hl)

nexty:    
    jp shift_0
    jp shift_1
    jp shift_2
    jp shift_3
    jp shift_4
    jp shift_5
    jp shift_6
    jp shift_7
shift_0:
    pop hl
    SHIFT 0
    jp done
shift_1:
    pop hl
    SHIFT 1
    jp done
shift_2:
    pop hl
    SHIFT 2
    jp done
shift_3:
    pop hl
    SHIFT 3
    jp done
shift_4:
    pop hl
    SHIFT 4
    jp done
shift_5:
    pop hl
    SHIFT 5
    jp done
shift_6:
    pop hl
    SHIFT 6
    jp done
shift_7:
    pop hl
    SHIFT 7
    jp done        
done:       
    .endm  
   
    ; -----------------------------------------------
    ; loop for all the tile pairs in the table struct
    ; -----------------------------------------------
    ld a, (#_g_tile_pairs_size)
    ld b,a    
    xor a
    ld c,a
    ld hl, #_tile_pairs
loopa1: 
    push bc

    ld a,c
    push af

    ;-------------------- ref
#ifdef TRACK_TILE_REF_COUNT    
    inc hl      
#endif    
        
    ;-------------------- p1: de = _s_tiles[tile_pairs[i].a*8];   //TODO: Use 16 bit math as otherwise corruption happens when there are more than 32 scrolling tiles
    ld a, (hl)
    inc hl
    sla a
    sla a    
    sla a
    
    ld de,#_s_tiles    
    
    add a,e
    ld e,a
    adc a,d
    sub e
    ld d,a  

    ;-------------------- p2:  bc = _s_tiles[tile_pairs[i].b*8]; i++  //TODO: Use 16 bit math as otherwise corruption happens when there are more than 32 scrolling tiles
    ld a, (hl)
    inc hl
    sla a
    sla a    
    sla a
    
    ld bc,#_s_tiles    
    
    add a,c
    ld c,a
    adc a,b
    sub c
    ld b,a      

    ;--------------------- dest: bc = g_patterns + (i*8)    //TODO: Use 16 bit math as otherwise corruption happens when there are more than 32 scrolling tiles
    pop af    ; get i
    
    ; g_patterns + dest*8
    sla a
    sla a    
    sla a

    push hl  

    ld hl,#_g_patterns

    add a,l
    ld l,a
    adc a,h
    sub l
    ld h,a            
    
    JUMP_TABLE    

    pop hl
    
    ;-----------------------------------------------
    pop bc
    inc c
    dec b
    jr z, exit
    jp loopa1  
    
exit:    
    __endasm;  
#endif    
}
#endif

////////////////////////////// 16x16 tiles

inline unsigned char get_data(const unsigned char * level, unsigned char x, unsigned char y) 
{
    return g_metatile[4*level[x/2] + (x&1) + y];
}

inline void get_meta(const unsigned char * level, unsigned char x, unsigned char y, unsigned char * a, unsigned char * b)
{
    *a = get_data(level, x, y);
    *b = get_data(level, x + 1, y);    
}

void scroll_level_meta(const unsigned char * level)
{
    for(unsigned char y=0;y<2;y++)
    {
        for(unsigned char o=0;o<32;o++)
        {
            unsigned char a,b;
            get_meta(level, o + g_offset_h, y*2, &a, &b);

            track_tile_pairs(a, b);
                       
            g_p_table_names++;
        }
    }        
}

//#define SAFE_SCROLL //safe but super slow, just there as a reference

void update_vram_names()
{
    VDP_set_vram_dest_16K(g_ScreenLayoutLow+32*6); //starts drawing at y=6 to center vertically things a bit
    u8 *dest = g_table_names;
    for (u8 i = 0; i< NAME_ROWS;i++)
    {   
        #ifdef SAFE_SCROLL
        VDP_write_block_16K(32, dest);
        #else
        unsigned char n = ((u8)g_offset_h) & 31;
        VDP_write_16K(32-n, dest + n);
        VDP_write_16K(n   , dest + 0);
        #endif
        dest+=32;
    }
}

void update_vram_patterns()
{
    VDP_set_vram_dest_16K(g_ScreenPatternLow + 8);
    #ifdef CACHE_ROTATIONS
    VDP_write_block_16K(g_tile_pairs_size*8, rotated_tiles_cache[g_offset_l]);
    #else
    VDP_write_block_16K(g_tile_pairs_size*8, g_patterns);    
    #endif
}

void update_all_frame()
{
    g_tile_pairs_size = 0;

    g_p_table_names = g_table_names;
    for (u8 i = 0; i< NAME_ROWS/2;i++)
    {       
        scroll_level_meta(s_levels[i]);
    }
    scroll_patterns();       

    Halt();

    update_vram_names();
    update_vram_patterns();
}

void update_right_column()
{    
    #ifdef SAFE_SCROLL
    g_p_table_names = g_table_names + 31;    
    #else
    g_p_table_names = g_table_names + ((((u8)g_offset_h)+31)&31);    
    #endif

    for(unsigned int y=0;y<NAME_ROWS;y++)
    {
        #ifdef SAFE_SCROLL
        unsigned char *p = g_table_names + y*32;
        for(unsigned char x=0;x<32-1;x++)
            p[x] = p[x+1];        
        #endif
        
        unsigned char a,b;
        get_meta(s_levels[y/2], g_offset_h + 31, (y&1)*2, &a, &b);
               
        track_tile_pairs_reffed(a, b);    
                
        g_p_table_names+=32;
                
    }         
}

void update_left_column()
{    
    #ifdef SAFE_SCROLL
    g_p_table_names = g_table_names + 0;    
    #else
    g_p_table_names = g_table_names + ((((u8)g_offset_h)+0)&31);    
    #endif

    for(unsigned int y=0;y<NAME_ROWS;y++)
    {
        #ifdef SAFE_SCROLL    
        unsigned char *p = g_table_names + y*32;
        for(signed char x=31;x>0;x--)
            p[x] = p[x-1];        
        #endif
        
        unsigned char a,b;
        get_meta(s_levels[y/2], g_offset_h, (y&1)*2, &a, &b);
               
        track_tile_pairs_reffed(a, b);        

        g_p_table_names+=32;
    }         
}

//=============================================================================
// MAIN LOOP
//=============================================================================
//-----------------------------------------------------------------------------
/// Program entry point
void main()
{
    VDP_SetMode(VDP_MODE_GRAPHIC2);
    
    VDP_SetColor(0xF0);
	VDP_FillVRAM(0x00, 0x0000, 0, 256*64); // Clear 16KB VRAM
    VDP_EnableVBlank(true);

    // use just 1 tiles set for all the screen
    VDP_RegWrite(3, 0x9f); g_ScreenColorLow    = 0x2000;
    VDP_RegWrite(4, 0x00); 
    
    Print_SetTextFont(PRINT_DEFAULT_FONT, 1);
        
    unsigned int offset = 0;
    signed char offset8 = 0;
    bool show_debug = false;

    #ifdef REALLOC_TILE_SLOTS
    tile_init_reallocs();
    #endif

    set_offset(offset);
    update_all_frame();

    while(1)
    {        
        if(Keyboard_IsKeyPressed(KEY_RIGHT) && offset<(sizeof(s_level6)-16)*2*8)
        {
            offset+=1;
            set_offset(offset);           
            offset8++;
            if (offset8>7)
            {
                offset8=0;
                update_right_column();
            }
        }
        
        if(Keyboard_IsKeyPressed(KEY_LEFT) && offset>0)
        {
            offset-=1;
            set_offset(offset);           
            offset8--;
            if (offset8<0)
            {
                offset8=7;
                update_left_column();
            }
        }

        if(Keyboard_IsKeyPressed(KEY_D))
        {
            show_debug=!show_debug;
        }
        
        scroll_patterns();               
        
        Halt();
        update_vram_patterns();
        update_vram_names();    
        
        if (show_debug)
        {                        
#ifdef TRACK_TILE_REF_COUNT
            Print_SetPosition(0, 0);

            Print_DrawHex8(tile_allocated_size); 
            Print_DrawChar(' ');
            Print_DrawHex8(g_tile_pairs_size);
/*
            for(u8 i=0;i<g_tile_pairs_size;i++)
            {
                Print_DrawHex8(tile_pairs[i].ref); 
                Print_DrawChar((char)(i+1)); 
                Print_DrawChar(' ');
            }
            Print_DrawChar('*');
            Print_DrawChar('*');
*/            
#endif                         
            
        }
        
   }
}

