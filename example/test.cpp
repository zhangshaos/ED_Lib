// Created by zxm on 2022/12/8.
//
// 经过测试，发现EDColor+EDLine的组合还不错。
//

#include "EDLib.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  int delay = 2000;
  cv::CommandLineParser parser(argc, argv, "{ @input | | }");
  string input = parser.get<string>("@input");
  cv::Mat testImg = cv::imread(input, cv::IMREAD_GRAYSCALE);
  if (testImg.empty()) {
    std::cout
      << "Source image is empty !"
      << std::endl;
    exit(EXIT_FAILURE);
  }
  cv::imshow("Source Image", testImg);

  //***************************** ED Edge Segment Detection *****************************
  //Detection of edge segments from an input image
  ED testED = ED(testImg, SOBEL_OPERATOR, 36, 8, 1, 10, 1.0, true); // apply ED algorithm
  cv::Mat edgeImg = testED.getEdgeImage();
  cv::imwrite("ED Edge Image.jpg", edgeImg);
  //
  int noSegments = testED.getSegmentNo();
  std::cout << "ED: Number of edge segments: " << noSegments << std::endl;
  //Get edges in segment form (getSortedSegments() gives segments sorted w.r.t. lengths)
  std::vector<std::vector<cv::Point> > lines = testED.getSegments();


  //************************** EDPF Parameter-free Edge Segment Detection **************************
  // Detection of edge segments with parameter free ED (EDPF)
  EDPF testEDPF = EDPF(testImg);
  cv::Mat edgePFImage = testEDPF.getEdgeImage();
  cv::imwrite("EDPF Edge Image.jpg", edgePFImage);
  std::cout << "EDPF: Number of edge segments: " << testEDPF.getSegmentNo() << std::endl;


  //*********************** EDCOLOR Edge Segment Detection from Color Images **********************
  cv::Mat colorImg = cv::imread(input, cv::IMREAD_COLOR);
  EDColor testEDColor = EDColor(colorImg, 20, 4, 1.2, true); //last parameter for validation
  cv::imwrite("EDColor Edge Image.jpg", testEDColor.getEdgeImage());
  std::cout << "EDColor: Number of edge segments: " << testEDColor.getSegmentNo() << std::endl;

//  segments = testEDColor.getSegments();
//  cv::Mat result(testEDColor.getHeight(), testEDColor.getWidth(), CV_8U, cv::Scalar_<uint8_t>(0));
//  for (const auto &seg : segments) {
//    for (const auto &pt : seg) {
//      result.at<uint8_t>(pt.y, pt.x) = 0xff;
//    }
//    cv::imshow("result", result);
//    cv::waitKey(2000);
    //经过测试，发现每一段的线段并不是一条直线，而且可能非常奇怪的曲线
//  }

  EDLines edLines(testEDColor);
  //cv::imwrite("EDLines Edge.jpg", edLines.getEdgeImage());
  auto edgeLineMap = edLines.getLineImage();
  cv::imwrite("EDLines Lines.jpg", edgeLineMap);
  //cv::resize(edgeLineMap, edgeLineMap, cv::Size_<int>(256, 192));
  //cv::imwrite("EDLines Lines 2.jpg", edgeLineMap);
  auto line_s = edLines.getLines();
  cv::Mat result(192, 256, CV_8U, cv::Scalar_<uint8_t>(0));
  const double scaleY = 192.0 / edgeLineMap.size[0],
               scaleX = 256.0 / edgeLineMap.size[1];
  size_t n = 0;
  bool progressDrawing = false;
  for (const auto &line : line_s) {
    cv::Point start, end;
    start.y = int((line.start.y + 0.5) * scaleY);
    start.x = int((line.start.x + 0.5) * scaleX);
    end.y = int((line.end.y + 0.5) * scaleY);
    end.x = int((line.end.x + 0.5) * scaleX);
    cv::line(result, start, end, cv::Scalar_<uint8_t>(255));
    if (progressDrawing) {
      cv::imshow("result", result);
      cv::waitKey(500);
    }
    ++n;
  }
  std::cout << "Detect " << n << " lines" << std::endl;
  cv::imwrite("EDLines Lines 2.jpg", result);

  cv::waitKey(delay);
  return 0;
}



