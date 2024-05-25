#include "header.h"

int main() {
    char work;
#ifdef TEST
    test();
    printf("Введите любое число: ");
    getchar();
    scanf("%c", &work);
#endif
    work = '1';
    while(work == '1') {
        system("clear");
        printf("Меню:\n"
               "Выберите действие:\n"
               "0 - выйти из программы\n"
               "1 - добавить запись\n"
               "2 - удалить запись\n"
               "3 - найти запись\n"
               "4 - редактировать запись\n"
               "5 - вывести все содержимое\n");
        int i;
        int l;
        scanf("%d", &i);
        switch (i) {
            case 0:
                work = 0;
                break;
            case 1:
                add();
                break;
            case 2:
                system("clear");
                printf("Введите 0 если вы хотите удалить все,\n или введите номер квартиры"
                       "запись о которой вы хотите удалить:\n");
                int n;
                scanf("%d", &n);
                if(!n) {
                    clearall();
                }
                else {
                    del(n);
                }
                break;
            case 3:
                find();
                break;
            case 4:
                system("clear");
                printf("Какую запись вы хотите изменить.\nВведите номер квартиры:\n"
                       "Если передумали введите 0: \n");
                scanf("%d", &l);
                edit(l);
                break;
            case 5:
                output();
                break;
            default:
                system("clear");
                printf("Введите число, которое соответствует действию!\n");
        }
    }
    return 0;
}