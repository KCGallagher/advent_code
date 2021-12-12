# Using readlines()
file1 = open('Week_2/Inputs/day_12_input_c.txt', 'r')
Lines = file1.readlines()

cave_paths = {}
 
count = 0
# Strips the newline character
for line in Lines:
    count += 1
    print("Line{}: {}".format(count, line.strip()))
    line = line.strip()
    line_caves = line.split('-')
    end_cave_index = 1
    for start_cave in line_caves:
        if start_cave in cave_paths:
            cave_paths[start_cave].add(line_caves[end_cave_index])
        else:
            cave_paths[start_cave] = {line_caves[end_cave_index]}
        end_cave_index -= 1  # So that we can consider reverse direction of all paths

print(cave_paths)

def count_paths2(room, small_visited,revisit, cave):
    now_visited = small_visited
    if room == 'end':
        return 1
    if room in small_visited:
        if revisit or room == 'start':
            return 0    
        else:
            revisit = True
    if room.islower():
        now_visited = small_visited|{room}
    return sum([count_paths2(neighbor, now_visited, revisit, cave) for neighbor in cave[room]])

#part 1:
#The code for part 2 will also answer part 1, by passing it revisited = True. 
print(count_paths2('start',set({}),True,cave_paths))
#part 2
print(count_paths2('start',set({}),False,cave_paths))
