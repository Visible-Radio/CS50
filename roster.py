from sys import argv, exit
import csv
from cs50 import SQL

#check for correct usage
if len(argv) != 2:
    print("Specify the house roster to print\n python roster.py Slitheryn")
    exit(1)

db = SQL("sqlite:///students.db")

#get a list of dicts back from db.execute with the results of the SQL querry
roster=db.execute('SELECT first,middle,last,birth FROM students WHERE house =? ORDER BY last, first', argv[1])

#print the results formated correctly
for row in roster:
    if row["middle"]!=None:
        print(f'{row["first"]} {row["middle"]} {row["last"]}, born {row["birth"]}')
    else:
        print(f'{row["first"]} {row["last"]}, born {row["birth"]}')
