import pytest
import subprocess
import os
import signal
import time
import sys


SERVICE_PATH = os.path.abspath(
    os.path.join(
        os.path.dirname(__file__),
        f"../build/build-{os.environ["BUILD_TYPE"]}/kvstorage_service".lower()
    )
)

CONFIG_PATH = os.path.abspath(
    os.path.join(
        os.path.dirname(__file__),
        "test_data/test_config.json"
    )
)

def run_tests():
    print("run tests")
    return pytest.main(["-v", "tests/integration"])


def run_service():
    env = os.environ.copy()
    env["LLVM_PROFILE_FILE"] = "coverage-%p.profraw"

    proc = subprocess.Popen(
        [SERVICE_PATH, "--config", CONFIG_PATH],
        env=env,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        preexec_fn=os.setsid 
    )

    return proc

def stop_process(proc):
    os.killpg(os.getpgid(proc.pid), signal.SIGTERM)
    proc.wait()


def main():
    service_proc = run_service()
    time.sleep(1)
    try:
        test_result = run_tests()
    except Exception as e:
        print(f"An error occurred while running tests: {e}")
        test_result = 1
    finally:
        stop_process(service_proc)
    
    sys.exit(test_result)


if __name__ == "__main__":
    main()
