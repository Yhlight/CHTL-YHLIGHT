import os
import subprocess

def run_command(command):
    """Runs a command and prints its output."""
    print(f"Executing: {' '.join(command)}")
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in iter(process.stdout.readline, b''):
        print(line.decode('utf-8'), end='')
    process.stdout.close()
    return_code = process.wait()
    if return_code:
        raise subprocess.CalledProcessError(return_code, command)

def main():
    """Builds the project and runs the tests."""
    if not os.path.exists("build"):
        os.makedirs("build")

    os.chdir("build")

    try:
        run_command(["cmake", ".."])
        run_command(["make"])
        run_command(["./tests/run_tests"])
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
        exit(1)

if __name__ == "__main__":
    main()
