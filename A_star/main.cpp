#include <iostream>
#include <vector>
#include <set>

#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

using namespace std;
using namespace cv;

//=========================================================================================================================//

struct PointData
{
	int x, y, State;
};

//=========================================================================================================================//

double getColorDistance(Mat &Img, Point P1, Point P2)
{
	int R1, G1, B1, R2, G2, B2;
	double Dist = 0;

	R1 = (int)(Img).at<Vec3b>(P1.y, P1.x)[0];
	G1 = (int)(Img).at<Vec3b>(P1.y, P1.x)[1];
	B1 = (int)(Img).at<Vec3b>(P1.y, P1.x)[2];

	R2 = (int)(Img).at<Vec3b>(P2.y, P2.x)[0];
	G2 = (int)(Img).at<Vec3b>(P2.y, P2.x)[1];
	B2 = (int)(Img).at<Vec3b>(P2.y, P2.x)[2];

	Dist = sqrt((R1 - R2)*(R1 - R2) + (G1 - G2)*(G1 - G2) + (B1 - B2)*(B1 - B2));
	//cout << "Dist: " << Dist << endl;
	return Dist;
}

//=========================================================================================================================//

void findPath(Mat &Img, PointData &_P1, PointData &_P2)
{
	Point P1, P2, temp;
	vector<Point> Candidates(4); // 4
	vector<Point> Neighbors(9);
	vector<Point> visited;
	size_t counter = 0;
	P1.x = _P1.x; P1.y = _P1.y;
	P2.x = _P2.x; P2.y = _P2.y;

	//line(Img, Point(P1.x, P1.y), Point(P2.x, P2.y), Scalar(255, 255, 255));

	//getColor(Img, P1, R1, G1, B1);
	//getColor(Img, P2, R2, G2, B2);

	//for (size_t j = 0; j < 100; j++)
	while(P1 != P2)
	{
		counter++;
		if (counter > Img.cols * Img.rows) break;

		visited.push_back(P1);
		double minDist = numeric_limits<double>::max();

		if (P1.x - P2.x <= 0 && P1.y - P2.y <= 0)
		{
			Candidates[0] = Point(P1.x, P1.y + 1);
			Candidates[1] = Point(P1.x + 1, P1.y + 1);
			Candidates[2] = Point(P1.x + 1, P1.y);
			Candidates[3] = Point(P1.x + 1, P1.y - 1);
		}
		if (P1.x - P2.x <= 0 && P1.y - P2.y >= 0)
		{
			Candidates[0] = Point(P1.x + 1, P1.y);
			Candidates[1] = Point(P1.x + 1, P1.y - 1);
			Candidates[2] = Point(P1.x, P1.y - 1);
			Candidates[3] = Point(P1.x - 1, P1.y - 1);
		}
		if (P1.x - P2.x >= 0 && P1.y - P2.y >= 0)
		{
			Candidates[0] = Point(P1.x, P1.y - 1);
			Candidates[1] = Point(P1.x - 1, P1.y - 1);
			Candidates[2] = Point(P1.x - 1, P1.y);
			Candidates[3] = Point(P1.x - 1, P1.y + 1);
		}
		if (P1.x - P2.x >= 0 && P1.y - P2.y <= 0)
		{
			Candidates[0] = Point(P1.x - 1, P1.y);
			Candidates[1] = Point(P1.x - 1, P1.y + 1);
			Candidates[2] = Point(P1.x, P1.y + 1);
			Candidates[3] = Point(P1.x + 1, P1.y + 1);
		}

		//for (int i = -1; i < 2; i++)
		//{
		//	for (int j = -1; j < 2; j++)
		//	{
		//		Neighbors.push_back(Point(P1.x + i, P1.y + j));
		//		//cout << "{" << i << ", " << j << "} ";
		//	}
		//	//cout << endl;
		//}

		//cout << "//=================================================//" << endl;

		for (size_t i = 0; i < Candidates.size(); i++)
		{

		//	if (find(Neighbors.begin(), Neighbors.end(), P2) == Neighbors.end())
		//	{
		//		temp = *find(Neighbors.begin(), Neighbors.end(), P2);
		//		break;
		//	}

			//cout << "Candidat:" << i << " X: " << Candidates[i].x << " Y: " << Candidates[i].y << endl;
			if (find(visited.begin(), visited.end(), Candidates[i]) != visited.end()) continue;
			if (minDist > getColorDistance(Img, P1, Candidates[i]))
			{
				minDist = getColorDistance(Img, P1, Candidates[i]);
				temp = Candidates[i];
			}
		}

		//cout << "//=================================================//" << endl;

		line(Img, P1, temp, Scalar(10, 10, 255));
		//imshow("Process", tempImg);
		//waitKey(10);
		P1 = temp;
	}
}

//=========================================================================================================================//

void mouseEvent(int evt, int x, int y, int flags, void* param)
{
	PointData* P = (PointData*)param;
	if (evt == CV_EVENT_LBUTTONDOWN)
	{
		P->x = x;
		P->y = y;
		
		if (P->State == 0)
		{
			P->State = 1;
		}
		else if (P->State == 1)
		{
			P->State = 0;
		}
		else if (P->State == -1)
		{
			P->State = 0;
		}
	}
}

//=========================================================================================================================//

int main()
{
	Mat Image;
	PointData temp, P1, P2;
	temp.State = -1;

	P1.x = 0; P1.y = 0;
	P2.x = 0; P2.y = 0;

	//Image = imread("Test_001.bmp");
	Image = imread("Test_002.bmp");
	//Image = imread("test.jpg");
	namedWindow("Image");

	setMouseCallback("Image", mouseEvent, &temp);
	
	while (true)
	{
		if (temp.State == 0)
		{
			P1 = temp;
			circle(Image, Point(P1.x, P1.y), 2, Scalar(255, 10, 10), CV_FILLED);
		}
		if (temp.State == 1)
		{
			P2 = temp;
			circle(Image, Point(P2.x, P2.y), 2, Scalar(255, 10, 10), CV_FILLED);
			
			findPath(Image, P1, P2);
		}
		imshow("Image", Image);
		waitKey(1);
	}

	return 0;
}

//=========================================================================================================================//