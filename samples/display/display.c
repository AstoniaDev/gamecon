#include "gamecon.h"

enum
{
    WIDTH = 20,
    HEIGHT = 20,
    BLOCK = 20
};

int main()
{
    GC_Init("Game Console Sample: Display", WIDTH, HEIGHT, BLOCK);

    int x;
    for(x = 0; x < WIDTH; ++x)
    {
        int y;
        for(y = 0; y < HEIGHT; ++y)
        {
            if(x == 1 || y == 1 || x == WIDTH - 2 || y == HEIGHT - 2)
                GC_Set(x, y, 255, 255, 0);
            else
                GC_Set(x, y, 100, 100, 100);
        }
    }

    GC_Flip();
    GC_Sleep(0);
    GC_Finalize();
    return 0;
}
