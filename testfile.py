import os, subprocess


def factorial_test(output):
    expected = [1, 2, 4, 5, 10, 20, 25, 50, 100]
    output =  map(int,output)
    
    if output == expected:
        print "Test for factorial:\t PASSED"
    else:
        print "Test for factorial:\t FAILED"

def lmn_test(output):
    output = map(int, output)
    expected = [12]

    if output == expected:
        print "Test for lmn:\t\t PASSED"
    else:
        print "Test for lmn:\t\t FAILED"

def even_test(output):
    output = map(int, output)
    expected = [20, 18, 16, 14, 12, 10, 8, 6, 4, 2]

    if output == expected:
        print "Test for even:\t\t PASSED"
    else:
        print "Test for even:\t\t FAILED"

def loop_test(output):
    output = map(int, output)
    expected = [1, 1 ,1, 1, 1]

    if output ==expected:
        print "Test for loop:\t\t PASSED"
    else:
        print "Test for loop:\t\t FAILED"
        

if __name__ == "__main__":
    files = [f for f in os.listdir("testcodes") if f.endswith(".pyc")]
    for f in files:
        try:
            each_program = subprocess.Popen(['./pyvm', 'testcodes/'+f], stdout = subprocess.PIPE )
            file_name = f.split(".")
            program_output = each_program.stdout.readlines()
            program_output = program_output[3:]
            if(file_name[0] == "fac"):
                factorial_test(program_output)
            elif(file_name[0] == "lmn"):
                lmn_test(program_output)
            elif(file_name[0] == "even"):
                even_test(program_output)
            elif(file_name[0] == "loop"):
                loop_test(program_output)
        except:
            print "Error in opening "
