#!/bin/bash

rm tmp/c_compiler_internal-stdout-sterr.txt

if [[ "$1" != "" ]] ; then
    compiler="$1"
else
    compiler="bin/c_compiler"
fi

have_compiler=0
if [[ ! -f bin/c_compiler ]] ; then
    >&2 echo "Warning : cannot find compiler at path ${compiler}. Only checking C reference against python reference."
    have_compiler=1
fi

input_dir="test_internal"

working="tmp/internal"
mkdir -p ${working}

for i in ${input_dir}/*.c ; do
    base=$(echo $i | sed -E -e "s|${input_dir}/([^.]+)[.]c|\1|g");
    
    # Compile the reference C version
    mips-linux-gnu-gcc -ansi -static $i -o $working/$base
    
    # Run the reference C version
    qemu-mips $working/$base
    REF_C_OUT=$?

    
    if [[ ${have_compiler} -eq 0 ]] ; then
        
        # Create the assembly
        $compiler -S $i -o ${working}/$base-got.s
		mips-linux-gnu-gcc -static -o ${working}/$base-got ${working}/$base-got.s &>> tmp/c_compiler_internal-stdout-sterr.txt
        
        # Run the new
        qemu-mips ${working}/$base-got
        GOT_C_OUT=$?
    fi
    
    if [[ ${have_compiler} -ne 0 ]] ; then
        echo "$base, Fail, No C compiler/translator"
    elif [[ $REF_C_OUT -ne $GOT_C_OUT ]] ; then
        echo "$base, Fail, Expected ${REF_C_OUT}, got ${GOT_C_OUT}"
    else
        echo "$base, Pass"
    fi
done
