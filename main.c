#include "address_map_arm.h"
#include "defines.h"
#include "interrupt_ID.h"

volatile int key_dir;
volatile int pattern;
volatile int pixel_buffer_start; // global variable
volatile int currentPlayer = 1;
volatile int board[64]; // 0 for none, 1 for black, 2 for white
volatile int isError = 0; // [bool] for whether to draw error msg
volatile int errMsgId = 0; // int id for error msg ID

volatile int isLegal; // local usage

/* errMsgID:
 * 1: invalid move
 */

const unsigned short CHESSBLACK[169] = {
        0x0000, 0x0000, 0x0000, 0x0000, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4208,   // 0x0010 (16) pixels
        0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x18C3, 0x0000, 0x0000, 0x0000, 0x18C3, 0x1082, 0x0861, 0x1082, 0x1082,   // 0x0020 (32) pixels
        0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x4208, 0x0000, 0x0000, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082,   // 0x0030 (48) pixels
        0x1082, 0x0861, 0x1082, 0x0000, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082,   // 0x0040 (64) pixels
        0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082,   // 0x0050 (80) pixels
        0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082,   // 0x0060 (96) pixels
        0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082,   // 0x0070 (112) pixels
        0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x0000, 0x1082, 0x0861, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082,   // 0x0080 (128) pixels
        0x1082, 0x0000, 0x0000, 0x4208, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x0861, 0x1082, 0x18C3, 0x0000, 0x0000,   // 0x0090 (144) pixels
        0x0000, 0x18C3, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x4208, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x00A0 (160) pixels
        0x1082, 0x1082, 0x1082, 0x1082, 0x1082, 0x0000, 0x0000, 0x0000, 0x0000
};

const unsigned short CHESSWHITE[169] = {
        0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF,   // 0x0010 (16) pixels
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0020 (32) pixels
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0030 (48) pixels
        0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0040 (64) pixels
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0050 (80) pixels
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0060 (96) pixels
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0070 (112) pixels
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0080 (128) pixels
        0xFFFF, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000,   // 0x0090 (144) pixels
        0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x00A0 (160) pixels
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000
};

// high level API
void draw_layout();
void draw_chess_on_board();
void draw_board();
void clear_screen();

// board manipulation
void play_chess(int row, int col, int player);

// chess logic
void tryMove(int row, int col, int player); // NOT DONE, need to play chess HERE!!!
void newChessMove(int row, int col, int player);
void updateChessInDirection(int row, int col, int player, int deltaRow, int deltaCol);
void chessMove(int row, int col, int player);
void checkLegalInDirection(int row, int col, int player, int deltaRow, int deltaCol);

void setErrorMsg(int errorID);
void clearErrorMsg();

// low level API
void draw_chess(int row, int col, int player);
void draw_line(int xa, int ya, int xb, int yb, short int color);
void draw_box(int xa, int ya, int width, int height, short int color);
void draw_circle(int xCenter, int yCenter, int player);
void plot_pixel(int x, int y, short int line_color);
void wait_for_vsync();

// interrupt config
void set_A9_IRQ_stack(void);
void enable_A9_interrupts(void);
void config_GIC(void);
void config_KEYs();
void pushbutton_ISR(void);

// Define the IRQ exception handler
void __attribute__((interrupt)) __cs3_isr_irq(void) {
    // Read the ICCIAR from the processor interface
    int address = MPCORE_GIC_CPUIF + ICCIAR;
    int int_ID = *((int *)address);
    if (int_ID == KEYS_IRQ) { // check if interrupt is from the KEYs
        pushbutton_ISR();
    } else {
        while (1);
    } // if unexpected, then stay here
    // Write to the End of Interrupt Register (ICCEOIR)
    address = MPCORE_GIC_CPUIF + ICCEOIR; *((int *)address) = int_ID;
    return;
}

// Define the remaining exception handlers
void __attribute__((interrupt)) __cs3_reset(void) {
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_undef(void) {
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_swi(void) {
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_pabort(void) {
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_dabort(void) {
    while (1);
}
void __attribute__((interrupt)) __cs3_isr_fiq(void) {
    while (1);
}

int main(void){
    /* set interrupt START*/
    set_A9_IRQ_stack();

    config_GIC();
    config_KEYs();

    enable_A9_interrupts();
    /* set interrupt END*/

    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the
    // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer

    clear_screen();
    // initialize drawing

    // initialize board
    int i = 0;
    for(; i < 8; i++){
        int j = 0;
        for(; j < 8; j++){
            board[i * 8 + j] = 0;
        }
    }

    board[3 * 8 + 3] = 1;
    board[3 * 8 + 4] = 2;
    board[4 * 8 + 3] = 2;
    board[4 * 8 + 4] = 1;

    while (1){
        /* Erase any boxes and lines that were drawn in the last iteration */
        clear_screen();

        // code for drawing
        draw_layout();
        draw_board();
        draw_chess_on_board();
        // code for updating


        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}

void draw_layout(){
    // draw board
    int i = 22;
    for(; i < 239; i ++) {
        int j = 22;
        for (; j < 239; j ++) {
            plot_pixel(i, j, 0x0300);
        }
    }
}

void draw_chess_on_board(){
    volatile int i = 0;
    for(; i < 8; i++){
        volatile int j = 0;
        for(; j < 8; j++){
            draw_chess(i, j, board[i * 8 + j]);
        }
    }
}

void draw_board(){
    // draw top line
    draw_line(21, 21, 237, 21, 0xFFFF);

    // draw bottom line
    draw_line(21, 237, 237, 237, 0xFFFF);

    // draw left line
    draw_line(21, 21, 21, 237, 0xFFFF);

    // draw right line
    draw_line(237, 21, 237, 237, 0xFFFF);

    // draw horizontal lines
    int i = 0; int current_y = 27 + 22;
    for(; i < 7; i++){
        draw_line(21, current_y, 237, current_y, 0xFFFF);
        draw_line(21, current_y + 1, 237, current_y + 1, 0xFFFF);
        current_y += 27;
    }

    // draw vertical lines
    i = 0; int current_x = 27 + 22;
    for(; i < 7; i++){
        draw_line(current_x, 21, current_x, 237, 0xFFFF);
        draw_line(current_x + 1, 21, current_x + 1, 237, 0xFFFF);
        current_x += 27;
    }
}

void clear_screen(){
    int i = 0;
    for(; i < 320; i++){
        int j = 0;
        for(; j < 240; j++){
            plot_pixel(i, j, 0x0000);
        }
    }
}


void play_chess(int row, int col, int player){
    board[row * 8 + col] = player;
}


// chess logic
void tryMove(int row, int col, int player){
    volatile int avaliable = 0; // [bool]
    volatile int occupied = 0; // [bool]

    if(board[row * 8 + col] != 0){
        occupied = 1;
    }

    if (!occupied){
        volatile int rowCheckDir = -1;
        for(; rowCheckDir <= 1; rowCheckDir++){
            volatile int colCheckDir = -1;
            for(; colCheckDir <= 1; colCheckDir++){
                if ( !avaliable && !(rowCheckDir == 0 && colCheckDir == 0)){
                    isLegal = 0;
                    checkLegalInDirection(row, col, player, rowCheckDir, colCheckDir);
                    if(isLegal){
                        avaliable = 1;
                        // PLAY CHESS HEREE!!!!
                        newChessMove(row, col, player);
                    }else{
                        setErrorMsg(1);
                    }
                }
            }
        }
    }else{
        setErrorMsg(1);
    }
}

void newChessMove(int row, int col, int player){
    volatile int rowCheckDir, colCheckDir;

    //################# UPDATE IN EIGHT DIRECTIONS####################//
    rowCheckDir = -1;
    for(; rowCheckDir <= 1; rowCheckDir++){
        colCheckDir = -1;
        for(; colCheckDir <= 1; colCheckDir++){
            if (!(rowCheckDir==0 && colCheckDir==0)){
                updateChessInDirection(row, col, player, rowCheckDir, colCheckDir);
            }
        }
    }
    chessMove(row, col, player);
}

// change the board in one direction
void updateChessInDirection(int row, int col, int player, int deltaRow, int deltaCol){
    volatile int originalRow = row;
    volatile int originalCol = col;
    volatile int changeColor = 1; // [bool]
    volatile int opponent;

    // determine the color of opponent
    if (player == 1){
        opponent = 2;
    }else{
        opponent = 1;
    }

    // If valid then change the color
    //
    isLegal = 0;
    checkLegalInDirection(row, col, player, deltaRow, deltaCol);
    if (isLegal){
        volatile int checkCol = originalCol + deltaCol;
        volatile int checkRow = originalRow + deltaRow;
        while (checkCol < 8 && checkRow < 8 && checkCol >= 0 && checkRow >= 0){
            if(board[checkRow * 8 + checkCol] == 0 || board[checkRow * 8 + checkCol] == player){
                changeColor = 0;
            }

            if (changeColor == 1) {
                chessMove(checkRow, checkCol, player);
            }
            checkRow += deltaRow;
            checkCol += deltaCol;
        }
    }
}

// Move single chess
void chessMove(int row, int col, int player){
    board[row * 8 + col] = player;
}

void checkLegalInDirection(int row, int col, int player, int deltaRow, int deltaCol){
    // convert letter expression to number expression
    volatile int originalRow = row;
    volatile int originalCol = col;
    volatile int number = 0;

    // the first encountered grid is opponent's chess or not
    volatile int opponentflag = 0; // [bool]

    volatile int checkCol = originalCol + deltaCol;
    volatile int checkRow = originalRow + deltaRow;
    volatile int opponent;

    // determine the opponent
    if (player == 1){
        opponent = 2;
    }else{
        opponent = 1;
    }

    // check for the first chess, whether it is opponent and set opponentflag according
    if((originalRow + deltaRow < 8) &&
        (originalRow + deltaRow >= 0) &&
        (originalCol + deltaCol < 8) &&
        (originalCol + deltaCol >= 0) &&
        (board[(originalRow + deltaRow) * 8 + (originalCol + deltaCol)] == opponent)){
        opponentflag = 1; // set flag to true
    }

    isLegal = 0;
    while (opponentflag && checkCol < 8 && checkRow < 8 && checkCol >= 0 && checkRow >= 0){
        if(board[checkRow * 8 + checkCol] == 0){
            isLegal = 0; // return false;
            return;
        }
        if(board[checkRow * 8 + checkCol] == player){
            isLegal = 1; // return true;
            return;
        }
        checkRow += deltaRow;
        checkCol += deltaCol;
        number++;
    }

    // isLegal = 0; // return false;
}


void setErrorMsg(int errorID){}

void clearErrorMsg(){}


void draw_chess(int row, int col, int player){
    int leftUpperCorner_X = 22 + col * (2 + 25);
    int leftUpperCorner_Y = 22 + row * (2 + 25);

    draw_circle(leftUpperCorner_X + 13, leftUpperCorner_Y + 13, player);
}

void draw_line(int xa, int ya, int xb, int yb, short int color){
    int is_steep;

    if(abs(yb - ya) > abs(xb - xa)){
        is_steep = 1;
    } else {
        is_steep = 0;
    }

    if(is_steep){
        int tmp = xa;
        xa = ya;
        ya = tmp;

        tmp = xb;
        xb = yb;
        yb = tmp;
    }

    if(xa > xb){
        int tmp = xa;
        xa = xb;
        xb = tmp;

        tmp = ya;
        ya = yb;
        yb = tmp;
    }


    int deltax = xb - xa;
    int deltay = abs(yb - ya);
    int error = - (deltax / 2);

    int y = ya;
    int y_step;
    if(ya < yb){
        y_step = 1;
    } else {
        y_step = -1;
    }

    int x = xa;
    for(; x < xb; x++){
        if(is_steep){
            plot_pixel(y, x, color);
        }else{
            plot_pixel(x, y, color);
        }
        error += deltay;
        if(error >= 0){
            y += y_step;
            error -= deltax;
        }
    }
}

// xa, ya is left bottom corner
// all parameters must be valid, please check before using this func.
void draw_box(int xa, int ya, int width, int height, short int color){
    int x_increment = 0;
    for(; x_increment < width; x_increment++){
        int y_increment = 0;
        for(; y_increment < height; y_increment++){
            plot_pixel(xa + x_increment, ya + y_increment, color);
        }
    }
}

void draw_circle(int xCenter, int yCenter, int player){
    int currentX = xCenter - 6;
    int currentY = yCenter - 6;

    if(player == 1){
        volatile int i = 0;
        for(; i < 13; i++){
            volatile int j = 0;
            for(; j < 13; j++){
                if( CHESSBLACK[i * 13 + j] != 0x0000){
                    plot_pixel(currentX + j, currentY + i, CHESSBLACK[i * 13 + j]);
                }
            }
        }
    }else if(player == 2){
        volatile int i = 0;
        for(; i < 13; i++){
            volatile int j = 0;
            for(; j < 13; j++){
                if( CHESSWHITE[i * 13 + j] != 0x0000){
                    plot_pixel(currentX + j, currentY + i, CHESSWHITE[i * 13 + j]);
                }
            }
        }
    }
}

void plot_pixel(int x, int y, short int line_color){
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}


void wait_for_vsync(){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    register int status;

    *pixel_ctrl_ptr = 1; // start synchronized process

    status = *(pixel_ctrl_ptr + 3);
    while((status & 0x01) != 0){
        status = *(pixel_ctrl_ptr + 3);
    }
}

/*
 * Initialize the banked stack pointer register for IRQ mode
*/

void set_A9_IRQ_stack(void) {
    int stack, mode;
    stack = A9_ONCHIP_END - 7; // top of A9 onchip memory, aligned to 8 bytes /* change processor to IRQ mode with interrupts disabled */
    mode = INT_DISABLE | IRQ_MODE;
    asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
/* set banked stack pointer */
    asm("mov sp, %[ps]" : : [ps] "r"(stack));
    /* go back to SVC mode before executing subroutine return! */
    mode = INT_DISABLE | SVC_MODE;
    asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
}

/*
 * Turn on interrupts in the ARM processor
*/
void enable_A9_interrupts(void) {
    int status = SVC_MODE | INT_ENABLE;
    asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}

/*
 * Configure the Generic Interrupt Controller (GIC)
*/
void config_GIC(void) {
    int address; // used to calculate register addresses
    /* configure the HPS timer interrupt */
    *((int *) 0xFFFED8C4) = 0x01000000;
    *((int *) 0xFFFED118) = 0x00000080;
    /* configure the FPGA interval timer and KEYs interrupts */
    *((int *) 0xFFFED848) = 0x00000101;
    *((int *) 0xFFFED108) = 0x00000300;
    // Set Interrupt Priority Mask Register (ICCPMR). Enable interrupts of all // priorities
    address = MPCORE_GIC_CPUIF + ICCPMR;
    *((int *) address) = 0xFFFF;
    // Set CPU Interface Control Register (ICCICR). Enable signaling of // interrupts
    address = MPCORE_GIC_CPUIF + ICCICR;
    *((int *) address) = ENABLE;
    // Configure the Distributor Control Register (ICDDCR) to send pending // interrupts to CPUs
    address = MPCORE_GIC_DIST + ICDDCR;
    *((int *) address) = ENABLE;
}

/* setup the KEY interrupts in the FPGA */
void config_KEYs() {
    volatile int * KEY_ptr = (int *)KEY_BASE; // pushbutton KEY address
    *(KEY_ptr + 2) = 0x3; // enable interrupts for KEY[1]
}

void pushbutton_ISR(void) {
    volatile int * KEY_ptr = (int *)KEY_BASE;
    int press;

    press = *(KEY_ptr + 3); // read the pushbutton interrupt register
    *(KEY_ptr + 3) = press; // Clear the interrupt

    key_dir ^= 1; // Toggle key_dir value

    volatile int * SW_ptr = (int *)SW_BASE;
    int user_sw = *SW_ptr;
    int col = user_sw & 0x0000000F;
    int row = user_sw & 0x000003C0;
    row = row >> 6;
    // check Valid First
    /*
     * To Be implemented
     */

    tryMove(row - 1, col - 1, currentPlayer);
    if (currentPlayer == 1){
        currentPlayer = 2;
    } else {
        currentPlayer = 1;
    }

    return;
}