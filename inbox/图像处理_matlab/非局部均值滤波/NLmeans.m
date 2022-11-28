function DenoisedImg=NLmeans(I,ds,Ds,h)
%I:������ͼ��
%ds:���򴰿ڰ뾶
%Ds:�������ڰ뾶
%h:��˹����ƽ������
%DenoisedImg��ȥ��ͼ��
[m,n]=size(I);
DenoisedImg=zeros(m,n);
% ��չͼ��߽�
PaddedImg = padarray(I,[ds,ds],'symmetric','both');
% ����dֵ
kernel=ones(2*ds+1,2*ds+1);
kernel=kernel./((2*ds+1)*(2*ds+1));
% ������������
h2=h*h;
for i=1:m
    for j=1:n
        % ԭͼ���Ӧ��չͼ���ƫ����
        i1=i+ds;
        j1=j+ds;
        % ����չͼ������(i1,j1)Ϊ���ĵ����򴰿�1
        W1=PaddedImg(i1-ds:i1+ds,j1-ds:j1+ds);
        average=0; % ��Ȩ��
        sweight=0; % ��һ��ϵ��
        % ��������
        rmin = max(i1-Ds,ds+1); % ���������ϱ߽�������Ƶ�ԭͼ���ϱ߽�
        rmax = min(i1+Ds,m+ds); % ���������±߽�������Ƶ�ԭͼ���±߽�
        smin = max(j1-Ds,ds+1); % ����������߽�������Ƶ�ԭͼ����߽�
        smax = min(j1+Ds,n+ds); % ���������ұ߽�������Ƶ�ԭͼ���ұ߽�
        % r��sΪ�������������ص������,�����������ڵ�ÿ�����ص������ƶ�
        for r=rmin:rmax
            for s=smin:smax
                % �������Լ��Ƚ����ƶ�
                if(r==i1&&s==j1) 
                    continue;
                end
                % �����������ڵ����ص�Ϊ���ĵ����򴰿�2
                W2=PaddedImg(r-ds:r+ds,s-ds:s+ds);
                % ������������
                Dist2=sum(sum(kernel.*(W1-W2).*(W1-W2)));
                % ����Ȩֵw(x,y)
                w=exp(-Dist2/h2);
                sweight=sweight+w;
                average=average+w*PaddedImg(r,s);
            end
        end
        % ����Ȩ�͹�һ�����滻ԭ���ص�
        DenoisedImg(i,j)=average/sweight;
    end
end