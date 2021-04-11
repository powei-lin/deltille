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
  _indexer.loadFromJson("../../script/board.json");
  cout << _indexer.board_defs.size() << endl;
  // _indexer.board_defs[0].corner_locations = Mat(11, 11, CV_32FC3, Vec3f(0,0,0));

  // _indexer.board_defs[0].corner_locations.at<Vec3f>(1, 5) = Vec3f(1,2,4);

  // return 0;

  const cv::Size board_size(_indexer.chessboard_col, _indexer.chessboard_row);
  std::vector<orp::calibration::BoardObservation> boards;
  while(true){

    Mat img, img1, colored;
    v1 >> colored;
    resize(colored, colored, Size(), 0.5, 0.5);

    cvtColor(colored, img, COLOR_BGR2GRAY);
    FindBoards<orp::calibration::MonkeySaddlePointSpherical>(img, board_size,
                                                             boards);
    _indexer.fixTriangleBoards(img, boards);

    for (auto &b : boards) {
      cout << b.board << endl;
      // if(b.indexed)
        for (size_t i = 0; i < b.corner_locations.size(); ++i) {
          const auto &c = b.corner_locations[i];
          const int row = i / b.board.cols;
          const int col = i % b.board.cols;
          const int id = *b.board.ptr<int>(row, col);
          if(id == -1)
            continue;
          circle(colored, c, 3, Scalar(0, 0, 255), -1);
          // cout << id << " ";
          Vec3f p3d = *_indexer.board_defs[0].corner_locations.ptr<Vec3f>(row, col);
          putText(colored, to_string(id), c, 1, 1,
                  Scalar(0, 255, 0));
          // putText(colored, to_string(p3d[0]), c, 1, 1,
                  // Scalar(0, 255, 0));
          // putText(colored, to_string(p3d[1]), c, 1, 1,
          //         Scalar(0, 255, 0));
        }
    }
    imshow("color", colored);
    waitKey(1);
  }

  return 0;
}