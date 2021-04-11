#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <vector>

#include <deltille/target_detector.h>

#include <deltille/apriltags/Tag16h5.h>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
  string vname = "/Users/powei/Downloads/04052021_100751_";
  VideoCapture v1(vname+"A.mov");
  VideoCapture v0(vname+"B.mov");
  cout << v0.get(CAP_PROP_FRAME_COUNT) << endl;
  cout << v0.get(CAP_PROP_FRAME_HEIGHT) << endl;
  cout << v0.get(CAP_PROP_FRAME_WIDTH) << endl << endl;
  cout << v1.get(CAP_PROP_FRAME_COUNT) << endl;
  cout << v1.get(CAP_PROP_FRAME_HEIGHT) << endl;
  cout << v1.get(CAP_PROP_FRAME_WIDTH) << endl;
  // return 0;
  orp::calibration::TaggedBoardIndexer _indexer;
  _indexer.loadFromJson("../script/board.json");
  cout << _indexer.board_defs.size() << endl;
  _indexer.board_defs[0].corner_locations = Mat(11, 11, CV_32FC3);
  // _indexer.board_defs[0].corner_locations.at<Vec3f>(2,3)

  // return 0;

  const cv::Size board_size(_indexer.chessboard_col, _indexer.chessboard_row);
  std::vector<orp::calibration::BoardObservation> boards;
  while(true){

    Mat img, img1, colored;
    v1 >> colored;
    resize(colored, colored, Size(), 0.25, 0.25);

    cvtColor(colored, img, COLOR_BGR2GRAY);
    FindBoards<orp::calibration::MonkeySaddlePointSpherical>(img, board_size,
                                                             boards);
    _indexer.fixTriangleBoards(img, boards);

    for (auto &b : boards) {
      cout << b.board << endl;
      // if(b.indexed)
        for (size_t i = 0; i < b.corner_locations.size(); ++i) {
          const auto &c = b.corner_locations[i];
          circle(colored, c, 3, Scalar(0, 0, 255), -1);
          int row = i / b.board.cols;
          int col = i % b.board.cols;
          putText(colored, to_string(*b.board.ptr<int>(row, col)), c, 1, 1,
                  Scalar(0, 255, 0));
        }
    }
    imshow("color", colored);
    waitKey(0);
  }

  return 0;
}