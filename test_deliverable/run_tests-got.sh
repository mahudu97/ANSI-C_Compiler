rm bin/*
rm asm/*
rm test_results.txt

mkdir bin
mkdir asm

echo IF_T >> test_results.txt
./c_compiler -S test_cases/IF_T.c -o asm/IF_T.s
mips-linux-gnu-gcc -ansi -static -o bin/IF_T asm/IF_T.s test_cases/IF_T_driver.c
qemu-mips bin/IF_T
echo $? >> test_results.txt

echo IF_F >> test_results.txt
./c_compiler -S test_cases/IF_F.c -o asm/IF_F.s
mips-linux-gnu-gcc -ansi -static -o bin/IF_F asm/IF_F.s test_cases/IF_F_driver.c
qemu-mips bin/IF_F
echo $? >> test_results.txt

echo IF_ELSE_T >> test_results.txt
./c_compiler -S test_cases/IF_ELSE_T.c -o asm/IF_ELSE_T.s
mips-linux-gnu-gcc -ansi -static -o bin/IF_ELSE_T asm/IF_ELSE_T.s test_cases/IF_ELSE_T_driver.c
qemu-mips bin/IF_ELSE_T
echo $? >> test_results.txt

echo IF_ELSE_F >> test_results.txt
./c_compiler -S test_cases/IF_ELSE_F.c -o asm/IF_ELSE_F.s
mips-linux-gnu-gcc -ansi -static -o bin/IF_ELSE_F asm/IF_ELSE_F.s test_cases/IF_ELSE_F_driver.c
qemu-mips bin/IF_ELSE_F
echo $? >> test_results.txt

echo FOR_N >> test_results.txt
./c_compiler -S test_cases/FOR_N.c -o asm/FOR_N.s
mips-linux-gnu-gcc -ansi -static -o bin/FOR_N asm/FOR_N.s test_cases/FOR_N_driver.c
qemu-mips bin/FOR_N
echo $? >> test_results.txt

echo LOCAL_ARRAY >> test_results.txt
./c_compiler -S test_cases/LOCAL_ARRAY.c -o asm/LOCAL_ARRAY.s
mips-linux-gnu-gcc -ansi -static -o bin/LOCAL_ARRAY asm/LOCAL_ARRAY.s test_cases/LOCAL_ARRAY_driver.c
qemu-mips bin/LOCAL_ARRAY
echo $? >> test_results.txt

echo SELF_RECURSION >> test_results.txt
./c_compiler -S test_cases/SELF_RECURSION.c -o asm/SELF_RECURSION.s
mips-linux-gnu-gcc -ansi -static -o bin/SELF_RECURSION asm/SELF_RECURSION.s test_cases/SELF_RECURSION_driver.c
qemu-mips bin/SELF_RECURSION
echo $? >> test_results.txt

echo GLOBAL >> test_results.txt
./c_compiler -S test_cases/GLOBAL.c -o asm/GLOBAL.s
mips-linux-gnu-gcc -ansi -static -o bin/GLOBAL asm/GLOBAL.s test_cases/GLOBAL_driver.c
qemu-mips bin/GLOBAL
echo $? >> test_results.txt

echo MAIN >> test_results.txt
./c_compiler -S test_cases/MAIN.c -o asm/MAIN.s
mips-linux-gnu-gcc -ansi -static -o bin/MAIN asm/MAIN.s test_cases/MAIN_driver.c
qemu-mips bin/MAIN
echo $? >> test_results.txt

echo ATOF >> test_results.txt
./c_compiler -S test_cases/ATOF.c -o asm/ATOF.s
mips-linux-gnu-gcc -ansi -static -o bin/ATOF asm/ATOF.s test_cases/ATOF_driver.c
qemu-mips bin/ATOF
echo $? >> test_results.txt