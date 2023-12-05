/*

The following pass2.c code takes the input from two files first one is the intermediate_code.txt ,
symbol_table.txt and optab.txt 
pass2.c generates two output files assmlist.txt and object_code.txt

*/



#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

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
void intToUppercaseHex(int value, char *result, int size)
{
    snprintf(result, size, "%X", value);
}
void main()
{
    char a[10], ad[10], label[10], opcode[10], operand[10], symbol[10], ch;
    int st, diff, i, len, actual_len, j = 0;
    char prevaddr[100], address[100], finaddr[100], add[100], starting_adress[100];
    // char mnemonic[15][15] = {"LDA", "STA", "LDCH", "STCH"};
    // char code[15][15] = {"33", "44", "53", "57"};
    char mnemonic[15][15], code[15][15];
    FILE *fp1, *fp2, *fp3, *fp4, *fp5;
    fp1 = fopen("assmlist.txt", "w");
    fp2 = fopen("symbol_table.txt", "r");
    fp3 = fopen("intermediate_code.txt", "r");
    fp4 = fopen("object_code.txt", "w");
    fp5 = fopen("optab.txt", "r");

    char val1[10], val2[10];
    int cnt = 0;
    while (fscanf(fp5, "%s%s", val1, val2) != EOF)
    {
        if (strcmp(val1, "START") != 0)
        {
            strcpy(mnemonic[cnt], val1);
            strcpy(code[cnt], val2);
            cnt++;
        }
    }
    rewind(fp5);
    fscanf(fp3, "%s%s%s", label, opcode, operand);
    strcpy(starting_adress, operand);

    while (strcmp(opcode, "END") != 0)
    {
        strcpy(prevaddr, address);
        fscanf(fp3, "%s%s%s%s", address, label, opcode, operand);
    }
    strcpy(finaddr, address);

    fclose(fp3);
    fp3 = fopen("intermediate_code.txt", "r");

    fscanf(fp3, "%s%s%s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0)
    {
        fprintf(fp1, "\t%s\t%s\t%s\n", label, opcode, operand);
        char temp[100];
        decToHexa(hexaToDec(finaddr) - hexaToDec(operand), temp);
        fprintf(fp4, "H^%s^00%s^0000%s\n", label, operand, temp);
        fscanf(fp3, "%s%s%s%s", address, label, opcode, operand);
        fprintf(fp4, "T^00%s^%s", address, decToHexa(hexaToDec(prevaddr) - hexaToDec(address), temp));
    }
    // right now at 2000    first     lda    five
    //              address label   opcode   operand
    while (strcmp(opcode, "END") != 0)
    {
        if (strcmp(opcode, "BYTE") == 0)
        {
            fprintf(fp1, "%s\t%s\t%s\t%s\t", address, label, opcode, operand);
            len = strlen(operand);
            actual_len = len - 3;
            fprintf(fp4, "^");
            for (i = 2; i < (actual_len + 2); i++)
            {
                // itoa(operand[i], ad, 16);
                intToUppercaseHex(operand[i], ad, sizeof(ad));
                fprintf(fp1, "%s", ad);
                fprintf(fp4, "%s", ad);
            }
            fprintf(fp1, "\n");
        }
        else if (strcmp(opcode, "WORD") == 0)
        {
            len = strlen(operand);
            itoa(atoi(operand), a, 10);
            fprintf(fp1, "%s\t%s\t%s\t%s\t00000%s\n", address, label, opcode, operand, a);
            fprintf(fp4, "^00000%s", a);
        }
        else if ((strcmp(opcode, "RESB") == 0) || (strcmp(opcode, "RESW") == 0))
            fprintf(fp1, "%s\t%s\t%s\t%s\n", address, label, opcode, operand);
        else
        {
            j = 0; // added by myself
            while (strcmp(opcode, mnemonic[j]) != 0 && mnemonic[j] != "END")
                j++;
            if (strcmp(operand, "COPY") == 0)
                fprintf(fp1, "%s\t%s\t%s\t%s\t%s0000\n", address, label, opcode, operand, code[j]);
            else
            {
                if ((strcmp(opcode, "ADDR")==0)||(strcmp(opcode, "TIXR")==0)||(strcmp(opcode, "COMPR")==0))
                {
                    char tempo[100], ans[100];
                    strcpy(tempo, operand);
                    len = strlen(tempo);
                    int k = 0;
                    ans[0] = '\0';
                    char var[100], adrval[100];
                    rewind(fp5);
                    while (fscanf(fp5, "%s%s", adrval, var) != EOF)
                    {
                        if ((strcmp(adrval, "ADDR")==0)||(strcmp(adrval, "TIXR")==0)||(strcmp(adrval, "COMPR")==0))
                            break;
                    }
                    strcat(ans, var);
                    while (k < len)
                    {
                        char ch = tempo[k];
                        if (ch == 'T' || ch=='B')
                        {
                            strcat(ans, "5");
                        }
                        else if (ch == 'S' || ch=='A')
                        {
                            strcat(ans, "6");
                        }
                        else if(ch=='X')
                        {
                            strcat(ans,"1");
                        }
                        k++;
                    }
                    fprintf(fp1, "%s\t%s\t%s\t%s\t00%s\n", address, label, opcode, operand, ans);
                    fprintf(fp4, "^00%s", ans);
                }
                else
                {
                    rewind(fp2);
                    fscanf(fp2, "%s%s", symbol, add);
                    while (strcmp(operand, symbol) != 0)
                        fscanf(fp2, "%s%s", symbol, add);
                    fprintf(fp1, "%s\t%s\t%s\t%s\t%s%s\n", address, label, opcode, operand, code[j], add);
                    fprintf(fp4, "^%s%s", code[j], add);
                }
            }
        }
        fscanf(fp3, "%s%s%s%s", address, label, opcode, operand);
    }
    fprintf(fp1, "%s\t%s\t%s\t%s\n", address, label, opcode, operand);
    fprintf(fp4, "\nE^00%s", starting_adress);
    printf("\n Intermediate file is converted into object code");
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);

    printf("\n\nThe contents of Intermediate file:\n\n\t");
    fp3 = fopen("intermediate_code.txt", "r");
    ch = fgetc(fp3);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = fgetc(fp3);
    }
    printf("\n\nThe contents of Symbol Table :\n\n");
    fp2 = fopen("symbol_table.txt", "r");
    ch = fgetc(fp2);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = fgetc(fp2);
    }
    printf("\n\nThe contents of Output file :\n\n");
    fp1 = fopen("assmlist.txt", "r");
    ch = fgetc(fp1);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = fgetc(fp1);
    }
    printf("\n\nThe contents of Object code file :\n\n");
    fp4 = fopen("object_code.txt", "r");
    ch = fgetc(fp4);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = fgetc(fp4);
    }
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    getch();
}