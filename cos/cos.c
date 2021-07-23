#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "unity.h"
#include "unity_fixture.h"

#define N_RAND_CHECKS 		1000
#define N_PERIODS_TO_CHECK	1000

#define TOLERANCE	0.000001
#define DX		0.001

#define RAND_NUM_SEED 10344 // some value ... can be passed as a parameter in future

//#define VERBOSE

TEST_GROUP(test_cos);

TEST_SETUP(test_cos){

    srand48(RAND_NUM_SEED); //can be passed as a parameter in future
}

TEST_TEAR_DOWN(test_cos){ }


TEST(test_cos, symmetry__arg_random_in_the_first_N_periods){
//cos(x) is a symmetric function which means that for every argument x: cos(x) must be equal to cos(-x)
//it should be true for every CPU architecture, in case of some tiny value rounds, in this case, it doesn't matter
//left and right "wing" of cos must be the same, any possible rounds at left and right "wing" must be the same

// arg x is randomly selected in a range (-2*M_PI : 2*M_PI)

    int i;
    double x_rand, val_left, val_right;
    char str_error_msg[512]; //be careful not to exceed the size

#ifdef VERBOSE
fprintf(stderr, "N_RANDOM_CHECKS: %d\n", N_RAND_CHECKS);
fprintf(stderr, "symmetry__arg_rand_in_the_first_period: selection range: (%lf : %lf)\n", -2*M_PI, 2*M_PI);
#endif

    for(i=0; i<N_RAND_CHECKS; i++){

	x_rand = 2*M_PI*drand48(); //x_rand various from 0 to 2*M_PI --- with uniform distribution
	x_rand *= (double)N_PERIODS_TO_CHECK;//x_rand various from 0 to 2*M_PI*N_PERIODS_TO_CHECK --- with uniform distribution
	val_left  = cos(-x_rand);
	val_right = cos( x_rand);

#ifdef VERBOSE
fprintf(stderr, "x_rand: %lf, cos(-x_rand): %lf, cos(x_rand): %lf\n", x_rand, cos(-x_rand), cos(x_rand));
#endif

	if(val_left != val_right){
	    sprintf(str_error_msg, "SYMMETRY test fail: cos(%lf) != cos(%lf) ->> %lf != %lf", -x_rand, x_rand, val_left, val_right);
	    FAIL(str_error_msg);
	}
    }
}

TEST(test_cos, symmetry__arg_random_wide_range_selection){
// this function is almost the same as the one above, it tests symmetry in a whole argumet domain
// the range of selection x_rand could be passed as a parameter

// I coded all the testing function as independent, even if majority of the code is the same


//cos(x) is a symmetric function which means that for every argument x: cos(x) must be equal to cos(-x)
//it should be true for every CPU architecture, in case of some tiny value rounds, in this case, it doesn't matter
//left and right "wing" of cos must be the same, any possible rounds at left and right "wing" must be the same

// arg x is randomly selected in a range (-DBL_MAX : DBL_MAX)

    int i;
    double x_rand, val_left, val_right;
    char str_error_msg[1024]; //be careful not to exceed the size

#ifdef VERBOSE
fprintf(stderr, "symmetry__arg_rand_wide_range_selection: selection range: (%lf : %lf)\n", -DBL_MAX, DBL_MAX);
fprintf(stderr, "N_RANDOM_CHECKS: %d\n", N_RAND_CHECKS);
#endif

    for(i=0; i<N_RAND_CHECKS; i++){

	x_rand = DBL_MAX*drand48();  //x_rand various from 0 to 2*BDL_MAX --- with uniform distribution
	val_left  = cos(-x_rand);
	val_right = cos( x_rand);

#ifdef VERBOSE
fprintf(stderr, "x_rand: %lf, cos(-x_rand): %lf, cos(x_rand): %lf\n", x_rand, cos(-x_rand), cos(x_rand));
#endif

	if(val_left != val_right){
	    sprintf(str_error_msg, "SYMMETRY test fail: cos(%lf) != cos(%lf) ->> %lf != %lf", -x_rand, x_rand, val_left, val_right);
	    FAIL(str_error_msg);
	}
    }

}

TEST(test_cos, zeros_and_min_max_values_in_N_periods){

    int i;
    double val_arg;
    double val_cos;

    char str_error_msg[512]; //be careful not to exceed the size

    for(i=0; i<N_PERIODS_TO_CHECK; i++){ //testing only positive args (before symmetry was tested)

	val_arg  = 2*M_PI*(double)i;
	val_cos  = cos( val_arg ); //val_expected is: 1.0
	if( val_cos < 1.0-TOLERANCE || val_cos > 1.0+TOLERANCE ){
	    sprintf(str_error_msg, "error: cos(%lf) is: %lf, expected value is: 1.0 ---> val_arg: %lf should correspond to 2*PI*%d", val_arg, val_cos, val_arg, i);
	    FAIL(str_error_msg);
	}

	val_arg  = 2*M_PI*(double)i + M_PI/2;
	val_cos  = cos( val_arg ); //val_expected is: 0.0
	if( val_cos < 0.0-TOLERANCE || val_cos > 0.0+TOLERANCE ){
	    sprintf(str_error_msg, "error: cos(%lf) is: %lf, expected value is: 0.0 ---> val_arg: %lf should correspond to 2*PI*%d + PI/2", val_arg, val_cos, val_arg, i);
	    FAIL(str_error_msg);
	}


	val_arg  = 2*M_PI*(double)i + M_PI;
	val_cos  = cos( val_arg ); //val_expected is: -1.0
	if( val_cos < -1.0-TOLERANCE || val_cos > -1.0+TOLERANCE){
	    sprintf(str_error_msg, "error: cos(%lf) is: %lf, expected value is: -1.0 ---> val_arg: %lf should correspond to 2*PI*%d + PI", val_arg, val_cos, val_arg, i);
	    FAIL(str_error_msg);
	}

	val_arg  = 2*M_PI*(double)i + 3*M_PI/2;
	val_cos  = cos( val_arg ); //val_expected is: 0.0
	if( val_cos < 0.0-TOLERANCE || val_cos > 0.0+TOLERANCE ){
	    sprintf(str_error_msg, "error: cos(%lf) is: %lf, expected value is: 0.0 ---> val_arg: %lf should correspond to 2*PI*%d + PI/2", val_arg, val_cos, val_arg, i);
	    FAIL(str_error_msg);
	}
    }
}

TEST(test_cos, second_derivative__in_N_periods_random){

    int i;
    double x_rand;
    double val_y1, val_y2, val_y3;
    double val_sec_deriv;

    char str_error_msg[512]; //be careful not to exceed the size

    for(i=0; i<N_RAND_CHECKS; i++){

	x_rand = 2*M_PI*drand48(); //x_rand various from 0 to 2*M_PI --- with uniform distribution
	x_rand *= (double)N_PERIODS_TO_CHECK;//x_rand various from 0 to 2*M_PI*N_PERIODS_TO_CHECK --- with uniform distribution

	val_y1 = cos( x_rand );
	val_y2 = cos( x_rand + DX );
	val_y3 = cos( x_rand + 2*DX );

	val_sec_deriv = (val_y1 - 2*val_y2 + val_y3)/(DX*DX);

//	fprintf(stderr, "val1: %20.17lf val2: %20.17lf val3: %20.17lf deriv: %20.17lf\n", val_y1, val_y2, val_y3, val_sec_deriv);

	if( -val_sec_deriv < val_y2-TOLERANCE || -val_sec_deriv > val_y2+TOLERANCE ){ //second derivative of cos(x0) should be -cos(x0)
	    sprintf(str_error_msg, "second derivative of cos(x) in every point should correspond to -cos(x): in current sample: %10.7lf != %10.7lf", val_y2, -val_sec_deriv);
	    FAIL(str_error_msg);
	}
    }
}


TEST_GROUP_RUNNER(test_cos)
{
// there is some redundancy in the tests
// if a funtion is non-zero (in proper range), and is symmetric: cos(-x)==cos(x), and the second derivative is -cos(x): (d2f/dx)(cos(x)) == -cos(x)
// in such case the function must be cos(x), no other option - no other function complying those conditions

	RUN_TEST_CASE(test_cos, symmetry__arg_random_in_the_first_N_periods);
	RUN_TEST_CASE(test_cos, symmetry__arg_random_wide_range_selection);

	RUN_TEST_CASE(test_cos, zeros_and_min_max_values_in_N_periods);

	RUN_TEST_CASE(test_cos, second_derivative__in_N_periods_random);
}

void runner(void)
{
	RUN_TEST_GROUP(test_cos);
}

int main(int argc, char *argv[])
{
	UnityMain(argc, (const char **)argv, runner);
	return 0;
}
