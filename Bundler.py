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

Updated Features: 27-06-26
Features:
1. Intelligent Minification: Strips comments and unnecessary whitespace to
   maximize token density for AI reasoning.
2. API-First Manifest: Generates a high-level class and method hierarchy
   summary at the start of the output to prime the AI's understanding.
3. Dependency-Aware Sorting: Ensures header files (.h) are processed before
   implementation files (.cpp) to maintain logical dependency flow.
4. Token-Efficient Metadata: Replaces heavy ASCII borders with structured,
   low-token-overhead markers.
5. Optimized Scoping: Automatically prunes heavy third-party directories
   (e.g., vendor, _deps) while preserving project-specific architecture.
===============================================================================
"""
import os
import re
from datetime import datetime

TARGET_EXTENSIONS = ('.h', '.cpp', 'CMakeLists.txt', '.ini')
OUTPUT_FILE = 'FlowContext.txt'
DIRECTORY_EXCLUSIONS = {'.git', '.vs', '.cache', 'build', 'out', 'bin', '_deps', 'vendor', 'ThirdParty', '.idea', '.py'}


def minify_text(text):
    text = re.sub(r'//.*', '', text)
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.DOTALL)
    lines = [line.strip() for line in text.splitlines()]
    collapsed = []
    for line in lines:
        if line or (collapsed and collapsed[-1]):
            collapsed.append(line)
    return "\n".join(collapsed).strip()


def get_manifest(root_dir):
    manifest = "PROJECT MANIFEST (CLASS/METHOD HIERARCHY):\n"
    for root, _, files in os.walk(root_dir):
        if any(ex in root for ex in DIRECTORY_EXCLUSIONS): continue
        for file in files:
            if file.endswith('.h'):
                try:
                    with open(os.path.join(root, file), 'r', encoding='utf-8') as f:
                        content = f.read()
                        classes = re.findall(r'class\s+(\w+)', content)
                        for cls in classes:
                            manifest += f"- Class: {cls}\n"
                except:
                    continue
    return manifest


def generate_directory_tree(dir_path, prefix=""):
    """Recursive tree generation for project visualization."""
    tree_str = ""
    try:
        items = [i for i in os.listdir(dir_path) if i not in DIRECTORY_EXCLUSIONS and i != OUTPUT_FILE]
        items.sort(key=lambda x: (not os.path.isdir(os.path.join(dir_path, x)), x))
    except PermissionError:
        return ""

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
    code_content = []
    manifest = get_manifest('.')
    total_files = 0
    total_lines = 0

    for root, dirs, files in os.walk('.'):
        dirs[:] = [d for d in dirs if d not in DIRECTORY_EXCLUSIONS]
        for file in sorted(files, key=lambda x: not x.endswith('.h')):
            if file.endswith(TARGET_EXTENSIONS) and file != OUTPUT_FILE:
                path = os.path.join(root, file).replace('\\', '/')
                with open(path, 'r', encoding='utf-8') as f:
                    content = minify_text(f.read())
                    if not content: continue
                    code_content.append(f"\nFILE: {path}\n{'-' * 30}\n{content}")
                    total_files += 1
                    total_lines += len(content.splitlines())

    with open(OUTPUT_FILE, 'w', encoding='utf-8') as outfile:
        outfile.write(f"DATE: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
        outfile.write(f"FILES: {total_files} | LINES: {total_lines}\n\n")
        outfile.write(manifest + "\n\n")
        outfile.write("\n".join(code_content))

        # ATTACHING THE RECURSIVE TREE HERE
        outfile.write("\n\n" + "=" * 30 + "\nPROJECT HIERARCHY:\n")
        outfile.write(".\n" + generate_directory_tree('.'))

    print(f"Bundled {total_files} files into {OUTPUT_FILE}")


if __name__ == "__main__":
    main()
