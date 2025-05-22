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
