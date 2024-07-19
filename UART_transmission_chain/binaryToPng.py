def binary_string_to_file(binary_file):
    # Read the binary string from the input file
    with open(binary_file, 'r') as f:
        binary_string = f.read()
    
    # Convert the binary string to bytes
    byte_array = bytearray()
    for i in range(0, len(binary_string), 8):
        byte = binary_string[i:i+8]
        byte_array.append(int(byte, 2))
    
    # Write the bytes to the output file
    with open('output.png', 'wb') as f:
        f.write(byte_array)

binary_file = input("Binary file: ")
binary_string_to_file(binary_file=binary_file)
