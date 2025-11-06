import os
import subprocess
import sys

def run_command(command, cwd):
    """Runs a command and exits if it fails."""
    try:
        subprocess.run(command, check=True, cwd=cwd, shell=True)
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {' '.join(command)}")
        print(f"Return code: {e.returncode}")
        sys.exit(1)

def main():
    """Configures, builds, and tests the CHTL project."""
    build_dir = "build"

    # Create build directory if it doesn't exist
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    # Configure the project
    print("--- Configuring CMake... ---")
    run_command("cmake ..", cwd=build_dir)

    # Build the project
    print("\n--- Building project... ---")
    run_command("cmake --build .", cwd=build_dir)

    # Run the tests
    print("\n--- Running tests... ---")
    run_command("ctest --output-on-failure", cwd=build_dir)

if __name__ == "__main__":
    main()
