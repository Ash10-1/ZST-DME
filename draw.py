import matplotlib.pyplot as plt

def draw_netlist(file_path):
    # Reading the file and extracting coordinates
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Parsing the coordinates
    coordinates = []
    for line in lines:
        x, y = map(float, line.strip().split(', '))
        coordinates.append((x, y))

    # Plotting the lines
    plt.figure(figsize=(10, 10))
    for i in range(0, len(coordinates), 3):
        if i + 2 < len(coordinates):
            num1, num2, num3 = coordinates[i], coordinates[i + 1], coordinates[i + 2]
            plt.plot([num1[0], num2[0]], [num1[1], num1[1]], color='blue', linewidth=0.4)  # Reduced linewidth
            plt.plot([num2[0], num2[0]], [num1[1], num2[1]], color='blue', linewidth=0.4)  # Reduced linewidth
            plt.plot([num2[0], num2[0]], [num2[1], num3[1]], color='blue', linewidth=0.4)  # Reduced linewidth
            plt.plot([num2[0], num3[0]], [num3[1], num3[1]], color='blue', linewidth=0.4)  # Reduced linewidth

    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.title('Netlist Visualization')
    plt.show()

if __name__ == "__main__":
    file_path = "F:\\C++Project\\ZST_DME-master\\embedding\\netlist.txt"
    draw_netlist(file_path)