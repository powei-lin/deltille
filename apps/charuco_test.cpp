#include <vector>
#include <iostream>
#include <bitset>

#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

// using namespace std;
using namespace cv;

Mat drawTriangles(int patch_size, int flag = 0){

  auto bi = std::bitset<4>(flag);

  cv::Mat img = cv::Mat::zeros(patch_size, patch_size, CV_8U);
  const double sideLength = patch_size/2.0;
  std::vector<std::pair<double, double>> t_sets;
  t_sets.emplace_back(-sideLength/2.0, -sideLength/4.0);
  t_sets.emplace_back(-sideLength/2.0, sideLength*3.0/4.0);
  t_sets.emplace_back(sideLength/2.0, sideLength/4.0);

  std::vector<std::pair<double, double>> offsets;
  offsets.emplace_back(0,sideLength);
  offsets.emplace_back(sideLength,0.5*sideLength);
  offsets.emplace_back(sideLength,1.5*sideLength);
  offsets.emplace_back(2.0*sideLength,sideLength);
  
  if(!bi[0]){
    offsets.emplace_back(0,0);
    offsets.emplace_back(sideLength,-0.5*sideLength);
  }
  if(!bi[2]){
    offsets.emplace_back(2.0*sideLength,0);
  }
  if(!bi[3])
    offsets.emplace_back(2.0*sideLength,2.0*sideLength);

  std::vector<std::vector<Point2i>> tris;

  for(const auto &os:offsets) {
    std::vector<Point2i> t;
    for(const auto &ts:t_sets){
      t.emplace_back(ts.first+os.first, ts.second+os.second);
    }
    tris.push_back(t);
  }
  for(const auto &t:tris){
    cv::fillConvexPoly(img, t, Scalar(255), LINE_AA);
  }
  // std::vector<Point2f> corners;
  // corners.emplace_back(0, 0);
  // corners.emplace_back(0, patch_size);
  // corners.emplace_back(patch_size, 0);
  // corners.emplace_back(patch_size, patch_size);
  // // std::vector<std::pair<Point2f, Point2f>> lines;
  // // lines.emplace_back(Point2f(0,0), Point2f(0, patch_size));
  // // lines.emplace_back(Point2f(0,0), Point2f(patch_size, 0));
  // // lines.emplace_back(Point2f(patch_size,0), Point2f(patch_size, patch_size));
  // // lines.emplace_back(Point2f(0,patch_size), Point2f(patch_size, patch_size));
  // for(int i = 0 ; i < 4; i++){
  //   if(bi[i]){
  //     circle(img, corners[i], patch_size/6.0, Scalar(0), -1, LINE_AA);
  //     // line(img, lines[i].first, lines[i].second, Scalar(0), patch_size/6.0, LINE_AA);
  //   }
  // }

  return img;
}

int main() {
  cv::Ptr<cv::aruco::Dictionary> dictionary =
      cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_250);
  cv::Ptr<cv::aruco::CharucoBoard> board =
      cv::aruco::CharucoBoard::create(9, 9, 0.04f, 0.02f, dictionary);
  cv::Ptr<cv::aruco::DetectorParameters> params = cv::aruco::DetectorParameters::create();

  cv::Mat boardImage;
  board->draw(cv::Size(2880, 2880), boardImage, 0, 1);
  // cv::imwrite("BoardImage.png", boardImage);

  // cv::Mat image = cv::imread("2.png", 0);
  // std::vector<int> markerIds;
  // std::vector<std::vector<cv::Point2f>> markerCorners;
  // cv::aruco::detectMarkers(image, board->dictionary, markerCorners, markerIds,
  //                          params);
  // std::vector<cv::Point2f> charucoCorners;
  // std::vector<int> charucoIds;
  // cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, image, board,
  //                                      charucoCorners, charucoIds);
  // cv::Mat colored;
  // cv::cvtColor(image, colored, cv::COLOR_GRAY2BGR);
  // for(const auto &pt:charucoCorners){
  //   cv::circle(colored, pt, 5, cv::Scalar(0,0,255), -1);
  // }
  // cv::imshow("c", colored);
  // cv::waitKey(0);

  // drawTriangles(tri);
  // cv::resize(tri, tri, Size(), 0.25, 0.25);
  cv::Mat patch = drawTriangles(1000);

  constexpr int ul = 1;
  constexpr int ll = 2;
  constexpr int ur = 4;
  constexpr int lr = 8;

  for(int i = 2 ; i < 7;++i){
    for(int j = 2 ; j < 7 ; ++j){
      if( (i ==2 || i == 6) && (j == 2 || j == 6))
        continue;
      drawTriangles(320).copyTo(boardImage(cv::Rect(i*320,j*320,320,320)));
    }
  }
  {
    drawTriangles(320, ul+ur).copyTo(boardImage(cv::Rect(3*320,1*320,320,320)));
    drawTriangles(320, ul+ur).copyTo(boardImage(cv::Rect(5*320,1*320,320,320)));
    drawTriangles(320, ul).copyTo(boardImage(cv::Rect(2*320,2*320,320,320)));
    drawTriangles(320, ur).copyTo(boardImage(cv::Rect(6*320,2*320,320,320)));
    drawTriangles(320, ul+ll).copyTo(boardImage(cv::Rect(1*320,3*320,320,320)));
    drawTriangles(320, ur+lr).copyTo(boardImage(cv::Rect(7*320,3*320,320,320)));
    drawTriangles(320, ul+ll).copyTo(boardImage(cv::Rect(1*320,5*320,320,320)));
    drawTriangles(320, ur+lr).copyTo(boardImage(cv::Rect(7*320,5*320,320,320)));
    drawTriangles(320, ll).copyTo(boardImage(cv::Rect(2*320,6*320,320,320)));
    drawTriangles(320, lr).copyTo(boardImage(cv::Rect(6*320,6*320,320,320)));
    drawTriangles(320, ll+lr).copyTo(boardImage(cv::Rect(3*320,7*320,320,320)));
    drawTriangles(320, ll+lr).copyTo(boardImage(cv::Rect(5*320,7*320,320,320)));
  }
  imwrite("pattern.png", drawTriangles(640));

  imshow("b", boardImage);
  imwrite("tb.png", boardImage);
  cv::waitKey(0);

  return 0;
}