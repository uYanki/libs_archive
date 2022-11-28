% ԭͼ��
originImage = im2double(rgb2gray(imread('lena.jpg')));
% ������������ͼ��
imageWithNoise = im2double(imread('lena_noise.jpg'));
% ����˹������ͼ��
% imageWithNoise = im2double(imread('lena_gaussian_noise.jpg'));
[rows, cols] = size(originImage);
figure(1), imshow(originImage), title('ԭͼ��');
figure(2), imshow(imageWithNoise), title('����ͼ��');
y = imageWithNoise;

% ��ֵ�˲��㷨
% ָ��ģ��ߴ�
boxSize = 5;
template = zeros(boxSize);
for i = 1:rows-boxSize+1
    for j = 1:cols-boxSize+1
        % ȡģ��������
        template = imageWithNoise(i:i+(boxSize-1),j:j+(boxSize-1));
        % �þ�ֵ���ģ�����ĵ�����ֵ
        s = sum(template(:));
        y(i+(boxSize-1)/2,j+(boxSize-1)/2) = s/boxSize^2;
    end
end

figure(3), imshow(y), title('�˲���ͼ');

% �����˲���MSE
se2 = (y - originImage).^2;
MSE2 = sum(se2(:)) / (rows * cols);
fprintf('�˲���MSE=%f\n', MSE2);