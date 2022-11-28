ratio = input('radio=');
grayImage = rgb2gray(imread('lena.jpg'));
doubleData = im2double(grayImage);
% ������ͼ�����DCT�任
dctSeries = dct2(doubleData);
figure(1), imshow(log(abs(dctSeries)), []), title('DCTϵ������');
[rows, cols] = size(grayImage);
% ��ϵ����������ͬ����������
for i = 1:rows
    for j = 1:cols
        if (i+j>(rows+cols)*ratio)
            dctSeries(i,j) = 0;
        end
    end
end
figure(2), imshow(log(abs(dctSeries)),[]), title('������DCTϵ������');
% ��DCTϵ��������IDCT�任,�ָ�ͼ��
y = idct2(dctSeries);
figure(3), imshow(y), title('��ԭͼ��');

fprintf('ѹ������Ϊ%fʱ, MSE=\n', ratio)
se = (y - doubleData).^2;
sum(se(:)) / (rows * cols)