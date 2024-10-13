import matplotlib.pyplot as plt


def bezier(points: list[tuple[float, float] | tuple[float, ...]], t: float):
    prev_points = points
    while len(prev_points) > 1:
        current_points: list[tuple[float, float] | tuple[float, ...]] = []
        for i in range(0, len(prev_points) - 1):
            p1 = prev_points[i]
            p2 = prev_points[i + 1]
            current_points.append(tuple((p2[j] - p1[j]) * t + p1[j] for j in range(2)))
        prev_points = current_points
    return prev_points[0]


if __name__ == "__main__":
    bez = [
        bezier([(0, 0), (0.3, 0.9), (0.9, 0.3), (1, 1)], t / 100)[1] for t in range(100)
    ]
    x = [i / 100 for i in range(100)]
    plt.subplot(1, 1, 1)
    plt.plot(x, bez)
    plt.show()
    print(bez)
