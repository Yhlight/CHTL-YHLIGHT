#!/usr/bin/env python3
import os
import subprocess
import sys

def run_command(args, a_cwd='.'):
    """Executes a command and exits if it fails."""
    print(f"Executing: {' '.join(args)}")
    result = subprocess.run(args, cwd=a_cwd)
    if result.returncode != 0:
        print(f"Error: Command failed with exit code {result.returncode}")
        sys.exit(result.returncode)

def configure():
    """Configures the project using CMake."""
    os.makedirs('build', exist_ok=True)
    run_command(['cmake', '-S', '.', '-B', 'build'])

def build():
    """Builds the project using Make."""
    run_command(['make', '-C', 'build'])

def test():
    """Runs the tests."""
    run_command(['./runTests'], a_cwd='build/Test')

def clean():
    """Cleans the build directory."""
    if os.path.exists('build'):
        run_command(['rm', '-rf', 'build'])
    print("Build directory cleaned.")

def main():
    if len(sys.argv) == 1:
        print("Usage: ./build.py [configure|build|test|clean|all]")
        sys.exit(1)

    command = sys.argv[1]

    if command == 'configure':
        configure()
    elif command == 'build':
        build()
    elif command == 'test':
        test()
    elif command == 'clean':
        clean()
    elif command == 'all':
        configure()
        build()
        test()
    else:
        print(f"Unknown command: {command}")
        sys.exit(1)

if __name__ == "__main__":
    main()
