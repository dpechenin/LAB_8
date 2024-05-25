#include "header.h"
#ifdef TEST
void test() {
    printf("Тест программы. Все данные предыдущие удалятся.\n");
    flat flats[4];

    flats[0].flatNum = 1;
    strcpy(flats[0].type, "студия\n");
    strcpy(flats[0].fio.surname, "Пекаревич\n");
    strcpy(flats[0].fio.name, "Данила\n");
    strcpy(flats[0].fio.middleName, "Дмитриевич\n");
    flats[0].debtsUtilityBills = 1000;
    flats[0].amountResidents = 3;

    flats[1].flatNum = 2;
    strcpy(flats[1].type, "однокомнатная\n");
    strcpy(flats[1].fio.surname, "Печенин\n");
    strcpy(flats[1].fio.name, "Данила\n");
    strcpy(flats[1].fio.middleName, "Михайлович\n");
    flats[1].debtsUtilityBills = 0;
    flats[1].amountResidents = 1;

    flats[2].flatNum = 3;
    strcpy(flats[2].type, "трехкомнатная\n");
    strcpy(flats[2].fio.surname, "Алфеев\n");
    strcpy(flats[2].fio.name, "Владислав\n");
    strcpy(flats[2].fio.middleName, "Сергеевич\n");
    flats[2].debtsUtilityBills = 5000;
    flats[2].amountResidents = 5;

    flats[3].flatNum = 4;
    strcpy(flats[3].type, "трехкомнатная\n");
    strcpy(flats[3].fio.surname, "Шарапов\n");
    strcpy(flats[3].fio.name, "Георгий\n");
    strcpy(flats[3].fio.middleName, "Валерьевич\n");
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
    int t;
    scanf("%d", &t);
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
    flat flat1;
    printf("Введите номер квартиры: ");
    scanf("%d", &flat1.flatNum);
    getchar();
    printf("Введите тип квартиры: ");
    fgets(flat1.type, 99, stdin);
    printf("Введите Фамилию владельца: ");
    fgets(flat1.fio.surname, 100, stdin);
    printf("Введите Имя владельца: ");
    fgets(flat1.fio.name, 100, stdin);
    printf("Введите Отчество владельца: ");
    fgets(flat1.fio.middleName, 100, stdin);
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

void printFlat(flat flat1) {
    printf("Номер квартиры: %u\n", flat1.flatNum);
    printf("Тип квартиры: %s", flat1.type);
    printf("Фамилия владельца: %s", flat1.fio.surname);
    printf("Имя владельца: %s", flat1.fio.name);
    printf("Отчество владельца: %s", flat1.fio.middleName);
    printf("Сумма задолжности по ЖКХ: %u₽\n", flat1.debtsUtilityBills);
    printf("Кол-во людей живущих в квартире: %u\n", flat1.amountResidents);
    printf("\n");
}

flat del(int n) {
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
    if(i == 1) {
        int a;
        printf("Введите номер квартиры:\n");
        scanf("%d", &a);
        flat tmp = numFinder(a);
        if(tmp.flatNum == 0) {
            c = 0;
        }
        else {
            printFlat(tmp);
            c++;
        }
    }
    else if(i == 2) {
        printf("Введите фамилию:\n");
        char surname[101];
        getchar();
        fgets(surname, 100, stdin);
        FILE *f = fopen(FILE_NAME, "rb");
        if(!f) {
            printf("Не удалось открыть файл!");
        }
        else {
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
        FILE *f = fopen(FILE_NAME, "rb");
        if(!f) {
            printf("Не удалось открыть файл!");
        }
        else {
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
        FILE *f = fopen(FILE_NAME, "rb");
        if(!f) {
            printf("Не удалось открыть файл!");
        }
        else {
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
        FILE *f = fopen(FILE_NAME, "rb");
        if(!f) {
            printf("Не удалось открыть файл!");
        }
        else {
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
    int t = 0;
    flat tmp1 = numFinder(n);
    flat tmp2;
    if(tmp1.flatNum != 0) {
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
        if (b == 1) {
            t++;
            int flatNum;
            printf("Введите новый номер квартиры:\n");
            scanf("%d", &flatNum);
            tmp2.flatNum = flatNum;
            stpcpy(tmp2.type, tmp1.type);
            stpcpy(tmp2.fio.surname, tmp1.fio.surname);
            stpcpy(tmp2.fio.name, tmp1.fio.name);
            stpcpy(tmp2.fio.middleName, tmp1.fio.middleName);
            tmp2.debtsUtilityBills = tmp1.debtsUtilityBills;
            tmp2.amountResidents = tmp1.amountResidents;
        }
        else if (b == 0) {
            printf("Вы отменили\n");
        }
        else if (b == 2) {
            t++;
            tmp2.flatNum = tmp1.flatNum;
            printf("Введите новый тип квартиры:\n");
            getchar();
            fgets(tmp2.type, 100, stdin);
            stpcpy(tmp2.fio.surname, tmp1.fio.surname);
            stpcpy(tmp2.fio.name, tmp1.fio.name);
            stpcpy(tmp2.fio.middleName, tmp1.fio.middleName);
            tmp2.debtsUtilityBills = tmp1.debtsUtilityBills;
            tmp2.amountResidents = tmp1.amountResidents;
        }
        else if (b == 3) {
            t++;
            tmp2.flatNum = tmp1.flatNum;
            stpcpy(tmp2.type, tmp1.type);
            printf("Введите новую фамилию:\n");
            getchar();
            fgets(tmp2.fio.surname, 100, stdin);
            printf("Введите новое имя:\n");
            fgets(tmp2.fio.name, 100, stdin);
            printf("Введите новое отчество:\n");
            fgets(tmp2.fio.middleName, 100, stdin);
            tmp2.debtsUtilityBills = tmp1.debtsUtilityBills;
            tmp2.amountResidents = tmp1.amountResidents;
        }
        else if (b == 4) {
            t++;
            int debts;
            printf("Введите новый дюлг по комунальным платежам:\n");
            scanf("%d", &debts);
            tmp2.flatNum = tmp1.flatNum;
            stpcpy(tmp2.type, tmp1.type);
            stpcpy(tmp2.fio.surname, tmp1.fio.surname);
            stpcpy(tmp2.fio.name, tmp1.fio.name);
            stpcpy(tmp2.fio.middleName, tmp1.fio.middleName);
            tmp2.debtsUtilityBills = debts;
            tmp2.amountResidents = tmp1.amountResidents;
        }
        else if (b == 5) {
            t++;
            int amount;
            printf("Введите новый кол-во жителей:\n");
            scanf("%d", &amount);
            tmp2.flatNum = tmp1.flatNum;
            stpcpy(tmp2.type, tmp1.type);
            stpcpy(tmp2.fio.surname, tmp1.fio.surname);
            stpcpy(tmp2.fio.name, tmp1.fio.name);
            stpcpy(tmp2.fio.middleName, tmp1.fio.middleName);
            tmp2.debtsUtilityBills = tmp1.debtsUtilityBills;
            tmp2.amountResidents = amount;
        }
        else if (b == 6) {
            add();
        }
        else {
            printf("Введите число, которое соответствует действию!");
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
                fwrite(&tmp2, sizeof(flat), 1, f);
                fclose(f);
            }
        }
    }
    else {
        printf("Квартира для изменения не найдена.\n");
    }
    goMenu();
}

void output() {
    FILE *f = fopen(FILE_NAME, "rb");
    if(!f) {
        printf("Неудалось прочитать информацию из файла\n");
    }
    else {
        printf("Вся база данных:\n");
        rewind(f);
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