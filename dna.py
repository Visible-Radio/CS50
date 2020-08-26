from sys import argv, exit
import csv

# get command line arguments
# first argument is Database file
# second argument is DNA sequence file to look through

#argv list is zero indexed
#the python file being executed is in position 0

for i in range(len(argv)):
    print(i, end=" ")
    print(argv[i])

print (len(argv))

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
print(f"Chars in file: {char_count}")
print(f"Length as determined by len(){len(DNAsequence)}")

#DNA_STR = ""
occurencesS=[]
streak_locations = []
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

def STR_info(DNA_STR):
    #prints out information about where STRs occured in the file
    if len(streak_locations) > 0:
        print(f"Largest occuring streak of {DNA_STR}: {STR_find_and_count(DNA_STR)}, beginning at {streak_locations[-1]}",)
        print ("STR matches at: ", end="")
        for i in range(len(occurencesS)):
           print(f"{occurencesS[i]}", end=", ")
        print("")
    occurencesS.clear()
    streak_locations.clear()

#STR_info(DNA_STR)

# read csv file as a list of lists
with open(argv[1], 'r') as csv_file:
    # pass the file object to reader() to get the reader object
    csv_reader = csv.reader(csv_file)
    # Pass reader object to list() to get a list of lists
    list_of_rows = list(csv_reader)
    print(list_of_rows)



# create a list of the STR patterns from this database that
# we will search for in the supplied sequence text files
# apparently python can compare entire lists for equality
# this syntax creates a new list with the first row, excluding
# the first column
STR_list = list_of_rows[0][1:]
print(f"Searching supplied sequence for: {STR_list}")

for i in range(len(STR_list)):
    DNA_STR = STR_list[i]
    STR_info(STR_list[i])

print(mystery_Id)

print(list_of_rows)



tmp=[]
for lst in list_of_rows[1:]:
    tmp = lst[1:]
    tmp = [ int(x) for x in tmp ]
    print(tmp)
    if tmp == mystery_Id:
        print(lst[0])
        break
else:
    print("No Match")

csv_file.close()


