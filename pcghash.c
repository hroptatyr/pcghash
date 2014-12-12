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
#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#include "pcghash.h"


static size_t pcgz_random_r(pcgr_t rng[static 1U]);

static void
pcgz_srandom_r(pcgr_t r[static 1U], uint64_t initstate, uint64_t initseq)
{
	*r = (pcgr_t){0U, initseq << 1U | 1U};
	pcgz_random_r(r);
	r->state += initstate;
	pcgz_random_r(r);
	return;
}

static size_t
pcgz_random_imm(uint64_t rstate)
{
	size_t xor = ((rstate >> 18U) ^ rstate) >> 27U;
	size_t rot = rstate >> 59U;
	size_t res = (xor >> rot) | (xor << ((-rot) & 0x1fU));
	return res;
}

static size_t
pcgz_random_r(pcgr_t r[static 1U])
{
	size_t res = pcgz_random_imm(r->state);
	/* record new state */
	r->state = r->state * 6364136223846793005ULL + r->inc;
	return res;
}


/* public api */
pcgh_t
pcgmakehash(uintptr_t ptr, size_t htz)
{
	pcgh_t h = {.htz = htz};
	pcgz_srandom_r(&h.r, htz, ptr);
	return pcghash(h);
}

pcgh_t
pcghash(pcgh_t h)
{
	h.next = pcgz_random_r(&h.r) & (h.htz - 1U);
	return h;
}


#if defined STANDALONE
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

int
main(int argc, char *argv[])
{
	uint64_t set = 0ULL;

	for (int i = 1; i < argc; i++) {
		long unsigned int x = strtoul(argv[i], NULL, 0);
		pcgh_t sl;

		printf("h(%lx)", x);
		for (sl = pcgmakehash(x, 64U);
		     (printf(" -> %x", sl.next), set & (1ULL << sl.next));
		     sl = pcghash(sl));
		set |= 1ULL << sl.next;
		putchar('\n');
	}
	return 0;
}
#endif	/* STANDALONE */
