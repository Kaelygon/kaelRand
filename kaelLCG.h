//kaelLCG.h
//rotate right linear congruential generator
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned long long klcg_t;

#define R_BITS (sizeof(klcg_t) * CHAR_BIT)

typedef struct {
	klcg_t *current;
	klcg_t *state;
	klcg_t salt;
	size_t size;
} kaelLCG;

//LCG constants addend, multiplier
const klcg_t kaelLCG_const[] = { 
	(klcg_t)3343, 
	(klcg_t)11770513, 
	(klcg_t)47, 
	(klcg_t)7 
};

const klcg_t kaelLCG_shift[] = { 
	(R_BITS * 12 / 64) | 1,
	(R_BITS * 24 / 64) | 1,
	(R_BITS * 36 / 64) | 1,
	(R_BITS * 48 / 64) | 1,
};

//rotate right
klcg_t kaelLCG_rotr(klcg_t n, size_t shift) {
	size_t invShift = R_BITS - shift;
	return (n >> shift) | (n << invShift);
}

//rotate pair
void kaelLCG_rotrPair(klcg_t *n, klcg_t *m, size_t shift ){
	size_t invShift = R_BITS - shift;
	klcg_t o = *m;
	*m = (*n<<invShift	) | (o>>shift		);
	*n = (*n>>shift		) | (o<<invShift	);
}

//Iterate given pointer by rotate right LCG
klcg_t kaelLCG_rand(klcg_t n) {
	n = kaelLCG_rotr( n, kaelLCG_shift[1] ) * kaelLCG_const[0] + kaelLCG_const[1];
	return n;
}

klcg_t kaelLCG_step(kaelLCG *kaelLCG) {
	klcg_t index[2];
	klcg_t *value[2];

	//choose 2 random elements from state array. index[0] != index[1]
	index[0] =  (*kaelLCG->current >> kaelLCG_shift[1]) % kaelLCG->size;
	index[1] = ((*kaelLCG->current >> kaelLCG_shift[2]) % (kaelLCG->size-1) + index[0] + 1 ) % kaelLCG->size ; //calculated by offsetting index[0] but is never the same

	value[0] = &kaelLCG->state[index[0]]; 
	value[1] = &kaelLCG->state[index[1]]; 

	//mix the 2 elements and randomize
	kaelLCG_rotrPair(value[0],value[1], kaelLCG->salt % (R_BITS-1) );
	*value[0] = kaelLCG_rand( *value[0] ) ^ kaelLCG->salt;

	//update current pointer
	kaelLCG->current = value[0];
	kaelLCG->salt=kaelLCG_rotr(kaelLCG->salt,kaelLCG_shift[3]) * kaelLCG_const[2] + kaelLCG_const[3];
	return *kaelLCG->current;
}

uint kaelLCG_init(kaelLCG *kaelLCG, size_t stateSize, klcg_t seed) {
	kaelLCG->size = stateSize;
	if(kaelLCG->size<2){ 
		return 1000;
	}

	kaelLCG->state = (klcg_t *)malloc(sizeof(klcg_t) * kaelLCG->size);

	//seeding
	kaelLCG->state[0]=seed;
	kaelLCG->state[0]^=seed<<kaelLCG_shift[3];
	kaelLCG->state[0]^=seed>>kaelLCG_shift[2];
	kaelLCG->state[0]^=seed<<kaelLCG_shift[1];
	kaelLCG->state[0]^=seed>>kaelLCG_shift[0];

	for(size_t i=1; i<kaelLCG->size; i++ ){
		kaelLCG->state[i] = kaelLCG_rand(kaelLCG->state[i-1]);
	}

	kaelLCG->salt=81;
	kaelLCG->current=&kaelLCG->state[0];

	return 0;
}

void kaelLCG_free(kaelLCG *kaelLCG){
	free(kaelLCG->state);
	kaelLCG->current=NULL;
	kaelLCG=NULL;
}