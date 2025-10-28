import argparse
import subprocess
import os
import sys

def configure():
    """Configures the project using CMake."""
    print("--- Configuring CMake ---")
    os.makedirs("build", exist_ok=True)
    result = subprocess.run(["cmake", "-S", ".", "-B", "build"], check=False)
    if result.returncode != 0:
        print("CMake configuration failed.")
        sys.exit(1)
    print("--- CMake configuration successful ---")

def build():
    """Builds the project using CMake."""
    print("--- Building project ---")
    result = subprocess.run(["cmake", "--build", "build"], check=False)
    if result.returncode != 0:
        print("Build failed.")
        sys.exit(1)
    print("--- Build successful ---")

def run():
    """Runs the main executable."""
    print("--- Running CHTL executable ---")
    executable_path = os.path.join("build", "chtl")
    if not os.path.exists(executable_path):
        # Handle Windows executable extension
        if sys.platform == "win32" and os.path.exists(executable_path + ".exe"):
            executable_path += ".exe"
        else:
            print(f"Executable not found at {executable_path}. Please build the project first.")
            sys.exit(1)

    result = subprocess.run([executable_path], check=False)
    if result.returncode != 0:
        print("Executable exited with an error.")
        sys.exit(result.returncode)

def test():
    """Runs the tests using CTest."""
    print("--- Running tests ---")
    # CTest is the standard way to run tests in a CMake project.
    # It needs to be run from the build directory.
    if not os.path.exists("build"):
        print("Build directory not found. Please configure and build the project first.")
        sys.exit(1)

    result = subprocess.run(["ctest", "--output-on-failure"], cwd="build", check=False)
    if result.returncode != 0:
        print("Tests failed.")
        sys.exit(result.returncode)
    print("--- Tests passed ---")

def main():
    parser = argparse.ArgumentParser(description="CHTL Build Script")
    parser.add_argument("command", choices=["configure", "build", "run", "test", "all"],
                        help="The command to execute.")

    args = parser.parse_args()

    if args.command == "configure":
        configure()
    elif args.command == "build":
        # Ensure configuration is done before building
        if not os.path.exists("build/CMakeCache.txt"):
            configure()
        build()
    elif args.command == "run":
        run()
    elif args.command == "test":
        test()
    elif args.command == "all":
        configure()
        build()
        test()
        run()

if __name__ == "__main__":
    main()
