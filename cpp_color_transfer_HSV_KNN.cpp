#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include<tuple>
#include <cstdlib>
#include <string>
#include <cmath>
#include <unistd.h>

using namespace cv;
using namespace std;


struct MouseParams{
    Mat img;
    int i1;
    int j1;
    int i2;
    int j2;
    int flag = 0;
};

struct hLine{
    float a;
    float b;
    float c;
};

struct MouseParams2{
    Mat img;
    hLine p1;
    hLine l1;
};


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    MouseParams *vars = (MouseParams *) (userdata);
    Mat timg = vars->img.clone();

    if(event == EVENT_LBUTTONDOWN)
    {
        if(vars->flag==0){

            for(int i = max(0, x-2); i < min(timg.cols, 2+x); i++){
                for(int j = max(0, y-2); j < min(timg.rows, 2+y); j++){
                    timg.at<uchar>(j, i) = 255;
                }
            }
            vars->i1 = x;
            vars->j1 = y;
            vars->flag = 1;
        }
        
        else if(vars->flag==1){
            vars->i2 = x;
            vars->j2 = y;
            vars->flag = 2;

            float iT = (float)vars->i1;
            float jT = (float)vars->j1;

            line(timg, Point( iT, jT ), Point( iT, y), Scalar( 255, 255, 255 ),  2, 8 );
            line(timg, Point( iT, y ), Point( x, y), Scalar( 255, 255, 255 ),  2, 8 );
            line(timg, Point( x, y ), Point( x, jT), Scalar( 255, 255, 255 ),  2, 8 );
            line(timg, Point( x, jT ), Point( iT, jT), Scalar( 255, 255, 255 ),  2, 8 );

            vars->img = timg;
        }

    }

    if(event == EVENT_MOUSEMOVE){
        if(vars->flag==1){

            float iT = (float)vars->i1;
            float jT = (float)vars->j1;

            line(timg, Point( iT, jT ), Point( iT, y), Scalar( 255, 255, 255 ),  2, 8 );
            line(timg, Point( iT, y ), Point( x, y), Scalar( 255, 255, 255 ),  2, 8 );
            line(timg, Point( x, y ), Point( x, jT), Scalar( 255, 255, 255 ),  2, 8 );
            line(timg, Point( x, jT ), Point( iT, jT), Scalar( 255, 255, 255 ),  2, 8 );
        }
    }

    imshow("Select Area on Image", timg);
}


tuple<int, int, int> findDominantColor(Mat timg, int show){

    Mat imgO = timg.clone();
    Mat img;
    cvtColor( imgO, img, COLOR_BGR2HSV);

    Mat greyMat;
    cvtColor(img, greyMat, CV_BGR2GRAY);

    greyMat = greyMat / 15;
    greyMat = greyMat * 15;

    vector<int> h(255, 0);

    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            h[(int)greyMat.at<uchar>(i, j)]++;
        }                
    }

    int dominantClr = distance(h.begin(), max_element(h.begin(), h.end()));

    float dominantHue = 0;
    float dominantSat = 0;
    float dominantVal = 0;
    float total = 0;
    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            if((int)greyMat.at<uchar>(i, j) == dominantClr){
                dominantHue += ((int)img.at<cv::Vec3b>(i, j)[0] / 100.0);
                dominantSat += ((int)img.at<cv::Vec3b>(i, j)[1] / 100.0);
                dominantVal += ((int)img.at<cv::Vec3b>(i, j)[2] / 100.0);
                greyMat.at<uchar>(i, j) = 255;
                total++;
            }
            else{
                greyMat.at<uchar>(i, j) = 0;
            }
        }                
    }

    dominantHue = (dominantHue / total) * 100;
    dominantSat = (dominantSat / total) * 100;
    dominantVal = (dominantVal / total) * 100;

    if(show){

        Mat dConcat;
        cvtColor(greyMat, greyMat, CV_GRAY2BGR);
        hconcat(imgO, greyMat, dConcat);

        namedWindow("Dominant Color Regions", 1);
        imshow("Dominant Color Regions", dConcat);
        waitKey(0);

    }

    return make_tuple(dominantHue, dominantSat, dominantVal);
}


tuple<int, int, int, int> findDominantColorRegion(Mat timg, int show, int i1, int j1, int i2, int j2){

    Mat imgO = timg.clone();
    Mat img;
    cvtColor( imgO, img, COLOR_BGR2HSV);

    Mat greyMat;
    cvtColor(img, greyMat, CV_BGR2GRAY);

    blur(greyMat, greyMat, Size(5, 5));

    greyMat = greyMat / 15;
    greyMat = greyMat * 15;

    vector<int> h1(255, 0);

    for(int i = i1; i < i2; i++){
        for(int j = j1; j < j2; j++){
            h1[(int)greyMat.at<uchar>(i, j)]++;
        }                
    }

    int dominantClr = distance(h1.begin(), max_element(h1.begin(), h1.end()));

    float dominantHue = 0;
    float dominantSat = 0;
    float dominantVal = 0;
    float total = 0;

    for(int i = i1; i < i2; i++){
        for(int j = j1; j < j2; j++){
            if((int)greyMat.at<uchar>(i, j) == dominantClr){
                dominantHue += ((int)img.at<cv::Vec3b>(i, j)[0] / 100.0);
                dominantSat += ((int)img.at<cv::Vec3b>(i, j)[1] / 100.0);
                dominantVal += ((int)img.at<cv::Vec3b>(i, j)[2] / 100.0);
                greyMat.at<uchar>(i, j) = 255;
                total++;
            }
            else{
                greyMat.at<uchar>(i, j) = 0;
            }
        }                
    }

    dominantHue = (dominantHue / total) * 100;
    dominantSat = (dominantSat / total) * 100;
    dominantVal = (dominantVal / total) * 100;

    if(show){

        Mat dConcat;
        cvtColor(greyMat, greyMat, CV_GRAY2BGR);
        hconcat(imgO, greyMat, dConcat);

        namedWindow("Dominant Color Regions", 1);
        imshow("Dominant Color Regions", dConcat);
        waitKey(0);

    }

    return make_tuple(dominantHue, dominantSat, dominantVal, dominantClr);
}


void transferColor(Mat oimg, Mat timg){

    Mat img;
    cvtColor( timg, img, COLOR_BGR2HSV);

    int dHue, dSat, dVal;
    tie(dHue, dSat, dVal) = findDominantColor(oimg, 1);

    int iHue, iSat, iVal;
    tie(iHue, iSat, iVal) = findDominantColor(timg, 1);

    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            if((int)img.at<cv::Vec3b>(i, j)[0] < (iHue + 10) && (int)img.at<cv::Vec3b>(i, j)[0] > (iHue - 10)){
                img.at<cv::Vec3b>(i, j)[0] = dHue;
            }
        }
    }

    Mat res;
    cvtColor(img, res, CV_HSV2BGR);

    Mat dConcat;
    hconcat(timg, res, dConcat);

    namedWindow("Transfer Result", 1);
    imshow("Transfer Result", dConcat);
    waitKey(0);    
}


void transferColorRegion(Mat oimg, Mat timg){

    MouseParams mp;
    mp.img = oimg.clone();
    cout << "Click two points on source image to mark rectangle..." << endl;
    cin.get();

    namedWindow("Select Area on Image", 1);
    imshow("Select Area on Image", mp.img);
    setMouseCallback("Select Area on Image", CallBackFunc, (void*)&mp);
    imshow("Select Area on Image", mp.img);
    waitKey(0);

    if(mp.i1 > mp.i2 && mp.j1 > mp.j2){
        int temp = mp.i1;
        mp.i1 = mp.i2;
        mp.i2 = temp;

        temp = mp.j1;
        mp.j1 = mp.j2;
        mp.j2 = temp;
    }
    else if(mp.i1 > mp.i2 && mp.j1 < mp.j2){
        int temp = mp.i1;
        mp.i1 = mp.i2;
        mp.i2 = temp;
    }
    else if(mp.i1 < mp.i2 && mp.j1 > mp.j2){
        int temp = mp.j1;
        mp.j1 = mp.j2;
        mp.j2 = temp;
    }

    MouseParams mp2;
    mp2.img = timg.clone();
    cout << "Click two points on target image to mark region..." << endl;

    namedWindow("Select Area on Image", 1);
    imshow("Select Area on Image", mp2.img);
    setMouseCallback("Select Area on Image", CallBackFunc, (void*)&mp2);
    imshow("Select Area on Image", mp2.img);
    waitKey(0);

    if(mp2.i1 > mp2.i2 && mp2.j1 > mp2.j2){
        int temp = mp2.i1;
        mp2.i1 = mp2.i2;
        mp2.i2 = temp;

        temp = mp2.j1;
        mp2.j1 = mp2.j2;
        mp2.j2 = temp;
    }
    else if(mp2.i1 > mp2.i2 && mp2.j1 < mp2.j2){
        int temp = mp2.i1;
        mp2.i1 = mp2.i2;
        mp2.i2 = temp;
    }
    else if(mp2.i1 < mp2.i2 && mp2.j1 > mp2.j2){
        int temp = mp2.j1;
        mp2.j1 = mp2.j2;
        mp2.j2 = temp;
    }


    Mat img;
    cvtColor( timg, img, COLOR_BGR2HSV);

    Mat greyMat;
    cvtColor(img, greyMat, CV_BGR2GRAY);

    int dHue, dSat, dVal, dClr;
    tie(dHue, dSat, dVal, dClr) = findDominantColorRegion(oimg, 1, mp.j1, mp.i1, mp.j2, mp.i2);

    int iHue, iSat, iVal, iClr;
    tie(iHue, iSat, iVal, iClr) = findDominantColorRegion(timg, 1, mp2.j1, mp2.i1, mp2.j2, mp2.i2);

    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            if(img.at<cv::Vec3b>(i, j)[0] < (iHue + 10) && img.at<cv::Vec3b>(i, j)[0] > (iHue - 10) && img.at<cv::Vec3b>(i, j)[1] < (iSat + 30) && img.at<cv::Vec3b>(i, j)[1] > (iSat - 30)){
                img.at<cv::Vec3b>(i, j)[0] = dHue;
                img.at<cv::Vec3b>(i, j)[1] = (dSat + img.at<cv::Vec3b>(i, j)[1])/2;
            }
        }
    }

    Mat res;
    cvtColor(img, res, CV_HSV2BGR);

    Mat dConcat;
    hconcat(timg, res, dConcat);

    namedWindow("Transfer Result", 1);
    imshow("Transfer Result", dConcat);
    waitKey(0);    
}



int main(int argc, char** argv){

    String dir = "images/";

    String imageName(dir + "sunflower.jpg");
    if( argc > 1)
    {
        imageName = dir + argv[1];
    }

    // Read image from file     
    Mat imageMain = imread(imageName);
    //if fail to read the image
    if ( imageMain.empty() ) 
    { 
        cout << "Error loading the image" << endl;
        return -1; 
    }

    String imageName2(dir + "rose.jpeg");
    if( argc > 2)
    {
        imageName2 = dir + argv[2];
    }

    // Read image from file     
    Mat imageMain2 = imread(imageName2);
    //if fail to read the image
    if ( imageMain2.empty() ) 
    { 
        cout << "Error loading the image" << endl;
        return -1; 
    }


    while(1){

        system("clear");

        int input = 0;
        cout << "Choose action: (enter number) " << endl;
        cout << "\t1. Direct Color Transfer." << endl;
        cout << "\t2. Selected Window Color Transfer." << endl;

        cin >> input;

        if(input == 1){

            Mat img1 = imageMain.clone();
            Mat img2 = imageMain2.clone();

            transferColor(img1, img2);

        }

        if(input == 2){

            Mat img1 = imageMain.clone();
            Mat img2 = imageMain2.clone();

            transferColorRegion(img1, img2);            

        }


    }


    return 0;
}
