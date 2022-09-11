#pragma once
#ifndef PREFIX_SUM_H
#define PREFIX_SUM_H
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include "spin_barrier.h"

void* compute_prefix_sum(void* a);
#endif //PREFIX_SUM_H