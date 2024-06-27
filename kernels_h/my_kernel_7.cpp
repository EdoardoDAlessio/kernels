#include "my_kernel_7.h"
#include "common.h"
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"



#define HISTO_ROWS 32
#define HISTO_COL 256
#define TARGET 128
#define START 192
#define END 224
#define LEN 8
#define IMM_SIZE 512
#define NUM_INPUT 16

//API REFERENCE for STREAM: 
// https://docs.amd.com/r/ehttps://docs.amd.com/r/en-US/ug1029ai-engine-kernel-coding/Reading-and-Advancing-an-Input-Streamn-US/ug1029ai-engine-kernel-coding/Reading-and-Advancing-an-Input-Stream

void my_kernel_function7(input_stream<uint8>* restrict input, input_stream<uint8>* restrict input2, output_stream<int>* restrict output)
{// read from one stream and write to another
    int i, j, k, hist=0, values=0;
    int red = 0, val = 0, print_on=0, stop = -1000000, finish_aie = -12345678;

    
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
    aie::mask<TARGET> mask_x1, mask_x2, mask_x3, mask_x4, mask_x5, mask_x6, mask_x7, mask_x8, mask_x9, mask_x10, mask_x11, mask_x12, mask_x13, mask_x14, mask_x15, mask_x16;
    aie::mask<TARGET> mask_y1, mask_y2, mask_y3, mask_y4, mask_y5, mask_y6, mask_y7, mask_y8, mask_y9, mask_y10, mask_y11, mask_y12, mask_y13, mask_y14, mask_y15, mask_y16;
    aie::mask<TARGET> mask1, mask2, mask3, mask4, mask5, mask6, mask7, mask8, mask9, mask10, mask11, mask12, mask13, mask14, mask15, mask16;


    
    aie::vector<uint8, TARGET> compare_y = aie::zeros<uint8, TARGET>(); //resetting compare_y to 0
    aie::vector<int, LEN> print = aie::zeros<int, LEN>();
    aie::vector<int, LEN> print_zeros = aie::zeros<int, LEN>();
    //writeincr( output, 1200 );

    int input_number = 0; // IMM_SIZE*(size+1)/TARGET;

    while( input_number < IMM_SIZE*(size+1)/TARGET ){
        aie::vector<uint8, TARGET> compare_x = aie::broadcast<uint8, TARGET>(START);
        aie::vector<uint8, TARGET> end = aie::broadcast<uint8, TARGET>(END);

        int getInput = 1;
        values = 0;
        while( getInput ){
            do{
                aie::vector<uint8, TARGET> x1 = readincr_v<TARGET>(input);
                input_number++;
                mask_x1 = aie::ge(x1, compare_x); //out[i] = v[i] >= START;
                mask_y1 = aie::lt(x1,end); //out[i] = v[i] < END;
                mask1 = mask_x1 & mask_y1;
            }while( !mask1.count() && input_number < IMM_SIZE*(size+1)/TARGET );
            values += mask1;
            if( input_number == IMM_SIZE*(size+1)/TARGET ){
                getInput = 0; 
                break;
            }

            do{
                aie::vector<uint8, TARGET> x2 = readincr_v<TARGET>(input);
                input_number++;
                mask_x2 = aie::ge(x2, compare_x); //out[i] = v[i] >= START;
                mask_y2 = aie::lt(x2,end); //out[i] = v[i] < END;
                mask2 = mask_x2 & mask_y2;
            }while( !mask2.count() && input_number < IMM_SIZE*(size+1)/TARGET );
            values += mask2;
            if( input_number == IMM_SIZE*(size+1)/TARGET ){
                getInput = 0; 
                break;
            }

            do {
                aie::vector<uint8, TARGET> x3 = readincr_v<TARGET>(input);
                input_number++;
                mask_x3 = aie::ge(x3, compare_x); // out[i] = v[i] >= START;
                mask_y3 = aie::lt(x3, end); // out[i] = v[i] < END;
                mask3 = mask_x3 & mask_y3;
            } while (!mask3.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask3;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x4 = readincr_v<TARGET>(input);
                input_number++;
                mask_x4 = aie::ge(x4, compare_x); // out[i] = v[i] >= START;
                mask_y4 = aie::lt(x4, end); // out[i] = v[i] < END;
                mask4 = mask_x4 & mask_y4;
            } while (!mask4.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask4;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x5 = readincr_v<TARGET>(input);
                input_number++;
                mask_x5 = aie::ge(x5, compare_x); // out[i] = v[i] >= START;
                mask_y5 = aie::lt(x5, end); // out[i] = v[i] < END;
                mask5 = mask_x5 & mask_y5;
            } while (!mask5.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask5;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x6 = readincr_v<TARGET>(input);
                input_number++;
                mask_x6 = aie::ge(x6, compare_x); // out[i] = v[i] >= START;
                mask_y6 = aie::lt(x6, end); // out[i] = v[i] < END;
                mask6 = mask_x6 & mask_y6;
            } while (!mask6.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask6;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x7 = readincr_v<TARGET>(input);
                input_number++;
                mask_x7 = aie::ge(x7, compare_x); // out[i] = v[i] >= START;
                mask_y7 = aie::lt(x7, end); // out[i] = v[i] < END;
                mask7 = mask_x7 & mask_y7;
            } while (!mask7.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask7;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x8 = readincr_v<TARGET>(input);
                input_number++;
                mask_x8 = aie::ge(x8, compare_x); // out[i] = v[i] >= START;
                mask_y8 = aie::lt(x8, end); // out[i] = v[i] < END;
                mask8 = mask_x8 & mask_y8;
            } while (!mask8.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask8;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x9 = readincr_v<TARGET>(input);
                input_number++;
                mask_x9 = aie::ge(x9, compare_x); // out[i] = v[i] >= START;
                mask_y9 = aie::lt(x9, end); // out[i] = v[i] < END;
                mask9 = mask_x9 & mask_y9;
            } while (!mask9.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask9;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x10 = readincr_v<TARGET>(input);
                input_number++;
                mask_x10 = aie::ge(x10, compare_x); // out[i] = v[i] >= START;
                mask_y10 = aie::lt(x10, end); // out[i] = v[i] < END;
                mask10 = mask_x10 & mask_y10;
            } while (!mask10.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask10;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x11 = readincr_v<TARGET>(input);
                input_number++;
                mask_x11 = aie::ge(x11, compare_x); // out[i] = v[i] >= START;
                mask_y11 = aie::lt(x11, end); // out[i] = v[i] < END;
                mask11 = mask_x11 & mask_y11;
            } while (!mask11.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask11;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x12 = readincr_v<TARGET>(input);
                input_number++;
                mask_x12 = aie::ge(x12, compare_x); // out[i] = v[i] >= START;
                mask_y12 = aie::lt(x12, end); // out[i] = v[i] < END;
                mask12 = mask_x12 & mask_y12;
            } while (!mask12.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask12;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x13 = readincr_v<TARGET>(input);
                input_number++;
                mask_x13 = aie::ge(x13, compare_x); // out[i] = v[i] >= START;
                mask_y13 = aie::lt(x13, end); // out[i] = v[i] < END;
                mask13 = mask_x13 & mask_y13;
            } while (!mask13.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask13;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x14 = readincr_v<TARGET>(input);
                input_number++;
                mask_x14 = aie::ge(x14, compare_x); // out[i] = v[i] >= START;
                mask_y14 = aie::lt(x14, end); // out[i] = v[i] < END;
                mask14 = mask_x14 & mask_y14;
            } while (!mask14.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask14;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x15 = readincr_v<TARGET>(input);
                input_number++;
                mask_x15 = aie::ge(x15, compare_x); // out[i] = v[i] >= START;
                mask_y15 = aie::lt(x15, end); // out[i] = v[i] < END;
                mask15 = mask_x15 & mask_y15;
            } while (!mask15.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask15;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            }

            do {
                aie::vector<uint8, TARGET> x16 = readincr_v<TARGET>(input);
                input_number++;
                mask_x16 = aie::ge(x16, compare_x); // out[i] = v[i] >= START;
                mask_y16 = aie::lt(x16, end); // out[i] = v[i] < END;
                mask16 = mask_x16 & mask_y16;
            } while (!mask16.count() && input_number < IMM_SIZE * (size + 1) / TARGET);
            values += mask16;
            if (input_number == IMM_SIZE * (size + 1) / TARGET) {
                getInput = 0;
                break;
            } 
        }

        aie::vector<uint8, TARGET> y1 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y2 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y3 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y4 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y5 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y6 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y7 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y8 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y9 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y10 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y11 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y12 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y13 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y14 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y15 = readincr_v<TARGET>(input2);
        aie::vector<uint8, TARGET> y16 = readincr_v<TARGET>(input2);


        if( values ){
            //setting inerting variables if the the vector x_i didn't take any input
            if( mask2 )
                x2 = aie::broadcast<uint8, TARGET>(END);
            if( mask3 )
                x3 = aie::broadcast<uint8, TARGET>(END);
            if( mask4 )
                x4 = aie::broadcast<uint8, TARGET>(END);
            if( mask5 )
                x5 = aie::broadcast<uint8, TARGET>(END);
            if( mask6 )
                x6 = aie::broadcast<uint8, TARGET>(END);
            if( mask7 )
                x7 = aie::broadcast<uint8, TARGET>(END);
            if( mask8 )
                x8 = aie::broadcast<uint8, TARGET>(END);
            if( mask9 )
                x9 = aie::broadcast<uint8, TARGET>(END);
            if( mask10 )
                x10 = aie::broadcast<uint8, TARGET>(END);
            if( mask11 )
                x11 = aie::broadcast<uint8, TARGET>(END);
            if( mask12 )
                x12 = aie::broadcast<uint8, TARGET>(END);
            if( mask13 )
                x13 = aie::broadcast<uint8, TARGET>(END);
            if( mask14 )
                x14 = aie::broadcast<uint8, TARGET>(END);
            if( mask15 )
                x15 = aie::broadcast<uint8, TARGET>(END);
            if( mask16 )
                x16 = aie::broadcast<uint8, TARGET>(END);

            val = 0, red = 0; 
            compare_y = zeros;
            
            for( i=START; i < END; i++ ){ //i is not reinizilized so that if i exit on VAL == TARGET i dont reenter
                for( j = 0; j < 256; j++ ){
                    //writeincr( output, 2123123123 );
                    mask_x1 = aie::eq(x1, compare_x);
                    mask_y1 = aie::eq(y1, compare_y);
                    mask1 = mask_x1 & mask_y1;
                    mask_x2 = aie::eq(x2, compare_x);
                    mask_y2 = aie::eq(y2, compare_y);
                    mask2 = mask_x2 & mask_y2;
                    mask_x3 = aie::eq(x3, compare_x);
                    mask_y3 = aie::eq(y3, compare_y);
                    mask3 = mask_x3 & mask_y3;
                    mask_x4 = aie::eq(x4, compare_x);
                    mask_y4 = aie::eq(y4, compare_y);
                    mask4 = mask_x4 & mask_y4;
                    mask_x5 = aie::eq(x5, compare_x);
                    mask_y5 = aie::eq(y5, compare_y);
                    mask5 = mask_x5 & mask_y5;
                    mask_x6 = aie::eq(x6, compare_x);
                    mask_y6 = aie::eq(y6, compare_y);
                    mask6 = mask_x6 & mask_y6;
                    mask_x7 = aie::eq(x7, compare_x);
                    mask_y7 = aie::eq(y7, compare_y);
                    mask7 = mask_x7 & mask_y7;
                    mask_x8 = aie::eq(x8, compare_x);
                    mask_y8 = aie::eq(y8, compare_y);
                    mask8 = mask_x8 & mask_y8;
                    mask_x9 = aie::eq(x9, compare_x);
                    mask_y9 = aie::eq(y9, compare_y);
                    mask9 = mask_x9 & mask_y9;
                    mask_x10 = aie::eq(x10, compare_x);
                    mask_y10 = aie::eq(y10, compare_y);
                    mask10 = mask_x10 & mask_y10;
                    mask_x11 = aie::eq(x11, compare_x);
                    mask_y11 = aie::eq(y11, compare_y);
                    mask11 = mask_x11 & mask_y11;
                    mask_x12 = aie::eq(x12, compare_x);
                    mask_y12 = aie::eq(y12, compare_y);
                    mask12 = mask_x12 & mask_y12;
                    mask_x13 = aie::eq(x13, compare_x);
                    mask_y13 = aie::eq(y13, compare_y);
                    mask13 = mask_x13 & mask_y13;
                    mask_x14 = aie::eq(x14, compare_x);
                    mask_y14 = aie::eq(y14, compare_y);
                    mask14 = mask_x14 & mask_y14;
                    mask_x15 = aie::eq(x15, compare_x);
                    mask_y15 = aie::eq(y15, compare_y);
                    mask15 = mask_x15 & mask_y15;
                    mask_x16 = aie::eq(x16, compare_x);
                    mask_y16 = aie::eq(y16, compare_y);
                    mask16 = mask_x16 & mask_y16;

                    
                    red = (int)mask1.count() + (int)mask2.count() + (int)mask3.count() + (int)mask4.count() +
                        (int)mask5.count() + (int)mask6.count() + (int)mask7.count() + (int)mask8.count() +
                        (int)mask9.count() + (int)mask10.count() + (int)mask11.count() + (int)mask12.count() +
                        (int)mask13.count() + (int)mask14.count() + (int)mask15.count() + (int)mask16.count();
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

                    if( val == values ){
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
                hist = (int)mask_x1.count() + (int)mask_x2.count() + (int)mask_x3.count() + (int)mask_x4.count() +
                        (int)mask_x5.count() + (int)mask_x6.count() + (int)mask_x7.count() + (int)mask_x8.count() +
                        (int)mask_x9.count() + (int)mask_x10.count() + (int)mask_x11.count() + (int)mask_x12.count() +
                        (int)mask_x13.count() + (int)mask_x14.count() + (int)mask_x15.count() + (int)mask_x16.count();
                if( hist ){
                    writeincr( output, -i-1);
                    writeincr( output, hist );
                }
            }
        }

    }
    writeincr( output, finish_aie );
}
