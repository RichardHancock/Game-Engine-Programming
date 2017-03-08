import os
import sys

argLength = len(sys.argv)
if argLength < 11:
    sys.exit()

# Arguments
srcDir = sys.argv[1]
compilerCommand = sys.argv[2]
compilerFlags = sys.argv[3]
compilerDebugFlags = sys.argv[4]
compilerReleaseFlags = sys.argv[5]
outputPath = sys.argv[6]
outputFileName = sys.argv[7]
resourceFolderName = sys.argv[8]
resourceFolderPath = sys.argv[9]
resourceFolderOutputPath = sys.argv[10]

cppFiles = []

print("Generating MakeFile")

# Loop through src folders looking for .cpp files
for root, dirs, files in os.walk(srcDir):
    for file in files:
        if file.endswith(".cpp"):
            fullFilePath = os.path.join(root, file)
            print(fullFilePath)
            cppFiles.append(fullFilePath)


makeFileLines = [
    "CC=" + compilerCommand,
    "",
    "FLAGS=" + compilerFlags,
    "DFLAGS=" + compilerDebugFlags,
    "RFLAGS=" + compilerReleaseFlags,
    "",
    "SOURCES=",  # src files will be appended later
    "OUTPUT=\"" + outputPath + outputFileName + "\"",
    "$(shell mkdir -p \"" + outputPath + "\")",
    "",
    "all: debug",
    "",
    "debug:",
    "\t$(CC) $(SOURCES) $(FLAGS) $(DFLAGS) -o $(OUTPUT)",
	"\trm -r -f \"" + resourceFolderOutputPath + resourceFolderName + "\"",
	"\tcp -r \"" + resourceFolderPath + resourceFolderName + "\" \"" + resourceFolderOutputPath + "\"",
    "",
    "release:",
    "\t$(CC) $(SOURCES) $(FLAGS) $(RFLAGS) -o $(OUTPUT)",
	"\trm -r -f \"" + resourceFolderOutputPath + resourceFolderName + "\"",
	"\tcp -r \"" + resourceFolderPath + resourceFolderName + "\" \"" + resourceFolderOutputPath + "\"",
]

for srcFile in cppFiles:
    makeFileLines[6] += "\"" + srcFile + "\" "


makePath = "Makefile"

with open(makePath, "w") as fileHandler:
    for line in makeFileLines:
        fileHandler.write(line + "\n")
