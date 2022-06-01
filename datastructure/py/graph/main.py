import pandas as pd
import numpy as np


''' 索引转节点编号 '''


def toIndex(i):
    # 循环所用索引是从0开始的,而编号是从1开始的
    return i + 1


''' load data 数据加载 '''


need = []
distance = []
DATA_LEN = 0


def load_data(filename):
    print(filename)
    global need, distance, DATA_LEN
    data = pd.read_csv(filename)
    # [start_row:end_row, start_col:end_col]
    need = np.array(data.fillna(value=40))[:1, 1:][0]  # 需求量
    distance = np.array(data.fillna(value=-1))[1:, 1:]  # 边权矩阵(不连接点设置为-1)
    DATA_LEN = len(need)  # 顶点数


''' check data 检验并修复不对称点 '''


def check_error():
    error = []
    for n in range(DATA_LEN):
        for m in range(n, DATA_LEN):
            if distance[n][m] != distance[m][n]:
                error.append({toIndex(n), toIndex(m)})
                # 修复不对称点
                if distance[n][m] < 1:
                    distance[n][m] = distance[m][n]
                if distance[m][n] < 1:
                    distance[m][n] = distance[n][m]
    print("error points:", error)  # 输出错误点


''' 数据可视化 '''


def view_data_on_html_with_path(routes, s="->", title="graph"):  # 带方向的路径可视化
    # routes 路径列表 eg:["1->8->9","2->3->5"], s 路径分割符
    from pyecharts import options as opts
    from pyecharts.charts import Graph
    nodes = []
    links = []

    for i in range(len(routes)):  # 补上分割符方便判断邻边是否包含在路径内
        routes[i] = s + routes[i] + s

    def is_passed_route(prev, next=-1):  # 判断是否为经过的路径
        if next != -1:
            d1, d2 = 0, 0
            for idx, route in enumerate(routes):

                if route.find(s + str(prev) + s + str(next) + s) != -1:
                    d1 = 1
                if route.find(s + str(next) + s + str(prev) + s) != -1:
                    d2 = 1

                if d1 or d2:
                    return idx, (d2 << 1) | d1  # 索引 & 方向(1单向 2反单向 3双向)
            return -1, -1
        for idx, route in enumerate(routes):
            if route.find(s + str(prev) + s) != -1:
                return idx  # 路线索引
        return -1

    def is_center(node):  # 判断节点是否为中心点
        # 路径起始点为中心点
        for idx, route in enumerate(routes):
            if route.startswith(s + str(node) + s):
                return idx
        return -1

    # 节点设置
    for n in range(DATA_LEN):

        point_color = ''
        point_bg_color = ''
        if is_center(toIndex(n)) != -1:
            point_color = 'white'
            point_bg_color = 'blue'
        elif is_passed_route(toIndex(n)) != -1:
            point_color = 'green'
        else:
            point_color = 'black'

        nodes.append(opts.GraphNode(name=f"{toIndex(n)}", symbol_size=need[n] / 4, value=need[n],
                                    label_opts=opts.LabelOpts(color=point_color, background_color=point_bg_color)))

        # 边设置
        for m in range(n, DATA_LEN):
            if distance[n][m] > 0:
                line_style = 'solid'
                line_arrow = []
                line_width = 4

                index, cnt = is_passed_route(toIndex(n), toIndex(m))

                if cnt == 1:  # 单向箭头
                    line_arrow = ['arrow', '']
                elif cnt == 2:  # 反单向箭头
                    line_arrow = ['arrow']
                elif cnt == 3:  # 双向箭头
                    line_arrow = ['arrow', 'arrow']
                else:  # 无箭头
                    line_width = 2
                    line_style = 'dot'

                links.append(opts.GraphLink(source=f"{toIndex(n)}", target=f"{toIndex(m)}", value=distance[n][m], symbol=line_arrow,
                                            linestyle_opts=opts.LineStyleOpts(type_=line_style, width=line_width)))  # 虚线 dot
    graph = (
        Graph()
        .add("", nodes, links, repulsion=1000,
             edge_label=opts.LabelOpts(is_show=True, position="middle", formatter="{c}"),
             label_opts=opts.LabelOpts(is_show=True, formatter="{b}({c})"),
             )
        .set_global_opts(title_opts=opts.TitleOpts(title=title))
    )

    graph.render("graph.html")


''' 显示关系图 (用于观察族群) '''


def view_data_on_plot():
    import matplotlib.pyplot as plt
    # 点的颜色深浅代表距离的远景
    plt.matshow(distance)
    plt.show()
    # 作用: 配合matlab的腐蚀膨胀来查看族群特性


''' 寻找所有奇点 '''


def find_all_odd_points():
    odd_point = []  # 奇点: 奇数条边的点
    for n in range(DATA_LEN):
        cnt = 0
        for m in range(DATA_LEN):
            if distance[n][m] > 0:
                cnt += 1
        if cnt % 2 == 1:
            odd_point.append(toIndex(n))
    print("odd nodes:", odd_point)


''' 路径连接性检查 '''


def check_connections(path, s="->"):
    # path: 路径, s: 分割符
    # eg: check_connections("9->8->5->7->8->9","->")
    node = path.split(s)
    pts = [int(i) for i in node if i != ""]  # 删除空元素
    unconnections = []
    sum = 0
    for n in range(len(pts) - 1):
        prev = pts[n]
        next = pts[n + 1]
        sum += distance[prev - 1][next - 1]
        if distance[prev - 1][next - 1] < 1:
            unconnections.append({prev, next})
    print("connected nodes:", len(pts))
    print("unconnections:", unconnections)
    print("distance", sum)


'''保存到csv文件'''


def save_data(filename="data_fix.csv"):
    data = pd.DataFrame(distance)
    data.to_csv(filename)


'''operations'''


# 数据加载
load_data(filename="./data1.csv")

# 检查数据是否含错
check_error()
# 检查是否修复
check_error()

# 数据保存
# save_data()

# 寻找所有奇点
find_all_odd_points()

# 显示高度图
view_data_on_plot()


routes = [
    "25->15->16->25->27->26->30->25",
    "5->2->5->7->8->5->8->9->10->9",
]

# 连接性检查
for route in routes:
    check_connections(route, s="->")

# 路径显示 (会在同目录下生成 graph.html)
view_data_on_html_with_path(
    routes, s="->", title="\n".join(routes))


''' 示例路径 '''
# 25->15->16->25->27->26->30->25
# 5->2->5->7->8->5->8->9->10->9
# 16->15->24->15->25->16->21->16->24->25->27->26->30->25->16
