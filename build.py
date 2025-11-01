import os
import subprocess
import sys
import platform

def run_command(command, cwd):
    """Runs a command in a specified directory and checks for errors."""
    print(f"Running command: {' '.join(command)} in {cwd}")
    result = subprocess.run(command, cwd=cwd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error: Command failed with exit code {result.returncode}")
        print(f"Stderr: {result.stderr}")
        print(f"Stdout: {result.stdout}")
        sys.exit(1)
    print(result.stdout)
    return result.stdout

def find_and_run_executables(build_dir):
    """Finds and runs the compiled executables."""
    print("\\n--- Running Executables ---")
    executable_extension = ".exe" if platform.system() == "Windows" else ""

    # Paths to the executables
    chtl_executable = os.path.join(build_dir, "CHTL", f"chtl{executable_extension}")
    chtljs_executable = os.path.join(build_dir, "CHTLJS", f"chtljs{executable_extension}")

    executables_to_run = [chtl_executable, chtljs_executable]

    for exe_path in executables_to_run:
        if os.path.exists(exe_path):
            print(f"\\nExecuting: {exe_path}")
            run_command([exe_path], cwd=os.path.dirname(exe_path))
        else:
            print(f"Warning: Executable not found at {exe_path}")

def main():
    """Main function to configure and build the project."""
    build_dir = "build"

    # Create build directory if it doesn't exist
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    # Configure the project with CMake
    cmake_configure_command = ["cmake", ".."]
    run_command(cmake_configure_command, os.path.abspath(build_dir))

    # Build the project
    cmake_build_command = ["cmake", "--build", "."]
    run_command(cmake_build_command, os.path.abspath(build_dir))

    print("\\nBuild completed successfully!")

    # Find and run the compiled executables
    find_and_run_executables(os.path.abspath(build_dir))

if __name__ == "__main__":
    main()
