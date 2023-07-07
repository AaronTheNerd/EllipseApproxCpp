import csv

def main():
    with open("data/test_data.csv") as file:
        reader = csv.reader(file, delimiter=",", quoting=csv.QUOTE_NONE)
        for row in reader:
            print("  {{" + row[0] + ", " + row[1] + "}, " + row[2] + "},")

if __name__ == "__main__":
    main()