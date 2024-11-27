def remove_duplicates(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
    unique_lines = set(lines)
    with open(filename, 'w') as file:
        file.writelines(unique_lines)

# Call the function with the filename
remove_duplicates('images.qrc')
