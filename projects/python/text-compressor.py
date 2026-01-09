import math

def main():
    choice = input("Enter 'c' to compress or 'd' to decompress: ").lower()
    if choice == 'c':
        compression_algorithm()
    elif choice == 'd':
        decompression_algorithm()
    else:
        print("Invalid choice. Please enter 'c' or 'd'.")

def compression_algorithm():
    """get data , start with first word , write is at it is and assign it a hexnumber , got to next and do the same until the same word comes and right the hex code"""
    file = input("Enter location name to compress: ")
    with open(file, "r") as f:
        data = f.read()
#    example = "Hello , this is me , grey , and i am known as grey because i am greyhat in nature and am a part of cybersecurity"
    new_data = ""
    dict_word = {}
    for word in data.split(" "):
        if len(word) < 4:
            new_data += word.lower() + " "
        else:
            key = word
            num = len(dict_word)+1
            dict_word[key] = num
            new_data += str(num) + " "
    with open("compressed.txt", "w") as f:
        f.write(dict_word.__str__() + "\n" + new_data)

def decompression_algorithm():
    """get data , read the dictionary from the first line , then read the rest of the data and replace the hex codes with the words from the dictionary"""
    file = input("Enter location name to decompress: ")
    with open(file, "r") as f:
        data = f.readlines()
    dict_word = eval(data[0].strip())
    reverse_dict = {v: k for k, v in dict_word.items()}
    new_data = ""
    for word in data[1].split(" "):
        if word in reverse_dict:
            new_data += reverse_dict[word] + " "
        else:
            new_data += word + " "
    with open("decompressed.txt", "w") as f:
        f.write(new_data)

main()