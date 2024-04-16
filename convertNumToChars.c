
 
// Driver Code
int main()
{

    char *str = (char *)malloc(4 * sizeof(char));

    long n = 0x0010fffd;

    str = &n;

    printf("%c%c%c%c", &str[0], &str[1], &str[2], &str[3]);

    
 
 
    return 0;
}