#include <iostream>

#include <deltille/target_detector.h>
// #include <deltille/

#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;



int main() {
  vector<string> names;
  // glob("/Users/powei/code/cpp/deltille-dataset/robot-hand/deltille/*.png",
  // names); names.push_back("/Users/powei/Desktop/0.png");
  // names.push_back("/Users/powei/Desktop/1.png");
  // names.push_back("tb.png");
  // names.push_back("4.png");
  // names.push_back("5.png");
  // names.push_back("6.png");
  names.push_back("fisheye/DJI_0255.jpg");
  names.push_back("fisheye/DJI_0256.jpg");
  names.push_back("fisheye/DJI_0257.jpg");
  names.push_back("fisheye/DJI_0258.jpg");
  names.push_back("fisheye/DJI_0259.jpg");
  names.push_back("fisheye/DJI_0260.jpg");
  // const string file =
  //     "/Users/powei/code/cpp/deltille-dataset/robot-hand/deltille/0000.png";


  cv::Ptr<cv::aruco::Dictionary> dictionary =
      cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_250);
  cv::Ptr<cv::aruco::CharucoBoard> board =
      cv::aruco::CharucoBoard::create(9, 9, 0.04f, 0.02f, dictionary);
  cv::Ptr<cv::aruco::DetectorParameters> params = cv::aruco::DetectorParameters::create();

  // cv::Mat boardImage;
  // board->draw(cv::Size(2560, 1600), boardImage, 0, 1);
  // cv::imwrite("BoardImage.png", boardImage);

  int count_out = 0;

  for (const auto &file : names) {
    Mat img = imread(file, 0);
    resize(img, img, Size(640, 480));
    Mat colored;
    cvtColor(img, colored, COLOR_GRAY2BGR);
    // imshow("test", img);

    orp::calibration::TaggedBoardIndexer _indexer;
    _indexer.chessboard_col = 24;
    _indexer.chessboard_row = 12;

    const cv::Size board_size;//(_indexer.chessboard_col, _indexer.chessboard_row);
    std::vector<orp::calibration::BoardObservation> boards;
    FindBoards<orp::calibration::MonkeySaddlePointSpherical>(img, board_size,
                                                             boards);
    // FindBoards<orp::calibration::SaddlePoint>(img, board_size, boards);
    _indexer.fixTriangleBoards(img, boards);


    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners;
    cv::aruco::detectMarkers(img, board->dictionary, markerCorners, markerIds,
                            params);
    std::vector<cv::Point2f> charucoCorners;
    std::vector<int> charucoIds;
    if(markerIds.size()>0)
      cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, img, board,
                                          charucoCorners, charucoIds);

    for (auto &b : boards) {
      for (size_t i = 0; i < b.corner_locations.size(); ++i) {
        const auto &c = b.corner_locations[i];
        circle(colored, c, 3, Scalar(0, 0, 255), -1);
        // cout << b.board_id << " " << b.indexed << endl;
        // corners.emplace_back(c.x, c.y, b.board_id, int(i), b.indexed);
      }
    }

    for(const auto &pt:charucoCorners){
      cv::circle(colored, pt, 3, cv::Scalar(255,0,0), -1);
    }
    imshow("c", colored);
    imwrite("result"+std::to_string(count_out++)+".png", colored);

    // waitKey(0);
  }

  // cout << "hello" << endl;

  return 0;
}