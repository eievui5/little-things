import sys
import time

# Create source string.
const_text = ""
# Check for command-line arguments.
if len(sys.argv) > 1:
    for i in range(1, len(sys.argv)):
        const_text += sys.argv[i] + " "
else:
    # Otherwise, just ask the user for an input string.
    const_text = input("Enter a string: ")

# Fix user string...
if not len(const_text):
    const_text = "Hello World!"
if len(const_text) < 2:
    print("Error: Input string must be at least 2 characters.")
    sys.exit()

# Display string as a gradient/wave.
string = const_text
while True:
    while len(string) > 1:
        string = string[0:len(string) - 1]
        time.sleep(0.05)
        print(string)
    while len(string) < len(const_text):
        string = const_text[0:len(string) + 1]
        time.sleep(0.05)
        print(string)
