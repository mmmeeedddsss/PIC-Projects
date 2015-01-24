#include <p18f4550.h>
#include <stdio.h>
#include <stdlib.h>

#include <xc.h>

#define _XTAL_FREQ 20000000 //This is the speed the controller is running at
#pragma config PLLDIV = 5 , CPUDIV = OSC1_PLL2 , USBDIV = 2 , FOSC = INTOSCIO_EC	// You can write this way

// OR
#pragma config FCMEN = OFF		  // OR this way
#pragma config IESO = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config BORV = 3
#pragma config VREGEN = OFF
#pragma config WDT = OFF
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF
#pragma config CPB = OFF
#pragma config CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF

#define HEAP_SIZE 40

void Delay1Second(void);
void DelayN10MS( int n );
void init();
void Out_D( char pin, char o );
void Out_B( char pin, char o );

void Init_Game();
void Draw();

void Move();
void DeleteLast();
int PlaceFeed( int s );
char ControllForCrush();
void p321();
void DrawMatris( char matris[][8] );


char feedx, feedy;

char dir;

char snake[HEAP_SIZE+1][2];

char isRButtonPressed;
char isRButtonPressing;
char isLButtonPressed;
char isLButtonPressing;

int start,end;

int size;

void main(int argc, char** argv) {

    begin:
    init();
    p321();


    /*Out_B(0,1);
    Out_B(1,1);
    Out_B(2,1);
    Out_B(3,1);
    Out_B(4,1);
    Out_B(5,1);
    Out_B(6,1);
    Out_B(7,1);
    
    Out_D(0,0);
    Out_D(1,0);
    Out_D(2,0);
    Out_D(3,0);
    Out_D(4,0);
    Out_D(5,0);
    Out_D(6,0);
    Out_D(7,0);*/

    Init_Game();
    
    int pseudoTime = 30;
    char t;
    char loopConstant = 12;
    char minLoop = 2;
    char divConstant = 1;
    if( PORTCbits.RC6 == 1 && PORTCbits.RC7 == 1 )
    {
        minLoop = 4;
        loopConstant = 24;
        divConstant = 2;
    }
    else if( PORTCbits.RC6 == 1 )
        loopConstant = 16;
    else if( PORTCbits.RC7 == 1 )
    {
        loopConstant = 20;
        divConstant = 2;
    }

    char loopCount;

    pseudoTime = 18;

    while( PlaceFeed( pseudoTime ) == 0  )
            {
                pseudoTime += ((pseudoTime % 5)+1)*9;
                pseudoTime %= 64;
                size--;
            }
    while(1) //infinite loop
    {
        loopCount = loopConstant-size/divConstant;
        if( loopCount < minLoop )
            loopCount = minLoop;
        
        for( t=0; t<loopCount; t++ )
            Draw();

        if( PORTCbits.RC1 == 1 && isRButtonPressed == 0 )
            isRButtonPressing = 1;
        if( PORTCbits.RC2 == 1 && isLButtonPressed == 0 )
            isLButtonPressing = 1;

        if( isRButtonPressed == 0 && isRButtonPressing == 1  ) // turn r
        {
            isRButtonPressed = 1;
            isRButtonPressing = 0;

            t = dir;
            switch( t )
            {
                case 'w': dir = 'd'; break;
                case 's': dir = 'a'; break;
                case 'a': dir = 'w'; break;
                case 'd': dir = 's'; break;
            }
        }
        if( isRButtonPressed == 1 && PORTCbits.RC1 == 0 )
            isRButtonPressed = 0;

        for( t=0; t<loopCount; t++ )
            Draw();

        if( isLButtonPressed == 0 && isLButtonPressing == 1  ) // turn r
        {
            isLButtonPressed = 1;
            isLButtonPressing = 0;
            
            t = dir;
            switch( t )
            {
                case 'w': dir = 'a'; break;
                case 's': dir = 'd'; break;
                case 'a': dir = 's'; break;
                case 'd': dir = 'w'; break;
            }
        }
        if( isLButtonPressed == 1 && PORTCbits.RC2 == 0 )
            isLButtonPressed = 0;


        Move();
        if( ( feedx == snake[start][0] && feedy == snake[start][1] ) == 0 )
        {
            if( end == 0 ) end = HEAP_SIZE;
            end--;
        }
        else
            while( PlaceFeed( pseudoTime ) == 0  )
            {
                pseudoTime += ((pseudoTime % 5)+1)*9;
                pseudoTime %= 64;
                size--;
            }

        for( t=0; t<loopCount; t++ )
            Draw();

        if( ControllForCrush() == 1 )
        {
            goto begin;
        }

        if( PORTCbits.RC1 == 1 && isRButtonPressed == 0 )
            isRButtonPressing = 1;
        if( PORTCbits.RC2 == 1 && isLButtonPressed == 0 )
            isLButtonPressing = 1;

        for( t=0; t<loopCount; t++ )
            Draw();

        pseudoTime++;
        if( pseudoTime == 64 ) pseudoTime = 0;

        //DelayN10MS(15);
    }
}

//int pseudoTime = 0;

void Init_Game()
{
    //PlaceFeed( pseudoTime );

    //feedx = 6;
    //feedy = 2;

    snake[0][0] = 2;
    snake[0][1] = 2;

    snake[1][0] = 3;
    snake[1][1] = 2;

    snake[2][0] = 4;
    snake[2][1] = 2;
    dir = 'a';

    end = 3;
    start = 0;

    size = 3;

    isRButtonPressed = 0;
    isLButtonPressed = 0;

}

//int x = 0;

char ControllForCrush()
{
    int it = start+1;
    for( ;; it++ )
    {
        it = (it%HEAP_SIZE);
        if( it == end )
            break;

        if( snake[it][0] == snake[start][0] && snake[it][1] == snake[start][1] )
            return 1;
    }
    return 0;
}

void Move()
{
    int nx=snake[start][0],ny=snake[start][1];
    switch( dir )
    {
        case 'w': if( ny == 0 ) ny = 8; ny--; break;
        case 's': ny++; if( ny == 8 ) ny = 0; break;
        case 'a': if( nx == 0 ) nx = 8; nx--; break;
        case 'd': nx++; if( nx == 8 ) nx = 0; break;
    }
    if( start == 0 ) start = HEAP_SIZE;
    start--;
    snake[start][0] = nx;
    snake[start][1] = ny;
}

int PlaceFeed( int s )
{
    size++;
    feedx = s%8;
    feedy = s/8;

    int it = start;
    for( ;; it++ )
    {
        it = (it%HEAP_SIZE);
        if( it == end )
            break;

        if( snake[it][0] == feedx && snake[it][1] == feedy )
            return 0;
    }
    return 1;
    
}

void Draw()
{
    // for feed
    Out_D( feedx, 1 ); // x
    Out_B( feedy, 0 ); // y

    __delay_ms(1);

    Out_D( feedx, 0 ); // x
    Out_B( feedy, 1 ); // y

    //for snake itself
    int it = start;
    Out_D( snake[it][0], 1 ); // x
    Out_B( snake[it][1], 0 ); // y

    __delay_ms(1);
    //DelayN10MS(1);

    Out_D( snake[it][0], 0 ); // x
    Out_B( snake[it][1], 1 ); // y
    for( ;; it++ )
    {
        it = (it%HEAP_SIZE);
        if( it == end )
            break;
        
        Out_D( snake[it][0], 1 ); // x
        Out_B( snake[it][1], 0 ); // y

        __delay_ms(1);
        //DelayN10MS(1);

        Out_D( snake[it][0], 0 ); // x
        Out_B( snake[it][1], 1 ); // y
    }
}

void DrawMatris( char matris[][8] )
{
    int i,j=0;
    for( i=0; i<8; i++ )
    {
        Out_D( i, 1 );
        for( j=0; j<8; j++ )
            if( matris[i][j] == 1 )
                Out_B( j, 0 );

        __delay_ms(1);

        for( j=0; j<8; j++ )
            if( matris[i][j] == 1 )
                Out_B( j, 1 );
        Out_D( i, 0 );
    }
}

void Out_D( char pin, char o )
{
    switch( pin )
    {
        case 0: LATD0 = o; break;
        case 1: LATD1 = o; break;
        case 2: LATD2 = o; break;
        case 3: LATD3 = o; break;
        case 4: LATD4 = o; break;
        case 5: LATD5 = o; break;
        case 6: LATD6 = o; break;
        case 7: LATD7 = o; break;
    }
}

void Out_B( char pin, char o )
{
    switch( pin )
    {
        case 0: LATB0 = o; break;
        case 1: LATB1 = o; break;
        case 2: LATB2 = o; break;
        case 3: LATB3 = o; break;
        case 4: LATB4 = o; break;
        case 5: LATB5 = o; break;
        case 6: LATB6 = o; break;
        case 7: LATB7 = o; break;
    }
}

void init()
{
    TRISBbits.RB0 = 0;
    TRISBbits.RB1 = 0;
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 0;
    TRISBbits.RB5 = 0;
    TRISBbits.RB6 = 0;
    TRISBbits.RB7 = 0;
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    TRISDbits.RD3 = 0;
    TRISDbits.RD4 = 0;
    TRISDbits.RD5 = 0;
    TRISDbits.RD6 = 0;
    TRISDbits.RD7 = 0;

    TRISCbits.RC1 = 1;
    TRISCbits.RC2 = 1;

    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;

    LATB0 = 0;
    LATB1 = 0;
    LATB2 = 0;
    LATB3 = 0;
    LATB4 = 0;
    LATB5 = 0;
    LATB6 = 0;
    LATB7 = 0;
    LATD0 = 1;
    LATD1 = 1;
    LATD2 = 1;
    LATD3 = 1;
    LATD4 = 1;
    LATD5 = 1;
    LATD6 = 1;
    LATD7 = 1;

    int i,j=0;
    for( i=0; i<HEAP_SIZE; i++ )
        for( j=0; j<2; j++ )
            snake[i][j] = 0;


    Delay1Second();

    Out_B(0,1);
    Out_B(1,1);
    Out_B(2,1);
    Out_B(3,1);
    Out_B(4,1);
    Out_B(5,1);
    Out_B(6,1);
    Out_B(7,1);

    Out_D(0,0);
    Out_D(1,0);
    Out_D(2,0);
    Out_D(3,0);
    Out_D(4,0);
    Out_D(5,0);
    Out_D(6,0);
    Out_D(7,0);
}

void p321()
{
    char mm[8][8];
    
    int i,j;

    for( i=0; i<8; i++ ) // 3 start
        for( j=0; j<8; j++ )
            mm[i][j] = 0;

    mm[1][2] = 1;
    mm[2][2] = 1;
    mm[3][2] = 1;
    mm[4][2] = 1;
    mm[5][2] = 1;
    mm[1][3] = 1;
    mm[1][4] = 1;
    mm[3][3] = 1;
    mm[5][3] = 1;
    mm[5][4] = 1;

    for( i=0;i<200;i++ )
        DrawMatris( mm );

    LATB0 = 1;
    LATB1 = 1;
    LATB2 = 1;
    LATB3 = 1;
    LATB4 = 1;
    LATB5 = 1;
    LATB6 = 1;
    LATB7 = 1;
    LATD0 = 0;
    LATD1 = 0;
    LATD2 = 0;
    LATD3 = 0;
    LATD4 = 0;
    LATD5 = 0;
    LATD6 = 0;
    LATD7 = 0; // 3 end

    for( i=0; i<8; i++ ) // 2 start
        for( j=0; j<8; j++ )
            mm[i][j] = 0;

    mm[1][4] = 1;
    mm[1][3] = 1;
    mm[1][2] = 1;
    mm[2][2] = 1;
    mm[3][2] = 1;
    mm[3][3] = 1;
    mm[3][4] = 1;
    mm[4][4] = 1;
    mm[5][4] = 1;
    mm[5][3] = 1;
    mm[5][2] = 1;

    for( i=0;i<200;i++ )
        DrawMatris( mm );

    LATB0 = 1;
    LATB1 = 1;
    LATB2 = 1;
    LATB3 = 1;
    LATB4 = 1;
    LATB5 = 1;
    LATB6 = 1;
    LATB7 = 1;
    LATD0 = 0;
    LATD1 = 0;
    LATD2 = 0;
    LATD3 = 0;
    LATD4 = 0;
    LATD5 = 0;
    LATD6 = 0;
    LATD7 = 0; // 2 end

    for( i=0; i<8; i++ ) // 1 start
        for( j=0; j<8; j++ )
            mm[i][j] = 0;

    mm[1][3] = 1;
    mm[2][4] = 1;
    mm[2][3] = 1;
    mm[3][3] = 1;
    mm[4][3] = 1;
    mm[5][3] = 1;
    mm[5][4] = 1;
    mm[5][2] = 1;


    for( i=0;i<200;i++ )
        DrawMatris( mm );
    

    LATB0 = 1;
    LATB1 = 1;
    LATB2 = 1;
    LATB3 = 1;
    LATB4 = 1;
    LATB5 = 1;
    LATB6 = 1;
    LATB7 = 1;
    LATD0 = 0;
    LATD1 = 0;
    LATD2 = 0;
    LATD3 = 0;
    LATD4 = 0;
    LATD5 = 0;
    LATD6 = 0;
    LATD7 = 0; // 1 end
    
}

void Delay1Second()
{
    int i=0;
    for(i=0;i<100;i++)
        __delay_ms(10);
}

void DelayN10MS( int n )
{
    int i=0;
    for(i=0;i<n;i++)
        __delay_ms(10);
}
