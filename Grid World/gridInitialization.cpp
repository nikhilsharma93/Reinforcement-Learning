/*
  gridInitialization.cpp: Implementation of gridInitialization.h
  @author Nikhil Sharma
*/

#include <iostream>
using namespace std;
#include <opencv2/opencv.hpp>
#include "states.h"
using namespace cv;
#include <math.h>
#include <string>
#include "zenityInterface.h"

extern map<string, state*> stateCoordsAccessible;
extern map<string, state*> stateCoordsBlocked;


void onMouse(int evt, int x, int y, int flags, void* param) {
  if(evt == CV_EVENT_LBUTTONDOWN) {
     Point* ptPtr = (Point*)param;
     ptPtr->x = x;
     ptPtr->y = y;
  }
}

void colorBox(Mat &image, int imageHeight, int xCord, int yCord, int widthLines, int heightBox, int widthBox, Vec3b color){
  int startX = widthBox*xCord + widthLines*(xCord+1);
  int startY = imageHeight - (heightBox*yCord + widthLines*(yCord+1));
  for (int loopX = startX; loopX < startX + widthBox; loopX++){
    for (int loopY = startY; loopY > startY - heightBox; loopY--){
      image.at<Vec3b>(loopY, loopX) = color;
    }
  }
}



void initializeGrid(int nH, int nW, float rewardNonTerminal, float rewardPosTerminal, float rewardNegTerminal, float initialUtilityNotTerminal, string blockedPresent){
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


  Point2i pt(-1,-1);//assume initial point
  namedWindow("Initialize The Grid",  WINDOW_NORMAL);


  setMouseCallback("Initialize The Grid", onMouse, (void*)&pt);
  int X, Y;

  if (blockedPresent.compare("yes\n") == 0){
    GetStdoutFromCommand("zenity  --info   --text \"Select the blocked points on the grid. Left-click anywhere in the box you want to choose and then press Enter. Press the escape key once you have selected all the blocked regions.\"");
    //For Blocked States
    while(1){
      imshow("Initialize The Grid", outputImage);

      char key = waitKey(0);
      if (key == 27){break;}
      else{
        X=pt.x;
        Y=pt.y;
        int xCord = floor((X - widthLines)/(widthBox + widthLines));
        int yCord = floor((imageHeight - Y - widthLines)/(heightBox + widthLines));
        new state(xCord, yCord, 0, 2, 0);
        colorBox(outputImage, imageHeight, xCord, yCord, widthLines, heightBox, widthBox, black);
      }
    }
  }

  GetStdoutFromCommand("zenity  --info   --text \"Select the Positive Terminal points\"");
  //For Positive States
  while(1){
    imshow("Initialize The Grid", outputImage);

    char key = waitKey(0);
    if (key == 27){break;}
    else{
      X=pt.x;
      Y=pt.y;
      int xCord = floor((X - widthLines)/(widthBox + widthLines));
      int yCord = floor((imageHeight - Y - widthLines)/(heightBox + widthLines));
      new state(xCord, yCord, rewardPosTerminal, 3, rewardPosTerminal);
      colorBox(outputImage, imageHeight, xCord, yCord, widthLines, heightBox, widthBox, blue);
    }
  }

  GetStdoutFromCommand("zenity  --info   --text \"Select the Negative Terminal points\"");
  //For Negative States
  while(1){
    imshow("Initialize The Grid", outputImage);

    char key = waitKey(0);
    if (key == 27){break;}
    else{
      X=pt.x;
      Y=pt.y;
      int xCord = floor((X - widthLines)/(widthBox + widthLines));
      int yCord = floor((imageHeight - Y - widthLines)/(heightBox + widthLines));
      new state(xCord, yCord, rewardNegTerminal, 3, rewardNegTerminal);
      colorBox(outputImage, imageHeight, xCord, yCord, widthLines, heightBox, widthBox, red);
    }
  }

  GetStdoutFromCommand("zenity  --info   --text \"All the remaining points will be selected as regular grid poins\"");


  //For Normal States
  for (int normalX = 0; normalX < nW; normalX++){
    for (int normalY = 0; normalY < nH; normalY++){
      string cordString = IntToString(normalX) + "," + IntToString(normalY);
      map<string, state*>::iterator iterBlocked;
      map<string, state*>::iterator iterTerminal;
      iterBlocked = stateCoordsBlocked.find(cordString);
      iterTerminal = stateCoordsAccessible.find(cordString);
      if (iterBlocked == stateCoordsBlocked.end() && iterTerminal == stateCoordsAccessible.end()){
        colorBox(outputImage, imageHeight, normalX, normalY, widthLines, heightBox, widthBox, green);
        new state(normalX, normalY, rewardNonTerminal, 1, initialUtilityNotTerminal);
      }

    }
  }

  imshow("Initialize The Grid", outputImage);
  cout << "\n\n** Press the escape key on the grid window to continue\n\n";
  waitKey(0);
}
