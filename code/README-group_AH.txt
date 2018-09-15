IF you want to run our codes on Mac, follow Method 1, 
IF you want to run our codes on Windows, follow Method 2.


Method 1: 

The software we use: Mac Terminal

The language we use: C/C++




Structure of our code:

We put our five algorithms code: five .h files and a main.cpp file into “src” folder. 
And there is a main excutable file that can be used to run. 

Also, there is a “data” folder and an “output” folder in the same path, before running, you should put all the .tsp files into the “data” folder. After running, you can find the output files from the “output” folder.




Algorithm Name for our project:



Branch and bound: bnb 

MST-approximation: mst 

Nearest neighbor: nn
2 exchange 
local search1: 2EXrevise

local search2: SA






How to compile the program: 




under src folder:

using command to compile and generate excutable file main: make main



We have already generate the excutable file main in src folder, it can be directly used to run. 

Excutable file is the "main" file under src folder. 



How to run our code: 

In the command line, four arguments are needed. They are filename, algorithm method, cut-off time and seed sequentially.  





The input format is: 

./main <cityname> <bnb|mst|2EXrevise|SA|nn> <cutofftime> <seed>



Note: the cityname should not include the ".tsp", you only need to input the name of the city; and the cutofftime and seed should be an integer; the cutofftime also should be an integer which represents seconds.

for example: 

./main Atlanta 10 4




Output:

Two output files will be generated. 

1.<cityname>_<algorithm>_<cutofftime>_<seed>.sol
	
   Eg. Atlanta_bnb_10_3.sol


2.<cityname>_<algorithm>_<cutofftime>_<seed>.trace
	
   Eg. Atlanta_bnb_10_3.trace


For bnb, mst, and nn, the output files does not contain the randSeed variable.


All of our output files are generated under the output folder under the output folder.



Method 2: 

If you want to run our program on windows environment, you should use the Eclipse IDE to run it. You cannot use Vistual Stdio, because our code combines C and C++ language. To do so, you need to set up Eclipse CDT on windows follow these steps: 

1. If you don't have a Java Runtime Environment (JRE) on your computer, install it. You will need a Java virtual machine (JVM) to run Eclipse. You can download a JRE/JDK installer from: http://www.oracle.com/technetwork/java/javase/downloads/index.html. Either would work. You may also need to add the path of the bin folder within your JRE folder to the system's PATH variable. (Control Panel\System and Security\System\System Properties\Advanced\System Variables\PATH.)

2. Download MingGW 64-bit from:  https://sourceforge.net/projects/mingw-w64/?source=recommended, and install it on your laptop. Attention: if you are running a 64-bit OS, change the Archeature from i686 to x86_64, otherwise, stick with the default i686.

3. Find the bin folder within the MinGW folder created after the installation, copy the path (something like C:\...\MinGW\bin) and paste it to the system environment variables as in Step 1. 

4. Download Eclipse for C/C++: https://eclipse.org/downloads/, and install it.

5. After setting up the Eclipse, you can just make a new C++ project,however, please do not "import" our project into Eclipse. To make the run correctly, you need to make a new C++ project, and then copy our code into this new C++ project.

6. After successfully copy our all codes into the project, you need to choose the "Build Project" choice,and then click the "Run" botton to run. In order to input the arguments, you need to click by follows: 
Run -> Run configurations -> choose this project -> arguments, and then input the arguments as mentioned above. 

7. Note: in order to run successfully in Eclipse, before you running, you also need to create a new folder, names "data", and put all the .tsp data files into the "data" folder. You also need to create a new folder, names "output" to hold the results of our running. You also need to notice that, the "data" and "output" folders should be in the same level of the "src" folder. 






