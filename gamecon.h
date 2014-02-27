/*
 * gamecon.h
 * A C library that allows students to write simple Windows GUI games
 * Created by J. Nan in August, 2008
 */
#ifndef J_NAN_GAMECON_H_
#define J_NAN_GAMECON_H_

enum
{
    E_SUCCESS		  =  0,
    E_NOT_INITIALIZED	  = -1,
    E_ALREADY_INITIALIZED = -2,
    E_PARAMETER		  = -3,
    E_INIT		  = -4,
};

int GC_Init(const char* title, int width, int height, int blockSize);
int GC_Finalize();
int GC_Sleep(int ms);
int GC_Set(int x, int y, int red, int green, int blue);
int GC_Flip();

int GC_UpPressed();
int GC_DownPressed();
int GC_LeftPressed();
int GC_RightPressed();
int GC_APressed();
int GC_BPressed();
int GC_CPressed();
int GC_DPressed();
int GC_ResetPressed();

#endif
