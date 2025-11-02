import os
import subprocess

def run_command(command):
    """Runs a command and checks for errors."""
    try:
        subprocess.run(command, check=True, shell=True)
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {e}")
        exit(1)

def main():
    """Builds and tests the CHTL project."""
    # Create build directory
    if not os.path.exists("build"):
        os.makedirs("build")

    # Configure CMake
    run_command("cmake -B build -S .")

    # Build the project
    run_command("cmake --build build")

    # Run tests
    run_command(os.path.join("build", "Test", "runTests"))

if __name__ == "__main__":
    main()
