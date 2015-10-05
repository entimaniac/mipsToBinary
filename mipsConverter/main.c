/* 
 * Mips Assembly to Binary Converter
 * AUTHOR: Jesse Strode
 *
 * Description:
 * Functions to accept an instruction in Mips32 and convert to its
 * binary interpretation. This is not comprehensive at the moment
 *
 * TODO:
 * add conversion for j type;
 * add remaining commands;
 * optimize
 * error check/exception handling
 * enumerate and set constants
 *
 * Monday, October 5, 2015
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// converts r type instructions
void convertRType ( int instruction, char ** arguments, int binaryCommand[] );
// converts i type instructions
void convertITyple ( int instruction, char ** arguments, int binaryCommand[] );
// looks up opcode by the instruction and sets the appropriate bits
void writeOpCode ( int instruction, int binaryCommand[] );
// looks up register and sets appropriate bits using offset
void writeRegister( int offset, char * argument, int binaryCommand[] );
// currently blank; will set appropriate bits when implemented
void writeSHAMT( char ** arguments, int binaryCommand[] );
// looks up funct codes and sets appropriate bits
void writefunct( int instruction, int binaryCommand[] );
// converts decimal immediate into binary and sets appropriate bits
void writeImmediate( int immediate, int binaryCommand[] );
// matches instruction and returns its value
int determineInstruction ( char * undeterminedInstruction );
// matches register and returns its value
int determineRegister ( char * argument );
// splits apart a string into command and arguments
char * parseCommand ( char * arguments [], char * unparsedInstruction );
// converts integer array into single integer
int writeInstruction ( int binaryCommand[] );
// calls all neccesary functions and return final 32bit integer result
int32_t instructionToIntenger( char * instruction );


int main( int argc, const char * argv[] ) {

    // ***** test cases ***** //
    //int32_t my_32bit_int;
    // char * testInstruction = "lw $s1,$s1,39321";
    //    char * testInstruction = "mov $ra,$k1,$t1";
    // char * testInstruction = "not $ra";
    char * testInstruction = "mov $ra,$k1";

    // only call needed
    instructionToIntenger( testInstruction );

    return 0;
}


void convertRType ( int instruction, char ** arguments, int binaryCommand[] ){

    /*  
        op code first, three registers next, then shamt and funct
        The registers may be forced into being set to certain values
        depending on what pseudo instruction is called
     */
    writeOpCode( instruction, binaryCommand );
    writeRegister( 16, arguments[ 1 ], binaryCommand ); // rd 15 - 11
    // case of not
    if ( instruction == 4)
        writeRegister( 6, arguments[ 1 ], binaryCommand ); // rs 25 - 21
    else
        writeRegister( 6, arguments[ 2 ], binaryCommand ); // rs 25 - 21
    // case of not and mov
    if ( instruction == 2 || instruction == 4)
        writeRegister( 11, "$zero" , binaryCommand ); // rt 20 - 16
    else
    writeRegister( 11, arguments[ 3 ], binaryCommand ); // rt 20 - 16
    writeSHAMT( arguments, binaryCommand );
    writefunct( instruction, binaryCommand );

}

void convertITyple ( int instruction, char ** arguments, int binaryCommand[] ){

    /*
        First sets opcode, then two registers followed by the immediate
     */
    writeOpCode( instruction, binaryCommand );
    writeRegister( 6, arguments[ 2 ], binaryCommand ); // rs 25 - 21
    writeRegister( 11, arguments[ 1 ], binaryCommand ); // rt 20 - 16
    writeImmediate( atoi( arguments[ 3 ]), binaryCommand );

}

void writeOpCode ( int instruction, int binaryCommand[] ) {

    /*
        sets opcode 'bits' of array;
        r instructions are included for verboseness and potential future
        exception handling
     */
    switch (instruction) { // 31 - 26
        case 0: // add
        case 1: // sub
        case 2: // mov (add)
        case 3: // and
        case 4: // not (nor)
            // opcode is 0
            break;
        case 5: // lw
            binaryCommand[ 0 ] = 1; // 31
            binaryCommand[ 4 ] = 1; // 27
            binaryCommand[ 5 ] = 1; // 26
            break;
        case 6: // sw
            binaryCommand[ 0 ] = 1; // 31
            binaryCommand[ 2 ] = 1; // 29
            binaryCommand[ 4 ] = 1; // 27
            binaryCommand[ 5 ] = 1; // 26
            break;
        case 7: // beq
            binaryCommand[ 3 ] = 1; // 28
            break;
        default:
            break;
    }
}

void writeRegister( int offset, char * argument, int binaryCommand[] ){

    // searches table for matching register
    int registerNumber = determineRegister( argument );

    /*
        sets appropriate bits based on register;
        bits in array are appropriately set due to offset
     */
    switch ( registerNumber ) {
        case 0: // do nothing
            break;
        case 1:
            binaryCommand[ 4 + offset ] = 1;
            break;
        case 2:
            binaryCommand[ 3 + offset ] = 1;
            break;
        case 3:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 3 + offset ] = 1;
            break;
        case 4:
            binaryCommand[ 2 + offset ] = 1;
            break;
        case 5:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 2 + offset ] = 1;
            break;
        case 6:
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 2 + offset ] = 1;
            break;
        case 7:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 2 + offset ] = 1;
            break;
        case 8:
            binaryCommand[ 1 + offset ] = 1;
            break;
        case 9:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            break;
        case 10:
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            break;
        case 11:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            break;
        case 12:
            binaryCommand[ 2 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            break;
        case 13:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 2 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            break;
        case 14:
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 2 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            break;
        case 15:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 2 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            break;
        case 16:
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 17:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 18:
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 19:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 20:
            binaryCommand[ 2 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 21:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 2 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 22:
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 2 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 23:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 2 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 24:
            binaryCommand[ 1 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 25:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 26:
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 27:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 28:
            binaryCommand[ 2 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 29:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 2 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 30:
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 2 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        case 31:
            binaryCommand[ 4 + offset ] = 1;
            binaryCommand[ 3 + offset ] = 1;
            binaryCommand[ 2 + offset ] = 1;
            binaryCommand[ 1 + offset ] = 1;
            binaryCommand[ 0 + offset ] = 1;
            break;
        default:
            break;
    }
}

void writeSHAMT( char ** arguments, int binaryCommand[] ){
    // no instruction uses this yet
}

void writefunct( int instruction, int binaryCommand[] ){

    /*
        r values use funct;
        last bits set; 
        based on command, sets appropriate bits
     */
    switch (instruction) { // 5 - 0
        case 0: // add
            binaryCommand[ 26 ] = 1;
            break;
        case 1: // sub
            binaryCommand[ 30 ] = 1;
            binaryCommand[ 26 ] = 1;
            break;
        case 2: // mov (add)
            binaryCommand[ 26 ] = 1;
            // will come back
            break;
        case 3: // and
            binaryCommand[ 29 ] = 1;
            binaryCommand[ 26 ] = 1;
            break;
        case 4: // not (nor)
            binaryCommand[ 26 ] = 1;
            binaryCommand[ 29 ] = 1;
            binaryCommand[ 30 ] = 1;
            binaryCommand[ 31 ] = 1;
            // will come back
            break;
        case 5: // lw
        case 6: // sw
        case 7: // beq
            // no funct
        default:
            break;
    }
}

void writeImmediate( int immediate, int binaryCommand[] ){

    /*
        converts decimalto binary; because of direction of bits array,
        second counter must be used to move in opposite direction
        or I didn't think it through well enough
     */
    int i = 0;
    int j = 16;
    for( i = 31; i >= 0; i-- ){
        if ( i < 16 ) {
            if(( immediate & ( 1 << i )) != 0 ){
                binaryCommand[ j ] = 1;
            }
            j++;
        }
    }
}


int determineInstruction ( char * undeterminedInstruction ){

    // comapares possible instructions and sets; -1 otherwise
    char * instructions [] =
    { "add", "sub", "mov", "and", "not", "lw", "sw", "beq" };
    int instruction = -1;

    for( int i = 0; i < 8; i++ ){
        if( !strcmp( undeterminedInstruction, instructions[ i ]) ){
            instruction = i;
        }
    }

    return instruction;
}

int determineRegister ( char * argument ){

    // comapres to all possible register values; -1 otherwise
    int determinedRegister = -1;
    if ( strcmp( argument, "$zero" ) == 0 ) { determinedRegister = 0; }
    else if ( strcmp( argument, "$at" ) == 0 ) { determinedRegister = 1; }
    else if ( strcmp( argument, "$v0" ) == 0 ) { determinedRegister = 2; }
    else if ( strcmp( argument, "$v1" ) == 0 ) { determinedRegister = 3; }
    else if ( strcmp( argument, "$a0" ) == 0 ) { determinedRegister = 4; }
    else if ( strcmp( argument, "$a1" ) == 0 ) { determinedRegister = 5; }
    else if ( strcmp( argument, "$a2" ) == 0 ) { determinedRegister = 6; }
    else if ( strcmp( argument, "$a3" ) == 0 ) { determinedRegister = 7; }
    else if ( strcmp( argument, "$t0" ) == 0 ) { determinedRegister = 8; }
    else if ( strcmp( argument, "$t1" ) == 0 ) { determinedRegister = 9; }
    else if ( strcmp( argument, "$t2" ) == 0 ) { determinedRegister = 10; }
    else if ( strcmp( argument, "$t3" ) == 0 ) { determinedRegister = 11; }
    else if ( strcmp( argument, "$t4" ) == 0 ) { determinedRegister = 12; }
    else if ( strcmp( argument, "$t5" ) == 0 ) { determinedRegister = 13; }
    else if ( strcmp( argument, "$t6" ) == 0 ) { determinedRegister = 14; }
    else if ( strcmp( argument, "$t7" ) == 0 ) { determinedRegister = 15; }
    else if ( strcmp( argument, "$s0" ) == 0 ) { determinedRegister = 16; }
    else if ( strcmp( argument, "$s1" ) == 0 ) { determinedRegister = 17; }
    else if ( strcmp( argument, "$s2" ) == 0 ) { determinedRegister = 18; }
    else if ( strcmp( argument, "$s3" ) == 0 ) { determinedRegister = 19; }
    else if ( strcmp( argument, "$s4" ) == 0 ) { determinedRegister = 20; }
    else if ( strcmp( argument, "$s5" ) == 0 ) { determinedRegister = 21; }
    else if ( strcmp( argument, "$s6" ) == 0 ) { determinedRegister = 22; }
    else if ( strcmp( argument, "$s7" ) == 0 ) { determinedRegister = 23; }
    else if ( strcmp( argument, "$t8" ) == 0 ) { determinedRegister = 24; }
    else if ( strcmp( argument, "$t9" ) == 0 ) { determinedRegister = 25; }
    else if ( strcmp( argument, "$k0" ) == 0 ) { determinedRegister = 26; }
    else if ( strcmp( argument, "$k1" ) == 0 ) { determinedRegister = 27; }
    else if ( strcmp( argument, "$gp" ) == 0 ) { determinedRegister = 28; }
    else if ( strcmp( argument, "$sp" ) == 0 ) { determinedRegister = 29; }
    else if ( strcmp( argument, "$fp" ) == 0 ) { determinedRegister = 30; }
    else if ( strcmp( argument, "$ra" ) == 0 ) { determinedRegister = 31; }

    return determinedRegister;
}

char * parseCommand ( char * arguments [], char * unparsedInstruction ){

    // allocates space on heap for new array of parsed command
    char * temp = strdup( unparsedInstruction );
    int tokenSize = 0;

    for ( temp = strtok( temp, " ," ); temp; temp = strtok( NULL, " ," )) {
        arguments[ tokenSize++ ] = temp;
    }
    // free allocated space
    free( temp );
    return arguments[ 0 ];
}

int writeInstruction ( int binaryCommand[] ){

    // insures a 32bit integer; fails if not available
    int32_t finalInt = 0;
    // because of array direction, uses bitwise 100... instead of ...001
    for( int i = 0; i < 32; i++ ){
        if( binaryCommand[ i ] > 0 ){
            finalInt = finalInt | (2147483648 >> i);
        }
    }

    return finalInt;
}

int32_t instructionToIntenger( char * instruction ){

    // neccesary variables for calling appropriate functions
    int32_t finalInt = 0;
    int instructionNum;
    int binaryCommand[ 32 ] = { 0 };
    char * arguments[ 4 ];

    parseCommand( arguments, instruction );
    instructionNum = determineInstruction( arguments[ 0 ]);

    switch( instructionNum ){
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            convertRType( instructionNum, arguments, binaryCommand );
            break;
        case 5:
        case 6:
        case 7:
            convertITyple( instructionNum, arguments, binaryCommand );
            break;
        default:
            printf( "command not recognized\n");
            break;
    }

    // left in to show properly working
    ////// test
    for( int i = 0; i < 32; i++ ){
        printf( "%d", binaryCommand[ i ] );
    }
    printf( "\n" );

    writeInstruction( binaryCommand );

    return finalInt;
}