/***************************************************************************

        Toshiba PASOPIA / PASOPIA7 emulation

        Skeleton driver.

****************************************************************************/

#include "emu.h"
#include "cpu/z80/z80.h"
#include "sound/ay8910.h"
#include "video/mc6845.h"

static UINT8 vram_sel;
static UINT8 *p7_vram;

static VIDEO_START( paso7 )
{
}

static VIDEO_UPDATE( paso7 )
{
	int x,y;
	int count;

	count = 0x10;

	for(y=0;y<25;y++)
	{
		for(x=0;x<40;x++)
		{
			int tile = p7_vram[count];

			drawgfx_opaque(bitmap,cliprect,screen->machine->gfx[0],tile,0,0,0,x*8,y*8);

			count+=8;
		}
	}


    return 0;
}

static READ8_HANDLER( vram_r )
{
	if(vram_sel)
		return 0xff;

	return p7_vram[offset];
}

static WRITE8_HANDLER( vram_w )
{
	if(!vram_sel)
		p7_vram[offset] = data;
}

// sketchy port 0x3c implementation to see what the CPU does...
// however, it writes 0x11 - in theory setting BASIC+BIOS in the lower banks
// and then it writes at 0x0000... maybe bank1 should be RAM? or
// should we have writes to RAM and only reads to BIOS/BASIC?
static WRITE8_HANDLER( paso7_bankswitch )
{
	UINT8 *cpu = memory_region(space->machine, "maincpu");
	UINT8 *basic = memory_region(space->machine, "basic");

	if (BIT(data, 0))
	{
		memory_set_bankptr(space->machine, "bank1", basic);
		memory_set_bankptr(space->machine, "bank2", cpu + 0x10000);
		memory_unmap_write(cputag_get_address_space(space->machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x0000, 0x7fff, 0, 0);
	}
	else if (BIT(data, 1))
	{
		memory_set_bankptr(space->machine, "bank1", cpu);
		memory_set_bankptr(space->machine, "bank2", cpu + 0x4000);
		memory_install_write_bank(cputag_get_address_space(space->machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x0000, 0x3fff, 0, 0, "bank1");
		memory_install_write_bank(cputag_get_address_space(space->machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x4000, 0x7fff, 0, 0, "bank2");
	}
	else
	{
		memory_set_bankptr(space->machine, "bank1", cpu + 0x10000);
		memory_set_bankptr(space->machine, "bank2", cpu + 0x10000);
		memory_unmap_write(cputag_get_address_space(space->machine, "maincpu", ADDRESS_SPACE_PROGRAM), 0x0000, 0x7fff, 0, 0);
	}

	vram_sel = data & 4;

	// bit 3? PIO2 port C

	// bank4 is always RAM
}

static READ8_HANDLER( test_r )
{
	return mame_rand(space->machine);
}


#if 0
static WRITE8_HANDLER( test_w )
{
	printf("%02x %02x\n",offset,data);
}
#endif

static READ8_HANDLER( test2_r )
{
	return 0xff;
}

static ADDRESS_MAP_START(paso7_mem, ADDRESS_SPACE_PROGRAM, 8)
	ADDRESS_MAP_UNMAP_HIGH
	AM_RANGE( 0x0000, 0x3fff ) AM_RAMBANK("bank1")
	AM_RANGE( 0x4000, 0x7fff ) AM_RAMBANK("bank2")
	AM_RANGE( 0x8000, 0xbfff ) AM_READWRITE(vram_r, vram_w ) AM_BASE(&p7_vram)
	AM_RANGE( 0xc000, 0xffff ) AM_RAMBANK("bank4")
ADDRESS_MAP_END

static ADDRESS_MAP_START( paso7_io , ADDRESS_SPACE_IO, 8)
	ADDRESS_MAP_UNMAP_HIGH
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE( 0x09, 0x09 ) AM_READ(test_r)
	AM_RANGE( 0x10, 0x10 ) AM_DEVWRITE("crtc", mc6845_address_w)
	AM_RANGE( 0x11, 0x11 ) AM_DEVWRITE("crtc", mc6845_register_w)
	AM_RANGE( 0x22, 0x22 ) AM_READ(test2_r) AM_WRITENOP
	AM_RANGE( 0x3c, 0x3c ) AM_WRITE(paso7_bankswitch)
//	AM_RANGE( 0x08, 0x0b )  // PIO0
//	AM_RANGE( 0x0c, 0x0f )  // PIO1
//	AM_RANGE( 0x20, 0x23 )  // PIO2
//	AM_RANGE( 0x28, 0x2b )  // CTC
//	AM_RANGE( 0x3a, 0x3a )  // PSG0
//	AM_RANGE( 0x3b, 0x3b )  // PSG1
//	AM_RANGE( 0xe0, 0xe6 )  // FLOPPY
ADDRESS_MAP_END

/* Input ports */
static INPUT_PORTS_START( paso7 )
INPUT_PORTS_END

static MACHINE_RESET( paso7 )
{
}

static const gfx_layout p7_chars_8x8 =
{
	8,8,
	RGN_FRAC(1,1),
	1,
	{ 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	8*8
};

static const gfx_layout p7_chars_16x16 =
{
	16,16,
	RGN_FRAC(1,1),
	1,
	{ 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
	{ 0*16, 1*16, 2*16, 3*16, 4*16, 5*16, 6*16, 7*16,8*16, 9*16, 10*16, 11*16, 12*16, 13*16, 14*16, 15*16 },
	16*16
};

static GFXDECODE_START( pasopia7 )
	GFXDECODE_ENTRY( "font",   0x00000, p7_chars_8x8,    0, 1 )
	GFXDECODE_ENTRY( "kanji",  0x00000, p7_chars_16x16,  0, 1 )
GFXDECODE_END

static const mc6845_interface mc6845_intf =
{
	"screen",	/* screen we are acting on */
	8,			/* number of pixels per video memory address */
	NULL,		/* before pixel update callback */
	NULL,		/* row update callback */
	NULL,		/* after pixel update callback */
	DEVCB_NULL,	/* callback for display state changes */
	DEVCB_NULL,	/* callback for cursor state changes */
	DEVCB_NULL,	/* HSYNC callback */
	DEVCB_NULL,	/* VSYNC callback */
	NULL		/* update address callback */
};

static MACHINE_DRIVER_START( paso7 )
    /* basic machine hardware */
    MDRV_CPU_ADD("maincpu",Z80, XTAL_4MHz)
    MDRV_CPU_PROGRAM_MAP(paso7_mem)
    MDRV_CPU_IO_MAP(paso7_io)
//	MDRV_CPU_VBLANK_INT("screen", irq0_line_hold)

    MDRV_MACHINE_RESET(paso7)

    /* video hardware */
    MDRV_SCREEN_ADD("screen", RASTER)
    MDRV_SCREEN_REFRESH_RATE(60)
    MDRV_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(2500)) /* not accurate */
    MDRV_SCREEN_FORMAT(BITMAP_FORMAT_INDEXED16)
    MDRV_SCREEN_SIZE(640, 480)
    MDRV_SCREEN_VISIBLE_AREA(0, 640-1, 0, 480-1)
	MDRV_MC6845_ADD("crtc", H46505, XTAL_3_579545MHz/4, mc6845_intf)	/* unknown clock, hand tuned to get ~60 fps */
    MDRV_PALETTE_LENGTH(2)
    MDRV_PALETTE_INIT(black_and_white)

	MDRV_GFXDECODE( pasopia7 )

    MDRV_VIDEO_START(paso7)
    MDRV_VIDEO_UPDATE(paso7)
MACHINE_DRIVER_END

/* ROM definition */
ROM_START( pasopia7 )
	ROM_REGION( 0x14000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD( "bios.rom", 0x10000, 0x4000, CRC(b8111407) SHA1(ac93ae62db4c67de815f45de98c79cfa1313857d))

	ROM_REGION( 0x8000, "basic", ROMREGION_ERASEFF )
	ROM_LOAD( "basic.rom", 0x0000, 0x8000, CRC(8a58fab6) SHA1(5e1a91dfb293bca5cf145b0a0c63217f04003ed1))

	ROM_REGION( 0x800, "font", ROMREGION_ERASEFF )
	ROM_LOAD( "font.rom", 0x0000, 0x0800, CRC(a91c45a9) SHA1(a472adf791b9bac3dfa6437662e1a9e94a88b412))

	ROM_REGION( 0x20000, "kanji", ROMREGION_ERASEFF )
	ROM_LOAD( "kanji.rom", 0x0000, 0x20000, CRC(6109e308) SHA1(5c21cf1f241ef1fa0b41009ea41e81771729785f))
ROM_END

static DRIVER_INIT( paso7 )
{
	UINT8 *bios = memory_region(machine, "maincpu");
	const address_space *space = cputag_get_address_space(machine, "maincpu", ADDRESS_SPACE_PROGRAM);

	memory_unmap_write(space, 0x0000, 0x7fff, 0, 0);
	memory_set_bankptr(machine, "bank1", bios + 0x10000);
	memory_set_bankptr(machine, "bank2", bios + 0x10000);
//	memory_set_bankptr(machine, "bank3", bios + 0x10000);
//	memory_set_bankptr(machine, "bank4", bios + 0x10000);
}

/* Driver */

/*    YEAR  NAME    PARENT  COMPAT   MACHINE    INPUT    INIT     COMPANY   FULLNAME       FLAGS */
COMP( 19??, pasopia7,  0,       0, 	 paso7,	paso7,   paso7,   "Toshiba",   "PASOPIA 7", GAME_NOT_WORKING | GAME_NO_SOUND )
