import subprocess
import random
# # Ejecuta el binario y espera a que termine
# # capture_output=True captura STDOUT y STDERR
# # text=True devuelve cadenas de texto en lugar de bytes
# result = subprocess.run(
#     ["./BoolTests", "--nnf-only", "AB="],
#     capture_output=True,
#     text=True
# )
#
# # Resultado en result.stdout, errores en result.stderr
# salida = result.stdout
# print("Salida del programa:")
# print(salida)

def create_cases():
    cases = [[52888, 21468], [10448, 51167], [31997, 56207], [6936, 45453], [45743, 57372], [4317, 63705], [35980, 46829], [25772, 39154], [62387, 43670], [28521, 63583], [27909, 20801], [23188, 51324], [32061, 46512], [23188, 38500], [19611, 56841], [12039, 50546], [9843, 42807], [9250, 8014], [19089, 41017], [34073, 46894], [28753, 48773], [60264, 10736], [44252, 33060], [8749, 38982], [10264, 40593], [53625, 64794], [5416, 27620], [1070, 32894], [32428, 4103], [61734, 61816], [62612, 46101], [5540, 10274], [20515, 36778], [61790, 46272], [28268, 3245], [30838, 18134], [48281, 306], [56629, 55486], [38761, 45049], [39619, 50793], [43555, 14026], [34535, 47036], [3933, 46447], [33688, 48789], [2013, 15307], [25006, 18800], [54741, 38578], [46377, 61035], [19308, 46661], [44700, 18], [53071, 44375], [31443, 8414], [19742, 64883], [5974, 25563], [9333, 47909]]

    all = ""
    for case in cases:
        x = case[0]
        y = case[1]
        code = normalize_morton(x, y)
        convert = subprocess.run(
            ["./BoolTests", f"--map-reverse "],
            capture_output=True,
            text=True
        )
    for i in range(0, 55):
        x = random.randint(0, 65535)
        y = random.randint(0, 65535)
        code = normalize_morton(x, y)
        mapped = subprocess.run(
            ["./BoolTests", f"--map-and-reverse {code:.17f}"],
            capture_output=True,
            text=True
        )
        cases.append([x,y])
        output = f"""
case>>> Case "{x} - {y}"
bin>>>./BoolTests
input>>>--map-and-reverse {x} {y}
expected>>>
Input X = {x} Y = {y} Mapped: {code:.17f} X' = {x} Y' = {y} consistency OK
<<<expected
<<<case
"""
        all += output
    print(all);
    print("-----")
    print(cases)
    # for case in cases:
    #     convert = subprocess.run(
    #         ["./BoolTests", "--cnf", case],
    #         capture_output=True,
    #         text=True
    #     )
    #     table = subprocess.run(
    #         ["./BoolTests", "--truth-table", case],
    #         capture_output=True,
    #         text=True
    #     )
    #     nnf = subprocess.run(
    #         ["./BoolTests", "--nnf-only", case],
    #         capture_output=True,
    #         text=True
    #     )
    #     ccf = subprocess.run(
    #         ["./BoolTests", "--cnf-only", case],
    #         capture_output=True,
    #         text=True
    #     )
#         output = f"""
# case>>> Case "{case}"
# bin>>>./BoolTests
# input>>>--cnf "{case}"
# expected>>>
# original: "{case}"
# {table.stdout.rstrip('\n')}
# NNF: "{nnf.stdout.rstrip('\n')}"
# {table.stdout.rstrip('\n')}
# CNF: "{ccf.stdout.rstrip('\n')}"
# {table.stdout.rstrip('\n')}
# <<<expected
# <<<case
# """
#         all += output
#     print(all);

def morton_interleave(x, y):
    def part1by1(n):
        n &= 0x0000FFFF  # asegurar 16 bits
        n = (n | (n << 8)) & 0x00FF00FF
        n = (n | (n << 4)) & 0x0F0F0F0F
        n = (n | (n << 2)) & 0x33333333
        n = (n | (n << 1)) & 0x55555555
        return n
    return (part1by1(y) << 1) | part1by1(x)

def normalize_morton(x, y):
    max_code = 0xFFFFFFFF  # máximo valor para 32 bits
    code = morton_interleave(x, y)
    return code / max_code



if __name__ == "__main__":
    # x = random.randint(0, 65535)
    # y = random.randint(0, 65535)
    create_cases()
    # # m = morton.Morton(dimensions=2, bits=32)
    # code = normalize_morton(x, y)       # Codifica (13, 42) a Morton code
    # # x, y = m.unpack(code)       # Decodifica el Morton code a (x, y)
    # print(f"Input X = {x} Y = {y} -> {code:.17f}")
    # print(f"--map {x} {y}")