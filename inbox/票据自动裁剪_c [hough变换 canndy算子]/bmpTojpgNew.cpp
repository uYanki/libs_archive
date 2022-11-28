// TestLibjpeg.cpp : Defines the entry point for the console application.
///
#include <malloc.h>
#include <time.h>
#include "memory.h"
#include "string.h"
//#include <cmath>
clock_t start, finish;
#include "canny.h"
#include "libjpeg/jpeglib.h"
#include "methods.h"

// �ҶȻ�,����canny���ӵõ���Ե
BYTE* ToGrayByCanny(BYTE* data, int w, int h, BYTE* getP) {
    int i, j;
    //_BITMAPINFOHEADER bih = bmp.bih;
    BYTE* pData24 = data;
    BYTE* pGray   = NULL;
    // BYTE *getP = NULL;
    int r, b, g;
    int nAdjust24 = w * 3 % 4;
    if (nAdjust24) nAdjust24 = 4 - nAdjust24;
    pGray = new BYTE[(w + nAdjust24) * h];
    // getP = new BYTE[(bih.biWidth+nAdjust24)*bih.biHeight];
    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            b = pData24[j * (w * 3 + nAdjust24) + i * 3];      // B
            g = pData24[j * (w * 3 + nAdjust24) + i * 3 + 1];  // G
            r = pData24[j * (w * 3 + nAdjust24) + i * 3 + 2];  // R
            // ���ݹ�ʽ����ҶȻ���ֵ,��������
            pGray[j * (w + nAdjust24) + i] = (r * 19595 + g * 38469 + b * 7472) >> 16;
            // pGray[j*(w+nAdjust24)+i]  =r * 0.3 + g* 59 + b * 0.11;
        }
    }
    SIZE sz;
    sz.cx = w;
    sz.cy = h;
    Canny(pGray, sz, 1, getP);
    delete[] pGray;
    // BYTE *pGray = NULL;
    return getP;
}

/**************************************************************************
������: RESULT Bmp_Cut2(PBYTE datas,int width,int height)
**************************************************************************/
RESULT Bmp_Cut2(PBYTE datas, int width, int height) {
    _BITMAPINFOHEADER bih = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // bmpͷ��Ϣ
    _RGBQUAD          rq[256];                                  // ��ɫ��

    int   i = 0, j = 0;
    int   withSize;           //ÿ�п��
    int   nAdjust;            // �����ֽڶ���
    int   nAdjust24;          // �����ֽڶ���
    int   nAdjust32;          // �����ֽڶ���
    BYTE* data       = NULL;  // new BYTE[bih.biWidth*bih.biHeight];
    BYTE* pData24    = NULL;  // new BYTE[bih.biWidth*bih.biHeight];
    int   nComponent = 0;
    BYTE* data1      = NULL;

    //������Ƿ��ϵ�ͼ������,������
    if (!((width == 2592 && height == 1944) || (width == 1600 && height == 1200))) {
        RESULT s = {width, height, datas};
        return s;
    }
    // FILE *fp2;
    // fp2 = fopen("f:\\a.txt","w");
    /*****************************/
    bih.biWidth  = width;
    bih.biHeight = height;

    nAdjust = bih.biWidth % 4;
    if (nAdjust) nAdjust = 4 - nAdjust;
    // 24λ�ֽڶ���
    nAdjust24 = bih.biWidth * 3 % 4;
    if (nAdjust24) nAdjust24 = 4 - nAdjust24;
    // ��ȡ�����ļ�
    int size   = (bih.biWidth * 3 + nAdjust24) * bih.biHeight;
    data       = new BYTE[size];
    pData24    = new BYTE[size];
    pData24    = datas;
    nComponent = 3;
    /********************************************/
    // fputs("------��װ��ʼ-----\n",fp2);

    //��λͼ��Ϣ��װ
    BMPINFOMATION bmp;
    bmp.bih        = bih;
    bmp.nComponent = nComponent;
    bmp.data       = datas;
    bmp.pData24    = pData24;
    bmp.nAdjust24  = nAdjust24;
    bmp.nAdjust    = nAdjust;
    nComponent     = 3;
    // ����ͼ���ȡ���
    /*********************************************/
    //���ݴ���
    BYTE* data_temp = new BYTE[(width * 3 + nAdjust24) * height];
    for (int i = 0; i < (width * 3 + nAdjust24) * height; i++) {
        data_temp[i] = datas[i];
    }
    RESULT s_temp = {0, 0, data_temp};
    s_temp.height = height;
    s_temp.width  = width;

    /********************************************/
    //��ȡͼ��Canny�任��Ķ�ֵ����
    BYTE* pp = new BYTE[(bih.biWidth + nAdjust24) * bih.biHeight];
    BYTE* g  = ToGrayByCanny(datas, bih.biWidth, bih.biHeight, pp);
    /********************************************/

    /********************************************/
    //����canny���
    int debug = 1;
    if (debug) {
        BMPINFOMATION bmp_t = bmp;
        BYTE*         pGray = new BYTE[(bih.biWidth + nAdjust24) * bih.biHeight];
        /*******************************************************************/
        //���caany��Ե���
        BYTE* g_T = new BYTE[(bih.biWidth + nAdjust24) * bih.biHeight];
        for (int i = 0; i < (bih.biWidth + nAdjust24) * bih.biHeight; i++) {
            g_T[i] = g[i];
        }

        bmp_t.bih        = bih;
        bmp_t.pData24    = g_T;
        bmp_t.nComponent = 1;

        saveAsJpg("canny_.jpg", bmp_t);

        // fprintf(fp2,"�ɹ����canny����Ҷ�ͼ!\n");
        delete[] pGray;
        // delete [] g_T;
    }
    /***********************************************/
    // hough�任
    int    x = bmp.bih.biWidth;
    int    y = bmp.bih.biHeight;
    double rho;
    int    rho_int;
    int    rho_max          = 0;
    rho_max                 = (int)floor(sqrt(x * x + y * y + 0.0)) + 1;  //��ԭͼ����������������ֵ,��ȡ�������ּ�1,��ֵ��Ϊ��,������ϵ�����ֵ
    int* c                  = new int[rho_max * (180 + 1)];
    int(*accarray)[180 + 1] = (int(*)[180 + 1]) c;
    for (i = 0; i < rho_max; i++)
        for (j = 0; j <= 180; j++)
            accarray[i][j] = 0;

    //���������,ȷ����ȡֵ��Χ
    /**
     *	x=[0:0.5:360] :x��һ����0��360��0.5Ϊ�������� (����)����pi����180 ����,
     */
    // Theta=[0:pi/180:pi];

    double Theta[181];  //���0~180�ȵĻ���ֵ
    // Theta=(double *)malloc(sizeof(double *)*(180 + 1)); // ���������,ȷ����ȡֵ��Χ
    for (int i = 0; i <= 180; i++) {
        Theta[i] = (i / 360.0) * 2 * PI;
    }
    //
    for (int n = 0; n < x; n++) {
        for (int m = 0; m < y; m++) {
            if (g[m * (bih.biWidth + nAdjust24) + n] == 255) {  //��Ե������
                for (int k = 0; k <= 180; k++) {
                    // printf("%d",k);
                    rho = (n * cos(Theta[k])) + (m * sin(Theta[k]));  //����ֵ����hough�任����,���ֵ
                    // printf("��:%d\n",rho);
                    //����ֵ������ֵ�ĺ͵�һ����Ϊ�ѵ�����ֵ(��������),��������Ϊ�˷�ֹ��ֵ���ָ���
                    int temp = rho / 2 + rho_max / 2;
                    //�ڦѦ�����(����)�б�ʶ��,�������ۼ�
                    accarray[temp][k]++;
                    // printf("%d",accarray[temp][k]);
                }
            }
        }
    }

    int* R;  //�ҳ�ÿ���Ƕȶ�Ӧ�����ֵ
    R = (int*)malloc(sizeof(int*) * (180 + 1));
    for (int k = 0; k <= 180; k++) {
        R[k] = 0;
    }
    for (int k = 0; k <= 180; k++) {
        int tempMax = 0;
        for (int i = 0; i < rho_max; i++) {
            if (accarray[i][k] > tempMax) {
                tempMax = accarray[i][k];
                R[k]    = accarray[i][k];
            }
        }
        // printf("%d-�Ƕ�:%d\n",R[k],k);
    }

    int max_angle = 1;
    int max_value = R[1];
    for (int k = 1; k <= 180; k++) {
        if (R[k] > max_value) {
            max_value = R[k];
            max_angle = k;
        }
    }
    int b_w = bih.biWidth;
    int b_h = bih.biHeight;
    // fprintf(fp2,"�õ���ת�Ƕ�\n");

    //�õ�Ӧ����ת�Ƕ�:max_angle
    /***********************************************/

    /***********************************************/
    //ȥ��ͼƬ��������:
    //������ʼ��:���Ϸ�
    int point_exist = 0;
    int color       = 0;
    /*******************************************/
    //���1600 * 1200ͼƬ�������
    /*******************************************/
    if (bih.biWidth == 1600) {
        point_exist = 0;
        //ȥ����߱���

        //��ʼ����:�� 1304
        int pox_    = (int)((1304 / 1600.0) * bih.biWidth * 1.0);
        point_exist = 0;

        for (int i = 0; i < bih.biHeight; i++) {
            if (g[i * (bih.biWidth + nAdjust24) + pox_] == 255) {
                point_exist = 1;
                break;
            }
        }
        if (point_exist == 0) {
            for (int i = 0; i < bih.biHeight; i++) {
                ///�˴� �����43��Ϊ��ȥ����ߵ�б��
                for (int j = pox_ - 43; j < bih.biWidth; j++) {
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3]     = color;
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3 + 1] = color;
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3 + 2] = color;
                }
            }
        }

        //ȥ���·�����:
        //��ʼ����:(0,1173)-(1600,1200)
        point_exist = 0;
        int pox_2   = (int)((112 / 1200.0) * bih.biHeight * 1.0);
        for (int i = 0; i < bih.biWidth; i++) {
            if (g[pox_2 * (bih.biWidth + nAdjust24) + i] == 255) {
                point_exist = 1;
                break;
            }
        }
        if (point_exist == 0) {
            for (int i = 0; i < pox_2; i++) {
                for (int j = 0; j < bih.biWidth; j++) {
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3]     = color;
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3 + 1] = color;
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3 + 2] = color;
                }
            }
        }
        //ȥ���Ҳ౳��:
        point_exist = 0;
        int pox_3   = (int)((18 / 1600.0) * bih.biWidth * 1.0);

        for (int i = 0; i < bih.biHeight; i++) {
            if (g[i * (bih.biWidth + nAdjust24) + pox_3] == 255) {
                point_exist = 1;
                break;
            }
        }

        if (point_exist == 0) {
            for (int i = 0; i < bih.biHeight; i++) {
                for (int j = 0; j < pox_3; j++) {
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3]     = color;
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3 + 1] = color;
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3 + 2] = color;
                }
            }
        }

        /***************************/
        //�����Ƕ�Ӧtest������Ӧ������
        /*point_exist = 0;
        //ȥ����߱���

        //��ʼ����:�� 1346
        int pox_4 = (int)((1346 / 1600.0 ) * bih.biWidth * 1.0);
        point_exist = 0;


        for (int i = 0;i < bih.biHeight;i++) {
                if (g[i*(bih.biWidth+nAdjust24)+ pox_ ] == 255){
                        point_exist = 1;
                        break;
                }
        }
        if (point_exist == 0) {
                for (int i = 0; i < bih.biHeight;i++) {
                                ///�˴� �����43��Ϊ��ȥ����ߵ�б��
                                for (int j = pox_ - 43; j < bih.biWidth;j++) {
                                        pData24[i*(bih.biWidth * 3+nAdjust24)+j * 3] = 0;
                                        pData24[i*(bih.biWidth * 3+nAdjust24)+j * 3 + 1] = 0;
                                        pData24[i*(bih.biWidth * 3+nAdjust24)+j * 3 + 2] = 0;
                                }
                }
        }*/
    }

    if (bih.biWidth > 2000) {
        point_exist = 0;
        //ȥ����߱���

        //��ʼ����:�� ԭ:2296 ��:2216  ����:2592 - 88 = 2504
        int pox_5   = (int)((2504 / 2592.0) * bih.biWidth * 1.0);
        point_exist = 0;

        for (int i = 0; i < bih.biHeight; i++) {
            if (g[i * (bih.biWidth + nAdjust24) + pox_5] == 255) {
                point_exist = 1;
                break;
            }
        }

        if (point_exist == 0) {
            for (int i = 0; i < bih.biHeight; i++) {
                for (int j = pox_5; j < bih.biWidth; j++) {
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3]     = color;
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3 + 1] = color;
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3 + 2] = color;
                }
            }
        }
        //ȥ���Ϸ�����: 132 �� 196
        point_exist = 0;
        int pox_7   = (int)((196 / 1944.0) * bih.biHeight * 1.0);
        /*for (int i = 0;i < bih.biWidth;i++) {
                if (g[pox_7*(bih.biWidth+nAdjust24)+i] == 255){
                        point_exist = 1;
                        break;
                }
        }*/

        if (point_exist == 0) {
            for (int i = pox_7; i >= 0; i--) {
                for (int j = 0; j < bih.biWidth; j++) {
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3]     = color;
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3 + 1] = color;
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3 + 2] = color;
                }
            }
        }
        //ȥ���Ҳ౳��: 40 �� 0
        point_exist = 0;
        int pox_8   = (int)((0 / 2592.0) * bih.biWidth * 1.0);

        /*	for (int i = 0;i < bih.biHeight;i++) {
                        if (g[i*(bih.biWidth+nAdjust24)+pox_8] == 255){
                                point_exist = 1;
                                break;
                        }
                }*/

        if (point_exist == 0) {
            for (int i = 0; i < bih.biHeight; i++) {
                for (int j = 0; j < pox_8; j++) {
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3]     = color;
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3 + 1] = color;
                    pData24[i * (bih.biWidth * 3 + nAdjust24) + j * 3 + 2] = color;
                }
            }
        }
    }
    // fprintf(fp2,"ȥ�������������\n");
    /***********************************************/
    if (max_angle == 0) {
        // none
    } else if (max_angle > 0 && max_angle <= 90) {
        pData24 = imRotate_CannyFindEdge(pData24, b_w, b_h, -(max_angle - 90), &bih.biWidth, &bih.biHeight, NULL, data1);
        // imRotate_CannyFindEdge		canny����
        // imRotate_BinaryzationFindEdge	��ֵ������
    } else if (max_angle > 90) {
        pData24 = imRotate_CannyFindEdge(pData24, b_w, b_h, 90 - max_angle, &bih.biWidth, &bih.biHeight, NULL, data1);
    }

    //�ҳ��µ�������
    POX pox_new[2];
    int x_min, y_min, x_max, y_max;

    int _r = 123;
    int _b = 123;
    int _g = 123;

    pox_new[0].x = bih.biWidth + 1;
    pox_new[0].y = bih.biHeight + 1;
    pox_new[1].x = 0;
    pox_new[1].y = 0;
    for (int j = 0; j < bih.biHeight; j++) {
        for (int i = 0; i < bih.biWidth; i++) {
            if (
                pData24[j * (bih.biWidth * 3 + nAdjust24) + i * 3] == 123 &&
                pData24[j * (bih.biWidth * 3 + nAdjust24) + i * 3 + 1] == 123 &&
                pData24[j * (bih.biWidth * 3 + nAdjust24) + i * 3 + 2] == 123) {
                //�� i,j
                if (i < pox_new[0].x) {
                    pox_new[0].x = i;
                }
                if (i > pox_new[1].x) {
                    pox_new[1].x = i;
                }
                if (j < pox_new[0].y) {
                    pox_new[0].y = j;
                }
                if (j > pox_new[1].y) {
                    pox_new[1].y = j;
                }
            }
        }
        // printf("new:{%d,%d}\n",pox_new[0].x,pox_new[0].y);
        // printf("new:{%d,%d}\n",pox_new[1].x,pox_new[1].y);
    }
    /***********************************************/
    int h = pox_new[1].y - pox_new[0].y - 18;  //����
    int w = pox_new[1].x - pox_new[0].x - 18;  //����
    int nAdjust1;
    nAdjust1 = w * 3 % 4;
    if (nAdjust1) nAdjust1 = 4 - nAdjust1;
    BYTE* data11 = new BYTE[(w * 3 + nAdjust1) * h];
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            int poxx1         = j * (w * 3 + nAdjust1) + i * 3;
            int poxx2         = (pox_new[0].y + j + 9) * (bih.biWidth * 3 + nAdjust24) + (pox_new[0].x + i + 9) * 3;
            data11[poxx1]     = pData24[poxx2 + 2];
            data11[poxx1 + 1] = pData24[poxx2 + 1];
            data11[poxx1 + 2] = pData24[poxx2 + 0];
        }
    }

    delete[] c;
    free(R);

    RESULT s = {0, 0, data11};
    s.height = h;
    s.width  = w;

    //����ó��ü����С���ⶨ��С�ɽ������(����ǰ���ر������֤���)Ӧ������ü�
    if (s.height * s.width < 278 * 412) {
        s = s_temp;
    }
    delete[] data;
    delete[] pData24;
    delete[] pp;
    return s;
}

/**************************************************************************
�ñߺ���2
PBYTE output2(PBYTE data,int width,int height)
PBYTE data:			ͼ������
int width:			ͼ����
int height:		ͼ��߶�
����ͼ������
**************************************************************************/
extern "C" __declspec(dllexport) RESULT output2(PBYTE data, int width, int height) {
    int nAdjust24 = width * 3 % 4;
    if (nAdjust24) nAdjust24 = 4 - nAdjust24;
    BYTE* data_T = new BYTE[(width * 3 + nAdjust24) * height];
    //�ı�˳��
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            data_T[j * (width * 3 + nAdjust24) + i * 3 + 0] = data[j * (width * 3 + nAdjust24) + (width * 3 + nAdjust24 - i * 3 - 1)];
            data_T[j * (width * 3 + nAdjust24) + i * 3 + 1] = data[j * (width * 3 + nAdjust24) + (width * 3 + nAdjust24 - i * 3 - 1 - 1)];
            data_T[j * (width * 3 + nAdjust24) + i * 3 + 2] = data[j * (width * 3 + nAdjust24) + (width * 3 + nAdjust24 - i * 3 - 2 - 1)];
        }
    }

    RESULT s = {0, 0, data_T};
    //�ñߺ���
    s = Bmp_Cut2(data_T, width, height);
    return s;
}

/**************************************************************************
�ñߺ���3
void output3(PBYTE data,int width,int height,char *out)
PBYTE data:			ͼ������
int width:			ͼ����
int height:		ͼ��߶�
char *out:			���·��
**************************************************************************/
extern "C" __declspec(dllexport) void output3(PBYTE data, int width, int height, char* out) {
    int nAdjust24 = width * 3 % 4;
    if (nAdjust24) nAdjust24 = 4 - nAdjust24;
    BYTE* data_T = new BYTE[(width * 3 + nAdjust24) * height];

    //�ı�˳��
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            data_T[j * (width * 3 + nAdjust24) + i * 3 + 0] = data[j * (width * 3 + nAdjust24) + (width * 3 + nAdjust24 - i * 3 - 1)];
            data_T[j * (width * 3 + nAdjust24) + i * 3 + 1] = data[j * (width * 3 + nAdjust24) + (width * 3 + nAdjust24 - i * 3 - 1 - 1)];
            data_T[j * (width * 3 + nAdjust24) + i * 3 + 2] = data[j * (width * 3 + nAdjust24) + (width * 3 + nAdjust24 - i * 3 - 2 - 1)];
        }
    }

    RESULT s = {0, 0, data_T};
    s        = Bmp_Cut2(data_T, width, height);

    /***********************************/
    _BITMAPINFOHEADER bih = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // bmpͷ��Ϣ
    int               nAdjust;                                  // �����ֽڶ���
    /*****************************/
    bih.biSize          = 40;
    bih.biWidth         = s.width;
    bih.biHeight        = s.height;
    bih.biPlanes        = 1;
    bih.biBitCount      = 24;
    bih.biCompression   = BI_RGB;
    bih.biSizeImage     = ((bih.biWidth % 4 == 0) ? bih.biWidth : (bih.biWidth + bih.biWidth % 4)) * bih.biHeight;
    bih.biXPelsPerMeter = 300;
    bih.biYPelsPerMeter = 300;
    bih.biClrUsed       = 0;
    bih.biClrImportant  = 0;

    nAdjust = bih.biWidth % 4;
    if (nAdjust) nAdjust = 4 - nAdjust;
    // 24λ�ֽڶ���
    nAdjust24 = bih.biWidth * 3 % 4;
    if (nAdjust24) nAdjust24 = 4 - nAdjust24;
    /****************************/

    //��λͼ��Ϣ��װ
    BMPINFOMATION bmp;

    bmp.bih        = bih;
    bmp.nComponent = 3;
    bmp.data       = s.data;
    bmp.pData24    = s.data;
    bmp.nAdjust24  = nAdjust24;
    bmp.nAdjust    = nAdjust;
    // ����ͼ���ȡ���
    saveAsBmp(bmp, out);
    /*********************************/
}
