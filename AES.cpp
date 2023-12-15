#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cmath>

using namespace std;

string state[5][5];
string cipherkey[5][5];
string sbox[17][17];
string invsbox[17][17];
string rcon[5][11];

int vong = 1;
int mbox[5][5];
int invmbox[5][5];

string Get2FromInt(int n)
{
    string da = "";
    while (n > 0)
    {
        if (n % 2 == 0)
        {
            da = "0" + da;
        }
        else
        {
            da = "1" + da;
        }
        n /= 2;
    }
    while (da.length() < 0)
    {
        da = "0" + da;
    }
    return da;
}

int GetIntFrom2(string s)
{
    int da = 0;
    int n = s.length();
    for (int i = 0; i < n; i++)
    {
        if (s[i] == '1')
        {
            da = da + pow(2, n - 1 - i);
        }
    }
    return da;
}

string Get2From16(string sHex)
{
    string sReturn = "";
    for (int i = 0; i < sHex.length(); i++)
    {
        switch (sHex[i])
        {
        case '0':
            sReturn.append("0000");
            break;
        case '1':
            sReturn.append("0001");
            break;
        case '2':
            sReturn.append("0010");
            break;
        case '3':
            sReturn.append("0011");
            break;
        case '4':
            sReturn.append("0100");
            break;
        case '5':
            sReturn.append("0101");
            break;
        case '6':
            sReturn.append("0110");
            break;
        case '7':
            sReturn.append("0111");
            break;
        case '8':
            sReturn.append("1000");
            break;
        case '9':
            sReturn.append("1001");
            break;
        case 'a':
            sReturn.append("1010");
            break;
        case 'b':
            sReturn.append("1011");
            break;
        case 'c':
            sReturn.append("1100");
            break;
        case 'd':
            sReturn.append("1101");
            break;
        case 'e':
            sReturn.append("1110");
            break;
        case 'f':
            sReturn.append("1111");
            break;
        }
    }
    return sReturn;
}

string Get16From2(string sBinary)
{
    string rest = "", tmp, chr = "0000";
    int len = sBinary.length() / 4;
    chr = chr.substr(0, len);
    for (int i = 0; i < sBinary.length(); i += 4)
    {
        tmp = sBinary.substr(i, 4);
        if (!tmp.compare("0000"))
        {
            rest = rest + "0";
        }
        else if (!tmp.compare("0001"))
        {
            rest = rest + "1";
        }
        else if (!tmp.compare("0010"))
        {
            rest = rest + "2";
        }
        else if (!tmp.compare("0011"))
        {
            rest = rest + "3";
        }
        else if (!tmp.compare("0100"))
        {
            rest = rest + "4";
        }
        else if (!tmp.compare("0101"))
        {
            rest = rest + "5";
        }
        else if (!tmp.compare("0110"))
        {
            rest = rest + "6";
        }
        else if (!tmp.compare("0111"))
        {
            rest = rest + "7";
        }
        else if (!tmp.compare("1000"))
        {
            rest = rest + "8";
        }
        else if (!tmp.compare("1001"))
        {
            rest = rest + "9";
        }
        else if (!tmp.compare("1010"))
        {
            rest = rest + "a";
        }
        else if (!tmp.compare("1011"))
        {
            rest = rest + "b";
        }
        else if (!tmp.compare("1100"))
        {
            rest = rest + "c";
        }
        else if (!tmp.compare("1101"))
        {
            rest = rest + "d";
        }
        else if (!tmp.compare("1110"))
        {
            rest = rest + "e";
        }
        else if (!tmp.compare("1111"))
        {
            rest = rest + "f";
        }
        else
        {
            continue;
        }
    }
    return rest;
}

string xtime(string temps)
{
    temps = Get2From16(temps);
    string tempbit = "00011011";
    char cc = temps[0];
    for (int i = 0; i <= 6; i++)
    {
        temps[i] = temps[i + 1];
    }
    temps[7] = '0';
    if (cc == '1')
    {
        for (int i = 0; i <= 7; i++)
        {
            if (temps[i] == tempbit[i])
                temps[i] = '0';
            else
                temps[i] = '1';
        }
    }
    return Get16From2(temps);
}

void dichbyte(int i)
{
    string temp1;
    temp1 = state[i][1];
    state[i][1] = state[i][2];
    state[i][2] = state[i][3];
    state[i][3] = state[i][4];
    state[i][4] = temp1;
}

string nhanmbox(string temps, int x)
{
    temps = Get2From16(temps);
    string tempbit = "00011011";
    if (x == 1)
    {
        return temps;
    }
    if (x == 2)
    {
        char cc = temps[0];
        for (int i = 0; i <= 6; i++)
        {
            temps[i] = temps[i + 1];
        }
        temps[7] = '0';
        if (cc == '1')
        {
            for (int i = 0; i <= 7; i++)
            {
                if (temps[i] == tempbit[i])
                    temps[i] = '0';
                else
                    temps[i] = '1';
            }
        }
        return temps;
    }
    if (x == 3)
    {
        string da = temps;
        char cc = temps[0];
        for (int i = 0; i <= 6; i++)
        {
            temps[i] = temps[i + 1];
        }
        temps[7] = '0';
        if (cc == '1')
        {
            for (int i = 0; i <= 7; i++)
            {
                if (temps[i] == tempbit[i])
                    temps[i] = '0';
                else
                    temps[i] = '1';
            }
        }
        for (int i = 0; i <= 7; i++)
        {
            if (da[i] == temps[i])
                da[i] = '0';
            else
                da[i] = '1';
        }
        return da;
    }
    return temps;
}

void getRcon(int i, string da[5])
{
    for (int j = 0; j <= 4; j++)
    {
        da[j] = rcon[j][i];
    }
    return;
}

string Xorbin(string s1, string s2)
{
    string da = s1;
    for (int i = 0; i < 7; i++)
    {
        if (s1[i] == s2[i])
            da[i] = '0';
        else
            da[i] = '1';
    }
    return da;
}

string invnhanmbox(string temps, int x)
{
    if (x == 9)
    {
        return Xorbin(Get2From16(xtime(xtime(xtime(temps)))), Get2From16(temps));
    }
    Xorbin(Get2From16(xtime(xtime(xtime(temps)))), Get2From16(temps));
    if (x == 11)
    {
        return Xorbin(Xorbin(Get2From16(xtime(temps)), Get2From16(xtime(xtime(xtime(temps))))), Get2From16(temps));
    }
    if (x == 13)
    {
        return Xorbin(Xorbin(Get2From16(xtime(xtime(temps))), Get2From16(temps)), Get2From16(xtime(xtime(xtime(temps)))));
    }
    if (x == 14)
    {
        return Xorbin(Xorbin(Get2From16(xtime(xtime(temps))), Get2From16(xtime(temps))),
                      Get2From16(xtime(xtime(xtime(temps)))));
    }
    return temps;
}

void Nhap()
{
    cout << "nhap text (length=16):";
    string s;
    getline(cin, s);
    while (s.length() < 16)
    {
        s = s + " ";
    }
    cout << s << endl;
    int ii = 0;
    for (int i = 0; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            char temp = s[ii];
            state[i][j] = Get16From2(Get2FromInt((int)temp));
            ii++;
        }
    }
    cout << "nhap key (length=16):";
    string sk;
    getline(cin, sk);
    while (sk.length() < 16)
    {
        sk = sk + " ";
    }
    cout << sk << endl;
    ii = 0;
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            char temp = sk[ii];
            cipherkey[i][j] = Get16From2(Get2FromInt((int)temp));
            ii++;
        }
    }
}

void xuat()
{
    cout << endl
         << "=========================" << endl
         << "text giai ma:" << endl;
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            cout << (char)(GetIntFrom2(Get2From16(state[i][j])));
        }
    }
}

void init()
{
    fstream fin2("sbox.txt", ios::in);
    for (int i = 0; i <= 15; i++)
    {
        for (int j = 0; j <= 15; j++)
        {
            fin2 >> sbox[i][j];
        }
    }
    fin2.close();
    fstream fin22("invsbox.txt", ios::in);
    for (int i = 0; i <= 15; i++)
    {
        for (int j = 0; j <= 15; j++)
        {
            fin22 >> invsbox[i][j];
        }
    }
    fin22.close();
    fstream fin3("mixcolumn.txt", ios::in);
    for (int i = 0; i <= 4; i++)
    {
        for (int j = 0; j <= 4; j++)
        {
            fin3 >> mbox[i][j];
        }
    }
    fin3.close();
    fstream fin33("invmixcolumn.txt", ios::in);
    for (int i = 0; i <= 4; i++)
    {
        for (int j = 0; j <= 4; j++)
        {
            fin33 >> invmbox[i][j];
        }
    }
    fin33.close();
    fstream fin4("rcon.txt", ios::in);
    for (int i = 0; i <= 4; i++)
    {
        for (int j = 0; j <= 10; j++)
        {
            fin4 >> rcon[i][j];
        }
    }
    fin4.close();
}

void show()
{
    cout << endl
         << "state" << endl;
    for (int i = 1; i < -4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            cout << state[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl
         << "cipher" << endl;
    for (int i = 1; i < -4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            cout << cipherkey[i][j] << " ";
        }
        cout << endl;
    }
}

void addRoundKey()
{
    string rconi[5];
    getRcon(vong, rconi);
    string temp[5];
    for (int i = 1; i <= 3; i++)
    {
        temp[i] = cipherkey[i + 1][4];
    }
    temp[4] = cipherkey[1][4];
    for (int i = 1; i <= 4; i++)
    {
        int x, y;
        string s = temp[i];
        if (s[0] >= '0' && s[0] <= '9')
        {
            x = s[0] - 48;
        }
        else
        {
            if (s[0] == 'a')
                x = 10;
            if (s[0] == 'b')
                x = 11;
            if (s[0] == 'c')
                x = 12;
            if (s[0] == 'd')
                x = 13;
            if (s[0] == 'e')
                x = 14;
            if (s[0] == 'f')
                x = 15;
        }
        if (s[1] >= '0' && s[1] <= '9')
        {
            y = s[1] - 48;
        }
        else
        {
            if (s[1] == 'a')
                y = 10;
            if (s[1] == 'b')
                y = 11;
            if (s[1] == 'c')
                y = 12;
            if (s[1] == 'd')
                y = 13;
            if (s[1] == 'e')
                y = 14;
            if (s[1] == 'f')
                y = 15;
        }
        s = sbox[x][y];
        string nptemp = Get2From16(cipherkey[i][1]);
        string nps = Get2From16(s);
        string nprconi = Get2From16(rconi[i]);
        string tempda = nps;

        for (int ii = 0; ii <= 7; ii++)
        {
            int dem = 0;
            if (nptemp[ii] == '1')
                dem++;
            if (nps[ii] == '1')
                dem++;
            if (nprconi[ii] == '1')
                dem++;
            if (dem % 2 == 0)
                tempda[ii] = '0';
            else
                tempda[ii] = '1';
        }
        cipherkey[i][1] = Get16From2(tempda);
    }
    for (int i = 2; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            cipherkey[j][i] = Get16From2(Xorbin(Get2From16(cipherkey[j][i - 1]), Get2From16(cipherkey[j][i])));
        }
    }
}

void subByte()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            string s = state[i][j];
            int x, y;
            if (s[0] >= '0' && s[0] <= '9')
            {
                x = s[0] - 48;
            }
            else
            {
                if (s[0] == 'a')
                    x = 10;
                if (s[0] == 'b')
                    x = 11;
                if (s[0] == 'c')
                    x = 12;
                if (s[0] == 'd')
                    x = 13;
                if (s[0] == 'e')
                    x = 14;
                if (s[0] == 'f')
                    x = 15;
            }
            if (s[1] >= '0' && s[1] <= '9')
            {
                y = s[1] - 48;
            }
            else
            {
                if (s[1] == 'a')
                    y = 10;
                if (s[1] == 'b')
                    y = 11;
                if (s[1] == 'c')
                    y = 12;
                if (s[1] == 'd')
                    y = 13;
                if (s[1] == 'e')
                    y = 14;
                if (s[1] == 'f')
                    y = 15;
            }
            state[i][j] = sbox[x][y];
        }
    }
}

void shiftRows()
{
    dichbyte(2);
    dichbyte(3);
    dichbyte(3);
    dichbyte(4);
    dichbyte(4);
    dichbyte(4);
}

void mixColumn()
{
    for (int j = 1; j <= 4; j++)
    {
        string temp[5];
        temp[1] = state[1][j];
        temp[2] = state[2][j];
        temp[3] = state[3][j];
        temp[4] = state[4][j];
        string da[5];
        for (int i = 1; i <= 4; i++)
        {
            string np1 = nhanmbox(temp[1], mbox[i][1]);
            string np2 = nhanmbox(temp[2], mbox[i][2]);
            string np3 = nhanmbox(temp[3], mbox[i][3]);
            string np4 = nhanmbox(temp[4], mbox[i][4]);
            string np = np1;
            for (int ii = 0; ii <= 7; ii++)
            {
                int dem = 0;
                if (np1[ii] == '1')
                    dem++;
                if (np2[ii] == '1')
                    dem++;
                if (np3[ii] == '1')
                    dem++;
                if (np4[ii] == '1')
                    dem++;
                if (dem % 2 == 0)
                    np[ii] = '0';
                else
                    np[ii] = '1';
            }
            da[i] = Get16From2(np);
        }
        state[1][j] = da[1];
        state[2][j] = da[2];
        state[3][j] = da[3];
        state[4][j] = da[4];
    }
}

void planTextAddKey()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            state[i][j] = Get16From2(Xorbin(Get2From16(state[i][j]), Get2From16(cipherkey[i][j])));
        }
    }
}

void invplanTextAddKey()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            state[i][j] = Get16From2(Xorbin(Get2From16(state[i][j]), Get2From16(cipherkey[i][j])));
        }
    }
}

void invshiftRows()
{
    dichbyte(2);
    dichbyte(2);
    dichbyte(2);
    dichbyte(3);
    dichbyte(3);
    dichbyte(4);
}

void invsubByte()
{
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            string s = state[i][j];
            int x, y;
            if (s[0] >= '0' && s[0] <= '9')
            {
                x = s[0] - 48;
            }
            else
            {
                if (s[0] == 'a')
                    x = 10;
                if (s[0] == 'b')
                    x = 11;
                if (s[0] == 'c')
                    x = 12;
                if (s[0] == 'd')
                    x = 13;
                if (s[0] == 'e')
                    x = 14;
                if (s[0] == 'f')
                    x = 15;
            }
            if (s[1] >= '0' && s[1] <= '9')
            {
                y = s[1] - 48;
            }
            else
            {
                if (s[1] == 'a')
                    y = 10;
                if (s[1] == 'b')
                    y = 11;
                if (s[1] == 'c')
                    y = 12;
                if (s[1] == 'd')
                    y = 13;
                if (s[1] == 'e')
                    y = 14;
                if (s[1] == 'f')
                    y = 15;
            }
            state[i][j] = invsbox[x][y];
        }
    }
}

void invmixColumn()
{
    for (int j = 1; j <= 4; j++)
    {
        string temp[5];
        temp[1] = state[1][j];
        temp[2] = state[2][j];
        temp[3] = state[3][j];
        temp[4] = state[4][j];
        string da[5];
        for (int i = 1; i <= 4; i++)
        {
            string np1 = invnhanmbox(temp[1], mbox[i][1]);
            string np2 = invnhanmbox(temp[2], mbox[i][2]);
            string np3 = invnhanmbox(temp[3], mbox[i][3]);
            string np4 = invnhanmbox(temp[4], mbox[i][4]);
            string np = np1;
            da[i] = Get16From2(Xorbin(Xorbin(Xorbin(np1, np2), np3), np4));
        }
        state[1][j] = da[1];
        state[2][j] = da[2];
        state[3][j] = da[3];
        state[4][j] = da[4];
    }
}

void invaddRoundKey()
{
    string rconi[5];
    getRcon(vong, rconi);
    for (int i = 4; i >= 2; i--)
    {
        for (int j = 1; j <= 4; j++)
        {
            cipherkey[j][i] = Get16From2(Xorbin(Get2From16(cipherkey[j][i]), Get2From16(cipherkey[j][i - 1])));
        }
    }
    string temp[5];
    temp[1] = cipherkey[2][4];
    temp[2] = cipherkey[3][4];
    temp[3] = cipherkey[4][4];
    temp[4] = cipherkey[1][4];
    for (int i = 1; i <= 4; i++)
    {
        int x, y;
        string s = temp[i];
        if (s[0] >= '0' && s[0] <= '9')
        {
            x = s[0] - 48;
        }
        else
        {
            if (s[0] == 'a')
                x = 10;
            if (s[0] == 'b')
                x = 11;
            if (s[0] == 'c')
                x = 12;
            if (s[0] == 'd')
                x = 13;
            if (s[0] == 'e')
                x = 14;
            if (s[0] == 'f')
                x = 15;
        }
        if (s[1] >= '0' && s[1] <= '9')
        {
            y = s[1] - 48;
        }
        else
        {
            if (s[1] == 'a')
                y = 10;
            if (s[1] == 'b')
                y = 11;
            if (s[1] == 'c')
                y = 12;
            if (s[1] == 'd')
                y = 13;
            if (s[1] == 'e')
                y = 14;
            if (s[1] == 'f')
                y = 15;
        }
        s = sbox[x][y];
        cipherkey[i][1] = (Get16From2(Xorbin(Xorbin(Get2From16(rconi[i]), Get2From16(s)), Get2From16(cipherkey[i][1]))));
    }
}

void maHoa()
{
    Nhap();
    show();
    cout << endl
         << "==========Ma Hoa==========";
    vong = 1;
    planTextAddKey();
    for (int i = 1; i <= 9; i++)
    {
        cout << endl
             << "vong " << i << endl;
        subByte();
        shiftRows();
        mixColumn();
        addRoundKey();
        planTextAddKey();
        cout << i << endl;
        show();
        vong++;
    }
    cout << endl
         << "vong 10" << endl;
    subByte();
    shiftRows();
    addRoundKey();
    planTextAddKey();
    cout << "10" << endl;
    show();
}

void giaiMa()
{
    cout << "==========Giai ma==========" << endl;
    string s1, s2;
    vong = 10;
    cout << "state(4x4):" << endl;
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            cin >> state[i][j];
        }
    }
    cout << "key(4x4):" << endl;
    for (int i = 1; i <= 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            cin >> cipherkey[i][j];
        }
    }
    show();
    invplanTextAddKey();
    invaddRoundKey();
    invshiftRows();
    invsubByte();
    for (int i = 1; i <= 9; i++)
    {
        cout << endl
             << "vong " << i << endl;
    }
}

int main()
{
    maHoa();
    giaiMa();
    return 0;
}
