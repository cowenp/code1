#include <bits/stdc++.h>
#include <windows.h>
using namespace std;


//---------------- bmp -> pic ---------------//
//---------------- bmp -> pic ---------------//
//---------------- bmp -> pic ---------------//


int biBitCount = 24; //���ݱ�����     //Ĭ�ϰ�ԭͼΪ24
struct pic_infor
{
    bool isColor;             //1��ʾΪ��ɫͼ��0��ʾΪ�Ҷ�ͼ��
    bool M;                   //����
    short int Notes_size;     //ע�����ֽ���
    short int Rows;           //��       ->Height
    short int Columns;        //��       ->Width
    short int Rows_start;     //�����
    short int Columns_start;  //�����
    short int T;              //����
    char some_information[48]; //��������

};

struct pic_Text_Notes       //ע��
{
    char *Text_Notes;
};

struct pic_RGB
{
    //--------ɫ����-------//
    struct RGB  //�졢�̡���
    {
        uint8_t rgbRed;     //ָ����ɫǿ��
        uint8_t rgbGreen;   //ָ����ɫǿ��
        uint8_t rgbBlue;    //ָ����ɫǿ��
    }ColorTable[256];
};

struct pic_data
{
    unsigned char *data_pic;
};

struct pic
{
    pic_infor infor;
    pic_Text_Notes Note;
    pic_RGB RGB;
    pic_data data;
};

pair<bool, pic> read_bmp(char *path)
{
    pic v;   //�����Ҫ����Ϣ

    FILE *fp = fopen(path, "rb");
    if (fp == 0)  return {0,v};

    fseek(fp, sizeof(BITMAPFILEHEADER), 0);//����λͼ�ļ�ͷ����Ϊû����Ҫ����Ϣ

    BITMAPINFOHEADER head;  //ȡ��λͼ��Ϣͷ
    fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);


    RGBQUAD *pColorTable = new RGBQUAD[256];
    fread(pColorTable, sizeof(RGBQUAD), 256, fp);

    //���RGB
    bool fl = true; //�ж��Ƿ��ǻҶ�ͼ
    for(int i = 0; i < 256; i++)
    {
        if(pColorTable[i].rgbBlue != pColorTable[i].rgbGreen || pColorTable[i].rgbGreen != pColorTable[i].rgbRed)
            fl = false;
        v.RGB.ColorTable[i].rgbRed = pColorTable[i].rgbRed;
        v.RGB.ColorTable[i].rgbGreen = pColorTable[i].rgbGreen;
        v.RGB.ColorTable[i].rgbBlue = pColorTable[i].rgbBlue;
    }
    if(fl)  v.infor.isColor = 0;
    else    v.infor.isColor = 1;

    v.infor.Rows = head.biHeight;
    v.infor.Columns = head.biWidth;

    biBitCount = head.biBitCount;

    v.infor.Notes_size = 0;
    v.Note.Text_Notes = 0;

    int line_byte = (head.biWidth * head.biBitCount / 8 + 3) / 4 * 4;

    v.data.data_pic = new unsigned char[line_byte * head.biHeight];
    fread(v.data.data_pic, 1, line_byte * head.biHeight, fp);

    fclose(fp);
    return {1,v};
}

bool save_pic(char *path, pic t)
{
    FILE *fp = fopen(path, "wb");
    if(fp == 0) {cout << "error in path" << endl;return 0;}

    //��Ϣ������(��ע����)
    fwrite(&t.infor, 64 * sizeof(unsigned char), 1, fp);

    //ע��������
    fwrite(&t.infor + 64, t.infor.Notes_size * sizeof(char), 1, fp);

	  //ɫ��������
    fwrite(&t.RGB, sizeof(pic_RGB), 1, fp);

    //��ֵ������
    int line_byte = (t.infor.Columns * biBitCount / 8 + 3) / 4 * 4;
    fwrite(t.data.data_pic, t.infor.Rows * line_byte * sizeof(unsigned char), 1, fp);

    fclose(fp);
    return 1;
}

void bmp_to_pic()
{
    char readPath[] = "C:\\Users\\86181\\Pictures\\ex\\lena.bmp";
    auto t = read_bmp(readPath);
    if(t.first == 0)    {cout << "error in path" << endl;return;}
    char writePath[] = "C:\\Users\\86181\\Pictures\\ex\\test.pic";
    // cout << biBitCount << endl;
    save_pic(writePath, t.second);

}

//---------------- pic -> bmp ---------------//
//---------------- pic -> bmp ---------------//
//---------------- pic -> bmp ---------------//
unsigned char *pBmpBuf;
pair<bool, pic> read_pic(char *path)
{
    pic v;
    FILE *fp = fopen(path, "rb");
    if (fp == 0)  return {0,v};

    fread(&v.infor, sizeof(pic_infor), 1, fp);
    fread(&v.Note, v.infor.Notes_size * sizeof(char), 1, fp);
    fread(&v.RGB, sizeof(pic_RGB), 1, fp);

    int line_byte = (v.infor.Columns * biBitCount / 8 + 3) / 4 * 4;

    pBmpBuf = new unsigned char[line_byte * v.infor.Rows];
    fread(pBmpBuf, 1, line_byte * v.infor.Rows, fp);
    fclose(fp);
    return {1,v};
}

void save_bmp(char *path,pic v)
{
    FILE *fp = fopen(path, "wb");
    if(fp == 0) {cout << "error in path (save)" << endl;return;}

    int line_byte = (v.infor.Columns * biBitCount / 8 + 3) / 4 * 4;
    // cout << line_byte << endl;
    int colorTablesize = 0;

    if (biBitCount == 8)
        colorTablesize = 1024;

    BITMAPFILEHEADER fileHead;
    fileHead.bfType = 0x4D42;

    fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + line_byte * v.infor.Rows;
    fileHead.bfReserved1 = 0;
    fileHead.bfReserved2 = 0;
    fileHead.bfOffBits = 56;
    fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

    BITMAPINFOHEADER head;
    head.biBitCount = biBitCount;
    head.biClrImportant = 0;
    head.biClrUsed = 0;
    head.biCompression = 0;
    head.biHeight = v.infor.Rows;
    head.biPlanes = 1;
    head.biSize = 40;
    head.biSizeImage = line_byte * v.infor.Rows;
    head.biWidth = v.infor.Columns;
    head.biXPelsPerMeter = 0;
    head.biYPelsPerMeter = 0;

    fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);
    // if (biBitCount == 8)
    // {
        RGBQUAD now_col[256];
        for(int i = 0; i < 256; i++)
        {
            now_col[i].rgbReserved = 0;
            now_col[i].rgbRed = v.RGB.ColorTable[i].rgbRed;
            now_col[i].rgbBlue = v.RGB.ColorTable[i].rgbBlue;
            now_col[i].rgbGreen = v.RGB.ColorTable[i].rgbGreen;
        }
        fwrite(&now_col, sizeof(RGBQUAD), 256, fp);

    // }
    // fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);
    // fwrite(&v.data.data_pic, line_byte * v.infor.Rows, 1, fp);

    fwrite(pBmpBuf, v.infor.Rows * line_byte, 1, fp);
    fclose(fp);
}


void pic_to_bmp()
{
    char readPath[] = "C:\\Users\\86181\\Pictures\\ex\\test.pic";
    auto t = read_pic(readPath);
    if(!t.first) {cout << "error in path" << endl;return;}
    char writePath[] = "C:\\Users\\86181\\Desktop\\test_cpy.bmp";
    save_bmp(writePath, t.second);
}

signed main()
{
    bmp_to_pic();
    pic_to_bmp();
    return 0;
}

