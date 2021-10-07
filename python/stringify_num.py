import sys

# Number lookup tables.

_number_digits: list = [
    "zero", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine"
]

_number_prefixes: list = [
    "twen", "thir", "four", "fif", "six", "seven", "eigh", "nine"
]

_hundred_suffixes: list = [
    "thousand", "million", "billion", "trillion", "quadrillion",
    "quintillion", "sextillion", "septillion", "octillion", "nonillion",
    "undecillion", "duodecillion", "tredecillion", "quattuordecillion", "quindecillion",
    "sexdecillion", "septendecillion", "octodecillion", "novemdecillion", "vigintillion"
]

# Convert a digit to 0 through 9.
def string_digit(digit: int) -> str:
    if digit >= 0 and digit <= 9:
        return _number_digits[digit]
    else:
        raise Exception(str(digit) + " is out of digit range (0-9)")
    
# Convert a teen to 10 through 19.
def _string_teen(teen: int) -> str:
    if teen == 0: return "ten"
    elif teen == 1: return "eleven"
    elif teen == 2: return "twelve"
    elif teen >= 3 and teen <= 9:
        return _number_prefixes[teen - 2] + "teen"
    else:
        raise Exception(str(teen) + " is out of teen range (10-19)")

# Convert a ten to 20 through 90.
def _string_ten(ten: int) -> str:
    if ten == 0:
        return ""
    if ten >= 2 and ten <= 9:
        return _number_prefixes[ten - 2] + "ty"
    else:
        raise Exception(str(ten) + " is out of tens range (20-90)")

def _stringify_hundred(number: int, insert_and: bool) -> str:
    if number >= 1000:
        raise Exception("number is greater than 3 digits!")

    result: str = ""
    # For an exact hundreds value, discard the previous number
    if number >= 100 and number % 100 == 0:
        result = string_digit(number // 100 % 10) + " hundred"
    else: 
        # Check for the teen range and handle it specially.
        if number % 100 < 20 and number % 100 > 9:
            result = _string_teen(number % 10)
        else:
            # Otherwise construct the first two digits.
            if number % 100 < 10:
                result = string_digit(number % 10)
            else:
                result = _string_ten(number // 10 % 10)
                if number % 10 != 0:
                    result += " " + string_digit(number % 10)
        # Now check hundreds
        if number >= 100:
            result = string_digit(number // 100 % 10) + " hundred " + \
                ("and " if insert_and else "") + result
    return result

def stringify_number(number: int, insert_and: bool = False) -> str:
    if number >= 10**64:
        raise Exception("Maximum number is 999 vigintillion (63 digits)")
    
    negative: bool = number < 0
    number = abs(number)
    result: str = _stringify_hundred(number % 1000, insert_and)

    factor: int = 1000
    factor_index: int = 0
    while number >= factor:
        if number % factor == 0:
            result = ""
        result = _stringify_hundred(number // factor % 1000, insert_and) + \
            " " + _hundred_suffixes[factor_index] + \
            ((" " + result) if result else "")
        factor *= 1000
        factor_index += 1

    if negative:
        result = "negative " + result
    return result

def __main__():
    try:
        if len(sys.argv) > 1:
            input_number: int = int(sys.argv[1])
        else:
            input_number: int = int(input("Enter a number: "))
    except:
        print("Invalid Input! Please enter a single number without any commas.")
        sys.exit()

    print("Number " + str(input_number) + " is " + stringify_number(input_number))

if __name__ == "__main__":
    __main__()