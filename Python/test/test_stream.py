import subprocess


if __name__ == "__main__":
    proc = subprocess.Popen("../../C++/bin/ADXL357_Logger",
    stdin=subprocess.PIPE,stdout=subprocess.PIPE)