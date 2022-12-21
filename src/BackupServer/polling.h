#ifndef __POLLING_H__
#define __POLLING_H__

#include "config.h"

void launch_polling(config_t* config, void(* callback)(int iptype, uint64_t address));

#endif
