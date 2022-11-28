ratio = input('radio=');
grayImage = rgb2gray(imread('lena.jpg'));
doubleData = im2double(grayImage);
% ��ͼ�����FFT�任�õ�FFTϵ������
dctSeries = fft2(doubleData);
figure(1), imshow(log(abs(dctSeries)), []), title('ԭFFTϵ������');

[rows, cols] = size(grayImage);
% ��FFTϵ����������ΪԲ�ģ��ֱ��Բ�ͬ�İ뾶��Բ��ϵ����Ϊ��
for i = 1:rows
    for j = 1:cols
        if sqrt((i-rows/2)^2+(j-cols/2)^2)/sqrt((rows/2)^2+(cols/2)^2) < 1-ratio
            dctSeries(i,j) = 0;
        end
    end
end
figure(2), imshow(log(abs(dctSeries)),[]), title('������FFTϵ������');

% ���������FFTϵ���������IFFT�任��ԭͼ��
y = ifft2(dctSeries);
figure(4), imshow(y), title('��ԭͼ��');

fprintf('ѹ������Ϊ%fʱ, MSE=\n', ratio)
se = abs((y - doubleData).^2);
sum(se(:)) / (rows * cols)