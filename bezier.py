import bisect
import matplotlib.pyplot as plt


def bezier(points: list[tuple[float, float] | tuple[float, ...]], t: float):
    prev_points = points
    while len(prev_points) > 1:
        current_points: list[tuple[float, float] | tuple[float, ...]] = []
        for i in range(0, len(prev_points) - 1):
            p1 = prev_points[i]
            p2 = prev_points[i + 1]
            current_points.append(
                tuple([(p2[j] - p1[j]) * t + p1[j] for j in range(2)])
            )
        prev_points = current_points
    return prev_points[0]


if __name__ == "__main__":
    bez = [
        bezier([(0, 0), (0.42, 0.0), (0.58, 1), (1, 1)], t / 500) for t in range(500)
    ]
    y: list[float] = []
    for i in range(100):
        ratio = i / 100
        index = bisect.bisect_left(bez, ratio, key=lambda x: x[0])
        if index >= len(bez):
            y.append(1.0)
            continue
        if len(y) > 0:
            y.append(bez[index][1] * 0.5 + y[-1] * 0.5)
        else:
            y.append(bez[index][1])
    plt.subplot(1, 1, 1)
    plt.plot([i / 100 for i in range(100)], y)
    plt.show()
    print(bez)
