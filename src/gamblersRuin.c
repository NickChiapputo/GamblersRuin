#include <gsl/gsl_randist.h>				// Used for random number distributions
#include <stdio.h>							// Used for input/output					
#include <time.h>							// Used to keep track of elapsed time

// Used only to check if directories exist
#include <dirent.h>
#include <errno.h>


// Define starting parameters of simulation
#define ALICE_STARTING 			5		// Starting money for Alice
#define BOB_STARTING			5		// Starting money for Bob
#define ALICE_WIN_PROBABILITY 	0.5		// Probability that Alice wins a draw
#define BET_AMOUNT				1 		// Amount of money each person bets at each round
#define NUM_GAMES 				10000000	// Number of games to play until a player loses


// Define color commands
#define RESET 					"\x1b[0m"
#define BOLD					"\x1b[1m"
#define F_RED					"\x1b[31m"
#define F_GREEN					"\x1b[32m"

int main()
{
	// Display game parameters
	printf( "Game Parameters:\n"
			"    Alice Starting: $%i\n"
			"    Bob Starting:   $%i\n"
			"    Alice Win Probability: %0.2f\n"
			"    Bet Amount: $%i\n"
			"    Number of Games: %i\n\n", 
			ALICE_STARTING, BOB_STARTING, ALICE_WIN_PROBABILITY, BET_AMOUNT, NUM_GAMES );


	// Get seed for random value. Only updates once per second and is a PRNG
	srand( time( NULL ) );

	// Set up random number generator from GNU Science Library (GSL)
	const gsl_rng_type *T;
	gsl_rng *r;

	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc( T );


	// Set amount of money Alice and Bob have
	int moneyA = ALICE_STARTING;
	int moneyB = BOB_STARTING;

	// Stores the pool for each round
	int pool;

	// Stores the outcome of the random draw at each round
	double outcome;

	// Track average number of rounds it takes for one person to win
	double averageRounds = 0;

	// Track number of wins and losses for each person
	int aliceWin = 0, aliceLose = 0;
	int bobWin = 0,   bobLose = 0;


	int i;
	for( i = 0; i < NUM_GAMES; i++ )
	{
		moneyA = ALICE_STARTING;
		moneyB = BOB_STARTING;

		// Count how many times it takes for one person to win
		int round = 0;


		// Play game until one player has no money left
		while( moneyA != 0 && moneyB != 0 )
		{
			round++;


			// Store starting states
			int moneyA_start = moneyA;
			int moneyB_start = moneyB;


			// Calculate bet amount. If both players have more than the default amount, then use default value.
			// Otherwise, use the amount of the player who has the least money remaining.
			int betAmount = ( moneyA >= BET_AMOUNT && moneyB >= BET_AMOUNT ?  BET_AMOUNT : moneyA < moneyB ? moneyA : moneyB );
			

			// Take money from players and put it in the pool
			moneyA = moneyA - betAmount;
			moneyB = moneyB - betAmount;
			pool = 2 * betAmount;


			// Randomly draw a value from [0, 1)
			outcome = gsl_ran_flat( r, 0, 1);


			// Give money to the winner
			moneyA = moneyA + ( outcome <= ALICE_WIN_PROBABILITY ? pool : 0 );
			moneyB = moneyB + ( outcome <= ALICE_WIN_PROBABILITY ? 0 	: pool );

		/*
			// Display round results
			printf( "Round %i (%i<%i):\n"
					"    Alice: " BOLD F_GREEN "$%i\n" RESET
					"    Bob:   " BOLD F_GREEN "$%i\n" RESET
					"    Pool:  " BOLD F_GREEN "$%i\n" RESET
					"\n"
					"    Results:\n"
					"        Outcome: %f\n"
					"        Alice %s%s" RESET "\n"
					"        Alice: " BOLD F_GREEN "$%i\n" RESET
					"        Bob:   " BOLD F_GREEN "$%i\n" RESET,
					round, i, NUM_GAMES, moneyA_start, moneyB_start, pool, outcome, ( outcome <= ALICE_WIN_PROBABILITY ? F_GREEN : F_RED ), ( outcome <= ALICE_WIN_PROBABILITY ? "wins" : "loses" ), moneyA, moneyB );
		*/
		}

		averageRounds += round;

		if( moneyA )
			aliceWin++;
		else
			bobWin++;
	}

	averageRounds /= NUM_GAMES;

	printf( "Average number of rounds needed: %.3f.\n"
			"Results:\n"
			"    " BOLD "%sAlice" RESET ":  %i (%.4f%%)\n"
			"    " BOLD "%sBob" RESET ":    %i (%.4f%%)\n",
			averageRounds, 
			aliceWin < bobWin ? F_RED : F_GREEN,
			aliceWin, 100 * (float) aliceWin / ( aliceWin + bobWin ), 
			bobWin < aliceWin ? F_RED : F_GREEN,
			bobWin, 100 * (float) bobWin / ( bobWin + aliceWin ) );
} 
