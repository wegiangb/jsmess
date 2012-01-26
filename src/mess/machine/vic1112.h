/**********************************************************************

    Commodore VIC-1112 IEEE-488 Interface Cartridge emulation

    SYS 45065 to start

    Copyright MESS Team.
    Visit http://mamedev.org for licensing and usage restrictions.

**********************************************************************/

#pragma once

#ifndef __VIC1112__
#define __VIC1112__

#define ADDRESS_MAP_MODERN

#include "emu.h"
#include "cpu/m6502/m6502.h"
#include "machine/6522via.h"
#include "machine/cbmipt.h"
#include "machine/ieee488.h"
#include "machine/vic20exp.h"



//**************************************************************************
//  MACROS / CONSTANTS
//**************************************************************************

#define VIC1112_TAG	"vic1112"



//**************************************************************************
//  INTERFACE CONFIGURATION MACROS
//**************************************************************************

#define MCFG_VIC1112_ADD() \
    MCFG_DEVICE_ADD(VIC1112_TAG, VIC1112, 0)



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> vic1112_device

class vic1112_device :  public device_t,
						public device_vic20_expansion_card_interface
{
public:
    // construction/destruction
    vic1112_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);

	// optional information overrides
	virtual const rom_entry *device_rom_region() const;
	virtual machine_config_constructor device_mconfig_additions() const;

	// not really public
	DECLARE_WRITE_LINE_MEMBER( via0_irq_w );
	DECLARE_READ8_MEMBER( via0_pb_r );
	DECLARE_WRITE8_MEMBER( via0_pb_w );
	DECLARE_WRITE_LINE_MEMBER( via1_irq_w );
	
protected:
    // device-level overrides
    virtual void device_start();
	virtual void device_reset();
    virtual void device_config_complete() { m_shortname = "vic1112"; }
	
	// device_vic20_expansion_card_interface overrides
	virtual UINT8 vic20_io2_r(address_space &space, offs_t offset);
	virtual void vic20_io2_w(address_space &space, offs_t offset, UINT8 data);
	virtual UINT8 vic20_blk5_r(address_space &space, offs_t offset);

private:
	required_device<via6522_device> m_via0;
	required_device<via6522_device> m_via1;
	required_device<ieee488_device> m_bus;

	UINT8 *m_rom;

	int m_via0_irq;
	int m_via1_irq;
};


// device type definition
extern const device_type VIC1112;



#endif
