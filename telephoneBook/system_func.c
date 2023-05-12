#include "user_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
 
#define WR_VALUE _IOW('a', 'a', char*)
#define GET_USER _IOR('g', 'b', char*)

long get_user(const char* surname, unsigned int len, struct user_data* output_data) 
{
    int fd = open("/dev/chardev", O_RDWR);
    if(fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
    }
    char tmp[len];
    strcpy(tmp, surname);
    ioctl(fd, GET_USER, surname);
    if (strcmp(tmp, surname) != 0) {
        output_data = (struct user_data*)(surname);
        printf("Name: %s\t Surname: %s\t Age: %d\n", output_data->name, output_data->surname, output_data->age);
        close(fd);
        return 0;
    }
    return 1; 
}

long add_user(struct user_data* input_data) 
{
    char* buf = (char*)((void*) input_data);
    FILE *file;
    file = fopen("/dev/chardev", "w");
    fwrite(buf, sizeof(struct user_data), 1, file);
    fclose(file);
}

long del_user(const char* surname, unsigned int len) 
{
    int fd = open("/dev/chardev", O_RDWR);
    if(fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
    }
    ioctl(fd, WR_VALUE, surname);
    close(fd); 
}

int main(int argc, char** argv) {
    struct user_data user;
    printf("%d\n", argc);

    
    int output1 = strcmp(argv[1], "add_user");
    int output2 = strcmp(argv[1], "get_user");
    int output3 = strcmp(argv[1], "del_user");  

    if (output1 == 0) {
        if (argc != 7) {
            printf("Not full info about user");
            return 1;
        }
        user.age = atoi(argv[4]);
        strcpy(user.name, argv[2]);
        strcpy(user.email, argv[5]);
        strcpy(user.surname, argv[3]);
        strcpy(user.phone_number, argv[6]);
        printf("Name: %s\t Surname: %s\t Age: %d\n", user.name, user.surname, user.age);        
        add_user(&user);
    }
    if (output2 == 0) {
        if (argc != 3) {
            printf("Unknown command");
            return 1; 
        }
        get_user(argv[2], sizeof(argv[2]), &user);
    }
    if (output3 == 0) {
        if (argc != 3) {
            printf("Unknown command");
            return 1; 
        }
        del_user(argv[2], sizeof(argv[2]));
    }
    return 0;
}