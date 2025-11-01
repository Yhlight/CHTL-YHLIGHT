import subprocess
import sys
import os
import shutil

def run_command(command):
    """Runs a command and exits if it fails."""
    try:
        subprocess.run(command, check=True, shell=True)
    except subprocess.CalledProcessError as e:
        print(f"Command '{e.cmd}' failed with exit code {e.returncode}")
        sys.exit(1)

def configure():
    """Configures the project with CMake."""
    print("--- Configuring project ---")
    os.makedirs("build", exist_ok=True)
    run_command("cmake -B build -S .")

def build():
    """Builds the project."""
    print("--- Building project ---")
    if not os.path.exists("build"):
        configure()
    run_command("cmake --build build")

def test():
    """Runs the tests."""
    print("--- Running tests ---")
    if not os.path.exists("build/run_tests"):
        build()
    # Use CTest to run tests
    run_command("cd build && ctest --output-on-failure")

def run():
    """Runs the main executable."""
    print("--- Running CHTL Compiler ---")
    executable_path = "build/chtl" # Placeholder name
    if not os.path.exists(executable_path):
        build()

    # Pass arguments to the executable
    args = sys.argv[2:]
    run_command(f"{executable_path} {' '.join(args)}")

def clean():
    """Cleans the build directory."""
    print("--- Cleaning build directory ---")
    if os.path.exists("build"):
        shutil.rmtree("build")

def all():
    """Cleans, configures, builds, and tests the project."""
    clean()
    configure()
    build()
    test()

def main():
    if len(sys.argv) < 2:
        print("Usage: python build.py <command>")
        print("Commands: configure, build, test, run, clean, all")
        sys.exit(1)

    command = sys.argv[1]

    if command == "configure":
        configure()
    elif command == "build":
        build()
    elif command == "test":
        test()
    elif command == "run":
        run()
    elif command == "clean":
        clean()
    elif command == "all":
        all()
    else:
        print(f"Unknown command: {command}")
        sys.exit(1)

if __name__ == "__main__":
    main()
