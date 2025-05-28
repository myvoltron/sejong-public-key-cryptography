# advanced_encryption_data.py

from typing import Dict, List, Tuple

from helper import word_to_hex_string
from advanced_encryption_data_constants import INV_SBOX, RCON, SBOX


def substitution_word(word: List[int]) -> List[int]:
    result = []
    for byte in word:
        row = byte // 16  # 상위 4비트 (hex의 첫 자리)
        col = byte % 16  # 하위 4비트 (hex의 두 번째 자리)
        substituted_byte = SBOX[row][col]
        result.append(substituted_byte)
    return result


def rotate_word(word: List[int]) -> List[int]:
    return word[1:] + word[:1]


def xor_words(a: List[int], b: List[int]) -> List[int]:
    return [x ^ y for x, y in zip(a, b)]


def key_expansion(key: List[int]) -> Tuple[List[List[int]], List[str]]:
    w = []
    log = []

    for i in range(4):
        word = [key[i * 4 + j] for j in range(4)]
        w.append(word)
        log.append(f"W{i} = {word_to_hex_string(word)}")

    for i in range(4, 44):
        previous_word = w[i - 1]
        if i % 4 == 0:
            rot_word = rotate_word(previous_word)
            sub_word = substitution_word(rot_word)
            rcon = [RCON[i // 4 - 1], 0x00, 0x00, 0x00]
            temp_log = (
                f"W{i} = W{i - 4} ⊕ SubWord(RotWord(W{i - 1})) ⊕ Rcon\n"
                f"  RotWord(W{i - 1}) = {word_to_hex_string(rot_word)}\n"
                f"  SubWord(...) = {word_to_hex_string(sub_word)}\n"
                f"  Rcon = {word_to_hex_string(rcon)}"
            )
            previous_word = xor_words(sub_word, rcon)
        else:
            temp_log = f"W{i} = W{i - 4} ⊕ W{i - 1}"

        new_word = xor_words(w[i - 4], previous_word)
        w.append(new_word)
        log.append(temp_log + f"\n  → W{i} = {word_to_hex_string(new_word)}")

    return w, log


def get_state_array(
    hex_values: List[int], round_key: List[List[int]]
) -> List[List[int]]:
    """
    Convert 16-byte hex array into a 4x4 AES state matrix (column-major order),
    then apply XOR with the given round_key (which should be W0~W3 combined).
    """
    assert len(hex_values) == 16, "Input hex_values must be 16 bytes."
    assert len(round_key) == 4 and all(len(word) == 4 for word in round_key), (
        "round_key must be 4x4."
    )

    # Make state matrix from hex_values (column-major)
    state = [[0] * 4 for _ in range(4)]
    for i in range(16):
        row = i % 4
        col = i // 4
        state[row][col] = hex_values[i]

    # Apply XOR with initial round key (W0 || W1 || W2 || W3)
    for col in range(4):
        for row in range(4):
            state[row][col] ^= round_key[col][row]

    return state


def add_round_key(
    state: List[List[int]], round_key: List[List[int]]
) -> List[List[int]]:
    return [
        [state[row][col] ^ round_key[row][col] for col in range(4)] for row in range(4)
    ]


def sub_bytes(state: List[List[int]]) -> List[List[int]]:
    return [[SBOX[byte >> 4][byte & 0x0F] for byte in row] for row in state]


def shift_rows(state: List[List[int]]) -> List[List[int]]:
    return [[state[row][(col + row) % 4] for col in range(4)] for row in range(4)]


def xtime(a: int) -> int:
    return ((a << 1) ^ 0x1B) & 0xFF if a & 0x80 else (a << 1)


def mix_single_column(col: List[int]) -> List[int]:
    a = col
    b = [xtime(x) for x in a]
    return [
        b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1],
        b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2],
        b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3],
        b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0],
    ]


def mix_columns(state: List[List[int]]) -> List[List[int]]:
    columns = list(zip(*state))
    mixed = [mix_single_column(list(col)) for col in columns]
    return [list(row) for row in zip(*mixed)]


def state_to_hex_string(state: List[List[int]]) -> str:
    return " ".join(f"{byte:02x}" for col in zip(*state) for byte in col)


def aes_encrypt(text: List[int], words: List[List[int]]) -> Tuple[List[Dict], str]:
    """
    AES encryption simulation with logging of all rounds.
    Takes precomputed state_array (with AddRoundKey already applied) and expanded key words.
    Returns a tuple of (rounds_data, ciphertext_hex).
    """
    # Convert round keys into 4x4 matrix form
    round_keys = [
        [[words[round * 4 + col][row] for col in range(4)] for row in range(4)]
        for round in range(11)
    ]

    rounds_data = []

    state = [[0] * 4 for _ in range(4)]
    for i in range(16):
        row = i % 4
        col = i // 4
        state[row][col] = text[i]

    state = add_round_key(state, round_keys[0])

    for rnd in range(1, 11):
        round_log = {
            "state_in": state_to_hex_string(state),
            "subkey": state_to_hex_string(round_keys[rnd]),
        }

        state = sub_bytes(state)
        round_log["byte_substitution"] = state_to_hex_string(state)
        state = shift_rows(state)
        round_log["shift_rows"] = state_to_hex_string(state)

        round_log["mix_columns"] = "10 rounds skip mix column step!!"
        if rnd != 10:
            state = mix_columns(state)
            round_log["mix_columns"] = state_to_hex_string(state)

        state = add_round_key(state, round_keys[rnd])
        round_log["round_result"] = state_to_hex_string(state)
        rounds_data.append(round_log)

    # Flatten final state to ciphertext hex string
    ciphertext = "".join(
        f"{state[row][col]:02x}" for col in range(4) for row in range(4)
    )
    return rounds_data, ciphertext


def inv_sub_bytes(state: List[List[int]]) -> List[List[int]]:
    return [[INV_SBOX[byte >> 4][byte & 0x0F] for byte in row] for row in state]


def inv_shift_rows(state: List[List[int]]) -> List[List[int]]:
    return [
        [state[0][0], state[0][1], state[0][2], state[0][3]],
        [state[1][3], state[1][0], state[1][1], state[1][2]],
        [state[2][2], state[2][3], state[2][0], state[2][1]],
        [state[3][1], state[3][2], state[3][3], state[3][0]],
    ]


def gmul(a: int, b: int) -> int:
    result = 0
    for _ in range(8):
        if b & 1:
            result ^= a
        high_bit = a & 0x80
        a = (a << 1) & 0xFF
        if high_bit:
            a ^= 0x1B
        b >>= 1
    return result


def inv_mix_single_column(col: List[int]) -> List[int]:
    return [
        gmul(col[0], 14) ^ gmul(col[1], 11) ^ gmul(col[2], 13) ^ gmul(col[3], 9),
        gmul(col[0], 9) ^ gmul(col[1], 14) ^ gmul(col[2], 11) ^ gmul(col[3], 13),
        gmul(col[0], 13) ^ gmul(col[1], 9) ^ gmul(col[2], 14) ^ gmul(col[3], 11),
        gmul(col[0], 11) ^ gmul(col[1], 13) ^ gmul(col[2], 9) ^ gmul(col[3], 14),
    ]


def inv_mix_columns(state: List[List[int]]) -> List[List[int]]:
    columns = list(zip(*state))
    mixed = [inv_mix_single_column(list(col)) for col in columns]
    return [list(row) for row in zip(*mixed)]


def aes_decrypt(text: List[int], words: List[List[int]]) -> Tuple[List[Dict], str]:
    """
    AES decryption simulation using state matrix and expanded key words.
    Returns a tuple of (rounds_data, plaintext_hex).
    """

    round_keys = [
        [[words[round * 4 + col][row] for col in range(4)] for row in range(4)]
        for round in range(11)
    ]

    rounds_data = []

    state = [[0] * 4 for _ in range(4)]
    for i in range(16):
        row = i % 4
        col = i // 4
        state[row][col] = text[i]
    state = add_round_key(state, round_keys[-1])
    state = inv_shift_rows(state)
    state = inv_sub_bytes(state)

    for rnd in range(9, 0, -1):
        round_log = {
            "state_in": state_to_hex_string(state),
            "subkey": state_to_hex_string(round_keys[rnd]),
        }

        state = add_round_key(state, round_keys[rnd])
        round_log["round_result"] = state_to_hex_string(state)

        state = inv_mix_columns(state)
        round_log["mix_columns"] = state_to_hex_string(state)

        state = inv_shift_rows(state)
        round_log["shift_rows"] = state_to_hex_string(state)

        state = inv_sub_bytes(state)
        round_log["byte_substitution"] = state_to_hex_string(state)

        rounds_data.append(round_log)

    round_log = {
        "state_in": state_to_hex_string(state),
        "subkey": state_to_hex_string(round_keys[0]),
    }
    state = add_round_key(state, round_keys[0])
    round_log["round_result"] = state_to_hex_string(state)
    round_log["mix_columns"] = "null"
    round_log["shift_rows"] = "null"
    round_log["byte_substitution"] = "null"
    rounds_data.append(round_log)

    # convert final state matrix into flat list of bytes
    plaintext_bytes = [state[row][col] for col in range(4) for row in range(4)]
    plaintext_str = "".join(chr(b) for b in plaintext_bytes)

    return rounds_data, plaintext_str  # return human-readable plaintext
