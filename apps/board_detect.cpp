#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <vector>

#include <deltille/target_detector.h>

#include <deltille/apriltags/Tag16h5.h>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
  vector<string> file_names;
  // string pattern = argv[1];
  // glob(pattern+
      //  "/*.png",
      //  file_names);
  // VideoWriter vw;
  VideoWriter writer("VideoTest.mov", VideoWriter::fourcc('a', 'v', 'c', '1'), 30.0, Size(848, 800));

  glob("/Users/powei/code/project/Intel-Realsense-T265-opencv/build/imgs/cam0/"
       "*.png",
       file_names);
  // file_names.resize(30);
  orp::calibration::TaggedBoardIndexer _indexer;
  // _indexer.chessboard_col = 11;
  // _indexer.chessboard_row = 11;
  cout << _indexer.board_defs.size() << endl;
  _indexer.board_defs.resize(1);
  // _indexer.board_defs[0].border_bits = 1.0;
  _indexer.board_defs[0].cols = 11;
  _indexer.board_defs[0].rows = 11;
  _indexer.board_defs[0].tag_locations[1] = Point2i(3, 8);
  _indexer.board_defs[0].tag_locations[2] = Point2i(3, 8);
  // _indexer.board_defs[0].detector =
  _indexer.detectors["t16h5"] =
      make_shared<orp::calibration::TagFamily>(AprilTags::tagCodes16h5, 1.0);
  _indexer.tag_to_board_map[1] = make_pair(0, Point2i(3, 8));
  _indexer.tag_to_board_map[2] = make_pair(0, Point2i(5, 8));
  _indexer.tag_to_board_map[3] = make_pair(0, Point2i(3, 6));
  _indexer.tag_to_board_map[4] = make_pair(0, Point2i(5, 6));
  _indexer.tag_to_board_map[7] = make_pair(0, Point2i(5, 4));
  _indexer.tag_to_board_map[8] = make_pair(0, Point2i(7, 4));
  _indexer.tag_to_board_map[9] = make_pair(0, Point2i(5, 2));
  _indexer.tag_to_board_map[10] = make_pair(0, Point2i(7, 2));

  // return 0;

  const cv::Size board_size(_indexer.chessboard_col, _indexer.chessboard_row);
  std::vector<orp::calibration::BoardObservation> boards;
  for (auto name : file_names) {
    // cout << name << endl;
    Mat img = imread(name, IMREAD_GRAYSCALE);
    Mat colored;
    cvtColor(img, colored, COLOR_GRAY2BGR);
    FindBoards<orp::calibration::MonkeySaddlePointSpherical>(img, board_size,
                                                             boards);
    _indexer.fixTriangleBoards(img, boards);

    for (auto &b : boards) {
      // cout << b.board << endl;
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
    writer << colored;
    waitKey(1);
  }
  writer.release();

  return 0;
}