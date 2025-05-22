import string


def is_valid_hex(s: str) -> bool:
    return len(s) == 16 and all(c in string.hexdigits for c in s)
