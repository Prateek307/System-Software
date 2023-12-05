#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to convert hexadecimal to decimal
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

void main()
{
    char start[10],opcode[10], operand[10], label[10], code[10], mnemonic[10], locctr[100];
    int length;

    FILE *fp1, *fp2, *fp3, *fp4;

    fp1 = fopen("input.txt", "r");
    fp2 = fopen("optab.txt", "r");
    fp3 = fopen("symbol_table.txt", "w");
    fp4 = fopen("intermediate_code.txt", "w");

    fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0)
    {
        strcpy(start,operand);
        strcpy(locctr, operand);
        fprintf(fp4, "\t%s\t%s\t%s\n", label, opcode, operand);
        fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
    }
    else
    {
        char s1[10] = " ";
        strcpy(locctr, s1);
    }
    char addedLabels[100][10];
    int addedLabelsCount = 0;
    while (strcmp(opcode, "END") != 0)
    {
        fprintf(fp4, "%s\t", locctr);

        if (strcmp(label, "**") != 0) // it is a label
        {
            int duplicate = 0;
            for (int i = 0; i < addedLabelsCount; i++)
            {
                if (strcmp(addedLabels[i], label) == 0)
                {
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate)
            {
                char eq[10] = "EQU";
                if (strcmp(opcode, eq) != 0)
                {
                    fprintf(fp3, "%s\t%s\n", label, locctr);
                }
                else
                {
                    fprintf(fp3, "%s\t%s\n", label, operand);
                    strcpy(addedLabels[addedLabelsCount], label);
                    addedLabelsCount++;
                }
            }
            else
            {
                printf("Duplicate symbols trying to occur in symbol table :(\n");
                // To exit the program after receiving this error message then
                // exit(1);
            }
        }
        rewind(fp2);
        fscanf(fp2, "%s\t%s", code, mnemonic);
        char temp[100];
        int val;
        val = hexaToDec(locctr);
        while (strcmp(code, "END") != 0)
        {
            if (strcmp(opcode, code) == 0 && strcmp(opcode,"ADDR")!=0 && strcmp(opcode,"TIXR")!=0 && strcmp(opcode,"COMPR")!=0 )
            {
                val += 3;
                strcpy(temp, decToHexa(val, temp));
                strcpy(locctr, temp);
                break;
            }
            fscanf(fp2, "%s\t%s", code, mnemonic);
        }
        if (strcmp(opcode, "WORD") == 0)
        {
            val = hexaToDec(locctr);
            val += 3;
            strcpy(temp, decToHexa(val, temp));
            strcpy(locctr, temp);
        }
        else if (strcmp(opcode, "RESW") == 0)
        {
            val += 3 * hexaToDec(operand);
            strcpy(temp, decToHexa(val, temp));
            strcpy(locctr, temp);
        }
        else if (strcmp(opcode, "RESB") == 0)
        {
            val += (hexaToDec(operand));
            strcpy(temp, decToHexa(val, temp));
            strcpy(locctr, temp);
        }
        else if (strcmp(opcode, "EQU") == 0)
        {
            val += 3;
            strcpy(temp, decToHexa(val, temp));
            strcpy(locctr, temp);
        }
        else if (strcmp(opcode, "BYTE") == 0)
        {
            if (operand[0] == 'C') // it is constant
            {
                val += strlen(operand) - 3;
            }
            else // it is hexadecdimal
            {
                val += (strlen(operand) - 3) / 2;
            }
            strcpy(temp, decToHexa(val, temp));
            strcpy(locctr, temp);
        }
        else if(strcmp(opcode,"ADDR")==0 || strcmp(opcode,"TIXR")==0 || strcmp(opcode,"COMPR")==0)
        {
            val += 2;
            strcpy(temp, decToHexa(val, temp));
            strcpy(locctr, temp);
        }
        fprintf(fp4, "%s\t%s\t%s\t\n", label, opcode, operand);
        fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
    }
    fprintf(fp4, "%s\t%s\t%s\t%s\n", locctr, label, opcode, operand);
    printf("The length of the program  = %d\n", hexaToDec(locctr) - hexaToDec(start));
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
}