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

% ����Ӧ��ֵ�˲���ʵ��
wmin = 3;   % ��С���ڳߴ�
wmax = 9;   % ��󴰿ڳߴ�
% ����ͼ�񣬴���ͼ��߽��
expandImage = [zeros((wmax-1)/2,cols+(wmax-1)); zeros(rows,(wmax-1)/2),imageWithNoise,zeros(rows,(wmax-1)/2);zeros((wmax-1)/2,cols+(wmax-1))];
% ����Ӧ�˲�
for i = 1:rows
    for j = 1:cols
        for n = wmin:2:wmax
            % ��ȡδ��չͼ����ĳ��(i,j)������,(x,y)��Ӧ��չ��ͼ��(i+(wmax-1)/2,j+(wmax-1)/2)
            S = expandImage(i+(wmax-1)/2-(n-1)/2:i+(wmax-1)/2+(n-1)/2,j+(wmax-1)/2-(n-1)/2:j+(wmax-1)/2+(n-1)/2);
            Pmax = max(S(:));   % �������������ֵ
            Pmin = min(S(:));   % ������������Сֵ
            Pmed = median(S(:));% �����������м�ֵ
            if Pmed>Pmin && Pmed < Pmax
                % �ж���ֵ�Ƿ�Ϊ���,�������󴰿ڳߴ����ж�
                if y(i,j)<=Pmin || y(i,j)>=Pmax
                    % �ж�ԭͼ��ĸ����ص��Ƿ�Ϊ���,��������㲻������,����������ֵ�滻
                    y(i,j) = Pmed;
                end
                break
            end
        end
        y(i,j)=Pmed; % ģ�����ʱ��ֵ�Բ����㣬ʹ����ֵ�滻
    end
end

figure(3), imshow(y), title('�˲���ͼ');

% �����˲���MSE
se2 = (y - originImage).^2;
MSE2 = sum(se2(:)) / (rows * cols);
fprintf('�˲���MSE=%f\n', MSE2);