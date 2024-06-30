

## How to use

1) compile and run `readAndNormalize.c` with command `gcc readAndNormalize.c -o readAndNormalize; ./readAndNormalize | tee data.txt` 
2) check the correct encoding with `iconv -f UTF-32 data.txt -o /dev/null; echo $? `  



