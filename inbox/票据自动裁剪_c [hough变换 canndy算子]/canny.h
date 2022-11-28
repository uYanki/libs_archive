
#include <time.h>
#include "math.h"

// һά��˹�ֲ�����,����ƽ�����������ɵĸ�˹�˲�ϵ��
void CreatGauss(double sigma, double** pdKernel, int* pnWidowSize) {
    LONG i;

    //�������ĵ�
    int nCenter;

    //������һ�㵽���ĵ����
    double dDis;

    //�м����
    double dValue;
    double dSum;
    dSum = 0;

    // [-3*sigma,3*sigma] ��������,�Ḳ�Ǿ��󲿷��˲�ϵ��
    *pnWidowSize = (int)(1 + 2 * ceil(3 * sigma));

    nCenter = (*pnWidowSize) / 2;

    *pdKernel = new double[*pnWidowSize];

    //���ɸ�˹����
    for (i = 0; i < (*pnWidowSize); i++) {
        dDis           = double(i - nCenter);
        dValue         = exp(-(1 / 2) * dDis * dDis / (sigma * sigma)) / (sqrt(2 * 3.1415926) * sigma);
        (*pdKernel)[i] = dValue;
        dSum += dValue;
    }
    //��һ��
    for (i = 0; i < (*pnWidowSize); i++) {
        (*pdKernel)[i] /= dSum;
    }
}

//�ø�˹�˲���ƽ��ԭͼ��
void GaussianSmooth(SIZE sz, LPBYTE pGray, LPBYTE pResult, double sigma) {
    LONG x, y;
    LONG i;

    //��˹�˲�������
    int nWindowSize;

    //���ڳ���
    int nLen;

    //һά��˹�˲���
    double* pdKernel;

    //��˹ϵ����ͼ�����ݵĵ��
    double dDotMul;

    //�˲�ϵ���ܺ�
    double dWeightSum;

    double* pdTemp;
    pdTemp = new double[sz.cx * sz.cy];

    //����һά��˹����
    CreatGauss(sigma, &pdKernel, &nWindowSize);

    nLen = nWindowSize / 2;

    // x�����˲�
    for (y = 0; y < sz.cy; y++) {
        for (x = 0; x < sz.cx; x++) {
            dDotMul    = 0;
            dWeightSum = 0;
            for (i = (-nLen); i <= nLen; i++) {
                //�ж��Ƿ���ͼ���ڲ�
                if ((i + x) >= 0 && (i + x) < sz.cx) {
                    dDotMul += (double)pGray[y * sz.cx + (i + x)] * pdKernel[nLen + i];
                    dWeightSum += pdKernel[nLen + i];
                }
            }
            pdTemp[y * sz.cx + x] = dDotMul / dWeightSum;
        }
    }

    // y�����˲�
    for (x = 0; x < sz.cx; x++) {
        for (y = 0; y < sz.cy; y++) {
            dDotMul    = 0;
            dWeightSum = 0;
            for (i = (-nLen); i <= nLen; i++) {
                if ((i + y) >= 0 && (i + y) < sz.cy) {
                    dDotMul += (double)pdTemp[(y + i) * sz.cx + x] * pdKernel[nLen + i];
                    dWeightSum += pdKernel[nLen + i];
                }
            }
            pResult[y * sz.cx + x] = (BYTE)((unsigned char)dDotMul / dWeightSum);
        }
    }

    delete[] pdKernel;
    pdKernel = NULL;

    delete[] pdTemp;
    pdTemp = NULL;
}

// ������,���ݶ�
void Grad(SIZE sz, LPBYTE pGray, int* pGradX, int* pGradY, int* pMag) {
    LONG y, x;

    // x����ķ�����
    for (y = 1; y < sz.cy - 1; y++) {
        for (x = 1; x < sz.cx - 1; x++) {
            pGradX[y * sz.cx + x] = (int)(pGray[y * sz.cx + x + 1] - pGray[y * sz.cx + x - 1]);
        }
    }

    // y��������
    for (x = 1; x < sz.cx - 1; x++) {
        for (y = 1; y < sz.cy - 1; y++) {
            pGradY[y * sz.cx + x] = (int)(pGray[(y + 1) * sz.cx + x] - pGray[(y - 1) * sz.cx + x]);
        }
    }

    //���ݶ�

    //�м����
    double dSqt1;
    double dSqt2;

    for (y = 0; y < sz.cy; y++) {
        for (x = 0; x < sz.cx; x++) {
            //���׷������ݶ�
            dSqt1               = pGradX[y * sz.cx + x] * pGradX[y * sz.cx + x];
            dSqt2               = pGradY[y * sz.cx + x] * pGradY[y * sz.cx + x];
            pMag[y * sz.cx + x] = (int)(sqrt(dSqt1 + dSqt2) + 0.5);
        }
    }
}

//���������
void NonmaxSuppress(int* pMag, int* pGradX, int* pGradY, SIZE sz, LPBYTE pNSRst) {
    LONG y, x;
    int  nPos;

    //�ݶȷ���
    int gx;
    int gy;

    //�м����
    int    g1, g2, g3, g4;
    double weight;
    double dTmp, dTmp1, dTmp2;

    //����ͼ���ԵΪ�����ܵķֽ��
    for (x = 0; x < sz.cx; x++) {
        pNSRst[x]                       = 0;
        pNSRst[(sz.cy - 1) * sz.cx + x] = 0;
    }
    for (y = 0; y < sz.cy; y++) {
        pNSRst[y * sz.cx]             = 0;
        pNSRst[y * sz.cx + sz.cx - 1] = 0;
    }

    for (y = 1; y < sz.cy - 1; y++) {
        for (x = 1; x < sz.cx - 1; x++) {
            //��ǰ��
            nPos = y * sz.cx + x;

            //�����ǰ�����ݶȷ���Ϊ0,���Ǳ߽��
            if (pMag[nPos] == 0) {
                pNSRst[nPos] = 0;
            } else {
                //��ǰ����ݶȷ���
                dTmp = pMag[nPos];

                // x,y������
                gx = pGradX[nPos];
                gy = pGradY[nPos];

                //���������y������x������,˵����������������y����
                if (abs(gy) > abs(gx)) {
                    //�����ֵ����
                    weight = (double)(fabs((double)gx) / fabs((double)gy));

                    g2 = pMag[nPos - sz.cx];
                    g4 = pMag[nPos + sz.cx];

                    //���x,y�����������ķ�����ͬ
                    // C Ϊ��ǰ����,��g1-g4 ��λ�ù�ϵΪ:
                    // g1 g2
                    // C
                    // g4 g3
                    if (gx * gy > 0) {
                        g1 = pMag[nPos - sz.cx - 1];
                        g3 = pMag[nPos + sz.cx + 1];
                    }

                    //���x,y��������ķ����������෴
                    // C�ǵ�ǰ����,��g1-g4�Ĺ�ϵΪ:
                    // g2 g1
                    // C
                    // g3 g4
                    else {
                        g1 = pMag[nPos - sz.cx + 1];
                        g3 = pMag[nPos + sz.cx - 1];
                    }
                }

                //���������x������y������,˵�������ķ���������x����
                else {
                    //��ֵ����
                    weight = (double)(fabs((double)gy) / fabs((double)gx));

                    g2 = pMag[nPos + 1];
                    g4 = pMag[nPos - 1];

                    //���x,y��������ķ�����������ͬ
                    //��ǰ����C�� g1-g4�Ĺ�ϵΪ
                    // g3
                    // g4 C g2
                    // g1
                    if (gx * gy > 0) {
                        g1 = pMag[nPos + sz.cx + 1];
                        g3 = pMag[nPos - sz.cx - 1];
                    }

                    //���x,y�����������ķ����෴
                    // C��g1-g4�Ĺ�ϵΪ
                    // g1
                    // g4 C g2
                    // g3
                    else {
                        g1 = pMag[nPos - sz.cx + 1];
                        g3 = pMag[nPos + sz.cx - 1];
                    }
                }

                //���� g1-g4 ���ݶȽ��в�ֵ
                {
                    dTmp1 = weight * g1 + (1 - weight) * g2;
                    dTmp2 = weight * g3 + (1 - weight) * g4;

                    //��ǰ���ص��ݶ��Ǿֲ������ֵ
                    //�õ�����Ǳ߽��
                    if (dTmp >= dTmp1 && dTmp >= dTmp2) {
                        pNSRst[nPos] = 128;
                    } else {
                        //�������Ǳ߽��
                        pNSRst[nPos] = 0;
                    }
                }
            }
        }
    }
}

// ͳ��pMag��ֱ��ͼ,�ж���ֵ
void EstimateThreshold(int* pMag, SIZE sz, int* pThrHigh, int* pThrLow, LPBYTE pGray, double dRatHigh, double dRatLow) {
    LONG y, x, k;

    //������Ĵ�С���ݶ�ֵ�ķ�Χ�й�,������ñ�������㷨
    //��ô�ݶȵķ�Χ���ᳬ��pow(2,10)
    int nHist[256];

    //���ܱ߽���
    int nEdgeNum;

    //����ݶ���
    int nMaxMag;

    int nHighCount;

    nMaxMag = 0;

    //��ʼ��
    for (k = 0; k < 256; k++) {
        nHist[k] = 0;
    }
    //ͳ��ֱ��ͼ,����ֱ��ͼ������ֵ
    for (y = 0; y < sz.cy; y++) {
        for (x = 0; x < sz.cx; x++) {
            if (pGray[y * sz.cx + x] == 128) {
                nHist[pMag[y * sz.cx + x]]++;
            }
        }
    }

    nEdgeNum = nHist[0];
    nMaxMag  = 0;

    //ͳ�ƾ���"�����ֵ����"���ж�������
    for (k = 1; k < 256; k++) {
        if (nHist[k] != 0) {
            nMaxMag = k;
        }

        //�ݶ�Ϊ0�ĵ��ǲ�����Ϊ�߽���
        //����non-maximum suppression���ж�������
        nEdgeNum += nHist[k];
    }

    //�ݶȱȸ���ֵ*pThrHigh С�����ص�����Ŀ
    nHighCount = (int)(dRatHigh * nEdgeNum + 0.5);

    k        = 1;
    nEdgeNum = nHist[1];

    //�������ֵ
    while ((k < (nMaxMag - 1)) && (nEdgeNum < nHighCount)) {
        k++;
        nEdgeNum += nHist[k];
    }

    //����ֵ
    //*pThrHigh =k;

    //����ֵ
    //*pThrLow = (int)((*pThrHigh) * dRatLow + 0.5);
}

void autoGetValue(SIZE sz, int* pThrHigh, int* pThrLow, int* pMag) {
    //����Ӧ����ֵ����
    double P[256];  //�Ҷȸ���ֵ
    for (int i = 0; i < 256; i++) {
        P[i] = 0;
    }
    // LPBYTE pGray ����
    for (LONG y = 0; y < sz.cy; y++) {
        for (LONG x = 0; x < sz.cx; x++) {
            P[pMag[y * sz.cx + x]]++;  //��Ӧ�ĻҶ�ֵ��1
                                       // printf ("%d\n",pGray[y*sz.cx+x]);
        }
    }
    for (int i = 0; i < 256; i++) {
        // printf ("h:%d,v:%d\n",i,P[i]);
        P[i] /= (sz.cx * sz.cy * 1.0);
        // printf ("h:%d,v:%f\n",i,P[i]);
    }

    //ǰn��ĸ��ʺ�
    double P_M[256];
    P_M[0] = P[0];
    for (int i = 1; i < 256; i++) {
        P_M[i] = P_M[i - 1] + P[i];
        // printf ("ǰn�Ҷȸ���:%d,v:%f\n",i,P_M[i]);
    }
    //ǰN���������
    double P_M_I[256];
    P_M_I[0] = 0;
    for (int i = 1; i < 256; i++) {
        P_M_I[i] = i * P[i] + P_M_I[i - 1];
        // printf ("ǰn�Ҷ�����:%d,v:%f\n",i,P_M_I[i]);
    }
    int k = 0;
    int m = 0;
    int go_on;
    for (k = 0; k < 256; k++) {
        go_on = 1;
        for (m = 0; m < 256; m++) {
            if (fabs(2.0 * k - (P_M_I[k] / (P_M[k] * 1.0)) - ((P_M_I[m] - P_M_I[k]) / (P_M[m] - P_M[k]))) <= 1 &&
                fabs(2.0 * m - ((P_M_I[m] - P_M_I[k]) / (P_M[m] - P_M[k])) - ((P_M_I[255] - P_M_I[m]) / (P_M[255] - P_M[m]))) <= 1

            ) {
                // printf ("have?");
                go_on = 0;
                break;
            }  //�ҵ������ֵ
        }
        if (!go_on) break;
    }
    *pThrHigh = m;
    *pThrLow  = k;
}
//����Hysteresis ִ�еĽ��,��һ�����ص㿪ʼ����,�����Ը����ص�Ϊ�߽�����һ���߽��
//һ���߽�����б߽��,���������˵ݹ��㷨
// ��(x,y)�������,���б߽��ĸ���,����ֻ����pResult��û�д����ҿ����Ǳ߽�
// �������(=128),����ֵΪ0�����õ㲻�����Ǳ߽��,����ֵΪ255�����õ��Ѿ��Ǳ߽��

void TraceEdge(int y, int x, int nThrLow, LPBYTE pResult, int* pMag, SIZE sz) {
    //��8�������ؽ��в�ѯ
    int xNum[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int yNum[8] = {0, 1, 1, 1, 0, -1, -1, -1};

    LONG yy, xx, k;

    for (k = 0; k < 8; k++) {
        yy = y + yNum[k];
        xx = x + xNum[k];
        //������Χ
        if (xx < 0 || yy < 0 || xx > 500 || yy > 500)
            continue;

        if (pResult[yy * sz.cx + xx] == 128 && pMag[yy * sz.cx + xx] >= nThrLow) {
            //�õ���Ϊ�߽��
            pResult[yy * sz.cx + xx] = 255;
            //�Ըõ�Ϊ�����ٽ��и���
            TraceEdge(yy, xx, nThrLow, pResult, pMag, sz);
        }
    }
}
//���ú���Ѱ�ұ߽����
void Hysteresis(int* pMag, SIZE sz, double dRatLow, double dRatHigh, LPBYTE pResult) {
    LONG y, x;

    int nThrHigh, nThrLow;

    int nPos;
    //����TraceEdge ������Ҫ�ĵ���ֵ,�Լ�Hysteresis����ʹ�õĸ���ֵ
    // EstimateThreshold(pMag, sz,&nThrHigh,&nThrLow,pResult,dRatHigh,dRatLow);
    //����Ӧ��ֵ
    autoGetValue(sz, &nThrHigh, &nThrLow, pMag);
    // printf("����ֵ:%d,����ֵ:%d\n",nThrHigh,nThrLow);
    //Ѱ�Ҵ���dThrHigh�ĵ�,��Щ�����������߽��,
    //Ȼ����TraceEdge�������ٸõ��Ӧ�ı߽�
    for (y = 0; y < sz.cy; y++) {
        for (x = 0; x < sz.cx; x++) {
            nPos = y * sz.cx + x;

            //����������ǿ��ܵı߽��,�����ݶȴ��ڸ���ֵ,
            //��������Ϊһ���߽�����
            if ((pResult[nPos] == 128)) {
                if (pMag[nPos] >= nThrHigh) {
                    //���øõ�Ϊ�߽��

                    pResult[nPos] = 255;
                    TraceEdge(y, x, nThrLow, pResult, pMag, sz);
                }
            }
        }
    }
    //�������Ѿ�������Ϊ�߽��
    for (y = 0; y < sz.cy; y++) {
        for (x = 0; x < sz.cx; x++) {
            nPos = y * sz.cx + x;

            if (pResult[nPos] != 255) {
                pResult[nPos] = 0;
            }
        }
    }
}

void gaussianFilter(LPBYTE data, SIZE sz) {
    int width  = sz.cx;
    int height = sz.cy;
    int i, j, index, sum;
    int templates[9] = {
        1, 2, 1,
        2, 4, 2,
        1, 2, 1};
    sum            = height * width * sizeof(BYTE);
    LPBYTE tmpdata = (LPBYTE)malloc(sum);
    memcpy((BYTE*)tmpdata, (BYTE*)data, sum);
    for (i = 1; i < height - 1; i++) {
        for (j = 1; j < width - 1; j++) {
            index = sum = 0;
            for (int m = i - 1; m < i + 2; m++) {
                for (int n = j - 1; n < j + 2; n++) {
                    sum += tmpdata[m * width + n] * templates[index++];
                }
            }
            data[i * width + j] = sum / 16;
        }
    }
    free(tmpdata);
}

//��ֵ�˲���
void MedianFilter(LPBYTE data, SIZE sz, int size) {
    // FILE *fp2;
    //  fp2 = fopen("f:\\MedianFilter��ͨ.txt","w");
    //  clock_t start, finish;
    //  start = clock();

    int width  = sz.cx;
    int height = sz.cy;
    int index, sum;
    // int size = 7 * 5;
    int  left_pox = ((int)sqrt(size * 1.0)) / 2;
    int* values   = new int[size];
    for (int i = 0; i < size; i++) {
        values[i] = 0;
    }
    sum = height * width * sizeof(BYTE);
    // LPBYTE tmpdata = (LPBYTE)malloc(sum);
    // memcpy((BYTE*)tmpdata,(BYTE*)data, sum);
    int count = 0;
    int a, b;
    for (int i = left_pox; i < height - left_pox; i++) {
        for (int j = left_pox; j < width - left_pox; j++) {
            index = sum = 0;
            a = b = 0;
            for (int m = i - left_pox; m < i + left_pox + 1; m++) {
                for (int n = j - left_pox; n < j + left_pox + 1; n++) {
                    values[index] = data[m * width + n];
                    if (values[index] == 0) {
                        a++;
                    } else {
                        b++;
                    }
                    index++;
                }
                //�Ѿ�ȷ����.
                if (a > size / 2) {
                    break;
                }
                if (b > size / 2) {
                    break;
                }
            }
            if (a > b) {
                data[i * width + j] = 0;
            } else {
                data[i * width + j] = 255;
            }

            /*

            index = 0;
            for(int m = i - left_pox;m < i + left_pox + 1;m++){
                    for(int n = j - left_pox; n < j + left_pox + 1;n++){
                            values[index] = data[m * width + n];
                            index++;
                    }
            }
            //ð��
            for(int i_=0;i_< size - 1;i_++){
                    for(int j_ =0;j_<size - i_ - 1;j_++){
                            if(values[j_]>values[j_+1]){
                                    int t = values[j_];
                                    values[j_] = values[j_+1];
                                    values[j_+1] = t;
                            }
                    }
            }

            //ȡ��ֵ
            if (data[i * width + j] != values[(int)(size/2)]) {
                    data[i * width + j] = values[(int)(size/2)];
            //count++;
            }

            */
        }
    }
    // printf("=======================================�˵�:%d\n",count);
    delete[] values;

    // finish = clock();
    // double duration = (double)(finish - start) / CLOCKS_PER_SEC;
    /// fprintf(fp2,"��ֵ�˲�:%lf.\n",duration);
    // fclose(fp2);
    // free(tmpdata);
}

// Canny����
void Canny(LPBYTE pGray, SIZE sz, double sigma, LPBYTE pResult) {
    double dRatLow  = 0;
    double dRatHigh = 0;
    //������˹�˲����ͼ��
    LPBYTE pGaussSmooth;
    pGaussSmooth = new unsigned char[sz.cx * sz.cy];

    // x��������ָ��
    int* pGradX;
    pGradX = new int[sz.cx * sz.cy];

    // y����
    int* pGradY;
    pGradY = new int[sz.cx * sz.cy];

    //�ݶȵķ���
    int* pGradMag;
    pGradMag = new int[sz.cx * sz.cy];
    //��ԭͼ��˹�˲�
    GaussianSmooth(sz, pGray, pGaussSmooth, sigma);
    //���㷽�������ݶȵķ���
    Grad(sz, pGaussSmooth, pGradX, pGradY, pGradMag);

    //Ӧ�÷��������
    NonmaxSuppress(pGradMag, pGradX, pGradY, sz, pResult);
    //Ӧ��Hysteresis,�ҵ����б߽�
    Hysteresis(pGradMag, sz, dRatLow, dRatHigh, pResult);
    //�Ա߽���и�˹�˲�
    // gaussianFilter(pResult,sz);
    //��ֵ�˲�
    // MedianFilter(pResult,sz,5 * 3);
    //��Եϸ��
    // ToOnePointWide(pResult,sz.cy,sz.cx,3);
    delete[] pGradX;
    pGradX = NULL;
    delete[] pGradY;
    pGradY = NULL;
    delete[] pGradMag;
    pGradMag = NULL;
    delete[] pGaussSmooth;
    pGaussSmooth = NULL;
}
