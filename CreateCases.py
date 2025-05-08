import subprocess

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
    cases = ["AB|!A|B|A|B|A|B|", "A!!!!!", "AB|!", "AB>B&A&B=", "AB&!",
             "AB>", "AB>B&A&B=!", "AB|C&!!", "A!B|C&!", "AB^C&D=", "AB^C&D=!",
             "AB^C&D>", "AB^B&BB=>!A=A|!B^B&BB=>!A=A|!!AB|B&&B|A|AB|B&&B|A|!!", "F!G!^H!&I!=",
             "AA^", "AA^!", "A!A^", "AA^A>", "AB=C>D=AB=C>D=!&", "AB^CD^=AB^CD^=!&",
             "F!G^H&I=", "F!G^H&I=!", "AC^B&CB==!", "AC^B&CB=>", "AC^B&CB>=", "AC^B&CB=&",
             "AB^!AB^&", "AB^!AB^&", "AB=X=", "AB^C>AB^C>!&", "AB^C&D=!", "AB=C&D|!", "AB^!AB^&!",
             "AB^!AB^&!!", "AB^!AB^&!AA&&", "AB^!A&B^&!", "AB^C>AB^C>!&!"]
    all = ""
    for case in cases:
        convert = subprocess.run(
            ["./BoolTests", "--cnf", case],
            capture_output=True,
            text=True
        )
        table = subprocess.run(
            ["./BoolTests", "--truth-table", case],
            capture_output=True,
            text=True
        )
        nnf = subprocess.run(
            ["./BoolTests", "--nnf-only", case],
            capture_output=True,
            text=True
        )
        ccf = subprocess.run(
            ["./BoolTests", "--cnf-only", case],
            capture_output=True,
            text=True
        )
        output = f"""
case>>> Case "{case}"
bin>>>./BoolTests
input>>>--cnf "{case}"
expected>>>
original: "{case}"
{table.stdout.rstrip('\n')}
NNF: "{nnf.stdout.rstrip('\n')}"
{table.stdout.rstrip('\n')}
CNF: "{ccf.stdout.rstrip('\n')}"
{table.stdout.rstrip('\n')}
<<<expected
<<<case
"""
        all += output
    print(all);

if __name__ == "__main__":
    create_cases();