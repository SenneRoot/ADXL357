import subprocess

proc = subprocess.Popen("../../C++/bin/ADXL357_Logger",
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE)

proc.stdin.write('l')
message = proc.stdout.read(1)
print ("return message -> " + message + " written by python \n")
proc.stdin.write('q')
proc.wait()