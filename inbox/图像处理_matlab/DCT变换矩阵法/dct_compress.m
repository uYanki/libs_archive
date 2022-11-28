% ͼ���ȡ��ҶȻ�
gray = rgb2gray(imread('lena.jpg'));

% ��ͼ������uint8����ת��Ϊdouble���Ͳ����й�һ��
doubleGray = im2double(gray);

% ����DCT�任����
t = dctmtx(8);

% ��ԭͼ��ÿ��8x8�����DCT�任
y = blkproc(doubleGray, [8 8], 'P1*x*P2', t, t');

figure(2), imshow(log(abs(y)), []), title('DCTϵ��');

 
% ������������
mask1 = [1 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0];
mask2 = [1 1 0 0 0 0 0 0;
         1 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0];
mask3 = [1 1 1 0 0 0 0 0;
         1 1 0 0 0 0 0 0;
         1 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0];
mask4 = [1 1 1 1 0 0 0 0;
         1 1 1 0 0 0 0 0;
         1 1 0 0 0 0 0 0;
         1 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0];
mask5 = [1 1 1 1 1 0 0 0;
         1 1 1 1 0 0 0 0;
         1 1 1 0 0 0 0 0;
         1 1 0 0 0 0 0 0;
         1 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0];
mask6 = [1 1 1 1 1 1 0 0;
         1 1 1 1 1 0 0 0;
         1 1 1 1 0 0 0 0;
         1 1 1 0 0 0 0 0;
         1 1 0 0 0 0 0 0;
         1 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0];
mask7 = [1 1 1 1 1 1 1 0;
         1 1 1 1 1 1 0 0;
         1 1 1 1 1 0 0 0;
         1 1 1 1 0 0 0 0;
         1 1 1 0 0 0 0 0;
         1 1 0 0 0 0 0 0;
         1 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 0];
mask8 = [1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 0;
         1 1 1 1 1 1 0 0;
         1 1 1 1 1 0 0 0;
         1 1 1 1 0 0 0 0;
         1 1 1 0 0 0 0 0;
         1 1 0 0 0 0 0 0;
         1 0 0 0 0 0 0 0];
mask9 = [0 0 1 1 1 1 1 1;
         0 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1];
mask10= [0 0 0 0 1 1 1 1;
         0 0 0 1 1 1 1 1;
         0 0 1 1 1 1 1 1;
         0 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1];
mask11= [0 0 0 0 0 0 0 0;
         0 0 0 0 0 0 0 1;
         0 0 0 0 0 0 1 1;
         0 0 0 0 0 1 1 1;
         0 0 0 0 1 1 1 1;
         0 0 0 1 1 1 1 1;
         0 0 1 1 1 1 1 1;
         0 1 1 1 1 1 1 1];
maskall=[1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1;
         1 1 1 1 1 1 1 1];
 
% ��DCTϵ�������������
my1 = blkproc(y, [8 8], 'P1.*x', mask1);
my2 = blkproc(y, [8 8], 'P1.*x', mask2);
my3 = blkproc(y, [8 8], 'P1.*x', mask3);
my4 = blkproc(y, [8 8], 'P1.*x', mask4);
my5 = blkproc(y, [8 8], 'P1.*x', mask5);
my6 = blkproc(y, [8 8], 'P1.*x', mask6);
my7 = blkproc(y, [8 8], 'P1.*x', mask7);
my8 = blkproc(y, [8 8], 'P1.*x', mask8);
my9 = blkproc(y, [8 8], 'P1.*x', mask9);
my10= blkproc(y, [8 8], 'P1.*x', mask10);
my11= blkproc(y, [8 8], 'P1.*x', mask11);
myall=blkproc(y, [8 8], 'P1.*x', maskall);

% ��ÿ��8x8�����IDCT�任���ָ�ͼ��
y1 = blkproc(my1, [8 8], 'P1*x*P2', t', t);
y2 = blkproc(my2, [8 8], 'P1*x*P2', t', t);
y3 = blkproc(my3, [8 8], 'P1*x*P2', t', t);
y4 = blkproc(my4, [8 8], 'P1*x*P2', t', t);
y5 = blkproc(my5, [8 8], 'P1*x*P2', t', t);
y6 = blkproc(my6, [8 8], 'P1*x*P2', t', t);
y7 = blkproc(my7, [8 8], 'P1*x*P2', t', t);
y8 = blkproc(my8, [8 8], 'P1*x*P2', t', t);
y9 = blkproc(my9, [8 8], 'P1*x*P2', t', t);
y10= blkproc(my10,[8 8], 'P1*x*P2', t', t);
y11= blkproc(my11,[8 8], 'P1*x*P2', t', t);
yall=blkproc(myall,[8 8],'P1*x*P2', t', t);
 
% ��ʾ��ѹ����ͼ�����ѹ����ͼ��
figure(3), imshow(doubleGray), title('ԭͼ');
figure(4), imshow(y1), title('����ϵ��Ϊ1��ѹ��ͼ��');
figure(5), imshow(y2), title('����ϵ��Ϊ2��ѹ��ͼ��');
figure(6), imshow(y3), title('����ϵ��Ϊ3��ѹ��ͼ��');
figure(7), imshow(y4), title('����ϵ��Ϊ4��ѹ��ͼ��');
figure(8), imshow(y5), title('����ϵ��Ϊ5��ѹ��ͼ��');
figure(9), imshow(y6), title('����ϵ��Ϊ6��ѹ��ͼ��');
figure(10),imshow(y7), title('����ϵ��Ϊ7��ѹ��ͼ��');
figure(11),imshow(y8), title('����ϵ��Ϊ8��ѹ��ͼ��');
figure(12),imshow(y9), title('������Ƶ2λ��ѹ��ͼ��');
figure(13),imshow(y10),title('������Ƶ4λ��ѹ��ͼ��');
figure(14),imshow(y11),title('������Ƶ8λ��ѹ��ͼ��');
figure(15),imshow(yall),title('��������');
 
% �����ӦMSEֵ
shape = size(doubleGray);
disp('����ϵ��Ϊ1��ѹ��ͼ��MSE: ');
se = (doubleGray-y1).^2;
sum(se(:)) / (shape(1)*shape(2))
disp('����ϵ��Ϊ2��ѹ��ͼ��MSE: ');
se = (doubleGray-y2).^2;
sum(se(:)) / (shape(1)*shape(2))
disp('����ϵ��Ϊ3��ѹ��ͼ��MSE: ');
se = (doubleGray-y3).^2;
sum(se(:)) / (shape(1)*shape(2))
disp('����ϵ��Ϊ4��ѹ��ͼ��MSE: ');
se = (doubleGray-y4).^2;
sum(se(:)) / (shape(1)*shape(2))
disp('����ϵ��Ϊ5��ѹ��ͼ��MSE: ');
se = (doubleGray-y5).^2;
sum(se(:)) / (shape(1)*shape(2))
disp('����ϵ��Ϊ6��ѹ��ͼ��MSE: ');
se = (doubleGray-y6).^2;
sum(se(:)) / (shape(1)*shape(2))
disp('����ϵ��Ϊ7��ѹ��ͼ��MSE: ');
se = (doubleGray-y7).^2;
sum(se(:)) / (shape(1)*shape(2))
disp('����ϵ��Ϊ8��ѹ��ͼ��MSE: ');
se = (doubleGray-y8).^2;
sum(se(:)) / (shape(1)*shape(2))
disp('������Ƶ2λ��ѹ��ͼ��MSE: ');
se = (doubleGray-y9).^2;
sum(se(:)) / (shape(1)*shape(2))
disp('������Ƶ4λ��ѹ��ͼ��MSE: ');
se = (doubleGray-y10).^2;
sum(se(:)) / (shape(1)*shape(2))
disp('������Ƶ8λ��ѹ��ͼ��MSE: ');
se = (doubleGray-y11).^2;
sum(se(:)) / (shape(1)*shape(2))
disp('����ȫ��MSE: ');
se = (doubleGray-yall).^2;
sum(se(:)) / (shape(1)*shape(2))