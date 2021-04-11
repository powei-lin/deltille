import json


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
    with open('board.json', 'w') as outfile:
        json.dump(j, outfile, indent=4)


if __name__ == '__main__':
    main()