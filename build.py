import os
import shutil
import subprocess
import argparse

class Config:
    build_dir = "build"
    build_type = "Debug"

def configure():
    os.makedirs(Config.build_dir, exist_ok=True)
    try:
        subprocess.run(
            ["cmake", "-S", ".", "-B", Config.build_dir, f"-DCMAKE_BUILD_TYPE={Config.build_type}"],
            check=True
        )
        print("Configuration complete.")
    except subprocess.CalledProcessError as e:
        print(f"Error during configuration: {e}")
        exit(1)

def build():
    try:
        subprocess.run(
            ["cmake", "--build", Config.build_dir],
            check=True
        )
        print("Build complete.")
    except subprocess.CalledProcessError as e:
        print(f"Error during build: {e}")
        exit(1)

def test():
    try:
        subprocess.run(
            ["ctest", "--test-dir", Config.build_dir, "--output-on-failure"],
            check=True
        )
        print("Tests passed.")
    except subprocess.CalledProcessError as e:
        print(f"Tests failed: {e}")
        exit(1)

def clean():
    if os.path.exists(Config.build_dir):
        shutil.rmtree(Config.build_dir)
        print("Build directory cleaned.")
    else:
        print("Build directory does not exist.")

def run_all():
    configure()
    build()
    test()

def main():
    parser = argparse.ArgumentParser(description="CHTL Build Script")
    parser.add_argument("command", choices=["configure", "build", "test", "clean", "all"], help="The command to execute")
    parser.add_argument("--build-type", default="Debug", choices=["Debug", "Release"], help="The build type")

    args = parser.parse_args()
    Config.build_type = args.build_type

    if args.command == "configure":
        configure()
    elif args.command == "build":
        build()
    elif args.command == "test":
        test()
    elif args.command == "clean":
        clean()
    elif args.command == "all":
        run_all()

if __name__ == "__main__":
    main()
