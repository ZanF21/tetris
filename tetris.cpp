#include<bits/stdc++.h>
using namespace std;

#include<Windows.h>

wstring tetromino[7];
int nFieldWidth=12;
int nFieldHeight=18;
unsigned char *playingField=NULL;

int nScreenWidth=120;
int nScreenHeight=30;

int rotate(int px,int py,int r)
{
    switch(r%4)
    {
        case 0: return py*4+px;         //0 Degree
        case 1: return 12+py-(px*4);    //90 degree
        case 2: return 15-(py*4)-px;    //180 degree
        case 3: return 3-py+(px*4);     //270 degree
        default:
                cout<<"INCORRECT rotation input entered!!";
    }
    //this will never happen :') ... but a good number tho
    return 6942069;
}

bool doesPieceFit(int nTetromino, int nRotation,int nPosX, int nPosY)
{
    for(int px=0;px<4;px++)
    {
        for(int py=0;py<4;py++)
        {
            int pi=rotate(px,py,nRotation);

            int fi=(nPosY+py)*nFieldWidth+(nPosX+px);

            //check if inside x limits
            if(nPosX+px>=0 && nPosX+px<nFieldWidth)
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
    tetromino[0].append(L"..X...X...X...X.");
    tetromino[1].append(L"..X..XX..X......");
    tetromino[2].append(L"..X..XX...X.....");
    tetromino[3].append(L".X...XX...X.....");
    tetromino[4].append(L"..X...X..XX.....");
    tetromino[5].append(L".....XX..XX.....");
    tetromino[6].append(L".X...X...XX.....");

    //
    playingField=new unsigned char[nFieldWidth*nFieldHeight];
    for(int x=0;x<nFieldWidth;x++)
        for(int y=0;y<nFieldHeight;y++)
            playingField[y*nFieldWidth+x]=(x==0||x==nFieldWidth-1||
                                            y==nFieldHeight-1) ? 9 : 0;

    wchar_t *screen=new wchar_t[nScreenWidth*nScreenHeight];
    for(int i=0;i<nScreenWidth*nScreenHeight;i++)
        screen[i]=L' ';
    HANDLE hConsole=CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten=0;

    bool gameOver=false;
    int nCurrentPiece=rand()%7;
    int nCurrentRotation=0;
    int nCurrentX=nFieldWidth/2;
    int nCurrentY=0;
    
    //R L D Z
    bool bKey[4];
    bool bRotateHold=false;

    int nSpeed=20;
    int nSpeedCounter=0;
    bool bForceDown=false;
    int nPieceCount=0;
    int nScore=0;

    vector<int> vLines;

    while(!gameOver)
    {
        //game timing ==================================
        this_thread::sleep_for(50ms);
        nSpeedCounter++;
        bForceDown = (nSpeedCounter==nSpeed);

        //input ========================================
        for(int k=0;k<4;k++)
            bKey[k]=(0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

        //game logic ===================================
            //moving left right down
        nCurrentX-=(bKey[1] && doesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX-1,nCurrentY)) ? 1:0;
        nCurrentX+=(bKey[0] && doesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX+1,nCurrentY)) ? 1:0;
        nCurrentY+=(bKey[2] && doesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX,nCurrentY+1)) ? 1:0;
            //rotate
        if(bKey[3])
        {
            nCurrentRotation+= (!bRotateHold&& doesPieceFit(nCurrentPiece,nCurrentRotation+1,nCurrentX,nCurrentY)) ? 1:0;
            bRotateHold=true;
        }
        else
            bRotateHold=false;

        if(bForceDown)
        {
            if(doesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX,nCurrentY+1))
                nCurrentY++;
            else
            {
                //add piece to play field
                for(int px=0;px<4;px++)
                    for(int py=0;py<4;py++)
                        if(tetromino[nCurrentPiece][rotate(px,py,nCurrentRotation)]==L'X')
                            playingField[(nCurrentY+py)*nFieldWidth+(nCurrentX+px)]=nCurrentPiece+1;

                nPieceCount++;
                nSpeed -= (nPieceCount%10 ==0 && nSpeed>=10) ? 1 : 0;

                //check for full lines
                for(int py=0;py<4;py++)
                {
                    if(nCurrentY+py<nFieldHeight-1)
                    {
                        bool bLine=true;
                        for(int px=1;px<nFieldWidth-1;px++)
                            bLine &=(playingField[(nCurrentY+py)*nFieldWidth +px]) != 0;
                        //draw complete line
                        if(bLine)
                        {
                            for(int px=1;px<nFieldWidth-1;px++)
                                playingField[(nCurrentY+py)*nFieldWidth +px]=8;

                            vLines.push_back(nCurrentY+py);
                        }

                    }
                }

                //calculating Score
                nScore+=25;
                if(!vLines.empty())
                    nScore+=(1<<vLines.size())*100;

                //choose next piece;
                nCurrentPiece=rand()%7;
                nCurrentRotation=0;
                nCurrentX=nFieldWidth/2;
                nCurrentY=0;

                //if piece does not fit
                gameOver=!doesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX,nCurrentY);
            }
            nSpeedCounter=0;
        }

        //render output ================================

        //draw field
        for(int x=0;x<nFieldWidth;x++)
            for(int y=0;y<nFieldHeight;y++)
                screen[(y+2)*nScreenWidth+(x+2)]=L" ABCDEFG=#"[playingField[y*nFieldWidth+x]];
        
        //draw current piece
        for(int px=0;px<4;px++)
            for(int py=0;py<4;py++)
                if(tetromino[nCurrentPiece][rotate(px,py,nCurrentRotation)]==L'X')
                    screen[(nCurrentY+py+2)*nScreenWidth+(nCurrentX+px+2)]=nCurrentPiece+65;
        //draw Score
        swprintf_s(&screen[2*nScreenWidth+nFieldWidth+6],16,L"SCORE : %8d",nScore);


        if(!vLines.empty())
        {
            WriteConsoleOutputCharacterW(hConsole,screen,nScreenWidth*nScreenHeight,{0,0},&dwBytesWritten);
            this_thread::sleep_for(400ms);//delay to see visual

            for(auto v:vLines)
            {
                for(int px=1;px<nFieldWidth-1;px++)
                {
                    for(int py=v;py>0;py--)
                        playingField[py*nFieldWidth+px]=playingField[(py-1)*nFieldWidth+px];
                    playingField[px]=0;
                }
            }
            vLines.clear();
        }

        


        //display frame
        WriteConsoleOutputCharacterW(hConsole,screen,nScreenWidth*nScreenHeight,{0,0},&dwBytesWritten);
    }
    CloseHandle(hConsole);
    cout<<"Game Over {p.s. nubde}!! \t Score : "<<nScore<<"\n";
    system("pause");
    return 0;
}