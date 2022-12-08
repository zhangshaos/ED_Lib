#include "EDLib.h"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{	
	//***************************** ED Edge Segment Detection *****************************
	//Detection of edge segments from an input image	

	int delay = 10;
	CommandLineParser parser(argc, argv, "{ @input | | }");
	string input = parser.get<string>("@input");
	Mat testImg = imread(input, IMREAD_GRAYSCALE);
  if (testImg.empty()) {
    std::cout
    << "Can not read the source image normally!"
    << std::endl;
    exit(EXIT_FAILURE);
  }
	imshow("Source Image", testImg);

	//Call ED constructor
	ED testED = ED(testImg, SOBEL_OPERATOR, 36, 8, 1, 10, 1.0, true); // apply ED algorithm
	
	//Show resulting edge image
	Mat edgeImg = testED.getEdgeImage();
	imshow("ED: Edge Image", edgeImg);
	waitKey(delay);
		
	//Output number of segments
	int noSegments = testED.getSegmentNo();
	std::cout << "ED: Number of edge segments: " << noSegments << std::endl;
		
	//Get edges in segment form (getSortedSegments() gives segments sorted w.r.t. legnths) 
	std::vector< std::vector<Point> > segments = testED.getSegments();
	
	
	//***************************** EDLINES Line Segment Detection *****************************
	//Detection of line segments from the same image
	EDLines testEDLines = EDLines(testImg);
	Mat lineImg = testEDLines.getLineImage();	//draws on an empty image
	imshow("EDLines: Line Image 1", lineImg);

	//Detection of lines segments from edge segments instead of input image
	//Therefore, redundant detection of edge segmens can be avoided
	testEDLines = EDLines(testED);
	lineImg = testEDLines.drawOnImage();	//draws on the input image
	imshow("EDLines: Line Image 2", lineImg);

	//Acquiring line information, i.e. start & end points
	vector<LS> lines = testEDLines.getLines();
	int noLines = testEDLines.getLinesNo();
	std::cout << "EDLines: Number of line segments: " << noLines << std::endl;

	waitKey(delay);

	//************************** EDPF Parameter-free Edge Segment Detection **************************
	// Detection of edge segments with parameter free ED (EDPF)

	EDPF testEDPF = EDPF(testImg);
	Mat edgePFImage = testEDPF.getEdgeImage();
	imshow("EDPF: Edge Image", edgePFImage);
	std::cout << "EDPF: Number of edge segments: " << testEDPF.getSegmentNo() << std::endl;
	waitKey(delay);

	//***************************** EDCIRCLES Circle Segment Detection *****************************
	//Detection of circles directly from the input image

	EDCircles testEDCircles = EDCircles(testImg);
	Mat circleImg = testEDCircles.drawResult(false, ImageStyle::CIRCLES);
	imshow("EDCircles: Circle Image 1", circleImg);

	//Detection of circles from already available EDPF or ED image
	testEDCircles = EDCircles(testEDPF);
	
	//Get circle information as [cx, cy, r]
	vector<mCircle> circles = testEDCircles.getCircles();

	//Get ellipse information as [cx, cy, a, b, theta]
	vector<mEllipse> ellipses = testEDCircles.getEllipses();

	//Circles and ellipses will be indicated in green and red, resp.
	circleImg = testEDCircles.drawResult(true, ImageStyle::BOTH);
	imshow("EDCircles: Circle Image with Src", circleImg);

	int noCircles = testEDCircles.getCirclesNo();
	std::cout << "EDCircles: Number of circles: " << noCircles << std::endl;
	waitKey(delay);
	
	//*********************** EDCOLOR Edge Segment Detection from Color Images **********************
		
	Mat colorImg = imread(input, IMREAD_COLOR);
	EDColor testEDColor = EDColor(colorImg, 36, 4, 1.5, true); //last parameter for validation
	imshow("EDColor: Edge Image", testEDColor.getEdgeImage());
	std::cout << "EDColor: Number of edge segments: " << testEDColor.getSegmentNo() << std::endl;
	waitKey(delay);	
	
	// get lines from color image
	EDLines colorLine = EDLines(testEDColor);
	imshow("EDColor->EDLines", colorLine.getLineImage());
	std::cout << "EDCorlor->EDlines: Number of line segments: " << colorLine.getLinesNo() << std::endl;
	waitKey(delay);

	// get circles from color image
	EDCircles colorCircle = EDCircles(testEDColor);
	// TO DO :: drawResult doesnt overlay (onImage = true) when input is from EDColor
	circleImg = colorCircle.drawResult(false, ImageStyle::BOTH);
	imshow("EDColor->EDCircles", circleImg);
	std::cout << "EDColor->EDCircles: Number of line segments: " << colorCircle.getCirclesNo() << std::endl;
	waitKey();

	return 0;
}



