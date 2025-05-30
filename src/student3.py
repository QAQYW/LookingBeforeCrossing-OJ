import math

def drone(dis, Llis, Rlis, tlis):
    def optimal_speed():
        # 修正后的最优速度计算（最小化 P(v)/v）
        a, b, c, d = 0.37, 0.136, -150, 1160
        # 通过牛顿迭代法求解 (0.74v³ + 0.136v² - d) = 0
        v = 10.0  # 初始猜测值
        for _ in range(100):
            f = 0.74*v**3 + 0.136*v**2 - d
            df = 2.22*v**2 + 0.272*v
            v -= f/df
            v = max(v, 0.1)
        return max(v, 1.0)

    n = len(Llis)
    points = sorted({0.0, float(dis)} | {float(L) for L in Llis} | {float(R) for R in Rlis})
    m = len(points) - 1

    # 按传感器顺序处理区间
    sensor_segments = []
    current_sensor = 0
    for i in range(m):
        x1, x2 = points[i], points[i+1]
        while current_sensor < n and Rlis[current_sensor] <= x1:
            current_sensor += 1
        if current_sensor < n and x2 > Llis[current_sensor]:
            end = min(x2, Rlis[current_sensor])
            sensor_segments.append((x1, end, current_sensor))
            if end < x2:
                sensor_segments.append((end, x2, -1))  # 无传感器区间
        else:
            sensor_segments.append((x1, x2, -1))  # -1 表示无传感器

    # 计算每个区间的速度
    v_list = []
    sensor_time_used = [0.0]*n
    current_sensor = 0
    
    for seg in sensor_segments:
        x1, x2, s = seg
        seg_len = x2 - x1
        
        if s == -1:  # 无传感器区间
            v_list.append(optimal_speed())
        else:
            if s != current_sensor:
                current_sensor = s
                required_time = tlis[current_sensor] - sensor_time_used[current_sensor]
                total_len = Rlis[current_sensor] - Llis[current_sensor]
                v = total_len / required_time
                v_list.append(v)
            else:
                v_list.append(v_list[-1])  # 延续相同速度
                
            # 累计已用时间
            sensor_time_used[current_sensor] += seg_len / v_list[-1]

    # 合并连续相同速度区间
    out_x = [points[0]]
    out_v = []
    current_v = v_list[0]
    for i in range(len(v_list)):
        if abs(v_list[i] - current_v) > 1e-6:
            out_x.append(sensor_segments[i][0])
            out_v.append(current_v)
            current_v = v_list[i]
    out_x.append(points[-1])
    out_v.append(current_v)
    
    return out_x[1:], out_v

if __name__ == "__main__":
    inpath_wyq = "./doc/input"
    outpath_wyq = "./test-special/hack3"

    results = [] # 输入测试组数
    with open(inpath_wyq, 'r') as fin:
        index = int(fin.readline().strip())
        for _ in range(index): # 每一组
            dis = int(float(fin.readline().strip()))
            num = int(float(fin.readline().strip()))
            Llis = [] # 存放每个传感器范围左端点
            Rlis = [] # 存放每个传感器范围右端点
            tlis = [] # 存放每个传感器的传输时间
            for _ in range(num):
                # zancun = []
                # zancun = list(map(int, fin.readline().strip()))
                zancun = fin.readline().strip().split()
                Llis.append(int(float(zancun[0])))
                Rlis.append(int(float(zancun[1])))
                tlis.append(int(float(zancun[2])))
            out1, out2 = drone(dis, Llis, Rlis, tlis)
            m = len(out1)
            group_output = []
            group_output.append(str(m))
            group_output.append(' '.join(f"{x:.6f}" for x in out1))
            group_output.append(' '.join(f"{v:.6f}" for v in out2))
            results.append(group_output)
    with open(outpath_wyq, 'w') as fout:
        for group_output in results:
            for line in group_output:
                fout.write(line + "\n")

