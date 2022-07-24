#include<bits/stdc++.h>
using namespace std;

#include<windows.h>

wstring tetromino[7];
int nFieldWeidth=12;
int nFieldHeight=18;
unsigned char *playingField=NULL;

int nScreenWidth=80;
int nScreenHeight=30;

int rotate(int px,int py,int r)
{
    switch(r)
    {
        case 0: return py*4+px;         //0 Degree
        case 1: return 12+py-(px*4);    //90 degree
        case 2: return 15-(py*4)-px;    //180 degree
        case 3: return 3-py+(px*4);     //270 degree
        default:
                cout<<"INCORRECT rotation input entered!!";
    }
    return 6942069;
}


int main()
{
    //creating assets i.e. the tetris blocks
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");

    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L".X..");
    tetromino[1].append(L"....");

    tetromino[3].append(L".X..");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"..X.");
    tetromino[3].append(L"....");

    tetromino[4].append(L"..X.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"....");

    tetromino[5].append(L"....");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"....");

    tetromino[6].append(L".X..");
    tetromino[6].append(L".X..");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L"....");

    playingField=new unsigned char[nFieldWeidth*nFieldHeight];
    for(int x=0;x<nFieldWeidth;x++)
        for(int y=0;y<nFieldHeight;y++)
            playingField[y*nFieldWeidth+x]=(x==0||x==nFieldWeidth-1||
                                            y==nFieldHeight) ? 9 : 0;

    wchar_t *screen=new wchar_t[nScreenWidth*nScreenHeight];
    for(int i=0;i<nScreenWidth*nScreenHeight;i++)
        screen[i]=L' ';
    HANDLE hConsole=CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten=0;

    bool gameOver=false;

    while(!gameOver)
    {
        //draw field
        for(int x=0;x<nFieldWeidth;x++)
            for(int y=0;y<nFieldHeight;y++)
                screen[(y+5)*nScreenWidth+(x+2)]=L" ABCDEFG=#"[playingField[y*nFieldWeidth+x]];
        //display frame
        WriteConsoleOutputCharacter(hConsole,screen,nScreenWidth*nScreenHeight,{0,0},&dwBytesWritten);
    }

    return 0;
}