import os
import subprocess

def run_command(command):
    """Runs a command and prints its output."""
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in iter(process.stdout.readline, b''):
        print(line.decode('utf-8').strip())
    process.stdout.close()
    return_code = process.wait()
    if return_code:
        raise subprocess.CalledProcessError(return_code, command)

def main():
    """Builds the CHTL project."""
    build_dir = "build"
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    os.chdir(build_dir)
    run_command("cmake ../CHTL")
    run_command("cmake --build .")
    os.chdir("..")

if __name__ == "__main__":
    main()
