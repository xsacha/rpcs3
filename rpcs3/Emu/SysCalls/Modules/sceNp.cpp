#include "stdafx.h"
#include "Emu/SysCalls/SysCalls.h"
#include "Emu/SysCalls/SC_FUNC.h"

#include "sceNp.h"

void sceNp_init();
Module sceNp(0x0016, sceNpTrophy_init);

void sceNpTrophy_init()
{
}
