#include "my_kernel_73.h"
#include "common.h"
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"


#define HISTO_ROWS 2
#define HISTO_COL 256
#define TARGET 128
#define START 144
#define END 146


//API REFERENCE for STREAM: 
// https://docs.amd.com/r/ehttps://docs.amd.com/r/en-US/ug1029ai-engine-kernel-coding/Reading-and-Advancing-an-Input-Streamn-US/ug1029ai-engine-kernel-coding/Reading-and-Advancing-an-Input-Stream

void my_kernel_function73 (input_stream<uint8>* restrict input, input_stream<uint8>* restrict input2, output_stream<int>* restrict output)
{ 
    // read from one stream and write to another
    int i, j, k;
    int red = 0, val = 0;
    int joint[HISTO_ROWS][HISTO_COL];
    int hist_x[HISTO_ROWS], hist_y[HISTO_COL];
    int pos_x = 0, pos_y = 0;

    for( i=0; i < HISTO_ROWS; i++ ){
        hist_x[i] = 0;
        for( j = 0; j < HISTO_COL; j++ ){
            joint[i][j] = 0;
            hist_y[i] = 0;
        }
    }

    //aie vectors with zeros and ones
    aie::vector<uint8, TARGET> zeros = aie::zeros<uint8, TARGET>();
    aie::vector<uint8, TARGET> ones = aie::broadcast<uint8, TARGET>(1);

    //masks for comparison
    aie::mask<TARGET> mask_x;
    aie::mask<TARGET> mask_y;
    aie::mask<TARGET> mask;

    
    aie::vector<uint8, TARGET> compare_y = aie::zeros<uint8, TARGET>(); //resetting compare_y to 0
    aie::vector<int, 4> print = aie::zeros<int, 4>();

    for( k=0; k<256*256/TARGET; k++ ){
        aie::vector<uint8, TARGET> x = readincr_v<TARGET>(input);
        aie::vector<uint8, TARGET> y = readincr_v<TARGET>(input2);

        //aie vector to compare floating and reference
        aie::vector<uint8, TARGET> compare_x = aie::broadcast<uint8, TARGET>(START);

        pos_x = 0, val = 0, red = 0;

        compare_y = zeros;

        for( i = START; i < END; i++ ){
            for( j = 0; j < 256; j++ ){
                mask_x = aie::eq( x, compare_x );
                mask_y = aie::eq( y, compare_y );

                mask = mask_x & mask_y ;
        
                red = (int) mask.count();
                val += red;
                
                joint[pos_x][pos_y] += red;
                
                /*writeincr(output, pos_x);
                writeincr(output, pos_y);
                writeincr(output, joint[pos_x][pos_y]);*/
                if(red){
                    print.set(pos_x + START, 0);
                    print.set(pos_y, 1);
                    print.set(joint[pos_x][pos_y], 2);
                    writeincr(output, print);
                }
                
                
                hist_y[pos_y] = (int) mask_y.count();

                if( val == TARGET ){
                    i = 1000;
                    j = 1000;
                }

                compare_y = aie::add( compare_y, ones);   
                pos_y++;
                red = 0;

            }
            hist_x[pos_x] += (int) mask_x.count();
            compare_x = aie::add( compare_x, ones);
            compare_y = zeros;
            pos_x++;
            pos_y = 0;
        }
    }
}
