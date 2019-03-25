volatile int pixel_buffer_start; // global variable

const unsigned short CHESSBLACK[169]={
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

const unsigned short CHESSWHITE[169]={
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
void draw_chess_on_board(volatile int* board);
void draw_board();
void clear_screen();

// low level API
void draw_chess(int row, int col, int player);
void draw_line(int xa, int ya, int xb, int yb, short int color);
void draw_box(int xa, int ya, int width, int height, short int color);
void draw_circle(int xCenter, int yCenter, int player);
void plot_pixel(int x, int y, short int line_color);
void wait_for_vsync();

int main(void){
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
    volatile int board[64]; // 0 for none, 1 for black, 2 for white

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
        draw_board();
        draw_chess_on_board(board);
        // code for updating


        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}

void draw_layout(){
    // draw board
    int i = 7;
    for(; i < 239; i ++) {
        int j = 7;
        for (; j < 239; j ++) {
            plot_pixel(i, j, 0x0300);
        }
    }
}

void draw_chess_on_board(volatile int* board){
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
                plot_pixel(currentX + j, currentY + i, CHESSBLACK[i * 13 + j]);
            }
        }
    }else if(player == 2){
        volatile int i = 0;
        for(; i < 13; i++){
            volatile int j = 0;
            for(; j < 13; j++){
                plot_pixel(currentX + j, currentY + i, CHESSWHITE[i * 13 + j]);
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
