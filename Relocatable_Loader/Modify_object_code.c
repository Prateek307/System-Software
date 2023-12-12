#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
// Function to convert the hexadecimal into decimal
int hexaToDec(char hexVal[])
{
    int len = strlen(hexVal), base = 1, dec_val = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        if (hexVal[i] >= '0' && hexVal[i] <= '9')
        {
            dec_val += (int)(hexVal[i] - '0') * base;
            base = base * 16;
        }
        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
        {
            dec_val += (int)(hexVal[i] - 'A' + 10) * base;
            base = base * 16;
        }
    }
    return dec_val;
}

// Function to convert decimal to hexadecimal
char *decToHexa(int n, char hexa[])
{
    int i = 0;
    while (n != 0)
    {
        int rem = 0;
        rem = n % 16;
        if (rem < 10)
        {
            hexa[i++] = rem + '0';
        }
        else
        {
            hexa[i++] = rem + 'A' - 10;
        }
        n = n / 16;
    }
    hexa[i] = '\0';
    int j = 0, k = i - 1;
    while (j < k)
    {
        char temp = hexa[j];
        hexa[j] = hexa[k];
        hexa[k] = temp;
        j++;
        k--;
    }
    return hexa;
}

int main()
{
    FILE *fp1, *fp2;
    fp1 = fopen("ObjectCode.txt", "r");
    fp2 = fopen("MyObjectCode.txt", "w");
    char line[1000];
    char a[1000];
    printf("Enter address\n");
    scanf("%s",a);
    while (fgets(line, sizeof(line), fp1) != NULL)
    {
        char text[1000];
        text[0] = '\0';
        strcat(text, line);
        int text_size = strlen(text);


        // for header record
        if(text[0]=='H')
        {
            int r = 0;
            char extra[1000];
            extra[0] = '\0';
            int i = 7;
            while(text[i]!='^')
            { 
                extra[r++] = text[i];
                i++;
            }
            extra[r] = '\0';
            int vval1 = hexaToDec(extra);
            int vval2 = hexaToDec(a);
            vval1+=vval2;
            char convert[1000];
            convert[0] = '\0';
            strcpy(convert,decToHexa(vval1,convert));
            text[7] = '0';
            text[8] = '0';
            int z = 9;
            i = 0;
             int convert_len = strlen(convert);
            while(i<convert_len)
            { 
                text[z++] = convert[i];
                i++;
            }
        }








        // for end record
        if(text[0]=='E')
        {
            int r = 0;
            char extra[1000];
            extra[0] = '\0';
            int i = 2;
            while(i<text_size)
            { 
                extra[r++] = text[i];
                i++;
            }
            extra[r] = '\0';
            int vval1 = hexaToDec(extra);
            int vval2 = hexaToDec(a);
            vval1+=vval2;
            char convert[1000];
            convert[0] = '\0';
            strcpy(convert,decToHexa(vval1,convert));
            text[0] = '\0';
            text[0] = 'E';
            text[1] = '^';
            text[2] = '0';
            text[3] = '0';
            int z = 4;
            i = 0;
             int convert_len = strlen(convert);
            while(i<convert_len)
            { 
                text[z++] = convert[i];
                i++;
            }
        }


        // for text record
        if(text[0]=='T')
        {
            int i = 0;
            while(i<text_size)
            { 
                if(text[i]=='^')
                {
                    i++;
                    int started_at = i;
                    started_at+=3;
                    int k = 0;
                    char str[1000];
                    str[0] = '\0';
                    while(text[i]!='^' && text[i]!='\0')
                    { 
                        str[k++] = text[i];                          
                        i++;
                    }
                    str[k]='\0';
                    i--;
                    if(strlen(str)==8)
                    {
                        // we need to modify
                        int len = strlen(str);
                        char strr[1000];
                        int p = 0;
                        int t = 3;
                        while(t<len)
                        { 
                            strr[p++] = str[t]; 
                            t++;
                        }
                        strr[p] = '\0';
                        strcpy(str,strr);
                        int temp1 = hexaToDec(str);
                        int temp2 = hexaToDec(a);
                        temp1+=temp2;
                        char modified_str[1000],ans[1000];
                        modified_str[0] = '\0';
                        strcpy(modified_str,decToHexa(temp1,ans));


                        // I got my modified string
                        int tr = started_at, q = 0;
                        len = strlen(modified_str);
                        if(strlen(modified_str)==4)
                        {
                            text[tr++] = '0';
                        }
                        while(q<len)
                        {
                            text[tr] = modified_str[q];
                            tr++;
                            q++;
                        }
                        // printf("%s\n",text);
                    }
                }
                i++;
            }
            fprintf(fp2, "%s", text);
        }
        else
        fprintf(fp2, "%s", text);
        line[0]='\0';
    }
    return 0;
}
