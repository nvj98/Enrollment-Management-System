

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 30
#define fieldLength 200


#define diskFile "diskFile.dat"
#define courseFile "course.txt"

struct db_type
{
   char name[fieldLength];
   int age;
   char course1[fieldLength];
   char course2[fieldLength];
   char status[fieldLength];
 };

struct courseInfo
{ 
  char code [20]; // e.g., EECS2030
  char title [fieldLength];
  char  date [20];
  char time_start [20];
  char time_end [20];
  char  location [20]; 
};
 
 
struct courseInfo courseArr[SIZE]; // global variable  array of struc
    
char prompt_menu(void);
void init_list(struct db_type * pArr[]); 
void clearDB(struct db_type * pArr[]);
void init_courseArr(void);

void writeDisk(struct db_type * pArr[]); 
void emptyDisk(void); 
void loadDisk(struct db_type * pArr[]); 
void enterNew(struct db_type * pArr[SIZE]);
int  getSubString(char *source, char *target,int from, int to);
void sort(struct db_type * pArr[]);
void displayCourses(void);
void displayDB(struct db_type * pArr[]);
void removeRecord (struct db_type * pArr[]);
void swap(struct db_type * pArr[]);
int getLength(struct db_type * pArr[]);


int main(int argc, char *argv[])
{
    
    struct db_type * db_pArr[SIZE];  // main db storage

    init_list(db_pArr);  // set to NULL
    
    init_courseArr();  // load course from diskfile
    
    char choice;
    for(; ;){
      choice = prompt_menu();
      switch (choice)
      {
         case 'n': enterNew(db_pArr); break;
         case 'd': displayDB(db_pArr); break;
         case 'w': writeDisk(db_pArr);  break;    
         case 'l': loadDisk(db_pArr); break;
         case 's': sort(db_pArr); break;
	   
         case 'c': clearDB(db_pArr); break;  
         case 'e': emptyDisk();break;

         case 'v': displayCourses();break;
         case 'p': swap(db_pArr); break;
         case 'r': removeRecord(db_pArr);break;
         
         case 'q': exit(1); // terminate the whole program
       }
	
     }
     return 0;
}

void init_list(struct db_type * pArr[]){
  int t;
  for (t=0; t<SIZE; t++)
  { 
     pArr[t]= NULL;
  }
}

void clearDB(struct db_type * pArr[]){
   char c3[3];
   printf("are you sure to clear db? (y) or (n)? ");
 
   fgets(c3,3,stdin);
   
   if(! strcmp(c3, "y\n"))  
      init_list(pArr);
}


char prompt_menu(void){
  char s[80];
  while(1){
    printf("\n-----------------------------------------------------------------\n");
    printf("|    %-20s","(N)ew record");
    printf("%-20s","(R)emove record");
    printf("Swa(p) records\t|\n");
    printf("|    %-20s","(S)ort database");
    printf("%-20s","(C)lear database");
    printf("(D)isplay db\t|\n");
  
    printf("|    %-20s","(L)oad disk");
    printf("%-20s","(W)rite disk");
    printf("(E)mpty disk\t|\n");
  
    printf("|    %-20s", "(V)iew courses");//|\tSwa(p) record\t(Q)uit\t\t\t\t|\n");
    printf("%-20s","(Q)uit");
    printf("*Case Insensitive*\t|\n");
    printf("-----------------------------------------------------------------\n");
    printf("choose one: ");
  
    fgets(s,50, stdin); // \n added
   
    if (strlen(s) == 2 && strchr("edlsuqrcwnvpr", tolower(*s))) 
       return tolower(*s); // s[0], return the first character of s  
    //else
    printf("not a valid input!\n");
	 
 }
}

/* display all or specified course */
void displayCourses(void){   
// the provide sample.out uses "%s\t%-40s%-5s %s-%s   %s\n" as formatting string for printing each course info
char choice[20];
char newChoice[8];
printf("course code (or 'a')? ");
scanf("%s",choice);
getchar();
//print all courses
if(strcmp(choice,"a") == 0){
  int i=0;
  printf("=================================================================================\n");
  while(strcmp(courseArr[i].code,"EECS0000") !=0){
    printf("%s\t%-40s%-5s %s-%s   %s\n",courseArr[i].code,courseArr[i].title,courseArr[i].date,courseArr[i].time_start,courseArr[i].time_end,courseArr[i].location);
    i++;
  }
  printf("=================================================================================");

}
else{
  int i=0;
  int found = 0;
  
  while(strcmp(courseArr[i].code,"EECS0000") !=0){
    
    if(strcmp(courseArr[i].code,choice)==0 || strstr(courseArr[i].code,choice) != NULL){
      printf("%s\t%-40s%-5s %s-%s   %s\n",courseArr[i].code,courseArr[i].title,courseArr[i].date,courseArr[i].time_start,courseArr[i].time_end,courseArr[i].location);
      found = 1;
    } 
    i++;
  }
  if(found==0)
     printf("error! course does not exist\n");

}

}



/* input items into the list */
void enterNew(struct db_type * pArr[SIZE]){
  //allocate an address for new student.
struct db_type * newStudent = (struct db_type*)malloc(sizeof(struct db_type));
int length = getLength(pArr);
char name[fieldLength];
char course1[9];
char course2[fieldLength];
char courseSubstring[5];
int age;
printf("name: ");
fgets(name,fieldLength,stdin);
strcpy(newStudent->name,name);
printf("age: ");
scanf("%d",&age);
getchar();
newStudent->age=age;

printf("course-1: ");
scanf("%s",course1);
getchar();
int indexforCourse1,indexforCourse2;
int foundFlag1=0;
while(foundFlag1==0){
  int i=0;
  while(strcmp(courseArr[i].code,"EECS0000") !=0){
    // get substring of EECSXXXX the last four digit into courseSubstring array.
    getSubString(courseArr[i].code,courseSubstring,4,7);
    
    if(strcmp(courseArr[i].code,course1)==0 || strcmp(courseSubstring,course1) == 0){
      sprintf(newStudent->course1,"%s\t%-40s%-5s %s-%s   %s",courseArr[i].code,courseArr[i].title,courseArr[i].date,courseArr[i].time_start,courseArr[i].time_end,courseArr[i].location); 
      foundFlag1=1;
      //store the index value for next operation
      indexforCourse1=i;
      break;
    }
    i++;
  }
  if(foundFlag1==0){
      printf("course does not exist, enter again: ");
      scanf("%s",course1);
      getchar();
     }  
}

int foundedFlag2 =0;
printf("course-2: ");
scanf("%s",course2);
getchar();
while(foundedFlag2==0){
  int j =0;
  while(strcmp(courseArr[j].code,"EECS0000") !=0){
    // get substring of EECSXXXX the last four digit into courseSubstring array.
    getSubString(courseArr[j].code,courseSubstring,4,7);
    
      if(strcmp(courseArr[j].code,course2)==0 || strcmp(courseSubstring,course2) == 0){
      sprintf(newStudent->course2,"%s\t%-40s%-5s %s-%s   %s",courseArr[j].code,courseArr[j].title,courseArr[j].date,courseArr[j].time_start,courseArr[j].time_end,courseArr[j].location); 
      foundedFlag2=1;
      //store the index value for next operation
      indexforCourse2=j;
      break;
    }
    j++;
  }
  if(foundedFlag2==0){
  printf("course does not exist, enter again: ");
  scanf("%s",course2);
  getchar();
  }  
}
//check if the course dates has same value. one or more. 
if(strstr(courseArr[indexforCourse1].date,courseArr[indexforCourse2].date)!=NULL ||strstr(courseArr[indexforCourse2].date,courseArr[indexforCourse1].date)!=NULL){
  char hour1s[5];
  char min1s[3];
  char hour1e[5];
  char min1e[3];
  char hour2s[5];
  char min2s[3];
  char hour2e[5];
  char min2e[3];
  //get the hour and minute information in following format.
  sscanf(courseArr[indexforCourse1].time_start,"%02s:%02s",hour1s,min1s);
  sscanf(courseArr[indexforCourse1].time_end,"%02s:%02s",hour1e,min1e);
  sscanf(courseArr[indexforCourse2].time_start,"%02s:%02s",hour2s,min2s);
  sscanf(courseArr[indexforCourse2].time_end,"%02s:%02s",hour2e,min2e);
  //concatinate hour and minute.
  strcat(hour1s,min1s);
  strcat(hour1e,min1e);
  strcat(hour2s,min2s);
  strcat(hour2e,min2e);
  // conver it to a four digit integer. ex.1030, 1100, 1930 
  int starthour1= atoi(hour1s);
  int endhour1=atoi(hour1e);
  int starthour2 = atoi(hour2s);
  int endhour2 = atoi(hour2e);
  //check if there is a time conflict
  if(starthour1==starthour2 || endhour1==endhour2 || (starthour1 >= starthour2 && starthour1 < endhour2) || (endhour2>starthour1 && endhour2<=endhour1)
    ||(endhour1 > starthour2 && endhour1<=endhour2)|| (starthour2 >=starthour1 && starthour2 < endhour1))
    {
    strcpy(newStudent->status,"ATTENTION! time conflict");
    printf("ATTENTION! time conflict");  
    }
  else{
    strcpy(newStudent->status,"SUCCESSFUL! no time conflict");
    printf("SUCCESSFUL! no time conflict");  
    }
}
else{
strcpy(newStudent->status,"SUCCESSFUL! no time conflict");
printf("SUCCESSFUL! no time conflict"); 
}

//adding a new student into database 
pArr[length] = newStudent;

}

/* display records */
void displayDB(struct db_type * pArr[]){
  int i;
  int length = getLength(pArr);
  for(i=0;i<length;i++){
    printf("\n");
    printf("name:\t %s",pArr[i]->name);
    printf("age:\t %d\n",pArr[i]->age);
    printf("course1: %s\n",pArr[i]->course1);
    printf("course2: %s\n",pArr[i]->course2);
    printf("remarks: %s\n",pArr[i]->status);
   
  }

  printf("========== %d records ==========\n",length);
 
}


/* remove an existing item */
void removeRecord (struct db_type * pArr[])	
{
  char name[fieldLength];
  char newName[fieldLength]={};
  int length = getLength(pArr);
  printf("enter full name to remove: ");
  fgets(name,fieldLength,stdin);
  //strncat is used for eleminating the newline char from fgets().
  strncat(newName,name,strlen(name)-1);
  
  int i;
  int found =0;
  for(i=0;i<length;i++){
    if(strcmp(pArr[i]->name,name)==0){

      printf("record [%s] removed successfully.\n",newName);
      //found is flag for record has been founded.
      found = 1;
    
      int j;
      //shift all of the array
      for (j=i;j<length-1;j++){
           pArr[j]=pArr[j+1];        
       }
    }
  }
  // if we removed an element make the last element NULL because of the shifting.
  if(found==1)
    pArr[length-1] = NULL;
  // if we cant found the element print following below.
  if(found==0)
    printf("record not found\n");
}

/* swap records */
void swap(struct db_type * pArr[]){
  int length = getLength(pArr);
  //if the length is 1 or 0 do nothing.
   if(length == 1 || length ==0){}
   // if the length is even do the following.
   if (length %2 ==0){
     int i;
     for(i=0;i<length;i=i+2){
       struct db_type *tmp1 = pArr[i];
       pArr[i] = pArr[i+1];
       pArr[i+1] = tmp1;
     }
   }
   // if the length is odd do the following.
   else{
     int i;
     for(i=0;i<length-1;i=i+2){
       struct db_type *tmp1 = pArr[i];
       pArr[i] = pArr[i+1];
       pArr[i+1] = tmp1;
     }
   }
} 

/* load from course.txt, store into (global) array of courses  */
void init_courseArr(void){ 
 
   FILE *fr; //file pointer
   fr = fopen (courseFile, "r");
   char arr[50];  
   int count = 0;
   while ((fgets(arr,100,fr)) != NULL)
   {  
     strncpy(courseArr[count].code,  arr, strlen(arr)-1); // remove \n  
     courseArr[count].code[strlen(arr)-1] = '\0';  //to be safe
	 
     fgets(arr,100,fr);  
     strncpy(courseArr[count].title ,  arr, strlen(arr)-1);
     courseArr[count].title[strlen(arr)-1] = '\0';  //to be safe
     
          
     fgets(arr,100,fr); 
     strncpy(courseArr[count].date,  arr, strlen(arr)-1);
     courseArr[count].date[strlen(arr)-1] = '\0';  //to be safe
	   
     // other ways, remove \n before
     fgets(arr,100,fr);
     arr[strlen(arr)-1] = '\0';
     strcpy(courseArr[count].time_start,  arr);
      
     fgets(arr,100,fr);
     arr[strlen(arr)-1] = '\0';
     strcpy(courseArr[count].time_end,  arr);
      
     fgets(arr,100,fr);
     arr[strlen(arr)-1] = '\0';
     strcpy(courseArr[count].location,  arr); 
	  
     count++;
     fgets(arr,100,fr); // read the empty line
   }

   strcpy(courseArr[count].code, "EECS0000"); // a terminator token structure

   fclose(fr);
	 
 }
    
/************************ DISK IO *****************************************************************/

void writeDisk(struct db_type * pArr[]){
  FILE *fp;
  int i;

  if ( (fp=fopen(diskFile,"ab")) == NULL)
  {
    printf("cannot open file\n");
    return;
  }

  
  for (i=0; i< SIZE ; i++)
  {
     if ( pArr[i] != NULL)
     {  
       if (fwrite( pArr[i], sizeof(struct db_type), 1, fp) != 1)
       { 
          printf("file write error\n");
       }
     } 
   }
   fclose(fp);

}

void emptyDisk(void){

  FILE *fp;
  
  char c3[3];
  printf("are you sure to empty disk? (y) or (n)? ");
  fgets(c3,3,stdin);
  if(strcmp(c3, "y\n"))  
     return;
 
  //else
  if ( (fp = fopen(diskFile,"w")) == NULL)
  {
     printf("no file to open\n");
     return;
  }
  fclose(fp); // open close, will empty the file
}


void loadDisk(struct db_type * pArr[]){
  FILE *fp;
  int i;
  char c3[3];
  printf("will overwrite current records. are you sure to load disk? (y) or (n)? ");
  fgets(c3,3,stdin);
  if(strcmp(c3, "y\n"))  
     return;

  struct db_type * tmp;  
  
  if ( (fp = fopen(diskFile,"r")) == NULL)
  {
    printf("cannot open file\n");
    return;
  }

  init_list(pArr);
  for (i=0; i < SIZE ; i++)
  {    
     
     tmp =  (struct db_type *) malloc (sizeof(struct db_type));   
	   
     if (fread( tmp, sizeof(struct db_type), 1, fp) != 1)
     {
       if (feof(fp))
       { fclose(fp); 
         return;
       }
       printf("file read error\n");
     }
     else pArr[i] = tmp;
  }
}

/****bonus*************************************************************************************/

/* sort the records by ages */
void sort(struct db_type * pArr[])
{
 int length = getLength(pArr);
 int i,j;
 for(i=0;i<length;i++)
	{
	    for(j=i+1;j<length;j++)
		{
			if(pArr[j]->age < pArr[i]->age)
				{
          struct db_type *tmp = pArr[i];
          pArr[i] = pArr[j];
          pArr[j] = tmp;	
				}
		}
  }
 
}  

 

int  getSubString(char *source, char *target,int from, int to)
{
	int length=0;
	int i=0,j=0;
	
	//get length
	while(source[i++]!='\0')
		length++;
	
	if(from<0 || from>length){
		printf("Invalid \'from\' index\n");
		return 1;
	}
	if(to>length){
		printf("Invalid \'to\' index\n");
		return 1;
	}	
	
	for(i=from,j=0;i<=to;i++,j++){
		target[j]=source[i];
	}
	
	//assign NULL at the end of string
	target[j]='\0'; 
	
	return 0;	
}

int getLength(struct db_type * pArr[]){
  int counter =0;
    while(pArr[counter]!=NULL)
    counter++;

    return counter;
}