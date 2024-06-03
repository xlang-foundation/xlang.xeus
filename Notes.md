1、compile project.
2、modify the path of xlang_kernel.exe in "xlang_kernel/kernel.json" to the output file.
3、copy "xlang_kernel" folder to jupyter (C:\ProgramData\jupyter\kernels).
4、launch jupyterlab.
5、create a new notebook using xlang_kernel.
6、in the first cell, input "xlang_connect(127.0.0.1:3142)" and click run button to connect to a xlang server.
   empty arg to run a local xlang server automatically and connect to it, need to copy xlang's exe and dll to the folder where xlang_kernel.exe in.
   connectting result will display in the output cell
7、input xlang code in the next input cell and click run button
   xlang_kernel will run these code line by line and display return value and print value in the output cell



