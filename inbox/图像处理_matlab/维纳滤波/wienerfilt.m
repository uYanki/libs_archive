% ԭͼ��
originImage = im2double(rgb2gray(imread('lena.jpg')));
% ������������ͼ��
% imageWithNoise = im2double(imread('lena_noise.jpg'));
% ����˹������ͼ��
imageWithNoise = im2double(imread('lena_gaussian_noise.jpg'));
[rows, cols] = size(originImage);
figure(1), imshow(originImage), title('ԭͼ��');
figure(2), imshow(imageWithNoise), title('����ͼ��');
y = imageWithNoise;

% ά���˲�ʵ��
% ָ��ģ��ߴ�
boxSize = [7 7];
g = imageWithNoise;
% ���ƾ�ֵ
localMean = filter2(ones(boxSize), g) / prod(boxSize);
% ���Ʒ���
localVar = filter2(ones(boxSize), g.^2) / prod(boxSize) - localMean.^2;
% ��������
noise = mean2(localVar);
% ά�ɷ�����
y = localMean + (max(0, localVar - noise) ./ max(localVar, noise)) .* (g - localMean);

figure(3), imshow(y), title('�˲���ͼ');

% �����˲���MSE
se2 = (y - originImage).^2;
MSE2 = sum(se2(:)) / (rows * cols);
fprintf('�˲���MSE=%f\n', MSE2);