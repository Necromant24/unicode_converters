

## How to use

1) compile and run `readandnormalize.c` with command `gcc randomchar.c -o sourceBinary; ./sourceBinary | tee data.txt` 
2) check the correct encoding with `iconv -f UTF-32 data.txt -o /dev/null; echo $? `  



