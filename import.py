from sys import argv, exit
import csv
from cs50 import SQL

if len(argv) != 2:
    print("Specify CSV file to be opened as follows:\npython import.py characters.csv")
    exit(1)

# Create database
open(f"students.db", "w").close()
db = SQL("sqlite:///students.db")

# Create tables
db.execute("CREATE TABLE students (id INTEGER PRIMARY KEY AUTOINCREMENT, first VARCHAR(255), middle VARCHAR(255), last VARCHAR(255), house VARCHAR(10), birth INTEGER)")

with open(argv[1], "r") as characters:
    reader = csv.DictReader(characters)
    for row in reader:
        #Split the names string in the csv file which clumps all names together
        #.split() function returns a list of strings
        #by default it splits when it finds whitespace
        names_ls = row["name"].split()
        if len(names_ls) != 3:
            names_ls.insert(-1, "NULL")

        #Insert fields from csv by substituting values into each ? placeholder
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", names_ls[0], names_ls[1], names_ls[2], row["house"], row["birth"])



