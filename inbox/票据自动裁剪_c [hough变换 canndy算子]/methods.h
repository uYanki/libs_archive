#define PI 3.1415926
typedef unsigned long  DWORD;
typedef int            BOOL;
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef long           LONG;
typedef BYTE*          PBYTE;
typedef void*          PVOID;
typedef PVOID          HANDLE;
typedef void*          LPVOID;

// ��ɫ����
typedef struct tag_RGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} _RGBQUAD;

// bmpͷ��Ϣ
typedef struct tag_BITMAPINFOHEADER {
    DWORD biSize;           //ָ�������ݽṹ����Ҫ���ֽ���
    LONG  biWidth;          //���
    LONG  biHeight;         //�߶�
    WORD  biPlanes;         //����豸��λƽ����,Ϊ1
    WORD  biBitCount;       //����ÿ�����ص�λ��,1 4,8,24,32
    DWORD biCompression;    //λͼ��ѹ������
    DWORD biSizeImage;      //ͼ���ֽ����Ķ���
    LONG  biXPelsPerMeter;  //ͼ��ˮƽ�ֱ���
    LONG  biYPelsPerMeter;  //ͼ��ֱ�ֱ���
    DWORD biClrUsed;        //��ɫ����ͼ��ʵ��ʹ�õ���ɫ��
    DWORD biClrImportant;   //������Ҫ��ɫ������ֵ
} _BITMAPINFOHEADER;

// bmp��Ϣ
typedef struct tag_BITMAPINFO {
    _BITMAPINFOHEADER bmiHeader;
    _RGBQUAD          bmiColors[1];
} _BITMAPINFO;

typedef struct RESULT {
    int   width;
    int   height;
    PBYTE data;
} RESULT;

// �ü��ľ���
typedef struct tag_RECT {
    int X;

    int Y;

    int width;

    int height;

} _RECT;
typedef struct POX {
    int x;
    int y;
} POX;
#include <pshpack2.h>
// bmp�ļ�ͷ
typedef struct tag_BITMAPFILEHEADER {
    WORD  bfType;       //λͼ���,���ݲ�ͬ�Ĳ���ϵͳ����ͬ,��Windows��,���ֶε�ֵ��Ϊ'BM'
    DWORD bfSize;       // BMPͼ���ļ��Ĵ�С
    WORD  bfReserved1;  // 0
    WORD  bfReserved2;  // 0
    DWORD bfOffBits;    // BMPͼ�����ݵĵ�ַ
} _BITMAPFILEHEADER;

// ������BMPͼ��Ϣ
typedef struct tagBMPINFOMATION {
    _BITMAPINFOHEADER bih;         // bmpͷ��Ϣ
    BYTE*             data;        // ͼ����Ϣ
    BYTE*             pData24;     // ͼ����Ϣ
    int               nComponent;  // 3 ��ɫ/1 �Ҷ�
    int               nAdjust;     // �����ֽڶ���
    int               nAdjust24;   // �����ֽڶ���
} BMPINFOMATION;

/***********************************************
 *�洢bmp��ʽ�ļ� ��ʽ1
 **************************************************/
void saveAsBmp(BMPINFOMATION bmp, char* strDestFileName) {
    _BITMAPFILEHEADER bfh;  // bmp�ļ�ͷ
    _BITMAPINFOHEADER bih;  // bmpͷ��Ϣ
    // FILE *fp2;
    // fp2 = fopen("f:\\2.txt","w");
    // fputs("1\n",fp2);
    bih = bmp.bih;
    int   nAdjust;            // �����ֽڶ���
    BYTE* data       = NULL;  // new BYTE[bih.biWidth*bih.biHeight];
    data             = bmp.pData24;
    int   nComponent = 3;
    FILE* f;
    nAdjust = bmp.bih.biWidth * 3 % 4;
    if (nAdjust)
        nAdjust = 4 - nAdjust;
    // printf("new:%d\n",nAdjust);
    f = fopen(strDestFileName, "wb");
    if (f == NULL) {
        delete[] data;
        // delete [] pDataConv;
        return;
    }
    // д�ļ�ͷ
    memset(&bfh, 0, sizeof(bfh));
    bfh.bfSize    = sizeof(bfh) + sizeof(bih);
    bfh.bfOffBits = sizeof(bfh) + sizeof(bih);

    bfh.bfSize += (bih.biWidth * 3 + nAdjust) * bih.biHeight;
    bfh.bfType = 0x4d42;
    // fprintf(fp2,"w:%d,h:%d\n",bih.biWidth,bih.biHeight);
    fwrite(&bfh, sizeof(bfh), 1, f);
    fwrite(&bih, sizeof(bih), 1, f);
    fwrite(data, (bih.biWidth * 3 + nAdjust) * bih.biHeight, 1, f);
    fclose(f);
    // fclose(fp2);
    delete[] data;
}

//�����һ��jpgѹ��ͼ
void saveAsJpg(char* strDestFileName, BMPINFOMATION bmp) {
    _BITMAPINFOHEADER bih;      // bmpͷ��Ϣ
    _RGBQUAD          rq[256];  // ��ɫ��
    int               i = 0, j = 0;
    int               withSize;  //ÿ�п��
    int               nAdjust;   // �����ֽڶ���
    nAdjust          = bmp.nAdjust;
    BYTE* pData24    = NULL;  // new BYTE[bih.biWidth*bih.biHeight];
    int   nComponent = 0;
    bih              = bmp.bih;
    pData24          = bmp.pData24;
    nComponent       = bmp.nComponent;

    struct jpeg_compress_struct jcs;
    struct jpeg_error_mgr       jem;
    FILE*                       f;
    jcs.err = jpeg_std_error(&jem);

    jpeg_create_compress(&jcs);

    f = fopen(strDestFileName, "wb");
    if (f == NULL) {
        delete[] pData24;
        return;
    }

    jpeg_stdio_dest(&jcs, f);
    jcs.image_width      = bih.biWidth;  // Ϊͼ�Ŀ�͸�,��λΪ����
    jcs.image_height     = bih.biHeight;
    jcs.input_components = nComponent;  // 1,��ʾ�Ҷ�ͼ, ����ǲ�ɫλͼ,��Ϊ3

    if (nComponent == 1)
        jcs.in_color_space = JCS_GRAYSCALE;  // JCS_GRAYSCALE��ʾ�Ҷ�ͼ,JCS_RGB��ʾ��ɫͼ��
    else
        jcs.in_color_space = JCS_RGB;

    jpeg_set_defaults(&jcs);

    // ���� density_unit { may be 0 for unknown, 1 for dots/inch, or 2 for dots/cm.}
    jcs.density_unit = 1;
    // ����ˮƽ/��ֱ�ֱ���
    jcs.X_density = 300;
    jcs.Y_density = 300;
    jpeg_set_quality(&jcs, 60, true);

    jpeg_start_compress(&jcs, TRUE);
    JSAMPROW row_pointer[1];                  // һ��λͼ
    int      row_stride;                      // ÿһ�е��ֽ���
    row_stride = (bih.biWidth) * nComponent;  // �����������ͼ,�˴���Ҫ����3

    // ��ÿһ�н���ѹ��
    while (jcs.next_scanline < jcs.image_height) {
        row_pointer[0] = &pData24[(jcs.image_height - jcs.next_scanline - 1) * (row_stride + nAdjust)];
        jpeg_write_scanlines(&jcs, row_pointer, 1);
    }
    jpeg_finish_compress(&jcs);
    jpeg_destroy_compress(&jcs);

    fclose(f);
    delete[] pData24;
}

/***********************************************
 *��ת�㷨+(����Ӧ��ֵ������)�ҳ���Ե
 **************************************************/
BYTE* imRotate_BinaryzationFindEdge(BYTE* data, int w, int h, float beta, LONG* biWidth, LONG* biHeight, char* strDestFileName, BYTE* obm) {
    int iw;
    int ih;
    iw = w;
    ih = h;
    // 1.������ת�����ͼ�������С��Χ�п��
    LONG   owh       = (LONG)sqrt(iw * iw + ih * ih + 0.5);
    double t         = beta / 180;
    BYTE*  pData24   = data;
    int    nAdjust24 = w * 3 % 4;
    if (nAdjust24) nAdjust24 = 4 - nAdjust24;
    // �õ�����ͼ��
    // FILE *fp2;
    // fp2 = fopen("f:\\data.txt","w");

    obm            = new BYTE[owh * owh * 4];
    float cos_beta = (float)cos(t * PI);
    float sin_beta = (float)sin(t * PI);

    for (int j = 0; j < owh; j++) {
        for (int i = 0; i < owh; i++) {
            int pox_new      = j * (owh * 3 + nAdjust24) + i * 3;
            obm[pox_new]     = 0;
            obm[pox_new + 1] = 0;
            obm[pox_new + 2] = 0;
        }
    }
    // fprintf(fp2,"1!\n");
    //  2.����ת�任,�������ͼ���p[i*(bih.biWidth*3+nAdjust24) + j * 3]����Ӧ��ԭͼ�������(x,y);
    for (int j = 0; j < owh; j++) {
        for (int i = 0; i < owh; i++) {
            //��ת�任����任
            float u = (i - owh / 2) * cos_beta + (j - owh / 2) * sin_beta;
            float v = (j - owh / 2) * cos_beta - (i - owh / 2) * sin_beta;

            // ���������ԭͼ��ľ�������
            u += iw / 2;
            //�����200��Ȩ��֮��,Ϊ��ʹͼ�����м�
            v += ih / 2;
            // v += + ih * 0.1543;

            int x = (int)u;
            int y = (int)v;

            // printf("i =  %d,j = %d\n",i,j);
            // 3.��������,�����������ĵ�(x,y),��ֵF(i,j) = f(x,y)
            if ((x >= 0) && (x < iw) && (y >= 0) && (y < ih)) {
                // ��x,y ȡֵ
                int pox_ = y * (w * 3 + nAdjust24) + x * 3;
                int b    = pData24[pox_];
                int g    = pData24[pox_ + 1];
                int r    = pData24[pox_ + 2];
                // printf("%d,%d,%d\n",b,g,r);
                //��ֵ����(i,j)
                int pox_new      = j * (owh * 3 + nAdjust24) + i * 3;
                obm[pox_new]     = b;
                obm[pox_new + 1] = g;
                obm[pox_new + 2] = r;
            }
        }
    }

    //��ֵ��
    BYTE*  obm_gray = new BYTE[(owh + nAdjust24) * owh];
    double p[257];  //����ÿ���Ҷ�ֵ�ĸ���
    //�趨һ��������
    p[256] = 1;
    for (int i = 0; i < 256; i++) {
        p[i] = 0;
    }
    for (int j = 0; j < owh; j++) {
        for (int i = 0; i < owh; i++) {
            int b = obm[j * (owh * 3 + nAdjust24) + i * 3];      // B
            int g = obm[j * (owh * 3 + nAdjust24) + i * 3 + 1];  // G
            int r = obm[j * (owh * 3 + nAdjust24) + i * 3 + 2];  // R
            // ���ݹ�ʽ����ҶȻ���ֵ,��������
            int pox = j * (owh + nAdjust24) + i;
            // obm_gray[pox]  = (r + b + g)/ 3.0;//�õ��Ҷ�ֵ
            // obm_gray[pox]=pow(( pow(static_cast<double>(r),2.2)* 0.2973 + pow(static_cast<double>(g),2.2)* 0.6274 + pow(static_cast<double>(b),2.2) * 0.0753),(1/2.2));
            obm_gray[pox] = (r * 19595 + g * 38469 + b * 7472) >> 16;
            // obm_gray[pox]=r*0.299 + g*0.587 +b*0.114;
            p[obm_gray[pox]]++;  //����Ӧ�ĻҶ�ֵ����1;
        }
    }

    //�ֶ����Ա任
    int bMap[257];  //�Ҷ�ӳ���
    int BX1 = 20;
    int BY1 = 4;
    int BX2 = 230;
    int BY2 = 214;
    int i;
    for (i = 0; i < BX1; i++) {  //(0-x1)
        //�ж�bx1�Ƿ����0
        if (BX1 > 0) {
            bMap[i] = (int)BY1 * i / BX1;
        } else {
            bMap[i] = 0;
        }
    }
    for (; i < BX2; i++) {  //(x1-x2)
        if (BX2 != BX1) {
            bMap[i] = BY1 + (int)((BY2 - BY1) * (i - BX1) / (BX2 - BX1));
        } else {
            //ֱ�Ӹ�ֵΪBY1
            bMap[i] = BY1;
        }
    }
    for (; i < 256; i++) {  //(x2-256)
        //�ж�bx2�Ƿ����255(��ֹ��ĸΪ0)
        if (BX2 != 255) {
            //���Ա任
            bMap[i] = BY2 + (int)((255 - BY2) * (i - BX2) / (255 - BX2));
        } else {
            //ֱ�� ��ֵ255
            bMap[i] = 255;
        }
    }
    //�Ը����ؽ��лҶ�ת��
    for (i = 0; i < owh; i++) {
        for (int j = 0; j < owh; j++) {
            //�����ؽ��лҶ�ӳ�䴦��
            int pox       = i * (owh + nAdjust24) + j;
            int T         = obm_gray[pox];
            obm_gray[pox] = bMap[T];
        }
    }

    //�����ǿ���
    if (1) {
        _BITMAPINFOHEADER bih = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // bmpͷ��Ϣ
        int               nAdjust;                                  // �����ֽڶ���
        //*****************************
        bih.biSize          = 40;
        bih.biWidth         = owh;
        bih.biHeight        = owh;
        bih.biPlanes        = 1;
        bih.biBitCount      = 24;
        bih.biCompression   = BI_RGB;
        bih.biSizeImage     = ((bih.biWidth % 4 == 0) ? bih.biWidth : (bih.biWidth + bih.biWidth % 4)) * bih.biHeight;
        bih.biXPelsPerMeter = 300;
        bih.biYPelsPerMeter = 300;
        bih.biClrUsed       = 0;
        bih.biClrImportant  = 0;

        //*******************************************************************
        BMPINFOMATION bmp_t;

        BYTE* g_T = new BYTE[(owh * 3 + nAdjust24) * owh];
        for (int j = 0; j < owh; j++) {
            for (int i = 0; i < owh; i++) {
                g_T[j * (owh * 3 + nAdjust24) + i * 3]     = obm_gray[j * (owh + nAdjust24) + i];  // B
                g_T[j * (owh * 3 + nAdjust24) + i * 3 + 1] = obm_gray[j * (owh + nAdjust24) + i];  // G
                g_T[j * (owh * 3 + nAdjust24) + i * 3 + 2] = obm_gray[j * (owh + nAdjust24) + i];  // R
            }
        }
        bmp_t.bih        = bih;
        bmp_t.nComponent = 3;
        bmp_t.data       = g_T;
        bmp_t.pData24    = g_T;
        bmp_t.nAdjust24  = nAdjust24;
        bmp_t.nAdjust    = nAdjust24;
        // ����ͼ���ȡ���
        saveAsBmp(bmp_t, "��ǿ_canny_��ֵ�˲�����.bmp");
    }
    // end

    // fprintf(fp2,"2!\n");

    //�������
    for (int i = 0; i < 256; i++) {
        p[i] /= (owh * owh * 1.0);
        // fprintf(fp2,"%d	%f\n",i,p[i]);
    }
    // fclose(fp2);
    //����Ҷ�ֵ��10~127��ļ�Сֵ

    int max   = 255;
    int mymin = 0;
    //���ֱ��ͼ�Ҷ˵�
    for (int i = 255; i >= 0; i--) {
        if (p[i] > 0.001) {
            max = i;
            break;
        }
    }

    //���ֱ��ͼ��˵�
    for (int i = 0; i < 255; i++) {
        if (p[i] > 0.001) {
            mymin = i;
            break;
        }
    }
    int min_pox = mymin;
    //���������Ҷ�����
    int t1 = mymin + (max - mymin) / 4;
    int t0 = mymin + (max - mymin) / 2;
    int t2 = max - (max - mymin) / 4;
    for (int i = t2; i >= t0; i--) {
        if (p[i] <= p[min_pox]) {
            min_pox = i;
        }
    }

    if (min_pox >= t2) {
        min_pox = 40;
    }
    // if(min_pox<t0*1.2)
    //	min_pox=0.8*t2;

    // �õ�����ͼ��
    /*FILE *fp2;
    fp2 = fopen("D:\\b.txt","w+");
    fprintf(fp2,"min_pox= %d,t0=%d,t2=%d",min_pox,t0,t2);*/

    for (int j = 0; j < owh; j++) {
        for (int i = 0; i < owh; i++) {
            int pox = j * (owh + nAdjust24) + i;
            if (obm_gray[pox] < min_pox) {
                obm_gray[pox] = 255;  //����Ϊ����
            } else {
                obm_gray[pox] = 0;  //����Ϊͼ��
            }
        }
    }
    SIZE szz;
    szz.cx = owh;
    szz.cy = owh;
    // printf("����ͼ���˲���ʼ\n");
    MedianFilter(obm_gray, szz, 13 * 13);
    // fprintf(fp2,"�˲�����!\n");

    BYTE* obm_gray_new = new BYTE[(owh + nAdjust24) * owh];

    for (int i = 0; i < (owh + nAdjust24) * owh; i++) {
        obm_gray_new[i] = obm_gray[i];
    }

    BMPINFOMATION bmp_t;
    bmp_t.pData24      = obm_gray_new;
    bmp_t.bih.biHeight = owh;
    bmp_t.bih.biWidth  = owh;
    bmp_t.nComponent   = 1;
    //�����ֵ�����
    int debug = 1;
    if (debug) {
        _BITMAPINFOHEADER bih = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // bmpͷ��Ϣ
        int               nAdjust;                                  // �����ֽڶ���
        //*****************************
        bih.biSize          = 40;
        bih.biWidth         = owh;
        bih.biHeight        = owh;
        bih.biPlanes        = 1;
        bih.biBitCount      = 24;
        bih.biCompression   = BI_RGB;
        bih.biSizeImage     = ((bih.biWidth % 4 == 0) ? bih.biWidth : (bih.biWidth + bih.biWidth % 4)) * bih.biHeight;
        bih.biXPelsPerMeter = 300;
        bih.biYPelsPerMeter = 300;
        bih.biClrUsed       = 0;
        bih.biClrImportant  = 0;

        //*******************************************************************
        BMPINFOMATION bmp_t;

        BYTE* g_T = new BYTE[(owh * 3 + nAdjust24) * owh];
        for (int j = 0; j < owh; j++) {
            for (int i = 0; i < owh; i++) {
                g_T[j * (owh * 3 + nAdjust24) + i * 3]     = obm_gray[j * (owh + nAdjust24) + i];  // B
                g_T[j * (owh * 3 + nAdjust24) + i * 3 + 1] = obm_gray[j * (owh + nAdjust24) + i];  // G
                g_T[j * (owh * 3 + nAdjust24) + i * 3 + 2] = obm_gray[j * (owh + nAdjust24) + i];  // R
            }
        }
        bmp_t.bih        = bih;
        bmp_t.nComponent = 3;
        bmp_t.data       = g_T;
        bmp_t.pData24    = g_T;
        bmp_t.nAdjust24  = nAdjust24;
        bmp_t.nAdjust    = nAdjust24;
        // ����ͼ���ȡ���
        saveAsBmp(bmp_t, "mybinary.bmp");
    }
    /****************************************/
    //�������������,���������߽�
    POX pox[4];
    int _w = owh;
    int _h = owh;
    int x_min, y_min, x_max, y_max;
    int go_on = 1;
    //�ҵ��ı߽��
    //  ����(x_min,y_min) ����(x_max,y_min) ���� (x_max,y_min) ���� (x_max,y_max)

    //��y_min
    for (int i = 0; i < _h; i++) {
        go_on = 1;
        for (int j = 0; j < _w; j++) {
            if (obm_gray[i * (owh + nAdjust24) + j] == 0) {  // 255�Ǳ߽�
                y_min    = i;
                go_on    = 0;
                pox[0].x = j;
                pox[0].y = i;
                break;
            }
        }
        if (!go_on) break;
    }
    //��x_min
    for (int j = 0; j < _w; j++) {
        go_on = 1;
        for (int i = 0; i < _h; i++) {
            if (obm_gray[i * (owh + nAdjust24) + j] == 0) {  // 255�Ǳ߽�
                x_min    = j;
                pox[1].x = j;
                pox[1].y = i;
                go_on    = 0;
                break;
            }
        }
        if (!go_on) break;
    }
    //��y_max
    for (int i = _h - 1; i >= 0; i--) {
        go_on = 1;
        for (int j = 0; j < _w; j++) {
            if (obm_gray[i * (owh + nAdjust24) + j] == 0) {  // 255�Ǳ߽�
                y_max    = i;
                go_on    = 0;
                pox[2].x = j;
                pox[2].y = i;
                break;
            }
        }
        if (!go_on) break;
    }
    //��x_max
    for (int j = _w - 1; j >= 0; j--) {
        go_on = 1;
        for (int i = _h - 1; i >= 0; i--) {
            if (obm_gray[i * (owh + nAdjust24) + j] == 0) {  // 255�Ǳ߽�
                x_max    = j;
                go_on    = 0;
                pox[3].x = j;
                pox[3].y = i;
                break;
            }
        }
        if (!go_on) break;
    }
    // fprintf(fp2,"�ұ�ǵ�!\n");

    // printf("%d,%d,%d,%d\n",x_min,y_min,x_max,y_max);
    //��ԭͼ���ж�Ӧ��ɫ���иı�
    for (int j = 0; j < owh; j++) {
        for (int i = 0; i < owh; i++) {
            if (
                obm[j * (owh * 3 + nAdjust24) + i * 3] == 123 &&
                obm[j * (owh * 3 + nAdjust24) + i * 3 + 1] == 123 &&
                obm[j * (owh * 3 + nAdjust24) + i * 3 + 2] == 123) {
                obm[j * (owh * 3 + nAdjust24) + i * 3]     = 123;
                obm[j * (owh * 3 + nAdjust24) + i * 3 + 1] = 123;
                obm[j * (owh * 3 + nAdjust24) + i * 3 + 2] = 123 - 1;
            }
        }
    }
    // fprintf(fp2,"��ԭͼ�޸�!\n");
    // fclose(fp2);
    //���ĸ�����б��
    for (int i = 0; i < 4; i++) {
        obm[(pox[i].y) * (owh * 3 + nAdjust24) + (pox[i].x) * 3]     = 123;  // B
        obm[(pox[i].y) * (owh * 3 + nAdjust24) + (pox[i].x) * 3 + 1] = 123;  // G
        obm[(pox[i].y) * (owh * 3 + nAdjust24) + (pox[i].x) * 3 + 2] = 123;  // R
    }

    /*******************************************************************/
    /*******************************/

    *biWidth  = owh;
    *biHeight = owh;
    // obmde = obm;
    delete[] obm_gray;
    // delete []obm;
    return obm;
}

/***********************************************
 *��ת�㷨+(canny��Ե����+С������ֵ�˲�)�����ҳ���Ե
 **************************************************/
BYTE* imRotate_CannyFindEdge(BYTE* data, int w, int h, float beta, LONG* biWidth, LONG* biHeight, char* strDestFileName, BYTE* obm) {
    int iw;
    int ih;
    iw = w;
    ih = h;
    // 1.������ת�����ͼ�������С��Χ�п��
    LONG   owh       = (LONG)sqrt(iw * iw + ih * ih + 0.5);
    double t         = beta / 180;
    BYTE*  pData24   = data;
    int    nAdjust24 = w * 3 % 4;
    if (nAdjust24) nAdjust24 = 4 - nAdjust24;
    // �õ�����ͼ��
    // FILE *fp2;
    // fp2 = fopen("f:\\b.txt","w");

    obm            = new BYTE[owh * owh * 4];
    float cos_beta = (float)cos(t * PI);
    float sin_beta = (float)sin(t * PI);

    for (int j = 0; j < owh; j++) {
        for (int i = 0; i < owh; i++) {
            int pox_new      = j * (owh * 3 + nAdjust24) + i * 3;
            obm[pox_new]     = 0;
            obm[pox_new + 1] = 0;
            obm[pox_new + 2] = 0;
        }
    }
    // fprintf(fp2,"1!\n");
    //  2.����ת�任,�������ͼ���p[i*(bih.biWidth*3+nAdjust24) + j * 3]����Ӧ��ԭͼ�������(x,y);
    for (int j = 0; j < owh; j++) {
        for (int i = 0; i < owh; i++) {
            //��ת�任����任
            float u = (i - owh / 2) * cos_beta + (j - owh / 2) * sin_beta;
            float v = (j - owh / 2) * cos_beta - (i - owh / 2) * sin_beta;

            // ���������ԭͼ��ľ�������
            u += iw / 2;
            //�����200��Ȩ��֮��,Ϊ��ʹͼ�����м�
            v += ih / 2;
            // v += + ih * 0.1543;

            int x = (int)u;
            int y = (int)v;

            // printf("i =  %d,j = %d\n",i,j);
            // 3.��������,�����������ĵ�(x,y),��ֵF(i,j) = f(x,y)
            if ((x >= 0) && (x < iw) && (y >= 0) && (y < ih)) {
                // ��x,y ȡֵ
                int pox_ = y * (w * 3 + nAdjust24) + x * 3;
                int b    = pData24[pox_];
                int g    = pData24[pox_ + 1];
                int r    = pData24[pox_ + 2];
                // printf("%d,%d,%d\n",b,g,r);
                //��ֵ����(i,j)
                int pox_new      = j * (owh * 3 + nAdjust24) + i * 3;
                obm[pox_new]     = b;
                obm[pox_new + 1] = g;
                obm[pox_new + 2] = r;
            }
        }
    }

    //��ֵ��
    BYTE*  obm_gray = new BYTE[(owh + nAdjust24) * owh];
    double p[257];  //����ÿ���Ҷ�ֵ�ĸ���
    //�趨һ��������
    p[256] = 1;
    for (int i = 0; i < 256; i++) {
        p[i] = 0;
    }
    for (int j = 0; j < owh; j++) {
        for (int i = 0; i < owh; i++) {
            int b = obm[j * (owh * 3 + nAdjust24) + i * 3];      // B
            int g = obm[j * (owh * 3 + nAdjust24) + i * 3 + 1];  // G
            int r = obm[j * (owh * 3 + nAdjust24) + i * 3 + 2];  // R
            // ���ݹ�ʽ����ҶȻ���ֵ,��������
            int pox = j * (owh + nAdjust24) + i;
            // obm_gray[pox]  = (r + b + g)/ 3.0;//�õ��Ҷ�ֵ
            // obm_gray[pox]=pow(( pow(static_cast<double>(r),2.2)* 0.2973 + pow(static_cast<double>(g),2.2)* 0.6274 + pow(static_cast<double>(b),2.2) * 0.0753),(1/2.2));
            obm_gray[pox] = (r * 19595 + g * 38469 + b * 7472) >> 16;
            // obm_gray[pox]=r*0.299 + g*0.587 +b*0.114;
            p[obm_gray[pox]]++;  //����Ӧ�ĻҶ�ֵ����1;
        }
    }

    //�ֶ����Ա任
    int bMap[257];  //�Ҷ�ӳ���
    int BX1 = 20;
    int BY1 = 4;
    int BX2 = 230;
    int BY2 = 214;
    int i;
    for (i = 0; i < BX1; i++) {  //(0-x1)
        //�ж�bx1�Ƿ����0
        if (BX1 > 0) {
            bMap[i] = (int)BY1 * i / BX1;
        } else {
            bMap[i] = 0;
        }
    }
    for (; i < BX2; i++) {  //(x1-x2)
        if (BX2 != BX1) {
            bMap[i] = BY1 + (int)((BY2 - BY1) * (i - BX1) / (BX2 - BX1));
        } else {
            //ֱ�Ӹ�ֵΪBY1
            bMap[i] = BY1;
        }
    }
    for (; i < 256; i++) {  //(x2-256)
        //�ж�bx2�Ƿ����255(��ֹ��ĸΪ0)
        if (BX2 != 255) {
            //���Ա任
            bMap[i] = BY2 + (int)((255 - BY2) * (i - BX2) / (255 - BX2));
        } else {
            //ֱ�� ��ֵ255
            bMap[i] = 255;
        }
    }
    //�Ը����ؽ��лҶ�ת��
    for (i = 0; i < owh; i++) {
        for (int j = 0; j < owh; j++) {
            //�����ؽ��лҶ�ӳ�䴦��
            int pox       = i * (owh + nAdjust24) + j;
            int T         = obm_gray[pox];
            obm_gray[pox] = bMap[T];
        }
    }
    // end

    BYTE* obm_gray_old = new BYTE[(owh + nAdjust24) * owh];
    for (int i = 0; i < (owh + nAdjust24) * owh; i++) {
        obm_gray_old[i] = obm_gray[i];
    }
    // canny
    SIZE sz;
    sz.cx = owh;
    sz.cy = owh;
    Canny(obm_gray_old, sz, 2, obm_gray);
    //��ֵ�˲�
    MedianFilter(obm_gray, sz, 5 * 5);
    //�����ǿ���
    int test = 1;
    if (test) {
        _BITMAPINFOHEADER bih = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // bmpͷ��Ϣ
        int               nAdjust;                                  // �����ֽڶ���
        //*****************************
        bih.biSize          = 40;
        bih.biWidth         = owh;
        bih.biHeight        = owh;
        bih.biPlanes        = 1;
        bih.biBitCount      = 24;
        bih.biCompression   = BI_RGB;
        bih.biSizeImage     = ((bih.biWidth % 4 == 0) ? bih.biWidth : (bih.biWidth + bih.biWidth % 4)) * bih.biHeight;
        bih.biXPelsPerMeter = 300;
        bih.biYPelsPerMeter = 300;
        bih.biClrUsed       = 0;
        bih.biClrImportant  = 0;

        //*******************************************************************
        BMPINFOMATION bmp_t;

        BYTE* g_T = new BYTE[(owh * 3 + nAdjust24) * owh];
        for (int j = 0; j < owh; j++) {
            for (int i = 0; i < owh; i++) {
                g_T[j * (owh * 3 + nAdjust24) + i * 3]     = obm_gray[j * (owh + nAdjust24) + i];  // B
                g_T[j * (owh * 3 + nAdjust24) + i * 3 + 1] = obm_gray[j * (owh + nAdjust24) + i];  // G
                g_T[j * (owh * 3 + nAdjust24) + i * 3 + 2] = obm_gray[j * (owh + nAdjust24) + i];  // R
            }
        }
        bmp_t.bih        = bih;
        bmp_t.nComponent = 3;
        bmp_t.data       = g_T;
        bmp_t.pData24    = g_T;
        bmp_t.nAdjust24  = nAdjust24;
        bmp_t.nAdjust    = nAdjust24;
        // ����ͼ���ȡ���
        saveAsBmp(bmp_t, "��ǿ_canny_��ֵ�˲�����1.bmp");
    }
    // end

    /****************************************/
    //�������������,���������߽�
    POX pox[4];
    int border = 255;

    int _w = owh;
    int _h = owh;
    int x_min, y_min, x_max, y_max;
    int go_on = 1;
    //�ҵ��ı߽��
    //  ����(x_min,y_min) ����(x_max,y_min) ���� (x_max,y_min) ���� (x_max,y_max)

    //��y_min
    for (int i = 0; i < _h; i++) {
        go_on = 1;
        for (int j = 0; j < _w; j++) {
            if (obm_gray[i * (owh + nAdjust24) + j] == border) {  // 255�Ǳ߽�
                y_min    = i;
                go_on    = 0;
                pox[0].x = j;
                pox[0].y = i;
                break;
            }
        }
        if (!go_on) break;
    }
    //��x_min
    for (int j = 0; j < _w; j++) {
        go_on = 1;
        for (int i = 0; i < _h; i++) {
            if (obm_gray[i * (owh + nAdjust24) + j] == border) {  // 255�Ǳ߽�
                x_min    = j;
                pox[1].x = j;
                pox[1].y = i;
                go_on    = 0;
                break;
            }
        }
        if (!go_on) break;
    }
    //��y_max
    for (int i = _h - 1; i >= 0; i--) {
        go_on = 1;
        for (int j = 0; j < _w; j++) {
            if (obm_gray[i * (owh + nAdjust24) + j] == border) {  // 255�Ǳ߽�
                y_max    = i;
                go_on    = 0;
                pox[2].x = j;
                pox[2].y = i;
                break;
            }
        }
        if (!go_on) break;
    }
    //��x_max
    for (int j = _w - 1; j >= 0; j--) {
        go_on = 1;
        for (int i = _h - 1; i >= 0; i--) {
            if (obm_gray[i * (owh + nAdjust24) + j] == border) {  // 255�Ǳ߽�
                x_max    = j;
                go_on    = 0;
                pox[3].x = j;
                pox[3].y = i;
                break;
            }
        }
        if (!go_on) break;
    }
    // fprintf(fp2,"�ұ�ǵ�!\n");

    // printf("%d,%d,%d,%d\n",x_min,y_min,x_max,y_max);
    //��ԭͼ���ж�Ӧ��ɫ���иı�
    for (int j = 0; j < owh; j++) {
        for (int i = 0; i < owh; i++) {
            if (
                obm[j * (owh * 3 + nAdjust24) + i * 3] == 123 &&
                obm[j * (owh * 3 + nAdjust24) + i * 3 + 1] == 123 &&
                obm[j * (owh * 3 + nAdjust24) + i * 3 + 2] == 123) {
                obm[j * (owh * 3 + nAdjust24) + i * 3]     = 123;
                obm[j * (owh * 3 + nAdjust24) + i * 3 + 1] = 123;
                obm[j * (owh * 3 + nAdjust24) + i * 3 + 2] = 123 - 1;
            }
        }
    }
    // fprintf(fp2,"��ԭͼ�޸�!\n");
    // fclose(fp2);
    //���ĸ�����б��
    for (int i = 0; i < 4; i++) {
        obm[(pox[i].y) * (owh * 3 + nAdjust24) + (pox[i].x) * 3]     = 123;  // B
        obm[(pox[i].y) * (owh * 3 + nAdjust24) + (pox[i].x) * 3 + 1] = 123;  // G
        obm[(pox[i].y) * (owh * 3 + nAdjust24) + (pox[i].x) * 3 + 2] = 123;  // R
    }

    /*******************************************************************/
    /*******************************/

    *biWidth  = owh;
    *biHeight = owh;
    // obmde = obm;
    delete[] obm_gray;
    // delete []obm;
    return obm;
}
