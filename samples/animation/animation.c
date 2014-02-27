#include "gamecon.h"

enum
{
    WIDTH = 20,
    HEIGHT = 20,
    BLOCK = 20
};

void draw(int x, int y);

int main()
{
    GC_Init("Game Console Sample: Animation", WIDTH, HEIGHT, BLOCK);

    int x;
    for(x = 0; x < WIDTH; ++x)
        draw(x, 0);

    int y;
    for(y = 1; y < HEIGHT; ++y)
        draw(WIDTH - 1, y);

    for(x = WIDTH - 2; x >= 0; --x)
        draw(x, HEIGHT - 1);

    for(y = HEIGHT - 2; y >= 0; --y)
        draw(0, y);

    GC_Sleep(0);
    GC_Finalize();
    return 0;
}

void draw(int x, int y)
{
    int i;
    for(i = 0; i < WIDTH; ++i)
    {
        int j;
        for(j = 0; j < HEIGHT; ++j)
            GC_Set(i, j, 0, 0, 100);
    }

    GC_Set(x, y, 255, 255, 0);
    GC_Flip();
    GC_Sleep(100);
}
