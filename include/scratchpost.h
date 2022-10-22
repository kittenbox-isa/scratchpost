#pragma once
#define SCRATCHPOST_H

#define NOVALUE 0xFFFFFFFF

#define STACK_TRACE_LIMIT 25

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <execinfo.h>
#include <signal.h>
#include <errno.h>

#include <SDL2/SDL.h>

#include "init/init_header.h"
#include "utils/utils_header.h"
#include "core/core_header.h"
#include "mmio/mmio_header.h"