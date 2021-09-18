#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define CLEAR_BUFFER() do { int ch; while ( (ch = getchar()) != EOF && ch != '\n' ) {} } while (0)
#define TOK_DELIM "-"

typedef struct date{
    int day, month, year;
}date;

typedef struct director_info{
    char* director_surname;
    char* director_name;
}director_info;

typedef struct movie{
    int id;
    char* title;
    director_info* director;
    date* release_date;
}movie;

void Insert(int *cell,movie **m);
void Delete(movie **m,int *cell);
void Search_Title(movie **m, int cell);
void Search_Director(movie **m,int cell);
int Read_File(int *fd,movie **m);
void split_Data(char [],int *dd,int *mm,int *yy);
void initialize_Movie_Array(movie **m,int size);
void print_Data(movie **m, int cell);
int generate_IDs(int cell);
void insert_to_Struct(movie **m,int *cell,char [],char [],char [],int dd,int mm,int yy,int ii);
void range_of_BinarySearch(movie **m,int mid,int cell,char [],int *lower,int *upper);
int search_Id(int idd, movie **m,int *cell);
void insert_to_File(movie **m,int cell);
size_t IntToString(char *s, int x);

int main(){
    int size = 50;
    int tempf = 0;
    int fd = open("VideoClub.txt",O_RDWR | O_CREAT | O_APPEND,0700);
    if( fd < 0 ){
        printf("Cannot Open the file\n");
        exit(-1);
    }

    movie **m = malloc(size*sizeof(movie *));
    initialize_Movie_Array(m,size);
    int cell = Read_File(&fd,m);
    
    close(fd);

    int choice;
    do{
        system("clear");
        printf("1. Insert a new movie\n"
                "2. Delete a movie based on movie’s id\n"
                "3. Search a movie based on movie’s title\n"
                "4. Search a movie based on movie’s director\n"
                "5. List Of Films\n"
                "6. Exit\n");

        scanf("%d",&choice);
        
        switch (choice){
        case 1:{
            CLEAR_BUFFER();

            Insert(&cell,m);

            printf("Press enter to continue ...\n");
            tempf = tempf + 1;
            getchar();
            break;
        }case 2:{
            printf("2\n");
            CLEAR_BUFFER();
           
            Delete(m,&cell);
            
            CLEAR_BUFFER();
            printf("Press enter to continue ...\n");
            tempf = tempf + 1;
            getchar();
            break;
        }case 3:{
            CLEAR_BUFFER();
            
            Search_Title(m,cell);
            printf("Press enter to continue ...\n");
            getchar();
            break;
        }case 4:{
            printf("4\n");
            CLEAR_BUFFER();
            
            Search_Director(m,cell);
            printf("Press enter to continue ...\n");
            getchar();
            break;
        }case 5:{
            CLEAR_BUFFER();
            if( cell >= 0){
                print_Data(m,cell);
            }else{
                printf("There is no entry in the list ...\n");
            }
            printf("Press enter to continue ...\n");
            getchar();
            break;
        }case 6:{
            if( tempf > 0 ){
                insert_to_File(m,cell);
            }
            exit(1);
            break;
        }
        default:
            break;
        }
    }while(1);

    return 0;
}

void insert_to_File(movie **m,int cell){
    int fd = open("VideoClub.txt",O_RDWR | O_CREAT | O_APPEND,0700);
    if( fd < 0 ){
        printf("Cannot Open the file\n");
        exit(-1);
    }
    char buf[5];
    size_t n;
    ftruncate(fd,0);
    for(int i = 0; i <= cell; i++ ){  
        n = IntToString(buf, m[i]->id);
        write(fd, buf, n);
        write(fd, "\n", 1);
        write(fd, m[i]->title , strlen(m[i]->title));
        write(fd, "\n", 1);
        write(fd, m[i]->director->director_surname , strlen(m[i]->director->director_surname));
        write(fd, "\n", 1);
        write(fd, m[i]->director->director_name , strlen(m[i]->director->director_name));
        write(fd, "\n", 1);
        n = IntToString(buf, m[i]->release_date->day);
        write(fd, buf, n);
        write(fd, "\n", 1);
        n = IntToString(buf, m[i]->release_date->month);
        write(fd, buf, n);
        write(fd, "\n", 1);
        n = IntToString(buf, m[i]->release_date->year);
        write(fd, buf, n);
        write(fd, "\n", 1);
    }
    close(fd);
}

size_t IntToString(char *s, int x){
    //  Set pointer to current position.
    char *p = s;

    //  Set t to absolute value of x.
    unsigned t = x;

    //  Write digits.
    do{
        *p++ = '0' + t % 10;
        t /= 10;
    } while(t);

    //  Remember the return value, the number of characters written.
    size_t r = p-s;

    //  Since we wrote the characters in reverse order, reverse them.
    while (s < --p){
        char t = *s;
        *s++ = *p;
        *p = t;
    }
    return r;
}

int generate_IDs(int cell){
    int base = 1111;
    return (base + cell);
}

//Insert the new entry to Struct
void Insert(int *cell,movie **m){
    system("clear");
    int gid = 0;
    int dd,mm,yy;
    char *title = NULL,*name = NULL, *surname = NULL, *date = NULL;
    char tempt[60];
    char tempn[15];
    char temps[15];
    size_t lent = 0,lenn = 0, lens = 0, lend = 0;
    ssize_t Tsize = 0, Nsize = 0, Ssize = 0, Dsize = 0;
    
    printf("Give the Title: ");
    Tsize = getline(&title, &lent, stdin);
    Tsize--;
    title[Tsize] = '\0';

    printf("Give the name of the Director: ");
    Nsize = getline(&name, &lenn, stdin);
    Nsize--;
    name[Nsize] = '\0';

    printf("Give the Surname of the Director: ");
    Ssize = getline(&surname, &lens, stdin);
    Ssize--;
    surname[Ssize] = '\0';

    printf("Give give the date of issue[Day-Month-Year]: ");
    Dsize = getline(&date, &lend, stdin);
    Dsize--;

    split_Data(date,&dd,&mm,&yy);
    gid = generate_IDs(*cell);
    
    *cell = *cell + 1;
    strcpy(tempt, title);
    tempt[Tsize] = '\0';
    strcpy(tempn, name);
    tempn[Nsize] = '\0';
    strcpy(temps, surname);
    temps[Ssize] = '\0';
    insert_to_Struct(m,cell,tempt,tempn,temps,dd,mm,yy,gid);

}

void Delete(movie **m,int *cell){
    system("clear");

    int idd = 0;

    printf("Give the id you want to delete: ");
    scanf("%d",&idd);

    int ans = search_Id(idd, m, cell);

    if( ans >= 0 ){
        for (size_t i = ans; i <= *cell; i++){
            memset(m[i]->title,' ',59);
            memset(m[i]->director->director_name,' ',14);
            memset(m[i]->director->director_surname,' ',14);
            strncpy(m[i]->title, m[i+1]->title, strlen(m[i+1]->title));
            strncpy(m[i]->director->director_name, m[i+1]->director->director_name, strlen(m[i+1]->director->director_name));
            strncpy(m[i]->director->director_surname, m[i+1]->director->director_surname, strlen(m[i+1]->director->director_surname));
            m[i]->title[strlen(m[i+1]->title)] = '\0';
            m[i]->director->director_name[strlen(m[i+1]->director->director_name)] = '\0';
            m[i]->director->director_surname[strlen(m[i+1]->director->director_surname)] = '\0';
            m[i]->release_date->day = m[i+1]->release_date->day;
            m[i]->release_date->month = m[i+1]->release_date->month;
            m[i]->release_date->year = m[i+1]->release_date->year; 
        }
        printf("The entry with Id: %d was deleted\n",idd);
        *cell = *cell - 1;
    }else{
        printf("There is no entry with Id: %d\n",idd);
    }
}

int search_Id(int idd, movie **m,int *cell){

    for (size_t i = 0; i <= *cell; i++){
        if( idd == m[i]->id ){
            return i;
        }
    }
    
    return -1;
}

void Search_Title(movie **m, int cell){
    system("clear");
    
    int j,temp,i;
    char c,c1;
    int ans = -1;

    char *t = (char*) malloc(60*sizeof(char));
    size_t lent = 0;
    ssize_t Tsize = 0;

    printf("Give the Title you want to Search: ");
    Tsize = getline(&t, &lent, stdin);

    t[Tsize] = '\0';
    Tsize = Tsize - 1;

    for ( j = 0; j <= cell; j++ ){
        temp = 0;
        i = 0;
        c = t[i];
        c1 = m[j]->title[i];
        while( c != '\0' || c1 != '\0' ){
            if( t[i] == m[j]->title[i] ){
                temp = temp + 1;
            }else{
                break;
            }
            i++;
            c = t[i];
            c1 = m[j]->title[i];
        }
        if( temp == Tsize ){
            ans = j;
            break;
        }
    }

    if( temp == Tsize ){
        printf("Result of the Search\n");
        printf("~ ~ ~ ~ ~ ~ ~ ~\n");
        printf("Id: %d Movies Name: %s\nDirector Surname: %s Name: %s\n",m[ans]->id,m[ans]->title,m[ans]->director->director_surname,m[ans]->director->director_name);
        printf("Release Date %d/%d/%d\n",m[ans]->release_date->day,m[ans]->release_date->month,m[ans]->release_date->year);
        printf("~ ~ ~ ~ ~ ~ ~ ~\n");
    }else{
        printf("No entry was found with this title in the list\n");
    }

}

void Search_Director(movie **m,int cell){
    system("clear");
    int start = 0;
    int end = cell;
    int mid;
    int temp = 0;
    int j = 0,i;
    int pst[cell];
    int upper = 0;
    int lower = 0;

    char *direc = (char*) malloc(60*sizeof(char));
    size_t lent = 0;
    ssize_t Tsize = 0;

    printf("Give the surname of the Director you want to Search: ");
    Tsize = getline(&direc, &lent, stdin);
    
    direc[Tsize] = '\0';
    Tsize = Tsize - 1;

    while( start <= end ){
        mid = (start + end)/2;
        if( direc[0] > m[mid]->director->director_surname[0] ){
            start = mid + 1;
        }else if( direc[0] < m[mid]->director->director_surname[0] ){
            end = mid - 1;
        }else if( direc[0] == m[mid]->director->director_surname[0] ){
            temp = 0;
            j = 0;
            while( direc[j] != '\0' || m[mid]->director->director_surname[j] != '\0' ){
                if( direc[j] == m[mid]->director->director_surname[j] ){
                    temp = temp + 1;
                }else{
                    break;
                }
                j++;
            }
            if( temp == Tsize ){
                break;
            }else{
                if( direc[j] > m[mid]->director->director_surname[j] ){
                    start = mid + 1;
                }else{
                    end = mid - 1;
                }
            }
        }
    }
    
    if( temp == Tsize ){
        range_of_BinarySearch(m,mid,cell,direc,&lower,&upper);
        printf("Result of the Binary Search\n");
        printf("~ ~ ~ ~ ~ ~ ~ ~\n");
        printf("Director: %s %s\n",m[mid]->director->director_surname,m[mid]->director->director_name);
        for( i = mid-lower; i <= mid + upper; i++){
            printf("Id: %d Movies Name: %s\n",m[i]->id,m[i]->title);
            printf("Release Date %d/%d/%d\n",m[i]->release_date->day,m[i]->release_date->month,m[i]->release_date->year);
        }
        printf("~ ~ ~ ~ ~ ~ ~ ~\n");
    }else{
        printf("No entry was found with this title in the list\n");
    }
}

void range_of_BinarySearch(movie **m,int mid,int cell,char direc[],int *lower,int *upper){
    
    int temp = 0;
    int j = 0;

    for (int i = mid+1; i < cell; i++){
        j = 0;
        temp = 0;
        while( direc[j] != '\0' || m[i]->director->director_surname[j] != '\0' ){
            if( direc[j] == m[i]->director->director_surname[j] ){
                temp = temp + 1;
            }else{
                break;
            }
            j++;
        }
        if( temp == strlen(direc) -1 ) {
            *upper = *upper + 1;
        }
    }
    
    temp = 0;
    for (int i = mid-1; i > 0; i--){
        j = 0;
        temp = 0;
        while( direc[j] != '\0' || m[i]->director->director_surname[j] != '\0' ){
            if( direc[j] == m[i]->director->director_surname[j] ){
                temp = temp + 1;
            }else{   
                break;
            }
            j++;
        }
        if( temp == strlen(direc) - 1 ) {
            *lower = *lower + 1;
            
        }
    }
   
}

int Read_File(int *fd,movie **m){
    lseek(*fd,0,SEEK_SET);

    int format = 1;
    int bytes = 1;
    int cell = 0,j = 0,x;
    char c;
    char buffer[5];
    int count = 0;

    int dd,mm,yy;
    char tt[60];
    char nn[15];
    char ss[15];
    int ii;

    while( bytes == 1 ){
        count++;
        bytes = read(*fd, &c, 1);
        if( bytes == 0 && count == 1 ){
            return -1;
        }
        if( format == 8 ) {
            cell++;
            format = 1;
        }

        if( c == '\n' || bytes == 0 ){
            if( format == 1 ){
                buffer[j] = '\0';
                sscanf(buffer, "%d", &ii);
            }else if( format == 2 ){
                tt[j] = '\0';
            }else if( format == 3){
                ss[j] = '\0';
            }else if( format == 4 ){
                nn[j] = '\0';
            }else if( format == 5 ){
                buffer[j] = '\0';
                sscanf(buffer, "%d", &dd);
            }else if( format == 6 ){
                buffer[j] = '\0';
                sscanf(buffer, "%d", &mm); 
            }else if( format == 7 ){
                buffer[j] = '\0';
                sscanf(buffer, "%d", &yy); 
            }
            
            if( format == 7 ){
                insert_to_Struct(m,&cell,tt,nn,ss,dd,mm,yy,ii);
                
                memset(tt,' ',59);
                memset(nn,' ',14);
                memset(ss,' ',14);
            }

            format++;
            j = 0;
            continue;
        }
        
        switch (format){
            case 1:{
                buffer[j++] = c;
                break;
            }case 2:{
                tt[j++] = c;
                break;
            }case 3:{
                ss[j++] = c;
                break;
            }case 4:{
                nn[j++] = c;
                break;
            }case 5:{
                buffer[j++] = c;
                break;
            }case 6:{
                buffer[j++] = c;
                break;
            }case 7:{
                buffer[j++] = c;
                break;
            }
            default:
                break;
        }

    }
   
    return cell-1;
}

void insert_to_Struct(movie **m,int *cell,char tt[],char nn[],char ss[],int dd,int mm,int yy,int ii){
    int td,tm,ty;
    char tempt[60];
    char tempn[15];
    char temps[15];
    int tempi;
    int pst = -1;

    if( cell == 0 ){
        
        strncpy(m[*cell]->title,tt,strlen(tt));
        strncpy(m[*cell]->director->director_name,nn,strlen(nn));
        strncpy(m[*cell]->director->director_surname,ss,strlen(ss));
        m[*cell]->release_date->day = dd;
        m[*cell]->release_date->month = mm;
        m[*cell]->release_date->year = yy;

    }else{

        int k = 0;

        for (size_t i = 0; i <= *cell; i++){
            if( ss[0] < m[i]->director->director_surname[0] ){
                pst = i;
                break;
            }else if( ss[0] == m[i]->director->director_surname[0] ){
               
                k = 1;
                do{ 
                    if( ss[k] < m[i]->director->director_surname[k] ){
                        pst = i;
                        break;
                    }if( ss[k] == m[i]->director->director_surname[k] ){
                        k++;
                    }if( ss[k] > m[i]->director->director_surname[k] ){
                        i++;
                    }
                }while(ss[0] == m[i]->director->director_surname[0]);
                if( ss[k] < m[i]->director->director_surname[k] ) break;   
            }
        }
 
        if( pst != -1 ){
            for (size_t i = pst; i <= *cell; i=i+2){

                memset(tempt,' ',60);
                memset(tempn,' ',15);
                memset(temps,' ',15);
                strncpy(tempt,m[i+1]->title,strlen(m[i+1]->title));
                strncpy(tempn,m[i+1]->director->director_name,strlen(m[i+1]->director->director_name));
                strncpy(temps,m[i+1]->director->director_surname,strlen(m[i+1]->director->director_surname));
                tempt[strlen(m[i+1]->title)] = '\0';
                tempn[strlen(m[i+1]->director->director_name)] = '\0';
                temps[strlen(m[i+1]->director->director_surname)] = '\0';
                td = m[i+1]->release_date->day;
                tm = m[i+1]->release_date->month;
                ty = m[i+1]->release_date->year;

                memset(m[i+1]->title,' ',59);
                memset(m[i+1]->director->director_name,' ',14);
                memset(m[i+1]->director->director_surname,' ',14);
                strncpy(m[i+1]->title, m[i]->title, strlen(m[i]->title));
                strncpy(m[i+1]->director->director_name, m[i]->director->director_name, strlen(m[i]->director->director_name));
                strncpy(m[i+1]->director->director_surname, m[i]->director->director_surname, strlen(m[i]->director->director_surname));
                m[i+1]->title[strlen(m[i]->title)] = '\0';
                m[i+1]->director->director_name[strlen(m[i]->director->director_name)] = '\0';
                m[i+1]->director->director_surname[strlen(m[i]->director->director_surname)] = '\0';
                m[i+1]->release_date->day = m[i]->release_date->day;
                m[i+1]->release_date->month = m[i]->release_date->month;
                m[i+1]->release_date->year = m[i]->release_date->year;

                memset(m[i]->title,' ',60);
                memset(m[i]->director->director_name,' ',15);
                memset(m[i]->director->director_surname,' ',15);
                strncpy(m[i]->title, tt, strlen(tt));
                strncpy(m[i]->director->director_name, nn, strlen(nn));
                strncpy(m[i]->director->director_surname, ss, strlen(ss));
                m[i]->title[strlen(tt)] = '\0';
                m[i]->director->director_name[strlen(nn)] = '\0';
                m[i]->director->director_surname[strlen(ss)] = '\0';
                m[i]->release_date->day = dd;
                m[i]->release_date->month = mm;
                m[i]->release_date->year = yy;

                memset(tt,' ',60);
                memset(nn,' ',15);
                memset(ss,' ',15);
                strncpy(tt,tempt,strlen(tempt));
                strncpy(nn,tempn,strlen(tempn));
                strncpy(ss,temps,strlen(temps));
                tt[strlen(tempt)] = '\0';
                nn[strlen(tempn)] = '\0';
                ss[strlen(temps)] = '\0';
                dd = td;
                mm = tm;
                yy = ty;
            }
        }else{      
            memset(m[*cell]->title,' ',60);
            memset(m[*cell]->director->director_name,' ',15);
            memset(m[*cell]->director->director_surname,' ',15);
            strncpy(m[*cell]->title, tt, strlen(tt));
            strncpy(m[*cell]->director->director_name, nn, strlen(nn));
            strncpy(m[*cell]->director->director_surname, ss, strlen(ss));
            m[*cell]->title[strlen(tt)] = '\0';
            m[*cell]->director->director_name[strlen(nn)] = '\0';
            m[*cell]->director->director_surname[strlen(ss)] = '\0';
            m[*cell]->release_date->day = dd;
            m[*cell]->release_date->month = mm;
            m[*cell]->release_date->year = yy;
            m[*cell]->id = ii;
        }  
    }

    m[*cell]->id = generate_IDs(*cell);
}

void split_Data(char date[],int *dd,int *mm,int *yy){
    int d[3],i = 0;
    char* token;

    token = strtok(date,TOK_DELIM);
    while( token != NULL ){
        sscanf(token, "%d", &d[i++]);
        token = strtok(NULL,TOK_DELIM);
    }

    *dd = d[0];
    *mm = d[1];
    *yy = d[2];
}

void initialize_Movie_Array(movie **m,int size){
    for (size_t i = 0; i < size; i++){
        m[i] = malloc(sizeof(movie));
    }
    for (size_t i = 0; i < size; i++){
        m[i]->title = (char*)malloc(60*sizeof(char));
    }
    
    director_info **di = malloc(size*sizeof(director_info *));
    for (size_t i = 0; i < size; i++){
        di[i] = malloc(sizeof(director_info));
    }

    for (size_t i = 0; i < size; i++){
        di[i]->director_surname = (char*)malloc(15*sizeof(char));
        di[i]->director_name = (char*)malloc(15*sizeof(char));
    }
    
    for (size_t i = 0; i < size; i++){
        m[i]->director = di[i];
    }
    
    date **dt  = malloc(size*sizeof(date *));
    for (size_t i = 0; i < size; i++){
        dt[i] = malloc(sizeof(date));
    }
    for (size_t i = 0; i < size; i++){
        m[i]->release_date = dt[i];
    }

}

void print_Data(movie **m, int cell){
    system("clear");
    printf("\t\tList\n");
    for (size_t i = 0; i <= cell; i++){
        printf("%ld) Id: %d Title: %s\nDirector Surname: %s Name: %s\n",i,m[i]->id,m[i]->title,m[i]->director->director_surname,m[i]->director->director_name);
        printf("Release Date %d/%d/%d\n",m[i]->release_date->day,m[i]->release_date->month,m[i]->release_date->year);
        printf("~ ~ ~ ~ ~ ~ ~ ~\n");
    }
}