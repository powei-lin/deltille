import math
import svgwrite
import tag_family


def genTag(top_x: float,
           top_y: float,
           tri_side: float,
           tag_code="",
           color='black',
           upSideDown=False):

    tri_h = tri_side * math.sqrt(3) / 2.0
    points = [(top_x + tri_side / 2.0, top_y), (top_x, top_y + tri_h),
              (top_x + tri_side, top_y + tri_h)]
    if (upSideDown):
        points = [(top_x + tri_side / 2.0, top_y + tri_h), (top_x, top_y),
                  (top_x + tri_side, top_y)]
    triangles = [(points, color)]

    if (len(tag_code) > 0):
        sh = math.sqrt(len(tag_code)) + 3
        s_x = top_x + tri_side / 2.0
        tri_side /= sh
        s_x -= tri_side / 2.0
        tri_h /= sh
        s_y = top_y + tri_h * 2.0
        s_col_size = 1
        current_row = 0
        current_col = 0
        for c in tag_code:
            if (c == '1'):
                triangles += genTag(s_x + current_col * tri_side / 2.0,
                                    s_y + current_row * tri_h,
                                    tri_side,
                                    "",
                                    color='white',
                                    upSideDown=current_col % 2 == 1)
            elif (c == '0'):
                triangles += genTag(s_x + current_col * tri_side / 2.0,
                                    s_y + current_row * tri_h,
                                    tri_side,
                                    "",
                                    upSideDown=current_col % 2 == 1)
            current_col += 1
            if (current_col >= s_col_size):
                current_col = 0
                s_col_size += 2
                current_row += 1
                s_x -= tri_side / 2.0
        pass
    return triangles


def main():
    # A4 size
    dwg = svgwrite.Drawing('deltille_board.svg',
                           size=('210mm', '297mm'),
                           viewBox=('0 0 210 297'))
    tagcodes = tag_family.getT16h5()

    # draw background
    dwg.add(dwg.rect(insert=(0, 0), size=(210, 297), fill='white'))

    col_size = 6
    rol_size = 10
    triangle_size = 30.0
    triangle_height = triangle_size * math.sqrt(3) / 2.0

    for r in range(rol_size):
        shift = 0
        if (r % 2 == 1):
            shift = triangle_size / 2.0
        for c in range(col_size):
            tag_c = ""
            if (r % 2 == 1 and c % 2 == (0 + (r % 4 - 1) // 2)):
                tag_c = tagcodes.pop(0)
            tris = genTag(c * triangle_size + shift, r * triangle_height,
                          triangle_size, tag_c)
            for tri in tris:
                dwg.add(dwg.polygon(tri[0], fill=tri[1], stroke='none'))

    dwg.save()


if __name__ == '__main__':
    main()