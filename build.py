import os
import subprocess
import sys
import shutil

def run_command(args):
    """Runs a command and prints its output."""
    print(f"Executing: {' '.join(args)}")
    subprocess.run(args, check=True)

def configure():
    """Configures the project with CMake."""
    os.makedirs("build", exist_ok=True)
    run_command(["cmake", "-S", ".", "-B", "build"])

def build():
    """Builds the project with CMake."""
    run_command(["cmake", "--build", "build"])

def test():
    """Runs the tests."""
    run_command(["ctest", "--test-dir", "build"])

def clean():
    """Cleans the build directory."""
    if os.path.exists("build"):
        shutil.rmtree("build")
        print("Removed build directory.")

def all():
    """Configures, builds, and tests the project."""
    configure()
    build()
    test()

if __name__ == "__main__":
    if len(sys.argv) > 1:
        command = sys.argv[1]
        if command == "configure":
            configure()
        elif command == "build":
            build()
        elif command == "test":
            test()
        elif command == "clean":
            clean()
        elif command == "all":
            all()
        else:
            print(f"Unknown command: {command}")
            sys.exit(1)
    else:
        all()
