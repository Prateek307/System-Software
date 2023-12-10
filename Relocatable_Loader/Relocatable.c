#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

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
    char a[100];
    printf("\nRead the address available in operating system :\n");
    scanf("%s", a);
    if (a[0] == '+')
    {
        int l = 0; // Start at index 0
        while (a[l] != '\0' && a[l + 1] != '\0')
        {
            a[l] = a[l + 1];
            l++;
        }
        a[l] = '\0';

        printf("%s\n", a);
        FILE *fp1, *fp2;
        fp1 = fopen("MyObjectCode.txt", "r");
        fp2 = fopen("NewObjectCode.txt", "w");
        char line[1000];
        printf("\nMemory address");
        printf("        Contents");
        printf("\n---------------------------------------------\n");
        while (fgets(line, sizeof(line), fp1) != NULL)
        {
            char text[1000];
            text[0] = '\0';
            strcat(text, line);
            // In string text firt text record in read
            if (text[0] == 'T')
            {
                // we enter inside the loop only if it is a text record
                char modified_address[1000], temp[1000], hexa[1000];
                int i = 2, k = 0;
                temp[k] = text[i];
                k++;
                while (text[i] != '^')
                {
                    temp[k] = text[i];
                    i++;
                    k++;
                }
                temp[k] = '\0';
                int val = hexaToDec(temp) + hexaToDec(a);
                decToHexa(val, hexa);
                strcpy(modified_address, hexa);
                printf("%s\t", modified_address);

                int mx = 4, cnt_for_space = 0, cnt_for_new_line = 1;
                i = 12;
                while (text[i + 1] != '\0')
                {
                    if (text[i] != '^' && cnt_for_space < 8)
                    {
                        printf("%c", text[i]);
                        cnt_for_space++;
                    }
                    else if (text[i] != '^' && cnt_for_space >= 8)
                    {
                        if (cnt_for_new_line < mx)
                        {
                            printf(" %c", text[i]);
                            cnt_for_space = 1;
                            cnt_for_new_line++;
                        }
                        else
                        {
                            printf("\n");
                            cnt_for_new_line = 1;
                            cnt_for_space = 0;
                            hexa[0] = '\0';
                            val = hexaToDec(modified_address) + 16;
                            decToHexa(val, hexa);
                            strcpy(modified_address, hexa);
                            printf("%s\t", modified_address);
                            i--;
                        }
                    }
                    i++;
                }
                // printf("cnt for new line = %d\n",cnt_for_new_line);
                while (cnt_for_new_line <= mx)
                {
                    while (cnt_for_space < 8)
                    {
                        printf("X");
                        cnt_for_space++;
                    }
                    printf(" ");
                    cnt_for_new_line++;
                    cnt_for_space = 0;
                }
                printf("\n.\n");
                printf(".\n");
            }
            line[0] = '\0';
        }

        fclose(fp1);
        fp1 = fopen("MyObjectCode.txt", "r");
        printf("\nObject Program with realocation by Modification records\n\n");
        line[0] = '\0';
        while (fgets(line, sizeof(line), fp1) != NULL)
        {
            char record[1000];
            record[0] = '\0';
            strcat(record, line);
            if (record[0] == 'M')
            {
                char temp[100];
                strcpy(temp, record);
                int cntr = strlen(temp) - 1;
                int i = 0;
                temp[cntr++] = '+';
                while (a[i] != '\0')
                {
                    temp[cntr++] = a[i];
                    i++;
                }
                temp[cntr++] = '\0';
                printf("%s\n", temp);
            }
            else
                printf("%s", record);
        }
        printf("\n\n");
    }

    else // a[0] == '-'
    {
        int l = 0; // Start at index 0
        while (a[l] != '\0' && a[l + 1] != '\0')
        {
            a[l] = a[l + 1];
            l++;
        }
        a[l] = '\0';
        FILE *fp1, *fp2;
        fp1 = fopen("ObjectCode.txt", "r");
        fp2 = fopen("NewObjectCode.txt", "w");
        char line[1000];
        printf("\nMemory address");
        printf("        Contents");
        printf("\n---------------------------------------------\n");
        while (fgets(line, sizeof(line), fp1) != NULL)
        {
            char text[1000];
            text[0] = '\0';
            strcat(text, line);
            // In string text firt text record in read
            if (text[0] == 'T')
            {
                // we enter inside the loop only if it is a text record
                char modified_address[1000], temp[1000], hexa[1000];
                int i = 2, k = 0;
                temp[k] = text[i];
                k++;
                while (text[i] != '^')
                {
                    temp[k] = text[i];
                    i++;
                    k++;
                }
                temp[k] = '\0';
                int val = hexaToDec(temp) + hexaToDec(a);
                decToHexa(val, hexa);
                strcpy(modified_address, hexa);
                printf("-%s\t", modified_address);

                int mx = 4, cnt_for_space = 0, cnt_for_new_line = 1;
                i = 12;
                while (text[i + 1] != '\0')
                {
                    if (text[i] != '^' && cnt_for_space < 8)
                    {
                        printf("%c", text[i]);
                        cnt_for_space++;
                    }
                    else if (text[i] != '^' && cnt_for_space >= 8)
                    {
                        if (cnt_for_new_line < mx)
                        {
                            printf(" %c", text[i]);
                            cnt_for_space = 1;
                            cnt_for_new_line++;
                        }
                        else
                        {
                            printf("\n");
                            cnt_for_new_line = 1;
                            cnt_for_space = 0;
                            hexa[0] = '\0';
                            val = hexaToDec(modified_address) + 16;
                            decToHexa(val, hexa);
                            strcpy(modified_address, hexa);
                            printf("-%s\t", modified_address);
                            i--;
                        }
                    }
                    i++;
                }
                // printf("cnt for new line = %d\n",cnt_for_new_line);
                while (cnt_for_new_line <= mx)
                {
                    while (cnt_for_space < 8)
                    {
                        printf("X");
                        cnt_for_space++;
                    }
                    printf(" ");
                    cnt_for_new_line++;
                    cnt_for_space = 0;
                }
                printf("\n.\n");
                printf(".\n");
            }
            line[0] = '\0';
        }

        fclose(fp1);
        fp1 = fopen("ObjectCode.txt", "r");
        printf("\nObject Program with realocation by Modification records\n\n");
        line[0] = '\0';
        while (fgets(line, sizeof(line), fp1) != NULL)
        {
            char record[1000];
            record[0] = '\0';
            strcat(record, line);
            if (record[0] == 'M')
            {
                char temp[100];
                strcpy(temp, record);
                int cntr = strlen(temp) - 1;
                int i = 0;
                temp[cntr++] = '-';
                while (a[i] != '\0')
                {
                    temp[cntr++] = a[i];
                    i++;
                }
                temp[cntr++] = '\0';
                printf("%s\n", temp);
            }
            else
                printf("%s", record);
        }
        printf("\n\n");
    }
    return 0;
}