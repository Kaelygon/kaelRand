//Pseudo random generator based on LCG with variable state array size
#include "./kaelLCG.h"
#include "./kaelMan.h"

#include <signal.h>

static kaelLCG *kaelLCG_PTR = NULL;
static Options *kaelMan_PTR = NULL;

void handle_sigint(int sig) {
	if (kaelLCG_PTR == NULL) { return; }
	if(kaelMan_PTR->flag_debug){
   		printf("\nSignal %d. Exiting...", sig);
	}
	if(kaelMan_PTR->flag_decimal || kaelMan_PTR->flag_debug){
		printf("\n");
	}
	kaelLCG_free(kaelLCG_PTR);
	exit(0);
}

int main(int argc, char **argv){
	Options opts;
	kaelMan_PTR = &opts;

	signal(SIGINT, handle_sigint);

	if (argc > 1) {
		parse_arguments(argc, argv, &opts);
	} else {
		print_usage();
		return EXIT_FAILURE;
	}

	if(opts.flag_debug){
		printf("flag_decimal     : %d\n"   , opts.flag_decimal    );   
		printf("flag_debug       : %d\n"   , opts.flag_debug      );     
		printf("flag_generate    : %d\n"   , opts.flag_generate   );    
		printf("generate_value   : %llu\n" , opts.generate_value  );     
		printf("flag_seed        : %d\n"   , opts.flag_seed       );	
		printf("seed_value       : %llu\n" , opts.seed_value      );	 
		printf("flag_stateSize   : %d\n"   , opts.flag_stateSize  );     
		printf("stateSize_value  : %llu\n" , opts.stateSize_value );     
	}

	kaelLCG kaelLCG;
	size_t stateArraySize = opts.stateSize_value;
	klcg_t seed = opts.seed_value;

	uint initError;
	initError = kaelLCG_init(&kaelLCG,stateArraySize,seed);

	if(initError!=0){
		if(initError==1000){
			printf("State array can't have size smaller than 2\n");
		}

		kaelLCG_free(&kaelLCG);
		return 0;
	}
	
	kaelLCG_PTR = &kaelLCG;

	klcg_t num;
	unsigned char indef = opts.generate_value==0;
	klcg_t loopEnd = opts.generate_value + indef;

	for(klcg_t i=0; i<loopEnd; i+=!indef ){
		num = kaelLCG_step(&kaelLCG);

		if(opts.flag_decimal){
			printf("%llu ", num); //print decimal
		}else{
			fwrite(&num, sizeof(klcg_t), 1, stdout); //print binary
		}
	}

	if(opts.flag_decimal){
		printf("\n");
	}
	kaelLCG_free(&kaelLCG);
	return 0;
}