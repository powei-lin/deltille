import json
import math


def main():
    j = {}
    j['tag_code'] = 't16h5'
    j['cols'] = 11
    j['rows'] = 11
    j['board'] = {}
    j['board']['0'] = {}
    j['board']['0']['0'] = {'id': 1, 'board_x': 3, 'board_y': 8}
    j['board']['0']['1'] = {'id': 2, 'board_x': 5, 'board_y': 8}
    j['board']['0']['2'] = {'id': 3, 'board_x': 3, 'board_y': 6}
    j['board']['0']['3'] = {'id': 4, 'board_x': 5, 'board_y': 6}
    j['board']['0']['4'] = {'id': 7, 'board_x': 5, 'board_y': 4}
    j['board']['0']['5'] = {'id': 8, 'board_x': 7, 'board_y': 4}
    j['board']['0']['6'] = {'id': 9, 'board_x': 5, 'board_y': 2}
    j['board']['0']['7'] = {'id': 10, 'board_x': 7, 'board_y': 2}
    j['corner3d'] = {}
    count = 0
    for r in range(1, 11):
        c_s = 5 - (r-1)//2
        x_s = 0.0 + ((r-1)%2)*15.0
        y = -(r-1)*30.0*math.sqrt(3)/2.0
        for c in range(5):
            j['corner3d'][str(count)] = {'b':0, 'row': r, 'col': c+c_s}
            j['corner3d'][str(count)]['x'] = x_s+c*30.0
            j['corner3d'][str(count)]['y'] = y
            j['corner3d'][str(count)]['z'] = 0.0
            count += 1
            # print(r, c+c_s)

#   _indexer.board_defs[0].corner_locations.at<Vec3f>(1, 5) = Vec3f(1,2,4);
    with open('board.json', 'w') as outfile:
        json.dump(j, outfile, indent=4)


if __name__ == '__main__':
    main()