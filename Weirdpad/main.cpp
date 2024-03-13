#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include <conio.h>
#include <string>
#include <vector>
#include <fstream>
#include <stack>
#include <tuple>
#include <cmath>

using namespace std;

// Deschide fisierul in modurile de intrare si iesire

string s,selectie, textCpy;
stack<tuple<string,int,int>> undo;
int width=12;
int height=21;
int font=10;
int rs=0;
int x_fereastra = 590;
int y_fereastra = 470-25;
int y_fereastra2 = y_fereastra+25;

int numerePeRand = x_fereastra/width+1;
int ok=0;
int prima=1;
int culoare=0;
int dinfisier=0;
int marimetext=2;


int yAbsolut = 0;

int x_curent = 0;
int y_curent = 45;
int y1_curentscroll;
int y2_curentscroll;
int x1_scroll;
int x2_scroll;

bool selectatText = false;
bool ctra = false;
void* image;
int page=1;


char c;

enum ButtonState //un fel de ok pt fiecare buton doar ca in loc de 0 si 1 ai cpmact si extins
{
    COMPACT,
    EXTENDED
};

ButtonState buttonStates[5] = {COMPACT, COMPACT, COMPACT, COMPACT, COMPACT}; // vector de ok

int buttonX[6] = {0, 90, 180, 270, 360, 450}; //de unde imi incepe fiecare buton
int buttonY = 0;
int buttonWidth = 90;
int buttonHeight = 30;
const char* buttonLabels[] = {"File", "Color", "View", "Themes", "Edit", "Help"};  // ce sa imi scrie in el


const char* extendedButtonLabels[][6] = //optiunile pentru fiecare
{
    {"Save as", "Open", "Delete", "Exit"},
    {"Black", "Red", "Blue", "Green"},
    {"Zoom-In", "Zoom-Out", "Default","Font 1","Font 2"},
    {"Light", "Dark", "Space"},
    {"Copy","Paste","Small","Full","Undo"},
    {"Info"}
};


///deschidere/ inchidere grafica
void initializare_fereastra()
{
    initwindow(x_fereastra+24, y_fereastra2+21, "WEIRDPAD");
    settextstyle(BOLD_FONT, HORIZ_DIR, 2);
    setbkcolor(WHITE);
    setcolor(BLACK);
    setfillstyle(COMPLEX_FONT, WHITE);
    floodfill(0, 0, BLUE);
}

void inchide_graphica()
{
    getch();
    closegraph();
}

void gestionaretema();
void gestioneazaculoare();



///FISIER
void saveToFile(const std::string& content)
{
    // Deschide o fereastra pentru a selecta locatia si numele fisierului
    OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = sizeof(szFileName);
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn))
    {
        // Salveaza continutul caracter cu caracter in fisier
        std::ofstream file(ofn.lpstrFile);
        if (file.is_open())
        {
            for (int i = 0; i <= content.size(); i++)
            {
                file.put(content[i]);
            }
            file.close();
            MessageBox(NULL, "Textul a fost salvat cu succes!", "Succes", MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            MessageBox(NULL, "Nu s-a putut salva fisierul.", "Eroare", MB_OK | MB_ICONERROR);
        }
    }
}

std::string readFromFile()
{
    // Deschide o fereastra pentru a selecta un fisier text
    OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = sizeof(szFileName);
    ofn.Flags = OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        // Citeste continutul caracter cu caracter din fisier
        std::ifstream file(ofn.lpstrFile);
        std::string content;
        if (file.is_open())
        {
            char ch;
            while (file.get(ch))
            {
                content.push_back(ch);
            }
            file.close();
        }
        else
        {
            MessageBox(NULL, "Nu s-a putut citi fisierul.", "Eroare", MB_OK | MB_ICONERROR);
        }
        return content;
    }
    else
    {
        return "";
    }
}

void deleteFilecartof()
{
    // Deschide o fereastra pentru a selecta fisierul de sters
    OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = sizeof(szFileName);
    ofn.Flags = OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        // Sterge fisierul
        if (DeleteFile(ofn.lpstrFile))
        {
            MessageBox(NULL, "Fisierul a fost sters cu succes!", "Succes", MB_OK | MB_ICONINFORMATION);
        }
        else
        {
            MessageBox(NULL, "Nu s-a putut sterge fisierul.", "Eroare", MB_OK | MB_ICONERROR);
        }
    }
}


/*void gestionarefont()
{
    if(font==0)
    {
        x_curent = x_curent/width;
        y_curent = (y_curent-45)/height;
        settextstyle(BOLD_FONT, HORIZ_DIR, 2);
        width=textheight("W");
        height=textwidth("W");
        x_curent = x_curent*width;
        y_curent = 45+(y_curent-45)*height;
    }
    else if(font==1)
    {
        settextstyle(1, HORIZ_DIR, 2);
        width=textheight("W");
        height=textwidth("W");
    }
    else if(font==2)
    {
        settextstyle(5, HORIZ_DIR, 2);
        width=textheight("W");
        height=textwidth("W");
    }
    else if(font==3)
    {
        settextstyle(6, HORIZ_DIR, 2);
        width=textheight("W");
        height=textwidth("W");
    }
}*/

void setTextColor(int colorCode)
{
    setcolor(colorCode);
}
///DESENARE BUTOANE
void drawButton(int x, int y, int height, int width, const char* label, int highlightColor = BLACK) //desenez butonul ok e doar tema in care sunt ca sa se potriveasca cu fereastra
{
    settextstyle(10, HORIZ_DIR, 2);
    if(ok==0)
        setfillstyle(SOLID_FILL, LIGHTGRAY);
    if(ok==1)
        setfillstyle(SOLID_FILL, DARKGRAY);
    bar(541, 0, getmaxx(), 31);
    if(ok==0)
        setfillstyle(SOLID_FILL, WHITE);
    else
        setfillstyle(SOLID_FILL, BLACK);


    bar(x, y, x + height, y + width);

    if(ok==0)
    {
        setcolor(BLACK);
        setbkcolor(WHITE);
    }
    else if(ok==1)
    {
        setcolor(WHITE);
        setbkcolor(BLACK);
    }
    else if(ok==2)
    {
        setcolor(BLACK);
        setbkcolor(BLACK);
    }

    rectangle(x, y, x + height, y + width); //desenat propriu zis

    char labelCopy[100];
    strcpy(labelCopy, label);// pun textul pe mijloc

    int textX = x + (height - textwidth(labelCopy)) / 2;
    int textY = y + (width - textheight(labelCopy)) / 2;

    // if(highlightColor==YELLOW)
    //{

    //}
//    else
    if(ok==1)
        highlightColor= WHITE;
    else if(ok==2)
        highlightColor= LIGHTMAGENTA;
    setTextColor(highlightColor);
    outtextxy(textX, textY, labelCopy);
    gestionaretema();
}




///BUTOANE EXTINSE
void drawExtendedButtons(int x, int y, int height, int width, const char* labels[], int numOptions) //extind butoanele fiecare subbuton avand inaltimea butonului initial ( de asta i+1)
{
    for (int i = 0; i < numOptions; ++i)
    {
        drawButton(x, y + (i + 1) * width, height, width, labels[i]);
    }
}



///MOUSE PESTE BUTON
bool isMouseOverButton(int mouseX, int mouseY, int buttonX, int buttonY, int buttonWidth, int buttonHeight) //verific daca sunt intr-un buton
{
    return (mouseX >= buttonX && mouseX <= buttonX + buttonWidth &&
            mouseY >= buttonY && mouseY <= buttonY + buttonHeight);
}





///TOATE COMPACTE
bool toatecompacte() // verific daca toate butoanele sunt compacte
{
    for (int i = 0; i <= 5; ++i)
        if (buttonStates[i] == EXTENDED)
            return 0;
    return 1;
}



///GESTIONARE CLICK BUTON
void gestioneazaClicButon()
{
    int mouseX = mousex();
    int mouseY = mousey();
    for (int i = 0; i <= 5; ++i)
        if (isMouseOverButton(mouseX, mouseY, buttonX[i], buttonY, buttonWidth, buttonHeight))
        {
            if (buttonStates[i] == EXTENDED)
                buttonStates[i] = COMPACT;
            else
                buttonStates[i] = EXTENDED;
        }
    clearmouseclick(WM_LBUTTONDOWN);
}





bool caracter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || strchr(" ", c) || (c=='.'||c==','||c==';'||c=='('||c==')'||c=='*'||c=='/'||c=='+'||c=='-'||c=='_'||c=='^'||c=='#'||c=='!'||c=='?'||c=='"'|| c=='|'|| c==':'|| c=='>'||c=='<'|| c=='&'|| c=='%'|| c=='@'|| c=='['|| c==']'|| c=='{' ||c=='}'||c=='~'||c=='$' ||c=='`'||c=='=') ;
}





void drawScrollBarVerticalAxisBackground()
{
    int x1 = getmaxx() - 10, y1 = 40;
    int x2 = getmaxx(), y2 = getmaxy();
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    bar(x1,y1,x2,y2);
}



int y1vaxis = 5;
int y2vaxis = y_fereastra+height-5;
int y1anterior;
int y1anteriorok = true;
int y_start;




///SCROLL
void drawScrollBarVerticalAxis()
{

    int x1 = getmaxx() - 8;
    int x2 = getmaxx()-2;

    if(yAbsolut >= y_fereastra)
        y1anterior = yAbsolut - y_fereastra;
    y1anterior = y1anterior/height * 5;
    int lungime_max_y = getmaxy() - 10 - y1anterior;
    if(y_curent >= y_fereastra)
        y2vaxis = y_curent - y_fereastra;
    y2vaxis = y2vaxis/height*5;
    y_start = 5 + y2vaxis + 42;

    ///y1anterior = y_curent;
    setfillstyle(SOLID_FILL,DARKGRAY);
    bar(x1,y_start,x2,y_start + lungime_max_y-42);
    //cout << y_curent << " " << yAbsolut << " " << y1anterior << endl;

}


void resizetext();
void afisarepos();

///DESENEZ BUTOANE
void gestioneazaButoane()
{
    int mouseX = mousex();
    int mouseY = mousey();

    for (int i = 0; i < 6; ++i)
    {
        if (buttonStates[i] == COMPACT)
        {
            if (isMouseOverButton(mouseX, mouseY, buttonX[i], buttonY, buttonWidth, buttonHeight))
            {
                drawButton(buttonX[i], buttonY, buttonWidth, buttonHeight, buttonLabels[i], YELLOW);
            }
            else
            {
                drawButton(buttonX[i], buttonY, buttonWidth, buttonHeight, buttonLabels[i]);
            }
        }
        else if (buttonStates[i] == EXTENDED)
        {
            drawButton(buttonX[i], buttonY, buttonWidth, buttonHeight, buttonLabels[i]);

            int numOptions = 4; //daca i este 3 (butoanele incep de la 0) nr de optiuni devine 5 altfel devine 4

            if(i==3)
                numOptions = 3;
            if(i==2)
                numOptions = 5;
            if(i==4)
                numOptions = 5;
            if(i==5)
                numOptions = 1;
            for (int j = 0; j < numOptions; ++j)
            {
                if (isMouseOverButton(mouseX, mouseY, buttonX[i], buttonY + (j + 1) * buttonHeight, buttonWidth, buttonHeight))
                {
                    drawButton(buttonX[i], buttonY + (j + 1) * buttonHeight, buttonWidth, buttonHeight, extendedButtonLabels[i][j], YELLOW);
                }
                else
                {
                    drawButton(buttonX[i], buttonY + (j + 1) * buttonHeight, buttonWidth, buttonHeight, extendedButtonLabels[i][j]);
                }
            }
        }
    }
}



///CURSOR
void cursor()
{
    if(ok==0)
    {
        setcolor(BLACK);
        int y_auxiliar = 0;
        if(y_curent > y_fereastra)
            y_auxiliar = y_fereastra;
        else y_auxiliar = y_curent;
        line(x_curent, y_auxiliar, x_curent, y_auxiliar + height);
        setcolor(YELLOW);
        line(x_curent, y_auxiliar, x_curent, y_auxiliar + height);
        setcolor(BLACK);
        line(x_curent, y_auxiliar, x_curent, y_auxiliar + height);
    }
    else if(ok==1)
    {
        setcolor(WHITE);
        int y_auxiliar = 0;
        if(y_curent > y_fereastra)
            y_auxiliar = y_fereastra;
        else y_auxiliar = y_curent;
        line(x_curent, y_auxiliar, x_curent, y_auxiliar + height);
        setcolor(YELLOW);
        line(x_curent, y_auxiliar, x_curent, y_auxiliar + height);
        setcolor(WHITE);
        line(x_curent, y_auxiliar, x_curent, y_auxiliar + height);
    }
    else if(ok==2)
    {
        setcolor(BLUE);
        int y_auxiliar = 0;
        if(y_curent > y_fereastra)
            y_auxiliar = y_fereastra;
        else y_auxiliar = y_curent;
        line(x_curent, y_auxiliar, x_curent, y_auxiliar + height);
        setcolor(YELLOW);
        line(x_curent, y_auxiliar, x_curent, y_auxiliar + height);
        setcolor(BLUE);
        line(x_curent, y_auxiliar, x_curent, y_auxiliar + height);
    }

}


void gestionaremarime(int marimetext);



///AFISARE
void afisare()
{
    free(image);
    image=malloc(imagesize(0,0,getmaxx(),getmaxy()));
    getimage(0,0,getmaxx(),getmaxy(),image);
    cleardevice();
    gestionaremarime(marimetext);
    if(rs==1)
        resizetext();
    rs=0;
    int i = 0;
    int x = 0;
    int y = 0;

    int y_poz = 0;
    char aux[2];

    if(dinfisier==1)
        while(y<45)
        {
            s.insert(0," ");
            if (x > x_fereastra - width)
            {
                x = 0;
                y += height;

            }
            else
                x += width;

        }
    dinfisier=0;



    if(y_curent >= y_fereastra)
    {
        y_poz = y_curent-y_fereastra;
        y_poz = (y_poz)/height * numerePeRand;
        y_poz += numerePeRand;
    }
    else
        y_poz = 0;

    ///daca y > y-fereastra numar de cate ori apoi numar caracterele din string si afisez dupa caracterele alea
    for (i = y_poz; i <= s.size(); i++)
    {
        aux[0] = s[i];
        aux[1] = NULL;
        if(y>30)
            outtextxy(x, y, aux);

        if (x > x_fereastra - width)
        {
            x = 0;
            y += height;

        }
        else
            x += width;
    }
    cursor();
    afisarepos();
    drawScrollBarVerticalAxisBackground();
    drawScrollBarVerticalAxis();
    gestioneazaButoane();

    //if(ok==0)
    gestioneazaculoare();
    setactivepage(page);
    putimage(0,0,image,1);
    setvisualpage(1-page);
    page=1-page;

}



///AFISARE CLICK
void afisareClick(bool selectat[])
{
    free(image);
    image=malloc(imagesize(0,0,getmaxx(),getmaxy()));
    getimage(0,0,getmaxx(),getmaxy(),image);
    cleardevice();
    drawScrollBarVerticalAxisBackground();
    gestioneazaButoane();
    int x = 0;
    int y = 0;
    char aux[2];
    for (int i = 0; i <= s.size(); i++)
    {
        aux[0] = s[i];
        aux[1] = NULL;
        if(selectat[i] == 1 /*&& (s[i] != ' ' || (s[i] == ' ' && x == 0))*/)
        {
            if(s[i] == ' ')
            {
                if(x == 0)
                {
                    setbkcolor(LIGHTBLUE);
                    outtextxy(x, y, aux);
                }
                else
                {
                    int pozmin = y/height*(x_fereastra/width + 1);
                    int pozmax = x_fereastra/width + y/height * (x_fereastra/width+1);
                    int pozCurenta = i;
                    bool stanga = false;
                    bool dreapta = false;
                    for(int j = pozmin; j < pozCurenta && j < s.size(); j++)
                        if(caracter(s[j]) && s[j] != ' ')
                            stanga = true;
                    for(int j = pozCurenta + 1; j <= pozmax && j < s.size(); j++)
                        if(caracter(s[j]) && s[j] != ' ')
                            dreapta = true;
                    if(dreapta && stanga)
                    {
                        setbkcolor(LIGHTBLUE);
                        outtextxy(x, y, aux);
                    }
                }

            }
            else
            {
                setbkcolor(LIGHTBLUE);
                outtextxy(x, y, aux);
            }
        }
        else if(selectat[i] == 0)
        {
            setbkcolor(WHITE);
            outtextxy(x, y, aux);
        }
        if (x > x_fereastra - width)
        {
            x = 0;
            y += height;
        }
        else
            x += width;
    }
    gestioneazaButoane();
    cursor();
    afisarepos();
    setactivepage(page);
    putimage(0,0,image,1);
    setvisualpage(1-page);
    page=1-page;

}

///STERGERE
void gestioneazaBackspace()
{
    int poz =  x_curent / width + (y_curent / height) * (x_fereastra / width + 1);
    if (s.size() < poz)
        s.resize(poz, ' ');
    if(y_curent < 40 || (x_curent==0 && y_curent==45))
    {

    }
    else if(x_curent == 0 && y_curent >= 45)
    {

        int xaux = 0,pozaux;
        bool ok = true;
        while(xaux <= x_fereastra + width-10)
        {
            pozaux = xaux/width + (y_curent/height) * (x_fereastra/width+1);
            if(s[pozaux] != ' ' && pozaux <= s.size()-1)
                ok = false;
            xaux += width;
        }

        if(ok)
        {
            xaux = 0;
            pozaux = xaux/width + (y_curent/height) * (x_fereastra/width+1);
            s.erase(pozaux,numerePeRand);
            y_curent -= height;
            pozaux = xaux/width + (y_curent/height) * (x_fereastra/width+1);
            ok = false;
            while(xaux <= x_fereastra && pozaux <= s.size()-2)
            {
                if(s[pozaux] != ' ')
                {
                    x_curent = xaux;
                    ok = true;
                }
                xaux += width;
                pozaux = xaux/width + (y_curent/height) * (x_fereastra/width+1);
            }
            if(ok)
                x_curent += width;

        }
        else if(!ok)
        {
            cout << 1<< " " << endl;
            xaux = 0;
            int x_randsus = 0, pozaux2;
            int y_auxiliar = y_curent - height;
            pozaux = xaux/width + (y_auxiliar/height) * (x_fereastra/width+1);
            while(xaux <= x_fereastra-10)
            {
                if(s[pozaux] != ' ')
                    x_randsus = xaux;
                xaux += width;
                pozaux = xaux/width + (y_auxiliar/height) * (x_fereastra/width+1);
            }
            pozaux = x_randsus/width + (y_auxiliar/height) * (x_fereastra/width+1);
            pozaux++;
            xaux = x_curent;
            string axu;
            while(xaux <= x_fereastra - x_randsus + width)
            {
                cout << '@' << xaux << " " << x_randsus << endl;
                pozaux2 = xaux/width + y_curent/height * (x_fereastra/width+1);
                axu += s[pozaux2];
                xaux += width;
            }

            for(int i = 0; i <= axu.size()-1; i++)
            {
                s[pozaux] = axu[i];
                pozaux++;
            }
            int xaux =x_randsus+width;
            pozaux = xaux/width + y_curent/height * (x_fereastra/width+1);
            int xcopiere = 0;
            pozaux2 = xcopiere/width + y_curent/height * (x_fereastra/width+1);
            //cout << '#' << xcopiere << " " << xaux << endl;
            while(xcopiere <= x_fereastra-10)
            {
                if(xcopiere < xaux && xaux <= x_fereastra)
                {
                    pozaux = xaux/width + y_curent/height * (x_fereastra/width+1);
                    pozaux2 = xcopiere/width + y_curent/height * (x_fereastra/width+1);
                    s[pozaux2] = s[pozaux];             ///posibil buguri
                    xcopiere+=width;
                    xaux+=width;
                }
                else
                {
                    pozaux2 = xcopiere/width + y_curent/height * (x_fereastra/width+1);
                    s[pozaux2] = ' ';
                    xcopiere+=width;
                }
            }
            gestioneazaBackspace();
        }
    }
    else if(x_curent != 0 && y_curent != 0)
    {
        int poz_aux = x_fereastra / width + (y_curent / height) * (x_fereastra / width + 1);
        poz = (x_curent-width) / width + (y_curent / height) * (x_fereastra / width + 1);
        cout <<'#' << s[poz] << " " << poz_aux;
        bool ok = true;

        for(int i = poz+1; i <= poz_aux && i <= s.size(); i++)
        {
            if(s[i] != ' ' && i <= s.size())
            {
                // cout << '*' << s[i] << " ";
                ok = false;
            }
        }
        if(ok == true || poz+1 == s.size() || poz + 1 == s.size() - 1)
        {
            s[poz] = ' ';
            x_curent -= width;
        }
        else
        {
            for(int i = poz; i<= poz_aux && i < s.size(); i++)
                s[i] = s[i+1];
            s[poz_aux] = ' ';
            x_curent -= width;
        }

    }
    afisare();

}

///ENTER
void gestioneazaEnter()
{
    /*string a = " ";
    int poz = x_curent / width + (y_curent / height) * (x_fereastra / width + 1);
    if (s.size() < (x_curent / width + (y_curent / height) * (x_fereastra / width + 1))) // daca imi iese din string ii dau resize
        s.resize((x_curent / width + (y_curent / height) * (x_fereastra / width + 1)), '*');
    s.insert(poz, a);
    x_curent += width;
    while (x_curent != x_fereastra)
    {
        poz = x_curent / width + (y_curent / height) * (x_fereastra / width + 1);
        s.insert(poz, a);
        if (x_curent > x_fereastra - width)
        {
            x_curent = 0;
            y_curent += height;
        }
        else
            x_curent += width;
    }*/

    int saveY = y_curent;
    int poz = x_curent / width + (y_curent / height) * (x_fereastra / width + 1);
    int poz_max = x_fereastra / width + (y_curent / height) * (x_fereastra / width + 1);
    int poz_auxiliar = poz ;
    string auxiliar = "";
    while(poz_auxiliar < poz_max &&  poz_auxiliar <= s.size())
    {
        auxiliar += s[poz_auxiliar];
        poz_auxiliar++;
    }
    while(poz != poz_max)
    {
        s[poz] = ' ';
        poz++;
    }
    int maxi = x_fereastra/width + 1;
    int lung = auxiliar.size();
    while(lung != maxi)
    {
        auxiliar += ' ';
        lung++;
    }
    poz = x_curent / width + (y_curent / height) * (x_fereastra / width + 1);
    if(s.size() < poz_max+1)
        s.resize(poz_max+1, ' ');
    s.insert(poz_max+1,auxiliar);
    //cout << auxiliar <<auxiliar.size()<< endl;
    //cout << x_curent << " " << y_curent << endl;
    x_curent = 0;
    y_curent = saveY + height;
    //cout << x_curent << " " << y_curent;
    afisare();
}


///SAGETI
void gestioneazaSageti()
{
    c = getch();
    if ((int)c == 75)
    {
        if(y_curent<=45 && x_curent==0)
    {

    }
    else
        if (x_curent == 0)
        {
            x_curent = x_fereastra;
            y_curent -= height;
        }
        else
            if(x_curent-width >= 0)
            x_curent -= width;
    }
    else if ((int)c == 77)
    {
        if (x_curent+width >= x_fereastra)
        {
            x_curent = 0;
            y_curent += height;
        }
        else
            x_curent += width;
    }
    else if ((int)c == 80) ///jos               /// la sagetile sus si jos trebuie modificat umpic pentru ultimul rand unde se duce cu o pozitie mai la dreapta
    {
        /// se rezolva cu doua ifuri

        y_curent += height;
        x_curent = 0;

        int x_auxiliar = 0;
        int poz_auxiliar = x_auxiliar/width + y_curent/height*(x_fereastra/width+1);
        if(poz_auxiliar > s.size())
            x_curent = 0;
        else
        {
            while(x_auxiliar <= x_fereastra-10)
            {
                int poz_auxiliar = x_auxiliar/width + y_curent/height*(x_fereastra/width+1);
                x_auxiliar += width;
                if(caracter(s[poz_auxiliar]) &&  s[poz_auxiliar] != ' ' && s[poz_auxiliar] != '*' && poz_auxiliar < s.size())
                    x_curent = x_auxiliar;
                ///x_auxiliar += width;
            }
        }
    }
    else if ((int)c == 72) ///sus
    {
        cout << x_curent << " " << y_curent;
        if(y_curent > 45)
        {
            y_curent -= height;
            x_curent = 0;
            int x_auxiliar = 0;
            int poz_auxiliar = x_auxiliar/width + y_curent/height*(x_fereastra/width+1);
            if(poz_auxiliar >= s.size())
                x_curent = 0;
            else
            {
                while(x_auxiliar <= x_fereastra-10)
                {
                    int poz_auxiliar = x_auxiliar/width + y_curent/height * (x_fereastra/width + 1);
                    x_auxiliar += width;
                    if(caracter(s[poz_auxiliar]) && s[poz_auxiliar] != ' ' && s[poz_auxiliar] != '*' && poz_auxiliar <= s.size())
                        x_curent = x_auxiliar;
                }
            }

        }

    }

    afisare();
}

///INSERARE
void gestioneazaCaracter()
{
    int poz = x_curent / width + (y_curent / height) * (x_fereastra / width + 1);
    char a = c;
    int poz_aux2;


    if (s.size() < poz) // daca imi iese din string ii dau resize
        s.resize(poz, ' ');
    if(caracter(s[poz]))
    {
        int poz_auxiliar = poz;
        int poz_max = x_fereastra/width + (y_curent/height) *(x_fereastra/width + 1);

        /* while(poz_auxiliar != ' ' && poz_auxiliar <= poz_max && s.size() >= poz_auxiliar)
                        poz_auxiliar++;
        if(poz_auxiliar == ' '){
            /*while(poz_auxiliar > poz){
                s[poz_auxiliar] = s[poz_auxiliar-1];
                poz_auxiliar--;
            }*/
        poz_aux2 = poz_auxiliar;
        while(poz_auxiliar <= poz_max)
        {
            if(caracter(s[poz_auxiliar]) && s[poz_auxiliar] != ' '){
                cout <<'%' <<  poz_aux2 << " " << s[poz_aux2] << endl;
                poz_aux2 = poz_auxiliar;
            }
            poz_auxiliar++;
        }
        if(poz_aux2 != poz_max - 1)
            poz_auxiliar = poz_aux2 + 1;
        else poz_auxiliar = s.size();
        if (s.size() < poz_auxiliar) // daca imi iese din string ii dau resize
            s.resize(poz_auxiliar, ' ');
        if(poz_auxiliar == s.size() && s[poz_auxiliar] == ' ')
            s.erase(poz_auxiliar,1);
        else   if(s[poz_auxiliar] == ' ') s.erase(poz_auxiliar,1);

        s.insert(poz,1,a);
        ///afisare();
        cout <<'2' <<  s << endl;
    }
   /* else if(s[poz] == ' ')
    {
        s[poz] = a;
        cout <<'3' <<  s << endl;
        afisare();
    }*/
    else
    {
        s.insert(poz, 1, a);
        cout <<'4' <<  s << endl;
        afisare();
    }

    if (x_curent > x_fereastra - 12)
    {
        x_curent = 0;

        y_curent += height;
    }
    else
        x_curent += width;
    afisare();

}
/*//inserare punctuatie
void gestioneazapunctuatie()
{
    int poz = x_curent / width + (y_curent / height) * (x_fereastra / width + 1);
    char a = c;


    if (s.size() < poz) // daca imi iese din string ii dau resize
        s.resize(poz, ' ');
    if(caracter(s[poz]) &&  s[poz] != ' ')
    {
        int poz_auxiliar = poz;
        int poz_max = x_fereastra/width + (y_curent/height) *(x_fereastra/width + 1);

        /* while(poz_auxiliar != ' ' && poz_auxiliar <= poz_max && s.size() >= poz_auxiliar)
                        poz_auxiliar++;
        if(poz_auxiliar == ' '){
            /*while(poz_auxiliar > poz){
                s[poz_auxiliar] = s[poz_auxiliar-1];
                poz_auxiliar--;
            }

        while(poz_auxiliar <= s.size())
        {
            if(s[poz_auxiliar] == ' ')
                break;
            poz_auxiliar++;
        }
        if (s.size() < poz_auxiliar)
            s.resize(poz_auxiliar, ' ');
        if(poz_auxiliar == s.size() && s[poz_auxiliar] == ' ')
            s.erase(poz_auxiliar,1);
        else   if(s[poz_auxiliar] == ' ') s.erase(poz_auxiliar,1);

        s.insert(poz,1,a);
        ///afisare();
        cout <<'2' <<  s << endl;
    }
    else if(s[poz] == ' ')
    {
        s[poz] = a;
        cout <<'3' <<  s << endl;
        afisare();
    }
    else
    {
        s.insert(poz, 1, a);
        cout <<'4' <<  s << endl;
        afisare();
    }

    int aux1,aux2;
    if (x_curent > x_fereastra - 21)
    {
        x_curent = 0;

        y_curent += 21;
    }
    else
        x_curent += 12;
    afisare();

}*/

///CLICK SCURT
void gestioneazaClickStangaScurt()
{
    int x_auxiliar;
    int y_auxiliar;
    getmouseclick(WM_LBUTTONDOWN, x_auxiliar, y_auxiliar);
    x_auxiliar = x_auxiliar - (x_auxiliar%width);
    y_auxiliar = y_auxiliar - (y_auxiliar%height);
    x_curent = x_auxiliar;
    y_curent = y_auxiliar;
    x_auxiliar = 0;
    int x_poz;
    int poz_auxiliar = x_auxiliar/width + y_curent/height * (x_fereastra/width + 1);
    if(poz_auxiliar >= s.size())
        x_curent = 0;
    else
    {
        while(x_auxiliar != x_fereastra)
        {
            int poz_auxiliar = x_auxiliar/width + y_curent/height * (x_fereastra/width + 1);
            x_auxiliar += width;
            if(caracter(s[poz_auxiliar]) && s[poz_auxiliar] != ' ' && s[poz_auxiliar] != '*' && poz_auxiliar < s.size())
                x_poz = x_auxiliar;
        }
        ///cout << x_poz << " " << x_curent  <<"cartof"<< endl;
        if(x_poz < x_curent)  /// la fel ca la sageata sus si jos la ultimul rand/ ultimul element din s se duce un element mai la dreapta
            x_curent = x_poz;
    }
    afisare();
}




///SELECTIE
void gestioneazaClickStangaSelectie(bool selectat[])
{
    int x,y;
    int xStart, yStart;
    int pozCurenta;
    int pozInitiala;
    ///bool selectat[s.size()];
    for(int i = 0; i <= s.size(); i++)
        selectat[i] = false;
    getmouseclick(WM_LBUTTONDOWN, x, y);
    xStart = x;
    yStart = y;
    xStart = xStart - xStart%width;
    yStart = yStart - yStart%height;
    x_curent = xStart;
    y_curent = yStart;
    pozCurenta = xStart/width + yStart/height * (x_fereastra/width+1);
    pozInitiala = pozCurenta;
    selectat[pozCurenta] = true;
    while(!ismouseclick(WM_LBUTTONUP))
    {
        if(ismouseclick(WM_MOUSEMOVE))
        {
            for(int i = 0; i <= s.size(); i++)
                selectat[i] = false;
            getmouseclick(WM_MOUSEMOVE, x, y);
            x = x - x % width;
            y = y - y % height;
            if(pozCurenta < pozInitiala)
            {
                x_curent = xStart + width;
            }
            else                                                   /// trebuie sa mai verific daca pozitia pe care vreau sa
            {
                x_curent = x + width;
                y_curent = y;
            }                                                                /// modificari sa imi lase x_curent/ y_curent unde trebe
            pozCurenta = x/width  + y/height * (x_fereastra/width + 1);
            if(pozCurenta > s.size())
            {
                continue;
            }
            else
            {
                if(pozCurenta <= pozInitiala)
                {
                    for(int i = pozCurenta; i <= pozInitiala; i++)
                    {
                        selectat[i] = true;
                    }
                }
                else if(pozCurenta >= pozInitiala)
                {
                    for(int i = pozInitiala; i <= pozCurenta; i++)
                        selectat[i] = true;
                }
                afisareClick(selectat);
            }
        }
    }
    getmouseclick(WM_LBUTTONUP,x,y);
    clearmouseclick(WM_LBUTTONUP);
    clearmouseclick(WM_LBUTTONDOWN);

    x = x - x % width;
    y = y - y % height;
    x_curent = x;
    y_curent = y;
    selectatText = true;
    //drawButonClickStangaSelectie(x_curent, y_curent);

    if(x_curent == xStart && y_curent == yStart)
    {
        selectatText = false;
        int ultima_pozitie;
        pozCurenta = x_curent/width + y_curent/height * (x_fereastra/width+1);
        int pozmax = x_fereastra/width + y_curent/height * (x_fereastra/width+1);
        int pozmin = y_curent/height * (x_fereastra/width+1);
        bool ok1 = false, ok2 = false;
        for(int i = pozCurenta + 1; i <= pozmax && i <= s.size(); i++)
        {
            if(caracter(s[i]) && s[i] != ' ')
                ok1 = true;
        }
        for(int i = pozmin; i < pozCurenta && i <= s.size(); i++)
            if(caracter(s[i]) && s[i] != ' ')
            {
                ok2 = true;
                ultima_pozitie = i;
            }
        if(ok2 == true && ok1 == false)
        {
            ultima_pozitie -= pozmin;
            ultima_pozitie *= width;
            x_curent = ultima_pozitie + width;
            pozCurenta = x_curent/width + y_curent/height*(x_fereastra/width+1);
        }
        else if(ok2 == false && ok1 == false)
            x_curent = 0;
        //   cleardevice();
        afisare();
    }

}



///TAB
void gestioneazaTab()
{
    int cnt = 1;
    c = ' ';
    while(cnt <= 4)
    {
        gestioneazaCaracter();
        cnt++;
    }
}
///UNDO
void gestioneazaUndo()
{
    if(undo.size() > 1)undo.pop();
    tuple<string, int, int> curent_tuple = undo.top();
    if(get<0>(curent_tuple).size() > 0)
    {

        s = get<0>(curent_tuple);
        x_curent = get<1>(curent_tuple);
        y_curent = get<2>(curent_tuple);
        afisare();
    }
}


///TEMA
void gestionaretema() //ok e ca sa stiu pe ce patratica am apasat ca sa pun ce tema trebuie
{
    if(ok==0)
    {
        setcolor(BLACK);
        setbkcolor(WHITE);
    }
    else if(ok==1)
    {
        setcolor(WHITE);
        setbkcolor(BLACK);
    }
    else if(ok==2)
    {
        setcolor(BLUE);
        setbkcolor(MAGENTA);
    }

}
///culoare
void gestioneazaculoare()
{
    if(culoare==0)
        setTextColor(BLACK);
    else if(culoare==1)
        setTextColor(RED);
    else if(culoare==2)
        setTextColor(BLUE);
    else if(culoare==3)
        setTextColor(GREEN);
}
///tema
void gestionaremarime( int marimetext)
{

    settextstyle(font, HORIZ_DIR, marimetext);
    width=textwidth("M");
    height=textheight("M");

}
///resize
void resizetext()
{
    int poz = (45 / height) * (x_fereastra / width + 1)+1;
    if(s[poz]==' ')
        while(s[poz]==' ')
            s.erase(s.begin());
    else
    {
        int pozaux=0;
        for(int j=0; j<s.size(); j++)
        {
            if(s[j]==' ')
                pozaux++;
            else
                break;
        }
        while(pozaux<=poz)
        {
            pozaux++;
            s.insert(s.begin(),' ');
        }
    }
    x_curent=0;
    y_curent=45;
    cursor();


}
///desenare linie coloana
void afisarepos()
{
    char positionText[50];
    sprintf(positionText, "Linia: %d, Coloana: %d", ((y_curent - 40) / height + 1), (x_curent / width));

    setcolor(BLACK);
    settextstyle(10, HORIZ_DIR, 2);
    if(ok==0)
        setfillstyle(SOLID_FILL, LIGHTGRAY);
    else if(ok==1)
    {
        setfillstyle(SOLID_FILL, DARKGRAY);
        setcolor(WHITE);
    }
    else
    {
        setfillstyle(SOLID_FILL, BLACK);
        setcolor(BLUE);
    }
    bar(0, getmaxy() - 25, getmaxx(), getmaxy());
    outtextxy(10, getmaxy() - 20, positionText);

}


int main()
{
    initializare_fereastra();
    int inchidere_fereastra = true;
    gestioneazaButoane();
    afisarepos();
    free(image);
    image=malloc(imagesize(0,0,getmaxx(),getmaxy()));
    getimage(0,0,getmaxx(),getmaxy(),image);
    setactivepage(page);
    putimage(0,0,image,1);
    setvisualpage(1-page);
    page=1-page;
    while (inchidere_fereastra)
    {
        /*if(mousey()<=30)
        {
            for(int i=0;i<6;i++)
            if(isMouseOverButton(mousex(),mousey(),buttonX[i],buttonY, buttonWidth, buttonHeight))
            afisare();

        }*/
        bool selectat[s.size()];
        if(ismouseclick(WM_LBUTTONDOWN))
        {
///scroll
            // setactivepage(page);
            // putimage(0,0,image,1);
            //setvisualpage(1-page);
            x1_scroll = getmaxx() - 8;
            x2_scroll = getmaxx() - 2;
            if(mousex() >= x1_scroll && mousex() <= x2_scroll)
            {
                ismouseclick(WM_MOUSEWHEEL);
            }

            if(ismouseclick(WM_MOUSEWHEEL))
            {
                WPARAM wParam;
                int directie = GET_WHEEL_DELTA_WPARAM(wParam);
                int x, y;
                getmouseclick(WM_MOUSEWHEEL,x,y);
                if(directie > 0)
                {
                    y_curent -= height;
                }
                else if(directie < 0)
                {
                    y_curent += height;
                }
                afisare();
                clearmouseclick(WM_MOUSEWHEEL);
            }

            else if(mousey()>45 && toatecompacte())
            {
                /// bool selectat[s.size()];
                gestioneazaClickStangaSelectie(selectat);

            }


///buton 1 FILE
            else if(buttonStates[0] == COMPACT&& mousex()>=0 && mousex()<90) /// daca butonul e compact
            {
                if(mousey()<=30) ///daca dau clic pe el trebuie sa devina extins
                {
                    cleardevice();
                    gestioneazaClicButon();
                    afisare();

                }
                else
                {
                    if(mousey()>45)
                        gestioneazaClickStangaSelectie(selectat);

                }

            }

            else if(buttonStates[0] == EXTENDED && mousex()>=0 && mousex()<90) /// daca butonul e extins VIEW
            {
                if(mousey()<=150)
                {
                    if(mousey()<=30) ///daca dau clic pe el trebuie sa devina compact inapoi
                    {
                        gestioneazaClicButon();
                        afisare();

                    }
                    else if (mousey()>30 && mousey()<=60) ///save as
                    {
                        saveToFile(s);
                        afisare();
                    }
                    else if (mousey()>60 && mousey()<=90)///open
                    {
                        dinfisier=1;
                        s = readFromFile();
                        afisare();

                    }
                    else if (mousey()>90 && mousey()<=120)///delete
                    {
                        deleteFilecartof();
                    }
                    else if (mousey()>120 && mousey()<=150)///exit
                    {
                        return 0;
                    }
                    clearmouseclick(WM_LBUTTONDOWN);
                }
                else
                {
                    gestioneazaClickStangaSelectie(selectat);
                }
            }




///buton 2 colors
            else if(buttonStates[1] == COMPACT&& mousex()>=90 && mousex()<180) /// daca butonul e compact
            {
                if(mousey()<=30) ///daca dau clic pe el trebuie sa devina extins
                {
                    cleardevice();
                    gestioneazaClicButon();
                    afisare();

                }
                else
                {
                    if(mousey()>45)
                        gestioneazaClickStangaSelectie(selectat);


                }

            }

            else if(buttonStates[1] == EXTENDED && mousex()>=90 && mousex()<180) /// daca butonul e extins FONT
            {
                if(mousey()<=150)
                {
                    if(mousey()<=30) ///daca dau clic pe el trebuie sa devina compact inapoi
                    {
                        gestioneazaClicButon();
                        afisare();

                    }
                    else if (mousey()>30 && mousey()<=60) ///black
                    {
                        setTextColor(BLACK);
                        culoare=0;

                        afisare();
                    }
                    else if (mousey()>60 && mousey()<=90) ///red
                    {
                        setTextColor(RED);
                        culoare=1;
                        afisare();

                    }
                    else if (mousey()>90 && mousey()<=120)///blue
                    {
                        setTextColor(BLUE);
                        culoare=2;

                        afisare();

                    }
                    else if (mousey()>120 && mousey()<=150) ///green
                    {
                        setTextColor(GREEN);

                        culoare=3;
                        afisare();
                    }
                    clearmouseclick(WM_LBUTTONDOWN);
                }
                else
                {
                    gestioneazaClickStangaSelectie(selectat);
                }
            }



///buton 3 VIEW
            else if(buttonStates[2] == COMPACT&& mousex()>=180 && mousex()<270) /// daca butonul e compact
            {
                if(mousey()<=30) ///daca dau clic pe el trebuie sa devina extins
                {
                    // cleardevice();
                    gestioneazaClicButon();
                    afisare();

                }
                else
                {
                    if(mousey()>45)
                        gestioneazaClickStangaSelectie(selectat);

                }

            }

            else if(buttonStates[2] == EXTENDED && mousex()>=180 && mousex()<270) /// daca butonul e extins VIEW
            {
                if(mousey()<=180)
                {
                    if(mousey()<=30) ///daca dau clic pe el trebuie sa devina compact inapoi
                    {
                        gestioneazaClicButon();
                        afisare();

                    }
                    else if (mousey()>30 && mousey()<=60)///zoom in
                    {
                        if(marimetext==2)
                                marimetext=5;
                        rs=1;
                        afisare();
                    }
                    else if (mousey()>60 && mousey()<=90)/// zoom out
                    {
                         if(marimetext==5)
                            marimetext=2;
                        rs=1;
                        afisare();

                    }
                    else if (mousey()>90 && mousey()<=120)/// default
                    {
                        marimetext=2;
                        rs=1;
                        font=10;
                        afisare();

                    }
                    else if (mousey()>120 && mousey()<=150)/// font 1
                    {
                        font=5;
                        rs=1;
                        afisare();

                    }
                    else if (mousey()>150 && mousey()<=180)/// font 2
                    {
                        font=8;
                        rs=1;
                        afisare();

                    }

                    clearmouseclick(WM_LBUTTONDOWN);

                }
                else
                {
                    gestioneazaClickStangaSelectie(selectat);
                }
            }



///buton 4 TEMA
            else if(buttonStates[3] == COMPACT&& mousex()>=270 && mousex()<360) /// daca butonul e compact
            {
                if(mousey()<=30) ///daca dau clic pe el trebuie sa devina extins
                {
                    cleardevice();
                    gestioneazaClicButon();
                    afisare();

                }
                else
                {
                    if(mousey()>45)
                        gestioneazaClickStangaSelectie(selectat);


                }

            }

            else if(buttonStates[3] == EXTENDED && mousex()>=270 && mousex()<360) /// daca butonul e extins TEMA
            {
                if(mousey()<=120)
                {
                    if(mousey()<=30) ///daca dau clic pe el trebuie sa devina compact inapoi
                    {
                        gestioneazaClicButon();
                        afisare();

                    }
                    else if (mousey()>30 && mousey()<=60)///light
                    {
                        ok=0;
                        culoare=0;
                    }
                    else if (mousey()>60 && mousey()<=90)///dark
                    {
                        ok=1;
                        culoare=5;
                    }
                    else if (mousey()>90 && mousey()<=120)///space
                    {
                        ok=2;
                        //culoare=3;
                    }
                    clearmouseclick(WM_LBUTTONDOWN);
                    cleardevice();
                    gestioneazaButoane();
                    gestionaretema();
                    afisare();

                }
                else
                {
                    gestioneazaClickStangaSelectie(selectat);
                }
            }




///buton 5 EDIT
            else if(buttonStates[4] == COMPACT&& mousex()>=360 && mousex()<450) /// daca butonul e compact
            {
                if(mousey()<=30) ///daca dau clic pe el trebuie sa devina extins
                {
                    cleardevice();
                    gestioneazaClicButon();
                    afisare();

                }
                else
                {
                    if(mousey()>45)
                        gestioneazaClickStangaSelectie(selectat);

                }

            }

            else if(buttonStates[4] == EXTENDED && mousex()>=360 && mousex()<450) /// daca butonul e extins edit
            {
                if(mousey()<=180)
                {
                    if(mousey()<=30) ///daca dau clic pe el trebuie sa devina compact inapoi
                    {
                        gestioneazaClicButon();
                        afisare();

                    }
                    else if (mousey()>30 && mousey()<=60)///copy
                    {
                        textCpy = s;
                        ctra = false;
                        afisare();
                        afisare();
                        clearmouseclick(WM_LBUTTONDOWN);
                    }
                    else if (mousey()>60 && mousey()<=90)///paste
                    {
                        for(int i = 0; i < textCpy.size(); i++)
                        {
                            c = textCpy[i];
                            gestioneazaCaracter();
                        }
                        afisare();
                        clearmouseclick(WM_LBUTTONDOWN);
                    }

              else if (mousey()>90 && mousey()<=120)///small
                    {
                         closegraph();
                        x_fereastra = 590;
                        y_fereastra2 = 470;
                        y_fereastra=y_fereastra2-25;
                        initializare_fereastra();
                        gestioneazaButoane();
                        afisarepos();
                        free(image);
                        image=malloc(imagesize(0,0,getmaxx(),getmaxy()));
                        getimage(0,0,getmaxx(),getmaxy(),image);
                        setactivepage(page);
                        putimage(0,0,image,1);
                        setvisualpage(1-page);
                        page=1-page;
                        afisare();
                        clearmouseclick(WM_LBUTTONDOWN);
                    }
                    else if (mousey()>120 && mousey()<=150)///full
                    {
                        closegraph();
                        x_fereastra = 1200;
                        initializare_fereastra();
                        gestioneazaButoane();
                        afisarepos();
                        free(image);
                        image=malloc(imagesize(0,0,getmaxx(),getmaxy()));
                        getimage(0,0,getmaxx(),getmaxy(),image);
                        setactivepage(page);
                        putimage(0,0,image,1);
                        setvisualpage(1-page);
                        page=1-page;
                        afisare();
                        clearmouseclick(WM_LBUTTONDOWN);
                    }
                    else if (mousey()>150 && mousey()<=180)///undo
                    {
                        gestioneazaUndo();
                        clearmouseclick(WM_LBUTTONDOWN);
                    }


                }
                else
                {
                    gestioneazaClickStangaSelectie(selectat);
                }
            }
            else if(buttonStates[5] == COMPACT&& mousex()>=450 && mousex()<540) /// daca butonul e compact
            {
                if(mousey()<=30) ///daca dau clic pe el trebuie sa devina extins
                {
                    // cleardevice();
                    gestioneazaClicButon();
                    afisare();

                }
                else
                {
                    gestioneazaClickStangaSelectie(selectat);

                }

            }

            else if(buttonStates[5] == EXTENDED && mousex()>=450 && mousex()<540) /// daca butonul e extins VIEW
            {
                if(mousey()<=60)
                {
                    if(mousey()<=30) ///daca dau clic pe el trebuie sa devina compact inapoi
                    {
                        gestioneazaClicButon();
                        afisare();

                    }
                    else if (mousey()>30 && mousey()<=60) ///info
                    {
                        MessageBox(NULL, "Proiectul a fost realizat de Tanasa Ionut si Cotea Carla", "Info", MB_OK | MB_ICONINFORMATION);
                        afisare();
                    }

                    clearmouseclick(WM_LBUTTONDOWN);

                }
                else
                {
                    gestioneazaClickStangaSelectie(selectat);
                }
            }
        }
        else if(ismouseclick(WM_RBUTTONDOWN))
        {
            int x, y, x_move, y_move, x1, y1;
            getmouseclick(WM_RBUTTONDOWN,x,y);
            x1_scroll = getmaxx() - 10;
            x2_scroll = getmaxx() ;
            x_move = x;
            y_move = y/2;
            bool seMisca = true;
            while(!ismouseclick(WM_RBUTTONUP))
            {
                if(ismouseclick(WM_MOUSEMOVE))
                {
                    getmouseclick(WM_MOUSEMOVE, x1,y1);
                    if(y1/2 > y_move)
                        y_curent += height;
                    if(y1/2  < y_move)
                        if(y1/2  < y_move)
                            y_curent -= height;
                    if(y_curent < height)
                        y_curent = height;
                    else if(y_curent > yAbsolut)
                        y_curent = yAbsolut;
                    y_move = y1/2;
                }
                afisare();
            }
            clearmouseclick(WM_MOUSEMOVE);
            clearmouseclick(WM_RBUTTONUP);



            clearmouseclick(WM_RBUTTONDOWN);
        }







///tastatura

        if (kbhit())
        {
            // setactivepage(page);
            // putimage(0,0,image,1);
            // setvisualpage(1-page);
            c = getch();
            if ((int)c == 8)
            {
                if(ctra)
                {
                    s = "";
                    x_curent = 0;
                    y_curent = 45;
                    afisare();
                    ctra = false;
                }
                else
                {
                    if(selectatText)
                    {
                        int x,y;
                        x = x_curent;
                        y = y_curent;
                        int ok = true;
                        for(int i = s.size(); i >= 0; i--)
                        {
                            if(selectat[i] == true)
                            {
                                //if(ok){ok = false;}
                                //else{
                                x_curent = i%numerePeRand*width;
                                y_curent = i/numerePeRand * height;
                                x_curent += width;
                                gestioneazaBackspace();
                                //}
                            }
                        }
                        //x_curent = x;
                        //y_curent = y;
                        selectatText = false;
                        afisare();

                    }
                    else
                    {
                        undo.push(make_tuple(s,x_curent, y_curent));
                        gestioneazaBackspace();
                        undo.push(make_tuple(s,x_curent, y_curent));
                    }
                }
            }
            if((int)c == 9)
            {
                //undo.push(s);
                if(ctra)
                {
                    ctra = false;
                    afisare();
                    gestioneazaTab();
                }
                else
                {
                    gestioneazaTab();
                }
            }
            else if((int)c == 27)
            {
                inchidere_fereastra = false;
                break;
            }
            else if ((int)c == 13)
            {
                if(ctra)
                {
                    ctra = false;
                    if(s.size() > 1)
                        undo.push(make_tuple(s,x_curent, y_curent));
                    gestioneazaEnter();
                    undo.push(make_tuple(s,x_curent, y_curent));
                }
                else
                {
                    if(s.size() > 1)
                        undo.push(make_tuple(s,x_curent, y_curent));
                    gestioneazaEnter();
                    undo.push(make_tuple(s,x_curent, y_curent));
                }

            }
            else if ((int)c == 224 || (int)c == 0)
            {
                if(ctra)
                    ctra = false;
                gestioneazaSageti();
            }
            else if (caracter(c))
            {
                if(ctra)
                    ctra = false;
                gestioneazaCaracter();
                // undo.push(make_tuple(s,x_curent,y_curent));

            }
            else if (caracter(c))
            {
                if(ctra)
                    ctra = false;

                // undo.push(make_tuple(s,x_curent,y_curent));

            }
            else if((int) c == 26)
            {
                if(ctra)
                    ctra = false;
                gestioneazaUndo();
            }
            else if((int)c == 3)
            {
                if(selectatText)
                {
                    textCpy = "";
                    for(int i = 0; i <= s.size(); i++)
                    {
                        if(selectat[i] == true)
                            textCpy += s[i];
                    }
                    //     cout << textCpy << " " ;
                    selectatText = false;
                    afisare();
                }
                if(ctra)
                {
                    textCpy = s;
                    ctra = false;
                    afisare();
                }

            }
            else if((int)c == 22)
            {
                for(int i = 0; i < textCpy.size(); i++)
                {
                    c = textCpy[i];
                    gestioneazaCaracter();
                }
            }
            else if((int)c == 46)
            {
                saveToFile(s);
                afisare();
            }
            else if((int)c == 44)
            {
                dinfisier=1;
                s = readFromFile();
                afisare();
            }
            else if((int)c == 47)
            {
                deleteFilecartof();
            }
            else if((int)c == 1)
            {
                for(int i = 0; i <= s.size()-1; i++)
                    selectat[i] = 1;
                afisareClick(selectat);
                ctra = true;
            }

        }
        if(y_curent > yAbsolut)
        {
            yAbsolut = y_curent;
        }

    }
    return 0;
}






