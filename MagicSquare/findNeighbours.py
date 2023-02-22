
import copy


def bfs():
    frontier = parent_list[0]
    del parent_list[0]

    #print_array(frontier)
    for i in find_location_neighbours(frontier):
        xi = copy.deepcopy(frontier)
        #print(find_location(frontier, input_char))
        #print(i)
        swap_element(xi, find_location(frontier, input_char), i)
        #print("===")
        #print_array(xi)
        if xi == globalArray:
            print('your goal state is :')
            print_array(xi)
            return xi
        if xi != parent_list:
            parent_list.append(xi)


# print(ADJ)
# # keep track of visited nodes
visited = {str(i): False for i in range(1, 16)}
visited['5'] = False
visited['16'] = False


def dls(start, goal):
    depth = 0
    limit = 200
    OPEN = []
    CLOSED = []
    OPEN.append(start)
    visited["5"] = True
    while OPEN != []:  # Step 2
        if depth <= limit:
            current = OPEN.pop()
            # visited[current] = False
            if current == goal:
                # CLOSED.append(current)
                print("Goal Node Found")
                # print(CLOSED)
                return True
            else:
                # CLOSED.append(current)
                lst = successors(current)
                for i in lst:
                    # try to visit a node in future, if not already been to it
                    if(not(visited[i])):
                        OPEN.append(i)
                        visited[i] = True
                depth += 1

        else:
            print("Not found within depth limit")
            return False

    return False


globalArray = [
    ['1', '2', '3', '4'],
    ['5', '6', '7', '8'],
    ['9', '10', '11', '12'],
    ['13', '14', '15', '16'],
]
x = copy.deepcopy(globalArray)


def find_location(myList, v):
    for i, x in enumerate(myList):
        if v in x:
            return i, x.index(v)


def find_neighbours(arr, current_place_val):
    """
    This method takes 2d array and return list of all elements
    with all horizontal and vertical neighbours
    :param arr: 2d array
    :return: list of array elements with  neighbours
    """
    neighbors = []

    for i in range(len(arr)):
        for j, value in enumerate(arr[i]):

            if i == 0 or i == len(arr) - 1 or j == 0 or j == len(arr[i]) - 1:
                # corners
                new_neighbors = []
                if i != 0:
                    new_neighbors.append(arr[i - 1][j])  # top neighbor
                if j != len(arr[i]) - 1:
                    new_neighbors.append(arr[i][j + 1])  # right neighbor
                if i != len(arr) - 1:
                    new_neighbors.append(arr[i + 1][j])  # bottom neighbor
                if j != 0:
                    new_neighbors.append(arr[i][j - 1])  # left neighbor

            else:
                # add neighbors
                new_neighbors = [
                    arr[i - 1][j],  # top neighbor
                    arr[i][j + 1],  # right neighbor
                    arr[i + 1][j],  # bottom neighbor
                    arr[i][j - 1]   # left neighbor
                ]

            neighbors.append({
                "index": i * len(arr[i]) + j,
                "value": value,
                "neighbors": new_neighbors})

            if value == current_place_val:
                return new_neighbors


input_char = input()


def swap_element(array, start, end):
    array[end[0]][end[1]], array[start[0]][start[1]
                                           ] = array[start[0]][start[1]], array[end[0]][end[1]]

    return 0


def print_array(array):
    for row in array:
        for cell in row:
            print(cell, end=" ")
        print()


def find_location_neighbours(arr):
    x = find_neighbours(arr, input_char)
    v = []
    for c in x:
        for i in range(0, 4):
            for j in range(0, 4):
                if arr[i][j] == c:
                    v.append((i, j))
    return v


parent_list = [[['5', '1', '2', '4'],
                ['6', '16', '3', '8'],
                ['9', '11', '7', '12'],
                ['13', '10', '14', '15'], ]]


# while True:
#     if bfs() == globalArray:
#         break

while True:
    if dls(start=input_char, goal=3) == globalArray:
        break
