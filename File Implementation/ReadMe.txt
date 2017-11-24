1)
To run the 3 files
$make -f clean.mk
$make -f "-Dt=t" makefile.mk
$./a.out

2)
To change key value or other changes,change in client file.
"asgn3_client.c"

3)
To change t value,change during compilation.
eg: 
	$make -f "-Dt=5 -Dsize=200000" makefile.mk
	$make -f "-Dt=50 -Dsize=200000" makefile.mk
	
4) 
Please run the clean.mk if you choose to change t value.
(Previous files may not get overwritten,otherwise)  

Thank you.