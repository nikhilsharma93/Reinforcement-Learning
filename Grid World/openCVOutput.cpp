/*
  openCVOutput.cpp: Implementation of openCVOutput.h
  @author Nikhil Sharma
*/

#include <iostream>
using namespace std;
//#include<opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

#include <map>
//#include "openCVOutput.h"
#include "states.h"

extern map<string, state*> stateCoordsAccessible;
extern map<string, state*> stateCoordsBlocked;

void colorBox(Mat &image, int imageHeight, int xCord, int yCord, int widthLines, int heightBox, int widthBox, Vec3b color);


void showOutput(map<state*, string> optimalPolicy, int nH, int nW){

  //---------------------------------------------------------------------------
  //Create the Base Image
  //---------------------------------------------------------------------------
  int widthLines = 5;
  int widthBox = 200;
  int heightBox = widthBox;
  int imageHeight = nH*heightBox + (nH+1)*widthLines;
  int imageWidth = nW*widthBox + (nW+1)*widthLines;
  Mat outputImage(imageHeight, imageWidth, CV_8UC3, Scalar(255, 255, 255));


  //Black color vector 3d
  Vec3b black;
  black[0] = 0;
  black[1] = 0;
  black[2] = 0;


  //Green color vector 3d
  Vec3b green;
  green[0] = 0;
  green[1] = 255;
  green[2] = 0;


  //Blue color vector 3d
  Vec3b blue;
  blue[0] = 255;
  blue[1] = 0;
  blue[2] = 0;


  //Light Blue color vector 3d
  Vec3b red;
  red[0] = 0;
  red[1] = 0;
  red[2] = 255;



  for (int heightLoop = 0; heightLoop < imageHeight; heightLoop = heightLoop + heightBox + widthLines){
    for (int heightLocal = heightLoop; heightLocal < heightLoop + widthLines; heightLocal++){
      for (int widthLocal = 0; widthLocal < imageWidth; widthLocal++){
        outputImage.at<Vec3b>(heightLocal, widthLocal) = black;
      }
    }
  }


  for (int widthLoop = 0; widthLoop < imageWidth; widthLoop = widthLoop + widthBox + widthLines){
    for (int widthLocal = widthLoop; widthLocal < widthLoop + widthLines; widthLocal++){
      for (int heightLocal = 0; heightLocal < imageHeight; heightLocal++){
        outputImage.at<Vec3b>(heightLocal, widthLocal) = black;
      }
    }
  }

  //---------------------------------------------------------------------------
  //Created the Base Image
  //---------------------------------------------------------------------------



  map<state*, string>::iterator iterPolicy;
  for (iterPolicy = optimalPolicy.begin(); iterPolicy != optimalPolicy.end(); iterPolicy++){
      int xCord = (iterPolicy->first)->getXCord();
      int yCord = (iterPolicy->first)->getYCord();
      string text = iterPolicy->second;
      int xLoc = widthBox*xCord + widthLines*(xCord+1) + widthBox/3;
      int yLoc = imageHeight - (heightBox*yCord + widthLines*(yCord+1) + heightBox*2/5);
      putText(outputImage, text, cvPoint(xLoc, yLoc), 2, 1, Scalar(0,0,0));
  }

  map<string, state*>::iterator iterBlocked;
  for (iterBlocked = stateCoordsBlocked.begin(); iterBlocked != stateCoordsBlocked.end(); iterBlocked++){
    int xCord = (iterBlocked->second)->getXCord();
    int yCord = (iterBlocked->second)->getYCord();
    colorBox(outputImage, imageHeight, xCord, yCord, widthLines, heightBox, widthBox, black);
  }

  map<string, state*>::iterator iterTerminal;
  for (iterTerminal = stateCoordsAccessible.begin(); iterTerminal != stateCoordsAccessible.end(); iterTerminal++){
    if ((iterTerminal->second)->getStateCat() == 3 && (iterTerminal->second)->getReward() > 0){
      int xCord = (iterTerminal->second)->getXCord();
      int yCord = (iterTerminal->second)->getYCord();
      colorBox(outputImage, imageHeight, xCord, yCord, widthLines, heightBox, widthBox, blue);
    }
    else if ((iterTerminal->second)->getStateCat() == 3 && (iterTerminal->second)->getReward() < 0){
      int xCord = (iterTerminal->second)->getXCord();
      int yCord = (iterTerminal->second)->getYCord();
      colorBox(outputImage, imageHeight, xCord, yCord, widthLines, heightBox, widthBox, red);
    }
  }

  namedWindow("Optimal Policies", WINDOW_NORMAL);
  imshow("Optimal Policies",outputImage);
  waitKey(0);

};
