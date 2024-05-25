#include "header.h"
#ifdef TEST
void test() {
    printf("Тест программы. Все данные предыдущие удалились.\n");
    flat flats[4];

    flats[0].flatNum = 1;
    strcpy(flats[0].type, "двухкомнатная");
    strcpy(flats[0].fio.surname, "Пекаревич");
    strcpy(flats[0].fio.name, "Данила");
    strcpy(flats[0].fio.middleName, "Дмитриевич");
    flats[0].debtsUtilityBills = 1000;
    flats[0].amountResidents = 3;

    flats[1].flatNum = 2;
    strcpy(flats[1].type, "однокомнатная");
    strcpy(flats[1].fio.surname, "Печенин");
    strcpy(flats[1].fio.name, "Данила");
    strcpy(flats[1].fio.middleName, "Михайлович");
    flats[1].debtsUtilityBills = 0;
    flats[1].amountResidents = 1;

    flats[2].flatNum = 3;
    strcpy(flats[2].type, "трехкомнатная");
    strcpy(flats[2].fio.surname, "Алфеев");
    strcpy(flats[2].fio.name, "Владислав");
    strcpy(flats[2].fio.middleName, "Сергеевич");
    flats[2].debtsUtilityBills = 5000;
    flats[2].amountResidents = 5;

    flats[3].flatNum = 4;
    strcpy(flats[3].type, "трехкомнатная");
    strcpy(flats[3].fio.surname, "Шарапов");
    strcpy(flats[3].fio.name, "Георгий");
    strcpy(flats[3].fio.middleName, "Валерьевич");
    flats[3].debtsUtilityBills = 0;
    flats[3].amountResidents = 2;

    FILE *f = fopen(FILE_NAME, "wb");
    if(!f) {
        printf("Неудалось записать информацию в файл\n");
    }
    else {
        for (int i = 0; i < 4; ++i) {
            fwrite(&flats[i], sizeof(flat), 1, f);
        }
    }
    fclose(f);
}
#endif

void goMenu() {
    printf("Чтобы вернутся в меню напишите любую цифру:\n");
    char t;
    getchar();
    scanf("%c", &t);
}

flat numFinder(int n) {
    flat smpl;
    smpl.flatNum = 0;
    FILE *f = fopen(FILE_NAME, "rb");
    if(!f) {
        printf("Не удалось открыть файл!");
    }
    else {
        while(!feof(f)) {
            flat tmp;
            fread(&tmp, sizeof(flat), 1, f);
            if(!feof(f) && tmp.flatNum == n) {
                return tmp;
            }
        }
    }
    return smpl;
}

void add() {
    system("clear");
    flat flat1;
    printf("Введите номер квартиры: ");
    scanf("%d", &flat1.flatNum);
    getchar();
    printf("Введите тип квартиры: ");
    fgets(flat1.type, 99, stdin);
    flat1.type[strlen(flat1.type) - 1] = 0;
    printf("Введите Фамилию владельца: ");
    fgets(flat1.fio.surname, 100, stdin);
    flat1.fio.surname[strlen(flat1.fio.surname) - 1] = 0;
    printf("Введите Имя владельца: ");
    fgets(flat1.fio.name, 100, stdin);
    flat1.fio.name[strlen(flat1.fio.name) - 1] = 0;
    printf("Введите Отчество владельца: ");
    fgets(flat1.fio.middleName, 100, stdin);
    flat1.fio.middleName[strlen(flat1.fio.middleName) - 1] = 0;
    printf("Введите задолжность по комнуальным платежам в рублях: ");
    scanf("%d", &flat1.debtsUtilityBills);
    printf("Введите количество жильцов: ");
    scanf("%d", &flat1.amountResidents);
    FILE *f = fopen(FILE_NAME, "ab");
    if(!f) {
        printf("Неудалось записать информацию в файл\n");
    }
    else {
        fwrite(&flat1, sizeof(flat1), 1, f);
        fclose(f);
    }
    goMenu();
}

void printHeader(){
    printf("Номер квартиры | Тип квартиры   | Фамилия владельца | Имя владельца | Отчество владельца | "
           "Сумма задолжности по ЖКХ | Кол-во людей живущих в квартире\n");
}

void printFlat(flat flat1) {
    printf("%u\t\t | %s\t| %s\t| %s\t| %s\t\t| %u\t\t\t| %u\n", flat1.flatNum, flat1.type, \
           flat1.fio.surname, flat1.fio.name, flat1.fio.middleName, flat1.debtsUtilityBills, flat1.amountResidents);
    printf("\n");
}

flat del(int n) {
    system("clear");
    flat smpl;
    smpl.flatNum = 0;
    smpl.debtsUtilityBills = 0;
    smpl.amountResidents = 0;
    FILE *f = fopen(FILE_NAME, "r+b");
    int c = 0;
    int index = 0;
    if(!f) {
        printf("Не удалось открыть файл!");
    }
    else {
        while(!feof(f)) {
            index++;
            flat tmp;
            fread(&tmp, sizeof(tmp), 1, f);
            if(!feof(f) && tmp.flatNum == n) {
                printf("Вы собираетесь удалить/изменить эту запись:\n");
                printHeader();
                printFlat(tmp);
                int z;
                printf("Хотите продолжить? Да - введите любое число, нет - 0:\n");
                scanf("%d", &z);
                if(z) {
                    c++;
                    smpl = tmp;
                }
                break;
            }
        }
        if(!c) {
            printf("Нет такой квартиры или вы сделали отмену.\n");
        }
        else {
            rewind(f);
            fseek(f, index * sizeof(flat), SEEK_SET);
            while(!feof(f)) {
                flat tmp1;
                if(!fread(&tmp1, sizeof(flat), 1, f)) break;
                fseek(f, -2 * sizeof(flat), SEEK_CUR);
                fwrite(&tmp1, sizeof(flat), 1, f);
                fseek(f, sizeof(flat), SEEK_CUR);
            }
            fseek(f, 0, SEEK_END);
            long curFileSize = ftell(f);
#ifdef __unix__
		    int desc = fileno(f);
		    ftruncate(desc, curFileSize - sizeof(flat));
#endif
#ifdef __APPLE__
		    int desc = fileno(f);
		    ftruncate(desc, curFileSize - sizeof(flat));
#else
            int desc = _fileno(f);
            _chsize(desc, curFileSize - sizeof(flat));
#endif
        }
    }
    fclose(f);
    return smpl;
}

void clearall() {
    system("clear");
    FILE *f = fopen(FILE_NAME, "wb");
    if(!f) {
        printf("Файл не удалось открыть\n");
    }
    else {
        printf("Файл отчищен\n");
    }
    fclose(f);
    goMenu();
}

void find() {
    system("clear");
    printf("По какому признаку вы хотите:\n"
           "1 - номер квартиры\n"
           "2 - фамилия\n"
           "3 - имя\n"
           "4 - отчество\n"
           "5 - тип квартиры\n"
           "6 - кол-во жильцов\n"
           "7 - долг по комунальным платежам\n");
    int c = 0;
    int i;
    scanf("%d", &i);
    system("clear");
    if(i == 1) {
        int a;
        printf("Введите номер квартиры:\n");
        scanf("%d", &a);
        flat tmp = numFinder(a);
        if(tmp.flatNum == 0) {
            c = 0;
        }
        else {
            printHeader();
            printFlat(tmp);
            c++;
        }
    }
    else if(i == 2) {
        printf("Введите фамилию:\n");
        char surname[101];
        getchar();
        fgets(surname, 100, stdin);
        surname[strlen(surname) - 1] = 0;
        FILE *f = fopen(FILE_NAME, "rb");
        if(!f) {
            printf("Не удалось открыть файл!");
        }
        else {
            printHeader();
            while(!feof(f)) {
                flat tmp;
                fread(&tmp, sizeof(flat), 1, f);
                if(!feof(f) && !strcmp(surname, tmp.fio.surname)) {
                    printFlat(tmp);
                    c++;
                }
            }
        }
    }
    else if(i == 3) {
        printf("Введите имя:\n");
        char name[101];
        getchar();
        fgets(name, 100, stdin);
        name[strlen(name) - 1] = 0;
        FILE *f = fopen(FILE_NAME, "rb");
        if(!f) {
            printf("Не удалось открыть файл!");
        }
        else {
            printHeader();
            while(!feof(f)) {
                flat tmp;
                fread(&tmp, sizeof(flat), 1, f);
                if(!feof(f) && !strcmp(name, tmp.fio.name)) {
                    printFlat(tmp);
                    c++;
                }
            }
        }
    }
    else if(i == 4) {
        printf("Введите отчество:\n");
        char middlename[101];
        getchar();
        fgets(middlename, 100, stdin);
        middlename[strlen(middlename) - 1] = 0;
        FILE *f = fopen(FILE_NAME, "rb");
        if(!f) {
            printf("Не удалось открыть файл!");
        }
        else {
            printHeader();
            while(!feof(f)) {
                flat tmp;
                fread(&tmp, sizeof(flat), 1, f);
                if(!feof(f) && !strcmp(middlename, tmp.fio.middleName)) {
                    printFlat(tmp);
                    c++;
                }
            }
        }
    }
    else if(i == 5) {
        printf("Введите тип квартиры:\n");
        char type[101];
        getchar();
        fgets(type, 100, stdin);
        type[strlen(type) - 1] = 0;
        FILE *f = fopen(FILE_NAME, "rb");
        if(!f) {
            printf("Не удалось открыть файл!");
        }
        else {
            printHeader();
            while(!feof(f)) {
                flat tmp;
                fread(&tmp, sizeof(flat), 1, f);
                if(!feof(f) && !strcmp(type, tmp.type)) {
                    printFlat(tmp);
                    c++;
                }
            }
        }
    }
    else if(i == 6) {
        printf("Введите количество жильцов:\n");
        int amountResidents;
        scanf("%d", &amountResidents);
        FILE *f = fopen(FILE_NAME, "rb");
        if(!f) {
            printf("Не удалось открыть файл!");
        }
        else {
            printHeader();
            while(!feof(f)) {
                flat tmp;
                fread(&tmp, sizeof(flat), 1, f);
                if(!feof(f) && amountResidents == tmp.amountResidents) {
                    printFlat(tmp);
                    c++;
                }
            }
        }
    }
    else if(i == 7) {
        printf("Введите задолженность по ЖКХ:\n");
        int debt;
        scanf("%d", &debt);
        FILE *f = fopen(FILE_NAME, "rb");
        if(!f) {
            printf("Не удалось открыть файл!");
        }
        else {
            printHeader();
            while(!feof(f)) {
                flat tmp;
                fread(&tmp, sizeof(flat), 1, f);
                if(!feof(f) && debt == tmp.debtsUtilityBills) {
                    printFlat(tmp);
                    c++;
                }
            }
        }
    }
    else {
        printf("Введите число, которое соответствует действию!");
    }
    if(c == 0) {
        printf("Такой записи нет.\n");
    }
    goMenu();
}

void edit(int n) {
    system("clear");
    int t = 0;
    flat tmp1 = numFinder(n);
    if(tmp1.flatNum != 0) {
        printf("Вы хотите изменить запись о данной квартире:\n");
        printHeader();
        printFlat(tmp1);
        printf("Чтобы вы хотели изменить?\n"
               "0 - отмена\n"
               "1 - номер квартиры\n"
               "2 - тип квартиры\n"
               "3 - ФИО владельца\n"
               "4 - задолжность по комунальным услугам\n"
               "5 - кол-во жильцов квартиры\n"
               "6 - все:\n");
        int b;
        scanf("%d", &b);
        system("clear");
        if (b == 1) {
            t++;
            int flatNum;
            printf("Введите новый номер квартиры:\n");
            scanf("%d", &flatNum);
            tmp1 = tmp1;
            tmp1.flatNum = flatNum;
        }
        else if (b == 0) {
            printf("Вы отменили\n");
        }
        else if (b == 2) {
            t++;
            printf("Введите новый тип квартиры:\n");
            getchar();
            fgets(tmp1.type, 100, stdin);
            tmp1.type[strlen(tmp1.type) - 1] = 0;
        }
        else if (b == 3) {
            t++;
            printf("Введите новую фамилию:\n");
            getchar();
            fgets(tmp1.fio.surname, 100, stdin);
            printf("Введите новое имя:\n");
            fgets(tmp1.fio.name, 100, stdin);
            printf("Введите новое отчество:\n");
            fgets(tmp1.fio.middleName, 100, stdin);
            tmp1.fio.surname[strlen(tmp1.fio.surname) - 1] = 0;
            tmp1.fio.name[strlen(tmp1.fio.name) - 1] = 0;
            tmp1.fio.middleName[strlen(tmp1.fio.middleName) - 1] = 0;
        }
        else if (b == 4) {
            t++;
            int debts;
            printf("Введите новый дюлг по комунальным платежам:\n");
            scanf("%d", &debts);
            tmp1.debtsUtilityBills = debts;
        }
        else if (b == 5) {
            t++;
            int amount;
            printf("Введите новый кол-во жителей:\n");
            scanf("%d", &amount);
            tmp1.amountResidents = amount;
        }
        else if (b == 6) {
            add();
        }
        else {
            printf("Введите число, которое соответствует действию\n");
        }
        if (t) {
            FILE *f = fopen(FILE_NAME, "r+b");
            if(!f) {
                printf("Не удалось открыть файл!");
            }
            else {
                long x = 0;
                while(!feof(f)) {
                    flat tmp;
                    fread(&tmp, sizeof(flat), 1, f);
                    if(!feof(f) && tmp.flatNum == n) {
                        x = ftell(f);
                        break;
                    }
                    }
                fseek(f, x - sizeof(flat), SEEK_SET);
                fwrite(&tmp1, sizeof(flat), 1, f);
                fclose(f);
            }
        }
    }
    else {
        system("clear");
        printf("Квартира для изменения не найдена.\n");
    }
    goMenu();
}

void output() {
    system("clear");
    FILE *f = fopen(FILE_NAME, "rb");
    if(!f) {
        printf("Неудалось прочитать информацию из файла\n");
    }
    else {
        printf("Вся база данных:\n");
        rewind(f);
        printHeader();
        while(!feof(f)) {
            flat flat1;
            fread(&flat1, sizeof(flat1), 1,f);
            if (!feof(f)) {
                printFlat(flat1);
            }
        }
    }
    fclose(f);
    goMenu();
}