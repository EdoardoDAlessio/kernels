#include "my_kernel_5.h"
#include "common.h"
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"



#define HISTO_ROWS 32
#define HISTO_COL 256
#define TARGET 128
#define START 128
#define END 160
#define LEN 8
#define IMM_SIZE 512

//API REFERENCE for STREAM: 
// https://docs.amd.com/r/ehttps://docs.amd.com/r/en-US/ug1029ai-engine-kernel-coding/Reading-and-Advancing-an-Input-Streamn-US/ug1029ai-engine-kernel-coding/Reading-and-Advancing-an-Input-Stream

void my_kernel_function5 (input_stream<uint8>* restrict input, input_stream<uint8>* restrict input2, output_stream<int>* restrict output)
{
    // read from one stream and write to another
    int i, j, k, hist=0;
    int red = 0, val = 0, print_on=0, stop = -1, finish_aie = -101;

    
    aie::vector<uint8, TARGET> x1 = readincr_v<TARGET>(input);
    int size = x1.get(0);

    //kprintf("2IMM_SIZE:%d, size:%d\n", IMM_SIZE, IMM_SIZE*size);
    writeincr( output, IMM_SIZE*(size+1) );
    long int max = 1 + ( HISTO_ROWS + HISTO_COL + HISTO_ROWS*HISTO_COL )*(IMM_SIZE*(size+1)/TARGET), pos=1 ; //+1 -> size 
    //kprintf("2Max:%ld\n", max);

    //aie vectors with zeros and ones
    aie::vector<uint8, TARGET> zeros = aie::zeros<uint8, TARGET>();
    aie::vector<uint8, TARGET> ones = aie::broadcast<uint8, TARGET>(1);

    //masks for comparison
    aie::mask<TARGET> mask_x1;
    aie::mask<TARGET> mask_x2;
    aie::mask<TARGET> mask_x3;
    aie::mask<TARGET> mask_x4;
    aie::mask<TARGET> mask_y1;
    aie::mask<TARGET> mask_y2;
    aie::mask<TARGET> mask_y3;
    aie::mask<TARGET> mask_y4;
    aie::mask<TARGET> mask1;
    aie::mask<TARGET> mask2;
    aie::mask<TARGET> mask3;
    aie::mask<TARGET> mask4;

    
    aie::vector<uint8, TARGET> compare_y = aie::zeros<uint8, TARGET>(); //resetting compare_y to 0
    aie::vector<int, LEN> print = aie::zeros<int, LEN>();
    aie::vector<int, LEN> print_zeros = aie::zeros<int, LEN>();
    //writeincr( output, 1200 );

    for( k=0; k<IMM_SIZE*(size+1)/TARGET; k++ ){
        aie::vector<uint8, TARGET> x1 = readincr_v<TARGET>(input);
        aie::vector<uint8, TARGET> x2 = readincr_v<TARGET>(input);
        aie::vector<uint8, TARGET> x5 = readincr_v<TARGET>(input);
        aie::vector<uint8, TARGET> x4 = readincr_v<TARGET>(input);
        aie::vector<uint8, TARGET> y1 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y2 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y5 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y4 = readincr_v<TARGET>(input2);
        //writeincr( output, 55555 );
        //aie vector to compare floating and reference
        aie::vector<uint8, TARGET> compare_x = aie::broadcast<uint8, TARGET>(START);
        val = 0, red = 0; 
        compare_y = zeros;
        
        for( i=START; i < END; i++ ){ //i is not reinizilized so that if i exit on VAL == TARGET i dont reenter
            for( j = 0; j < 256; j++ ){
                //writeincr( output, 2123123125 );
                mask_x1 = aie::eq( x1, compare_x );
                mask_y1 = aie::eq( y1, compare_y );
                mask1 = mask_x1 & mask_y1 ;
                mask_x2 = aie::eq( x2, compare_x );
                mask_y2 = aie::eq( y2, compare_y );
                mask2 = mask_x2 & mask_y2 ;
                mask_x5 = aie::eq( x3, compare_x );
                mask_y5 = aie::eq( y3, compare_y );
                mask5 = mask_x5 & mask_y5 ;
                mask_x4 = aie::eq( x4, compare_x );
                mask_y4 = aie::eq( y4, compare_y );
                mask4 = mask_x4 & mask_y4 ;

                red = (int) mask1.count() + (int) mask2.count() + (int) mask5.count() + (int) mask4.count();
                val += red;
                
                if( red ){
                    print.set(i, print_on);
                    print.set(j, print_on+1);
                    print.set(red, print_on+2);
                    print.set( 0, print_on+3);
                    print_on+=4;
                }
                /*
                print.set(pos_x+pos_y, print_on);
                print_on++;
                print.set(pos_y, print_on);
                print_on++;*/
                //if( red ){
                    ////kprintf("fValue:%d, i:%d, j:%d, k:%d\n", red, i, j, k);
                //}

                if( print_on == LEN ){//IT'S TIME TO PRINT THE JOINT
                    print_on = 0;
                    writeincr(output, print);
                    pos += LEN;
                }

                if( val == TARGET*4 ){
                    i = 1000;
                    j = 1000;
                    if( print_on == 0 ){//we already flushed the values
                        writeincr(output, stop );
                        pos++;
                    }else{//we don't have a full vector and we need to full it and flush it 
                        print.set( stop, print_on );
                        print_on++;
                        while( print_on < LEN ){
                            print.set( stop, print_on );
                            print_on++;
                        }
                        writeincr(output, print );
                        pos += LEN;
                        print_on = 0;
                    }
                }
            

                compare_y = aie::add( compare_y, ones);   
                red = 0; 
            }
            compare_x = aie::add( compare_x, ones);
            compare_y = zeros;

            //at the end i print in output the marginal histogram
            hist = (int) mask_x1.count() + (int) mask_x2.count() + (int) mask_x5.count() + (int) mask_x4.count();
            if( hist ){
                writeincr( output, -2);
                writeincr( output, hist );
            }
        }
    }
    writeincr( output, finish_aie );
}
