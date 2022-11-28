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

% ��˹�˲�ʵ���㷨
% ָ��ģ��ߴ�
boxSize = 5;
box = [boxSize boxSize];
% ָ��������׼��
sigma = 1;
siz = (box-1)/2;
% ����Ȩ�ؾ���
[x1,x2] = meshgrid(-siz(2):siz(2),-siz(1):siz(1));
h = exp(-(x1.^2 + x2.^2)/(2*sigma^2));
h(h<eps*max(h(:))) = 0;
sumh = sum(h(:));
if sumh ~= 0,
    h = h / sumh;
end;
template = zeros(boxSize);
for i = 1:rows-boxSize+1
    for j = 1:cols-boxSize+1
        % ȡģ��������
        template = imageWithNoise(i:i+(boxSize-1),j:j+(boxSize-1));
        % ��ģ����������Ȩ�ؾ����Ӧ���
        ws = sum(sum(template.*h));
        y(i+(boxSize-1)/2,j+(boxSize-1)/2) = ws;
    end
end

figure(3), imshow(y), title('�˲���ͼ');

% �����˲���MSE
se2 = (y - originImage).^2;
MSE2 = sum(se2(:)) / (rows * cols);
fprintf('�˲���MSE=%f\n', MSE2);