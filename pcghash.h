/*** pcghash.h -- simple hash map using pcg random numbers
 *
 * Copyright (C) 2014 Sebastian Freundt <devel@fresse.org>
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For additional information about the PCG random number generation scheme,
 * including its license and other licensing options, visit
 *
 *     http://www.pcg-random.org
 ***/
#if !defined INCLUDED_pcghash_h_
#define INCLUDED_pcghash_h_

#include <stdlib.h>
#include <stdint.h>

typedef struct {
	/* state, all values are possible */
	uint64_t state;
	/* controls which rng sequence (stream) is selected, must be odd */
	uint64_t inc;
} pcgr_t;

typedef struct {
	size_t next;
	size_t htz;
	pcgr_t r;
} pcgh_t;


/* public api */
/**
 * Generate hash iterator for PTR in a hash table of size HTZ. */
extern pcgh_t pcgmakehash(uintptr_t ptr, size_t htz);

/**
 * Calculate next slot candidate. */
extern pcgh_t pcghash(pcgh_t);

#endif	/* INCLUDED_pcghash_h_ */
