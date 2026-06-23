"""
===============================================================================
Arwin Studios - Codebase Context Bundler (Token-Optimized)
===============================================================================
Features:
1. Target Filtering: Only reads specific file types (.h, .cpp, .ini, CMakeLists.txt).
2. Noise Reduction: Skips compiled binaries and version control folders.
3. File Headers: Injects clear delimiters and relative file paths.
4. Metadata Injection: Adds generation timestamp, file count, and line count.
5. [UPDATED] Pruned Directory Tree: Appends a structural map of the project,
   but strictly excludes heavy third-party directories, build folders, and
   dependencies to prevent token wastage.
===============================================================================
"""

import os
from datetime import datetime

# Files to look for during the code-read phase
TARGET_EXTENSIONS = ('.h', '.cpp', 'CMakeLists.txt', '.ini')
OUTPUT_FILE = 'FlowContext.txt'

# Folders to completely ignore for BOTH code reading AND tree generation
# This prevents token bloat from CMake FetchContent (ImGui, SFML, Vulkan)
DIRECTORY_EXCLUSIONS = {
    '.git', '.vs', '.cache',
    'build', 'out', 'bin',
    '_deps', 'vendor', 'ThirdParty'
}


def generate_directory_tree(dir_path, prefix=""):
    """
    Recursively maps the directory structure into a visual tree format.
    Actively prunes excluded directories to save AI token limits.
    """
    tree_str = ""
    try:
        # Get all items, but filter out the heavy/excluded directories immediately
        raw_items = os.listdir(dir_path)
        items = [item for item in raw_items if item not in DIRECTORY_EXCLUSIONS and item != OUTPUT_FILE]

        # Sort items: folders first, then files
        items = sorted(items, key=lambda x: (not os.path.isdir(os.path.join(dir_path, x)), x))
    except PermissionError:
        return tree_str

    for i, item in enumerate(items):
        path = os.path.join(dir_path, item)
        is_last = (i == len(items) - 1)
        connector = "└── " if is_last else "├── "
        tree_str += f"{prefix}{connector}{item}\n"

        if os.path.isdir(path):
            extension = "    " if is_last else "│   "
            tree_str += generate_directory_tree(path, prefix + extension)

    return tree_str


def main():
    print("Bundling Flow Engine context...")

    total_files_read = 0
    total_lines_read = 0
    code_content = ""

    # Phase 1: Walk the directory and extract code
    for root, dirs, files in os.walk('.'):

        # Modify the dirs list in-place so os.walk skips excluded directories entirely
        dirs[:] = [d for d in dirs if d not in DIRECTORY_EXCLUSIONS]

        for file in files:
            if file.endswith(TARGET_EXTENSIONS) and file != OUTPUT_FILE:
                filepath = os.path.join(root, file)

                # Normalize path slashes for consistency
                display_path = filepath.replace('\\', '/')

                code_content += f"\n{'/' * 75}\n"
                code_content += f"// FILE: {display_path}\n"
                code_content += f"{'/' * 75}\n\n"

                try:
                    with open(filepath, 'r', encoding='utf-8') as infile:
                        lines = infile.readlines()
                        total_lines_read += len(lines)
                        code_content += "".join(lines) + "\n"
                        total_files_read += 1
                except Exception as e:
                    code_content += f"// ERROR reading file: {e}\n"

    # Phase 2: Generate the pruned project hierarchy
    print("Generating optimized project hierarchy...")
    tree_content = ".\n" + generate_directory_tree('.')

    # Phase 3: Write everything to the output file with Metadata
    with open(OUTPUT_FILE, 'w', encoding='utf-8') as outfile:
        # 1. Metadata Header
        outfile.write("=======================================================================\n")
        outfile.write("                  PROJECT CONTEXT & METADATA OVERVIEW                  \n")
        outfile.write("=======================================================================\n")
        outfile.write(f"Generated On : {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
        outfile.write(f"Files Parsed : {total_files_read}\n")
        outfile.write(f"Total Lines  : {total_lines_read}\n")
        outfile.write("=======================================================================\n\n")

        # 2. Source Code
        outfile.write(code_content)

        # 3. Hierarchy Footer
        outfile.write(f"\n{'=' * 75}\n")
        outfile.write("PRUNED PROJECT HIERARCHY (ENGINE CORE ONLY)\n")
        outfile.write(f"{'=' * 75}\n\n")
        outfile.write(tree_content)

    print(f"Success! {total_files_read} files ({total_lines_read} lines) packed into: {OUTPUT_FILE}")


if __name__ == "__main__":
    main()