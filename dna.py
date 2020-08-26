from sys import argv, exit
import csv

if len(argv) != 3:
    print("Specify DNA database followed by sequence sample\npython dna.py databases/database.csv sequences/sequence.txt")
    exit(1)

file = open(argv[2], 'r')
file.seek(0)
DNAsequence = file.read()

file.seek(0)
char_count=0
while file.read(1) != '':
    char_count+=1

file.close()

mystery_Id = []
def STR_find_and_count(DNA_STR):
    #returns the length of longest occuring streak of the input STR
    increment=len(DNA_STR)
    STR_counter = 0
    STR_streak =0
    left_index=0
    right_index=char_count
    marker =0

    while DNA_STR in DNAsequence[marker:char_count]:
        left_index=marker
        right_index=char_count
        STR_counter=0

        while True:
            left_index=DNAsequence.find(DNA_STR,left_index,right_index)
            if left_index!=-1:
                occurencesS.append(left_index)
                STR_counter+=1
                left_index+=increment
                #store left index in case .find evaluates to -1 in the next iteration
                #pick up from this marker to resume searching for other STR streaks
                marker=left_index
                right_index=left_index+increment
            else:
                if STR_counter > STR_streak:
                    STR_streak = STR_counter
                    streak_locations.append(occurencesS[(-1*STR_counter)])
                break

    mystery_Id.append(STR_streak)
    return STR_streak

# read csv file as a list of lists
with open(argv[1], 'r') as csv_file:
    # pass the file object to reader() to get the reader object
    csv_reader = csv.reader(csv_file)
    # Pass reader object to list() to get a list of lists
    list_of_rows = list(csv_reader)

# create a list of the STR patterns from this database that
# we will search for in the supplied sequence text files
# apparently python can compare entire lists for equality
# this syntax creates a new list with the first row, excluding
# the first column
STR_list = list_of_rows[0][1:]

for i in range(len(STR_list)):
    DNA_STR = STR_list[i]
    STR_find_and_count(STR_list[i])

tmp=[]
for lst in list_of_rows[1:]:
    tmp = lst[1:]
    tmp = [ int(x) for x in tmp ]

    if tmp == mystery_Id:
        print(lst[0])
        break
else:
    print("No Match")

csv_file.close()