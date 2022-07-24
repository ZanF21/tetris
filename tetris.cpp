#include<bits/stdc++.h>
using namespace std;

#include<Windows.h>

wstring tetromino[7];
int nFieldWeidth=12;
int nFieldHeight=18;
unsigned char *playingField=NULL;

int nScreenWidth=120;
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

bool doesPieceFit(int nTetromino, int nRotation,int nPosX, int nPosY)
{
    for(int px=0;px<4;px++)
    {
        for(int py=0;py<4;py++)
        {
            int pi=rotate(px,py,nRotation);

            int fi=(nPosY+py)*nFieldWeidth+(nPosX+px);

            //check if inside x limits
            if(nPosX+px>=0 && nPosX+px<nFieldWeidth)
            {   //check if inside y limits
                if(nPosY+py>=0 && nPosY+py<nFieldHeight)
                {
                    if(tetromino[nTetromino][pi]==L'X' && playingField[fi]!=0)
                        return false;
                }
            }
        }
    }
    return true;
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

    tetromino[2].append(L"..X.");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"....");

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
                                            y==nFieldHeight-1) ? 9 : 0;

    wchar_t *screen=new wchar_t[nScreenWidth*nScreenHeight];
    for(int i=0;i<nScreenWidth*nScreenHeight;i++)
        screen[i]=L' ';
    HANDLE hConsole=CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten=0;

    bool gameOver=false;
    int nCurrentPiece=4;
    int nCurrentRotation=0;
    int nCurrentX=nFieldWeidth/2;
    int nCurrentY=0;
    
    //R L D Z
    bool bKey[4];

    while(!gameOver)
    {
        //game timing ==================================
        this_thread::sleep_for(50ms);

        //input ========================================
        for(int k=0;k<4;k++)
            bKey[k]=(0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

        //game logic ===================================
            //moving left
        nCurrentX-=(bKey[1] && doesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX-1,nCurrentY)) ? 1:0;
            //moving right
        nCurrentX+=(bKey[0] && doesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX+1,nCurrentY)) ? 1:0;
            //moving down
        nCurrentY+=(bKey[2] && doesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX,nCurrentY+1)) ? 1:0;

        //render output ================================

        //draw field
        for(int x=0;x<nFieldWeidth;x++)
            for(int y=0;y<nFieldHeight;y++)
                screen[(y+2)*nScreenWidth+(x+2)]=L" ABCDEFG=#"[playingField[y*nFieldWeidth+x]];
        
        //draw current piece
        for(int px=0;px<4;px++)
            for(int py=0;py<4;py++)
                if(tetromino[nCurrentPiece][rotate(px,py,nCurrentRotation)]==L'X')
                    screen[(nCurrentY+py+2)*nScreenWidth+(nCurrentX+px+2)]=nCurrentPiece+65;

        //display frame
        WriteConsoleOutputCharacterW(hConsole,screen,nScreenWidth*nScreenHeight,{0,0},&dwBytesWritten);
    }

    return 0;
}