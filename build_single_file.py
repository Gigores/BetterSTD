#! /usr/sbin/python

import argparse
import pathlib

def get_macro(path: pathlib.Path):
    return f"BTR_{path.name.swapcase().split('.')[0]}_IMPLEMENTATION"

parser: argparse.ArgumentParser = argparse.ArgumentParser(
    prog="build_single_header",
    description="Just a small script dude",
)
parser.add_argument("header_file_path")
parser.add_argument("implementation_file_path")
parser.add_argument("result_file_path")

args: argparse.Namespace = parser.parse_args()
header_file_path: pathlib.Path = pathlib.Path(args.header_file_path)
implementation_file_path: pathlib.Path = pathlib.Path(args.implementation_file_path)
result_file_path: pathlib.Path = pathlib.Path(args.result_file_path)

print(f"Smashing together \"{header_file_path}\" and \"{implementation_file_path}\" into \"{result_file_path}\"")

header_file: str = ""
implementation_file: str = ""

with open(header_file_path, "r") as f:
    header_file = f.read()

with open(implementation_file_path, "r") as f:
    implementation_file = f.read()

included: set[str] = set()
result_file: str = ""

for line in header_file.split('\n', 1)[1].splitlines():
    if len(line) == 0: continue
    if line.startswith("#include"):
        included.add(line.removeprefix("#include \"").removesuffix("\""))
    result_file += line + "\n"

result_file += f"\n\n#ifdef {get_macro(result_file_path)}\n"

for line in implementation_file.split('\n', 1)[1].splitlines():
    if len(line) == 0: continue
    if line.startswith("#include"):
        if line.removeprefix("#include \"").removesuffix("\"") in included:
            continue
    result_file += line + "\n"
result_file += f"\n#endif  // {get_macro(result_file_path)}\n"

with open(result_file_path, "w+") as f:
    f.write(result_file)
