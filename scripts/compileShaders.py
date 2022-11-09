import argparse
import os
import platform

# Input arguments
argParser = argparse.ArgumentParser()
argParser.add_argument("vlkdir", help="Folder containing the glslc executable.")
argParser.add_argument("src", help="Input folder contining all shaders to be compiled.")
argParser.add_argument("dst", help="Output folder where all the compiled shaders will be stored.")

# Parse the input arguments
args = argParser.parse_args()

# Check argument validity
if not os.path.exists(args.vlkdir):
    print(F"[ERROR] - The directory {args.vlkdir} does not exist.")
    exit()

if platform.system() == "Windows":
    if not os.path.exists(os.path.join(args.vlkdir, "glslc.exe")):
        print(F"[ERROR] - No GLSL to SPIR-V compiler in {args.vlkdir}")
        exit()
elif platform.system() == "Linux":
    if not os.path.exists(os.path.join(args.vlkdir, "glslc")):
        print(F"[ERROR] - No GLSL to SPIR-V compiler in {args.vlkdir}")
        exit()
else:
    print(F"[ERROR] - Platform {platform.system()} not supported.")
    exit()

if not os.path.exists(args.src):
    print(F"[ERROR] - The directory {args.src} does not exist.")
    exit()

# Create the initial folders, if they do not exist.
if not os.path.exists(args.src):
    os.makedirs(args.src)

if not os.path.exists(args.dst):
    os.makedirs(args.dst)

baseRelPath = args.src
# Iterate over all files in the subfolders.
for (dirpath, dirnames, filenames) in os.walk(args.src):
    # Check whether the current subfolder exists in the output directory.
    currentRelPath = os.path.relpath(dirpath, baseRelPath)
    destFolder = os.path.join(args.dst, currentRelPath)
    if not os.path.exists(destFolder):
        os.makedirs(destFolder)

    for file in filenames:
        if file.endswith(".glsl") or file.endswith(".vert") or file.endswith(".frag"):
            newFileName = os.path.splitext(file)[0] + ".spv"

            if platform.system() == "Windows":
                os.system(F"{args.vlkdir}\\glslc.exe {dirpath}\\{file} -o {destFolder}\\{newFileName}")
            elif platform.system() == "Linux":
                os.system(F"./{args.vlkdir}/glslc {dirpath}/{file} -o {destFolder}/{newFileName}")
            else:
                print(F"[ERROR] - Platform {platform.system()} not supported.")
                exit()
