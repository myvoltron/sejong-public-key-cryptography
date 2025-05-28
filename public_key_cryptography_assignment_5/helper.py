import string
from typing import List


def is_valid_hex(s: str) -> bool:
    return len(s) == 16 and all(c in string.hexdigits for c in s)


# === Bit Helpers ===


def hex_to_bitlist(hex_str: str) -> List[int]:
    return [int(bit) for hex_char in hex_str for bit in f"{int(hex_char, 16):04b}"]


def bitlist_to_str(bits: List[int]) -> str:
    return "".join(str(b) for b in bits)


def bitlist_to_hex(bits: List[int]) -> str:
    return "".join(
        f"{int(''.join(map(str, bits[i : i + 4])), 2):X}"
        for i in range(0, len(bits), 4)
    )


def bitlist_to_decimal(bits: List[int]) -> int:
    return int("".join(map(str, bits)), 2)


def xor_for_bitlists(a: List[int], b: List[int]) -> List[int]:
    return [x ^ y for x, y in zip(a, b)]


def word_to_hex_string(word: List[int]) -> str:
    """
    Convert a list of 4 integers (a word) to a space-separated hex string.
    Example: [1, 2, 255, 16] -> "01 02 FF 10"
    """
    return " ".join(f"{b:02X}" for b in word)


def hex_string_to_int_list(hex_str: str) -> List[int]:
    assert len(hex_str) % 2 == 0, "Hex string length must be even."
    return [int(hex_str[i : i + 2], 16) for i in range(0, len(hex_str), 2)]


def hex_list_to_string(hex_list: List[int]) -> str:
    return "".join(f"{byte:02x}" for byte in hex_list)
