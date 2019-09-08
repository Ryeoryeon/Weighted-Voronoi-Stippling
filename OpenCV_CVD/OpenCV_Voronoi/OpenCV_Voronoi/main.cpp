//
//  main.cpp
//  OpenCV_Voronoi
//
//  Created by 이영현 on 12/07/2019.
//  Copyright © 2019 이영현. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp> // Header Search Paths에 /usr/local/include/opencv4를 추가 해 주면 된다!!
//#include <opencv4/opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class voronoi{
    public:
    //클래스 안에서 Point2d 자료형을 사용하려 하니 자꾸 오류가 나서 좌표를 하나하나 따로 적었다.
    //중심 좌표를 점의 좌표로 생각해서 짰다.
        double cent_x;
        double cent_y;
    
        double c_grav_x;
        double c_grav_y;
    
        int color_B;
        int color_G;
        int color_R;
    
        int num_pixel;
        void Init();
};

int distance(int i, int j, int index, int s_index, voronoi cell[]);
void give_dots(Mat &image, voronoi []);


int main(int argc, const char * argv[]) {
    //100개의 점을 준다고 가정.
    
    Mat image=imread("ming.jpg", IMREAD_COLOR); //,0이라고 하면 흑백 이미지로, jpeg는 jpg로 열어야함.

    int height=image.rows;
    int width=image.cols;
    voronoi cell[100];
    give_dots(image,cell);
    
    
    //한 픽셀 당 모든 점들과의 최소 거리를 판단한 후에 가장 최소인 점의 색으로 색칠한다.
    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            int s_pt=0;
            for(int pt=0; pt<100; pt++){
                //100개의 점에 대해 최소 거리를 구한다.

                if(cell[pt].cent_x==x&&cell[pt].cent_y==y) // 픽셀의 위치 = 점의 위치일 때는 예외처리를 해 줘야 한다.
                    break;
                
                else
                    s_pt=distance(y,x,pt,s_pt,cell);
                
                //픽셀의 위치 = 점의 위치일 때 예외처리의 연장선. 여기에 넣지 않고 for문 밖에다 넣어주면 흰 점들도 색이 입혀진다.
                if(pt==99){
                    image.at<Vec3b>(y,x)[0]=cell[s_pt].color_B;
                    image.at<Vec3b>(y,x)[1]=cell[s_pt].color_G;
                    image.at<Vec3b>(y,x)[2]=cell[s_pt].color_R;
                    
                }
            }
        }
    }
    
    double subtr=0;
    
    cout<<"진짜 시작"<<'\n';

    
    do{
        
        //이미지의 각 픽셀 순회. 색상을 얻어온 뒤 그 색을 대표색상으로 하는 셀을 찾고 무게중심에 좌표 더하고 픽셀 개수 증가
        for(int y=0; y<height; y++){
            for(int x=0; x<width; x++){
                for(int pt=0; pt<100; pt++){
                    if(image.at<Vec3b>(y,x)[0]==cell[pt].color_B&&image.at<Vec3b>(y,x)[1]==cell[pt].color_G&&image.at<Vec3b>(y,x)[2]==cell[pt].color_R){
                        cell[pt].c_grav_x+=x;
                        cell[pt].c_grav_y+=y;
                        cell[pt].num_pixel++;
                
                    }
                    
                    else{
                        continue;
                    }
                }
            }
        }
        
        //100개의 보로노이 셀에 대해 실행 (무게중심 좌표 업데이트)
        for(int i=0; i<100; i++){

            cell[i].c_grav_x=cell[i].c_grav_x/cell[i].num_pixel;
            cell[i].c_grav_y=cell[i].c_grav_y/cell[i].num_pixel;
            
        }
        
        for(int i=0; i<100; i++){
            subtr+=abs(cell[i].c_grav_x-cell[i].cent_x);
            subtr+=abs(cell[i].c_grav_y-cell[i].cent_y);
            //
            //내가 더 추가해서 생각한 부분 (중심의 좌표를 그 무게중심의 좌표로 옮기는 게 맞나?)
            cell[i].cent_x=cell[i].c_grav_x;
            cell[i].cent_y=cell[i].c_grav_y;
            //
            cell[i].num_pixel=0;
            cell[i].c_grav_x=0;
            cell[i].c_grav_y=0;
        }
        
        //한 픽셀 당 모든 점들과의 최소 거리를 판단한 후에 가장 최소인 점의 색으로 색칠한다.
        for(int y=0; y<height; y++){
            for(int x=0; x<width; x++){
                int s_pt=0;
                for(int pt=0; pt<100; pt++){
                    //100개의 점에 대해 최소 거리를 구한다.
                    
                    // 픽셀의 위치 = 점의 위치일 때는 예외처리를 해 줘야 한다.
                    if(cell[pt].cent_x==x&&cell[pt].cent_y==y)
                        break;
                    
                    else
                        s_pt=distance(y,x,pt,s_pt,cell);
                    
                    //픽셀의 위치 = 점의 위치일 때 예외처리의 연장선. 여기에 넣지 않고 for문 밖에다 넣어주면 흰 점들도 색이 입혀진다.
                    if(pt==99){
                        image.at<Vec3b>(y,x)[0]=cell[s_pt].color_B;
                        image.at<Vec3b>(y,x)[1]=cell[s_pt].color_G;
                        image.at<Vec3b>(y,x)[2]=cell[s_pt].color_R;
                        
                    }
                }
            }
        }
        
        cout<<subtr<<'\n';
        
    }while(subtr<7000);
    
    for(int pt=0; pt<100; pt++){
        int x=cell[pt].cent_x;
        int y=cell[pt].cent_y;
        image.at<Vec3b>(y,x)[0]=0;
        image.at<Vec3b>(y,x)[1]=0;
        image.at<Vec3b>(y,x)[2]=0;
    }
    
    imshow("voronoi", image);
    waitKey(0);
    
    
    return 0;
}

void give_dots(Mat &image, voronoi cell[]){
    for(int j=0; j<100; j++){
        cell[j].Init();
        int x=rand()%image.cols;
        cell[j].cent_x=x;
        int y=rand()%image.rows;
        cell[j].cent_y=y;
        cell[j].color_B=rand()%255+1; //B
        cell[j].color_G=rand()%255+1; //G
        cell[j].color_R=rand()%255+1; //R
        
        //만일 흑백 이미지라면
        if(image.channels()==1){
            image.at<uchar>(y,x)=0;
        }
        
        //칼라 이미지의 경우
        else if(image.channels()==3){
            //주의! 특정 화소에 접근할 때 (row,col)의 순서로 접근 해 줘야 한다.
            image.at<Vec3b>(y,x)[0]=0;
            image.at<Vec3b>(y,x)[1]=0;
            image.at<Vec3b>(y,x)[2]=0;
        }
    }
}

//픽셀들과 점들끼리의 거리를 구하고 더 가까운 점을 반환하는 Distance 함수
//(이 함수를 반복문 안에 넣으면 최소거리를 가지는 점을 구할 수 있게 도와줄 수 있도록)

int distance(int i, int j, int index, int s_index, voronoi cell[]){
    double dis;
    double s_dis;
    
    double dx=j-cell[index].cent_x;
    double dy=i-cell[index].cent_y;
    dis=sqrt(pow(dx,2)+pow(dy,2));
    
    double dx_s=j-cell[s_index].cent_x;
    double dy_s=i-cell[s_index].cent_y;
    s_dis=sqrt(pow(dx_s,2)+pow(dy_s,2));
    //s_dis=abs(dx_s)+abs(dy_s);
    
    if(s_dis<dis){
        return s_index;
    }
    
    else{
        return index;
    }
    
}

void voronoi::Init(){
    c_grav_x=0;
    c_grav_y=0;
    num_pixel=0;
}

