import os
import re

def convert_print_line(line):
    # Recherche une ligne indentée avec print sans parenthèses
    match = re.match(r'^(\s*)print\s+(.*)$', line)
    if match:
        indent, content = match.groups()
        content = content.strip()
        # Si ce n'est pas déjà une parenthèse, on convertit
        if not (content.startswith('(') and content.endswith(')')):
            return f"{indent}print({content})\n"
    return line

def main():
    print("Conversion basique des prints Python 2 → Python 3")

    for dirpath, _, filenames in os.walk('.'):
        for filename in filenames:
            if filename.endswith('.py'):
                filepath = os.path.join(dirpath, filename)
                print(f"Processing {filepath}...")
                with open(filepath, 'r', encoding='utf-8') as f:
                    lines = f.readlines()

                new_lines = [convert_print_line(line) for line in lines]

                if new_lines != lines:
                    with open(filepath, 'w', encoding='utf-8') as f:
                        f.writelines(new_lines)
                    print(f"Updated print statements in {filepath}")

    print("Conversion terminée.")

if __name__ == "__main__":
    main()
