img=imread("00 org.png");

% 结构元素
R=20; % 元素半径
B = zeros(R,R);
for k = -R:1:R
    for l = -R:1:R
        if(k^2+l^2<=R^2)
            B(l+R+1,k+R+1) = 1;
        end
    end
end

%% 闭操作
im_P = imdilate(img,B);% 膨胀
im_F = imerode(im_P,B);% 腐蚀
subplot(121);
imshow(im_F)

%% 开操作
im_F = imerode(im_F,B);% 腐蚀
im_P = imdilate(im_F,B);% 膨胀
subplot(122);
imshow(im_P)