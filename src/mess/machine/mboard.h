﻿
/**********************************************************************

     Mephisto Chess Computers

**********************************************************************/

#ifndef __MBOARD_H__
#define __MBOARD_H__

#include "emu.h"


/***************************************************************************
    MACROS
***************************************************************************/

enum
{
	EM,		/*No piece*/
	BP,
	BN,
	BB,
	BR,
	BQ,
	BK,
	WP,
	WN,
	WB,
	WR,
	WQ,
	WK
};

#define NOT_VALID		99
#define BOARDER_PIECE	64

#define IsPiece(x)		((m_board[x] >=1) && (m_board[x] <=12))

#define IsBitSet(x,y)	( y & (1<<x) )  //ersetzen durch BIT

/***************************************************************************
    TYPE DEFINITIONS
***************************************************************************/

typedef struct {
	int border_piece;
	UINT8 from;
	UINT8 piece;
} MOUSE_HOLD;

/***************************************************************************
    FUNCTION PROTOTYPES
***************************************************************************/

READ8_HANDLER( read_board_8 );
WRITE8_HANDLER( write_board_8 );
WRITE8_HANDLER( write_LED_8 );

READ16_HANDLER( read_board_16 );
WRITE16_HANDLER( write_board_16 );
WRITE16_HANDLER( write_LED_16 );

READ32_HANDLER( read_board_32 );
WRITE32_HANDLER( write_board_32 );
WRITE32_HANDLER( write_LED_32 );

TIMER_CALLBACK( update_artwork );

STATE_PRESAVE( m_board_presave );
STATE_POSTLOAD( m_board_postload );

void mboard_savestate_register(running_machine *machine);

void set_board( void );
void set_artwork (running_machine *machine );
void check_board_buttons(running_machine *machine );
void set_boarder_pieces (void);

INLINE UINT8 pos_to_num(UINT8 val)
{
	switch (val)
	{
		case 0xfe: return 7;
		case 0xfd: return 6;
		case 0xfb: return 5;
		case 0xf7: return 4;
		case 0xef: return 3;
		case 0xdf: return 2;
		case 0xbf: return 1;
		case 0x7f: return 0;
		default: return 0xff;
	}
}

/***************************************************************************
    GLOBALS
***************************************************************************/

extern UINT8 lcd_invert;
extern UINT8 key_select;
extern UINT8 key_selector;

#endif /* __MBOARD_H__ */
