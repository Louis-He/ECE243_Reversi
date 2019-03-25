volatile int pixel_buffer_start; // global variable

// high level API
void draw_layout();
void draw_chess_on_board(int** board);
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
    volatile int board[8][8]; // 0 for none, 1 for black, 2 for white

    while (1){
        /* Erase any boxes and lines that were drawn in the last iteration */
        clear_screen();

        // code for drawing
        draw_board();

        // code for updating


        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}

void draw_layout(){

}

void draw_chess_on_board(int** board){}

void draw_board(){
    // draw top line
    draw_line(6, 6, 237, 6, 0xFFFF);

    // draw bottom line
    draw_line(6, 237, 237, 237, 0xFFFF);

    // draw left line
    draw_line(6, 6, 6, 237, 0xFFFF);

    // draw right line
    draw_line(237, 6, 237, 237, 0xFFFF);

    // draw horizontal lines
    int i = 0; int current_y = 27 + 7;
    for(; i < 7; i++){
        draw_line(6, current_y, 237, current_y, 0xFFFF);
        draw_line(6, current_y + 1, 237, current_y + 1, 0xFFFF);
        current_y += 29;
    }

    // draw vertical lines
    i = 0; int current_x = 27 + 7;
    for(; i < 7; i++){
        draw_line(current_x, 6, current_x, 237, 0xFFFF);
        draw_line(current_x + 1, 6, current_x + 1, 237, 0xFFFF);
        current_x += 29;
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

void draw_chess(int row, int col, int player){}

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

void draw_circle(int xCenter, int yCenter, int player){}

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