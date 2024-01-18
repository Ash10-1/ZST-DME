import matplotlib.pyplot as plt
import matplotlib.patches as patches

def calculate_rect_vertices_custom(line, radius):
    # 计算斜率
    dx = line[1][0] - line[0][0]
    dy = line[1][1] - line[0][1]
    slope = dy / dx if dx != 0 else float('inf')

    # 确定使用哪个端点来计算顶点
    top_point = line[0] if line[0][1] > line[1][1] else line[1]
    bottom_point = line[1] if line[0][1] > line[1][1] else line[0]

    if slope == 1:
        # 斜率为1
        return [
            (top_point[0] + radius, top_point[1]),
            (bottom_point[0], bottom_point[1] - radius),
            (bottom_point[0] - radius, bottom_point[1]),
            (top_point[0], top_point[1] + radius)
        ]
    elif slope == -1:
        # 斜率为-1
        return [
            (top_point[0], top_point[1] + radius),
            (top_point[0] - radius, top_point[1]),
            (bottom_point[0], bottom_point[1] - radius),
            (bottom_point[0] + radius, bottom_point[1])
        ]
    else:
        # 对于其他斜率，使用原先的计算方式
        return [
            (line[0][0], line[0][1] + radius),
            (line[1][0] + radius, line[1][1]),
            (line[1][0], line[1][1] - radius),
            (line[0][0] - radius, line[0][1])
        ]

def draw_rectangle_and_lines_with_unique_coords(rect_vertices1, rect_vertices2, line1, line2):
    # 创建一个图和坐标轴
    fig, ax = plt.subplots()

    # 绘制矩形
    rect = patches.Polygon(rect_vertices1, closed=True, edgecolor='r', facecolor='none')
    rect1 = patches.Polygon(rect_vertices2, closed=True, edgecolor='y', facecolor='none')
    ax.add_patch(rect)
    ax.add_patch(rect1)

    # 绘制线段
    ax.plot([line1[0][0], line1[1][0]], [line1[0][1], line1[1][1]], color='b')
    ax.plot([line2[0][0], line2[1][0]], [line2[0][1], line2[1][1]], color='g')

    # 所有点的集合，确保不重复
    all_points = set(rect_vertices1 + rect_vertices2 + line1 + line2)

    # 标注每个独特点的坐标
    for point in all_points:
        ax.text(point[0], point[1], f'({point[0]}, {point[1]})', fontsize=8, ha='right')

    # 设置坐标轴范围
    all_x = [p[0] for p in all_points]
    all_y = [p[1] for p in all_points]
    ax.set_xlim(min(all_x) - 10000, max(all_x) + 10000)
    ax.set_ylim(min(all_y) - 10000, max(all_y) + 10000)

    # 显示图形
    plt.show()

# 示例数据
line1 = [(67234.959126427566,29064), (67120.959126427566,28950)]
radius1 = 3164.806377122356
rect_vertices1 = calculate_rect_vertices_custom(line1, radius1)

line2 = [(62780.685353566463, 27794.251643028107), (61030.068248690965,26043.634538152612)]
radius2 = 2331.2157527106401
rect_vertices2 = calculate_rect_vertices_custom(line2, radius2)

# 使用修改后的函数进行绘图
draw_rectangle_and_lines_with_unique_coords(rect_vertices1, rect_vertices2, line1, line2)
