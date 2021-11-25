import os
import argparse

def concatenate_files(hole):
    linecount = 0
    files = os.listdir(hole)
    with open(hole + ".txt", "w+") as courseMap:
        for file in files:
            with open(hole + os.sep + file, 'r') as element:
                name = element.name
                name = name.split("/")[1]
                name = name.split(".")[0]
                lines = element.readlines()
                if name[0] == "G":
                    courseMap.write("Green\n")
                elif name[0] == "B":
                    courseMap.write("Bunker\n")
                    print("found bunker")
                elif name[0] == "T":
                    courseMap.write("Tee\n")
                else:
                    print("Undefined file type, skipping")

                for line in lines:
                    if line[0] == "#":
                        continue
                    if linecount > 0 and name[0] == "T":
                        linecount = 0
                        continue
                    linecount += 1
                    courseMap.write("p " + line)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("hole", type=str, help="The hole")
    args = parser.parse_args()
    hole = args.hole
    concatenate_files(hole)

