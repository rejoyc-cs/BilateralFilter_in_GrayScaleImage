#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cmath>
#include <iostream>

#define pi 3.1413

using namespace std;
using namespace cv;

double gaussianFunc1D(int x,double s){
    double dtemp = 1/(sqrt(2*pi)*s);
	double etemp = exp(-((x*x))/(2*(s*s)));
	return dtemp*etemp;
}

double gaussianFunc2D(int x,int y,double s){
    double dtemp = 1/(sqrt(2*pi)*s);
	double etemp = exp(-((x*x) + (y*y))/(2*(s*s)));
	return dtemp*etemp;
}

Mat bilateralFilter(Mat img,int ks, int s){
    Mat temp = img.clone();
    Mat result = img.clone();
    int shift = (ks - 1)/2;

    for (int i=0; i< temp.rows - ks +1 ;i++){
        for (int j=0; j< temp.cols - ks + 1; j++){

            double productSum = 0;
            double normSum = 0;

            for (int m=0;m<ks;m++){
                for (int n=0;n<ks;n++){

                    int ox = i + shift;
                    int oy = j + shift;

                    int vx = m+i;
                    int vy = n+j;

                    int dx = abs(vx - ox);
                    int dy = abs(vy - oy);
                    
                    double di = (double)temp.at<uchar>(ox,oy) - (double)temp.at<uchar>(vx,vy);

                    double prod = (double)temp.at<uchar>(vx,vy) * gaussianFunc1D(di,s) * gaussianFunc2D(dx,dy,s); 

                    double prodNorm = gaussianFunc1D(di,s) * gaussianFunc2D(dx,dy,s);
                    
                    productSum += prod;
                    normSum += prodNorm;
                }
            }

            result.at<uchar>(i+shift,j+shift) = productSum/normSum;
        }
    }
    
    return result;
}

int main(int argc, char *argv[])
{
    Mat img = imread(argv[1],-1);
    if(img.empty()) return -1;
    Mat imgG = imread(argv[1],-1);
    cvtColor(img, imgG, COLOR_BGR2GRAY);

    int height = imgG.rows;
    int width = imgG.cols;

    int ksize;
    cout << "Enter the kernel size: " <<endl;
    cin >> ksize;

    double sigma;
    cout << "Enter the sigma: " <<endl;
    cin >> sigma;

    sigma = sigma*100;

    int khalf = (ksize - 1)/2;
    
    Mat operImg = Mat::zeros(height+ksize-1,width+ksize-1, CV_8UC1);

    for (int i=0;i< height;i++){
        for (int j=0;j< width;j++){
            operImg.at<uchar>(i+1,j+1) = imgG.at<uchar>(i,j);
        }
    }

    Mat resultTemp = bilateralFilter(operImg,ksize, sigma);
    Mat result = Mat::zeros(height,width, CV_8UC1);;

    for (int i=0;i< height;i++){
        for (int j=0;j< width;j++){
            result.at<uchar>(i,j) = resultTemp.at<uchar>(i+khalf,j+khalf);
        }
    }
    
    namedWindow("Original GRAY", WINDOW_AUTOSIZE);
    imshow("Original GRAY",imgG);

    namedWindow("Bilateral Blurred Image", WINDOW_AUTOSIZE);
    imshow("Bilateral Blurred Image",result);

    waitKey(0);
    destroyAllWindows();
    
    return 0;
}
