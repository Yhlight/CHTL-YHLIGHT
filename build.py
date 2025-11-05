import os
import shutil
import subprocess
import argparse

def configure():
    """Configures the project."""
    print("Configuring the project...")
    os.makedirs("build", exist_ok=True)
    subprocess.run(["cmake", "-S", ".", "-B", "build"], check=True)

def build():
    """Builds the project."""
    print("Building the project...")
    subprocess.run(["cmake", "--build", "build"], check=True)

def test():
    """Runs the tests."""
    print("Running tests...")
    subprocess.run(["ctest", "--test-dir", "build"], check=True)

def clean():
    """Cleans the build directory."""
    print("Cleaning the build directory...")
    if os.path.exists("build"):
        shutil.rmtree("build")

def main():
    parser = argparse.ArgumentParser(description="Build script for the CHTL project.")
    parser.add_argument("command", choices=["configure", "build", "test", "clean", "all"], help="The command to execute.")

    args = parser.parse_args()

    if args.command == "configure":
        configure()
    elif args.command == "build":
        build()
    elif args.command == "test":
        test()
    elif args.command == "clean":
        clean()
    elif args.command == "all":
        configure()
        build()
        test()

if __name__ == "__main__":
    main()
