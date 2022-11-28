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

% kNN�����˲�
% ָ��k��ģ��ߴ�
k=3;boxSize=3;
for i = 1:rows-boxSize+1
    for j = 1:cols-boxSize+1
        % ȡģ����Ԫ��
        template = imageWithNoise(i:i+(boxSize-1),j:j+(boxSize-1));
        % ��ģ����Ԫ�ذ��������ĵ�����ֵ�Ĳ�ľ���ֵ����������
        center = template((boxSize+1)/2,(boxSize+1)/2);
        dist = reshape(abs(template-center), 1,boxSize*boxSize);
        [sortDist, sortIndex] = sort(dist);
        % ��������ĵ㱾���ǰk������ֵ�ľ�ֵ
        kmean = mean(template(sortIndex(2:k+1)));
        % �þ�ֵ�滻���ĵ�����ֵ
        y(i+(boxSize-1)/2,j+(boxSize-1)/2) = kmean;
    end
end

figure(3), imshow(y), title('�˲���ͼ');

% �����˲���MSE
se2 = (y - originImage).^2;
MSE2 = sum(se2(:)) / (rows * cols);
fprintf('�˲���MSE=%f\n', MSE2);