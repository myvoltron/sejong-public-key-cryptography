from typing import Final, List, Tuple

PC1: Final[List[int]]= [
    57,49,41,33,25,17,9,
    1,58,50,42,34,26,18,
    10,2,59,51,43,35,27,
    19,11,3,60,52,44,36,
    63,55,47,39,31,23,15,
    7,62,54,46,38,30,22,
    14,6,61,53,45,37,29,
    21,13,5,28,20,12,4
]

PC2: Final[List[int]]= [
    14,17,11,24,1,5,3,28,
    15,6,21,10,23,19,12,4,
    26,8,16,7,27,20,13,2,
    41,52,31,37,47,55,30,40,
    51,45,33,48,44,49,39,56,
    34,53,46,42,50,36,29,32
]

SHIFT_SCHEDULE: Final[List[int]]= [
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
]

# Initial Permutation (IP) Table
IP: Final[List[int]]= [
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
]

# Final Permutation (IP^-1) Table
FP: Final[List[int]] = [
    40,8,48,16,56,24,64,32,
    39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30,
    37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28,
    35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26,
    33,1,41,9,49,17,57,25
]

# === Bit Helpers ===

def hex_to_bitlist(hex_str: str) -> List[int]:
    return [int(bit) for hex_char in hex_str for bit in f"{int(hex_char, 16):04b}"]

def bitlist_to_str(bits: List[int]) -> str:
    return ''.join(str(b) for b in bits)

def bitlist_to_hex(bits: List[int]) -> str:
    return ''.join(f"{int(''.join(map(str, bits[i:i+4])), 2):X}" for i in range(0, len(bits), 4))

# === Permutation and Shift ===

def apply_permutation(input_bits: List[int], table: List[int]) -> List[int]:
    return [input_bits[pos - 1] for pos in table]

def left_shift(bits: List[int], shift_count: int) -> List[int]:
    return bits[shift_count:] + bits[:shift_count]

# === Key Scheduling ===

def generate_Cn_Dn(key_64bit_hex: str) -> Tuple[List[List[int]], List[List[int]]]:
    key_bits = hex_to_bitlist(key_64bit_hex)
    permuted_key = apply_permutation(key_bits, PC1)
    C = [permuted_key[:28]]
    D = [permuted_key[28:]]

    for i in range(16):
        C.append(left_shift(C[-1], SHIFT_SCHEDULE[i]))
        D.append(left_shift(D[-1], SHIFT_SCHEDULE[i]))

    return C, D

def generate_round_keys(C: List[List[int]], D: List[List[int]]) -> List[List[int]]:
    round_keys = []
    for i in range(1, 17):
        CD = C[i] + D[i]
        Ki = apply_permutation(CD, PC2)
        round_keys.append(Ki)
    return round_keys

# === Initial Permutation (IP) ===

def apply_initial_permutation(message_64bit_hex: str) -> Tuple[List[int], List[int]]:
    message_bits = hex_to_bitlist(message_64bit_hex)
    permuted = apply_permutation(message_bits, IP)
    L0 = permuted[:32]
    R0 = permuted[32:]
    return L0, R0

# === Final Permutation (IP^-1) ===

def apply_final_permutation(L16: List[int], R16: List[int]) -> str:
    preoutput = R16 + L16  # note the swap before FP
    ciphertext_bits = apply_permutation(preoutput, FP)
    return bitlist_to_hex(ciphertext_bits)

__all__ = [
    "generate_Cn_Dn", "generate_round_keys", "hex_to_bitlist", "bitlist_to_str", "bitlist_to_hex",
    "apply_initial_permutation", "apply_final_permutation"
]

