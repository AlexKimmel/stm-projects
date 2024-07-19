def file_to_binary_string(file_path):
    with open(file_path, 'rb') as f:
        file_content = f.read()
    return ''.join(format(byte, '08b') for byte in file_content)

file_path = input("Image: ")  
binary_string = file_to_binary_string(file_path)

# Write the bytes to the output file
with open("input.txt", "w+") as f:
    f.write(binary_string)

f.close()