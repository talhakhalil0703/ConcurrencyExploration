#pragma once

#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include "spin_barrier.h"

void* compute_prefix_sum(void* a);
