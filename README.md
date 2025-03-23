# MSX1 smooth scroller

## Description

Tech demo to see how far can I get a smooth scroll on the MSX 1. 

As a kid, I hated the blocky scrolling of MSX1 games and envied the Spectrum’s smooth scroll. Now, I’m determined to see if I can push the MSX1 to achieve the smoothness I once admired so much.

There are two online demos:
1. [Precalculated demo](https://webmsx.org/?ROM=https://github.com/aguaviva/msx1_smooth_scroller/raw/refs/heads/main/precompiled/template.rom&MACHINE=MSX1A)
2. [on-the-fly demo](https://webmsx.org/?ROM=https://msxvillage.fr/upload/scroller_0f735.rom)

## Technical notes

### Precalculated demo

- No ASM, just unoptimized C code.
- Fixed point math for the physics
  - 8.8 for the Y axis
  - 24.8 for the X axis 
- A python script takes care of precalculating as much as possible:
  - Compiles the level, replaces each character with it's 16x16 pattern. Some patterns are taken from a png image of a spectrum port.
  - Computes the pairs of adjacent unique tiles
  - Pre-shifts the pairs of tiles 
  - Arranges the data so it can be uploaded fast to the VDP
  - The current demo is not yet saturating the VDP bandwidth
  - Generates sprites, right and back facing versions

TODO
- Update in VRAM only the tiles that show up on the screen. This would save about 50% of the transfers (or would allow a much richer background/colors)
- Race the beam to be able to upload more data (does anybody know how to do it?)
- compress the level using RLE, but then we'd only be able to move forward (as in the real game)
- Experiment with other levels
  - Ghost and Goblins 
  - R-Type
 
## On-the-fly demo

- This version is more limited as it needs to compute which pair or tiles need to be shifted
- Uses a lot of ASM, prob a bad idea as now it is difficult to change/refactor

Screenshot:

![image](https://user-images.githubusercontent.com/5200915/161431555-a4cd9210-c74f-4599-8297-bfc9144fef51.png)

## Notes

Feel free to fork this code to create your own games/demos (and let me know about it so I can link to them from here  ;) )

## How to compile

TODO

## Credits:
- Aoineko, for the great MSXgl
- SMB port for Spectrum, for the inspiration, the tile definitions and the sprites 

