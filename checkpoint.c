#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>  


//-------------CONSTS----------------------------
#define COURSESLEN 10
#define NAMELEN 15
#define LEVELS 13
#define CLASSES 10
#define CELLLEN 13
#define LINESIZE 256
#define READMODE "r"
#define WRITEBINARYMODE "wb"
#define WRITEMODE "w"
#define OUTPUTFILE "students_output.txt"
#define FILENAME "students_with_class.txt"
#define OPENFILEERR "Error openning file..\n"
#define WRITEFILEERR "Error opening file for writing.\n"
#define WRITETOFILEERR "Error opening file for writing.\n"
#define BINARYFILE "school_data.bin"
#define COURSES_SIZE 10
#define COURSES {"Algebra", "Physics", "Biology", "Chemistry", "Physical Educations", "Human Biology", "Core Maths", "Statistis", "French", "English"}
const char* courses[COURSES_SIZE] = COURSES;

//------------STRUCT SECTION-----------------------

//Student struct that stores all data of the user, first name, last name,
//cellphone number, level, class, and grades. In addtition theres a pointer
//of type student that points to the next student in the linked list.
struct student {
    char first_name[NAMELEN];
    char last_name[NAMELEN];
    char cellphone[CELLLEN];
    int level;
    int class;
    float grades[COURSESLEN];
    struct student* next;
};

//School struct represents a dynamic linked list of students, to identify
//a student use the level he is in and the class he attends.
struct school {
    struct student* DB[LEVELS][CLASSES];
};

static struct school S;


//---------------FUNCTION DECLARATIONS-----------------
void init_db();
void save_to_binary(const char* filename);
void display_students_to_file();
void display_menu();
void show_tasks_list();
void task_handler(int choice);
void add_new_student();
void add_student_to_list(struct student* newStudent);
struct student* find_student(const char* first_name, const char* last_name);
void display_statistics();
void get_deans_list();
void get_students_to_leave_soon();
void get_average_by_course_and_level();
void clear_list();
void get_statistics();
void clear_input_buffer();
void get_valid_cellphone_input(char* str, int max_len);
void get_valid_string_input(char* str, int max_len);
float get_valid_float_input(float min, float max);
int get_valid_integer_input(int min, int max);


//---------------FUNCTION SECTION----------------------
/*
    This function opens the file of data in read mode, checks if it was succesffully 
    opened if not displays an error message. 
    Then the function reads each line from the file, extracts each input according to
    its type, and saved it in our data structure under the name of newStudent, 
    (which was allocated dynamically). Then accordingly save the student at the
    end of the linked list. Finally the save the file in binary, and close it. 
*/
void init_db() {
    FILE* file = fopen(FILENAME, READMODE);
        if (file == NULL) {
        printf(OPENFILEERR);
        return;
    }

    char buffer[LINESIZE];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        struct student* newStudent = (struct student*)malloc(sizeof(struct student));
        int result = sscanf(buffer, "%s %s %s %d %d %f %f %f %f %f %f %f %f %f %f",
            newStudent->first_name, newStudent->last_name, newStudent->cellphone,
            &newStudent->level, &newStudent->class,
            &newStudent->grades[0], &newStudent->grades[1], &newStudent->grades[2],
            &newStudent->grades[3], &newStudent->grades[4], &newStudent->grades[5],
            &newStudent->grades[6], &newStudent->grades[7], &newStudent->grades[8],
            &newStudent->grades[9]);

        if (result != 15) {
            printf("Error parsing line: %s\n", buffer);
            free(newStudent);
            continue;
        }

        add_student_to_list(newStudent);
    }

    save_to_binary(BINARYFILE);

    fclose(file);
}

//---------------------------------------------------------------------------

/*This function receives two integers representing the min and max values of
an input provided by the user. It prints an appropriate message if the input
is not valid.*/
int get_valid_integer_input(int min, int max) {
    int value;
    while (1) {
        printf("Enter a value between %d and %d: ", min, max);
        int num_items_read = scanf("%d", &value);
        clear_input_buffer();
        if (num_items_read == 1 && value >= min && value <= max) {
            return value;
        } else {
            printf("Invalid input. Please enter a valid integer between %d and %d.\n", min, max);
        }
    }
}

//---------------------------------------------------------------------------

/*This function receives two float numbers representing the minimun and maximum 
values of possible grades, this function is used to check if the input provided
by the user is valid, if not displays an appropriate message.*/
float get_valid_float_input(float min, float max) {
    float value;
    while (1) {
        printf("Enter a value between %.2f and %.2f: ", min, max);
        int num_items_read = scanf("%f", &value);
        clear_input_buffer();
        if (num_items_read == 1 && value >= min && value <= max) {
            return value;
        } else {
            printf("Invalid input. Please enter a valid float between %.2f and %.2f.\n", min, max);
        }
    }
}

//---------------------------------------------------------------------------

/*This function receives a string representing a name, and the maximum length
of the name, checks if the string which is provided by the user as input, is 
valid, by checking if it contains digits, or is longer than the maximum length.*/
void get_valid_string_input(char* str, int max_len) {
    while (1) {
        printf("Enter a string (no numbers allowed): ");
        int num_items_read = scanf("%s", str);
        clear_input_buffer();
        int valid = 1;

        // Check if the string contains any digits
        for (int i = 0; i < strlen(str); i++) {
            if (isdigit(str[i])) {
                valid = 0;
                break;
            }
        }

        if (num_items_read == 1 && valid) {
            return;
        } else {
            printf("Invalid input. Please enter a string with no numbers.\n");
        }
    }
}

//---------------------------------------------------------------------------

/*This function receives a string representing the given cellphone number from
the user, and the maximum length of a cell phone number. The function simply
checks if the cellphone contains any letters in them, if so, it asks the user 
for a new valid input by displaying an appropriate message, if not, it accepts
the input.*/
void get_valid_cellphone_input(char* str, int max_len) {
    while (1) {
        printf("Enter a cellphone number (no letters allowed): ");
        int num_items_read = scanf("%s", str);
        clear_input_buffer();
        int valid = 1;

        // Check if the string contains any letters
        for (int i = 0; i < strlen(str); i++) {
            if (isalpha(str[i])) {
                valid = 0;
                break;
            }
        }

        if (num_items_read == 1 && valid) {
            return;
        } else {
            printf("Invalid input. Please enter a cellphone number with no letters.\n");
        }
    }
}

//---------------------------------------------------------------------------

/*This function checks if there are excess values added to the input buffer
and removes them to prevent them from affecting other inputs.*/
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

//---------------------------------------------------------------------------

/*This function receives a new student of type student* and adds him to the 
linked list.*/
void add_student_to_list(struct student* newStudent) {
    newStudent->next = NULL;

    if (S.DB[newStudent->level][newStudent->class] == NULL) {
        S.DB[newStudent->level][newStudent->class] = newStudent;
    } else {
        struct student* currentStudent = S.DB[newStudent->level][newStudent->class];
        while (currentStudent->next != NULL) {
            currentStudent = currentStudent->next;
        }
        currentStudent->next = newStudent;
    }
}

//---------------------------------------------------------------------------

/*This function displays all students from our struct onto a file, which is the data retrieved from
    the attached file, this function is used to ensure that we parsed and populated the 
    data properly and correctly.
*/
void display_students_to_file() {
    FILE* file = fopen(OUTPUTFILE, WRITEMODE);
    if (file == NULL) {
        printf(WRITEFILEERR);
        return;
    }

    for (int level = 1; level < LEVELS; level++) {
        for (int class = 1; class < CLASSES; class++) { 
            struct student* currentStudent = S.DB[level][class];

            while (currentStudent != NULL) {
                fprintf(file, "First Name: %s\n", currentStudent->first_name);
                fprintf(file, "Last Name: %s\n", currentStudent->last_name);
                fprintf(file, "Cellphone: %s\n", currentStudent->cellphone);
                fprintf(file, "Level: %d\n", currentStudent->level);
                fprintf(file, "Class: %d\n", currentStudent->class);

                fprintf(file, "Grades: ");
                for (int i = 0; i < COURSESLEN; i++) {
                    fprintf(file, "%s: ", courses[i]);
                    fprintf(file, "%.2f ", currentStudent->grades[i]);
                }
                fprintf(file, "\n\n");

                currentStudent = currentStudent->next;
            }
        }
    }
}

//---------------------------------------------------------------------------

/*This function prints onto the console the menu of available operations*/
void display_menu() {
    printf("========== Menu ==========\n");
    printf("1. Display all students into file\n");
    printf("2. Add a new student\n");
    printf("3. Update student data\n");
    printf("4. Delete a student\n");
    printf("5. Get a certain student info.\n");
    printf("6. statistics\n");
    printf("7. Exit\n");
    printf("==========================\n");
}

//---------------------------------------------------------------------------

/*This function prints onto the console the menu of available statistical operations*/
void display_statistics() {
    printf("========== Statisics menu ==========\n");
    printf("1. Get the deans list of each course\n");
    printf("2. Get the students who will leave soon\n");
    printf("3. Get the average of each course in each level\n");
    printf("4. Cancel\n");
    printf("==========================\n");
}

//---------------------------------------------------------------------------

/**
 This function receives a file name, opens it in write binary mode, and writes to
 the data from our data structure.
*/
void save_to_binary(const char* filename) {
    FILE* file = fopen(filename, WRITEBINARYMODE);

    if (file == NULL) {
        printf(WRITETOFILEERR);
        return;
    }

    fwrite(&S, sizeof(struct school), 1, file);

    fclose(file);
}

//---------------------------------------------------------------------------

/*This function displays the menu of the available operations upon the database,
it asks the user of a choice representing a wanted operation and checks if its 
a valid input.*/
void show_tasks_list(){
    while (true) {
        display_menu();

        int choice;
        printf("Enter your choice: ");
        int num_items_read = scanf("%d", &choice);
        clear_input_buffer();


        if (num_items_read != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}

            printf("Invalid input. Please enter a valid integer between 1 and 5.\n");
            continue; 
        }

        task_handler(choice);
    }    
}

//---------------------------------------------------------------------------

/*This function adds a new student to the linked list, it does so by firstly 
allocating memory to that new student, then asks the user to provide all info of
the new student, and finally adds him to the linked list.*/
void add_new_student(){
    struct student* newStudent = (struct student*)malloc(sizeof(struct student));
    if (newStudent == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter the level: ");
    newStudent->level = get_valid_integer_input(1, 12);

    printf("Enter the class: ");
    newStudent->class = get_valid_integer_input(1, 10);

    printf("Enter the first name: ");
    get_valid_string_input(newStudent->first_name, NAMELEN);

    printf("Enter the last name: ");
    get_valid_string_input(newStudent->last_name, NAMELEN);

    printf("Enter the cellphone number: ");
    get_valid_cellphone_input(newStudent->cellphone, CELLLEN);

    printf("Enter the grades for the following courses:\n");
    for (int i = 0; i < COURSESLEN; i++) {
        printf("Course %s: ", courses[i]);
        newStudent->grades[i] = get_valid_float_input(0.0f, 100.0f);
    }

    add_student_to_list(newStudent);

    add_student_to_list(newStudent);

    printf("The student, %s %s, was added successfully! \n", newStudent->first_name, newStudent->last_name);

}

//---------------------------------------------------------------------------

/*This function receives a first name and a last name and loops through each student
in the linked list until it finds a student with the same full name and returns him,
if not found it returns null.*/
struct student* find_student(const char* first_name, const char* last_name) {
    for (int level = 1; level < LEVELS; level++) {
        for (int class = 1; class < CLASSES; class++) {
            struct student* currentStudent = S.DB[level][class];
            while (currentStudent != NULL) {
                if (strcmp(currentStudent->first_name, first_name) == 0 &&
                    strcmp(currentStudent->last_name, last_name) == 0) {
                    return currentStudent; 
                }
                currentStudent = currentStudent->next;
            }
        }
    }
    return NULL; 
}

//---------------------------------------------------------------------------

/*This function receives a first and a last name, finds the student with the 
same full name in the linked list and removes him.*/
void delete_student(const char* first_name, const char* last_name) {
    int found = 0; // false
    int level, class;

    for (level = 1; level < LEVELS; level++) {
        for (class = 1; class < CLASSES; class++) {
            struct student* currentStudent = S.DB[level][class];
            struct student* previousStudent = NULL;

            while (currentStudent != NULL) {
                if (strcmp(currentStudent->first_name, first_name) == 0 &&
                    strcmp(currentStudent->last_name, last_name) == 0) {
                    found = 1; // true
                    break;
                }
                previousStudent = currentStudent;
                currentStudent = currentStudent->next;
            }

            if (found)
                break;
        }
        if (found)
            break;
    }

    if (!found) {
        printf("Student not found.\n");
        return;
    }

    struct student* studentToDelete = S.DB[level][class];
    if (studentToDelete == NULL) {
        printf("Error: Student not found in the linked list.\n");
        return;
    }

    if (studentToDelete == S.DB[level][class]) {
        S.DB[level][class] = studentToDelete->next;
    } else {
        struct student* previousStudent = S.DB[level][class];
        while (previousStudent->next != studentToDelete) {
            previousStudent = previousStudent->next;
        }
        previousStudent->next = studentToDelete->next;
    }

    free(studentToDelete);

    printf("Student %s %s deleted successfully.\n", first_name, last_name);
}

//---------------------------------------------------------------------------

/*This function receives a first and a last name of a student, finds him in
the linked list and asks the user to update the found student's info.*/
void update_student(const char* first_name, const char* last_name) {
    struct student* studentToUpdate = find_student(first_name, last_name);
    if (studentToUpdate == NULL) {
        printf("Student not found.\n");
        return;
    }

    printf("Enter the updated level: ");
    studentToUpdate->level = get_valid_integer_input(1, 12);

    printf("Enter the updated class: ");
    studentToUpdate->class = get_valid_integer_input(1, 10);

    printf("Enter the updated first name: ");
    get_valid_string_input(studentToUpdate->first_name, NAMELEN);

    printf("Enter the updated last name: ");
    get_valid_string_input(studentToUpdate->last_name, NAMELEN);

    printf("Enter the updated cellphone number: ");
    get_valid_cellphone_input(studentToUpdate->cellphone, CELLLEN);

    printf("Enter the updated grades for the following courses:\n");
    for (int i = 0; i < COURSESLEN; i++) {
        printf("Updated Course %s: ", courses[i]);
        studentToUpdate->grades[i] = get_valid_float_input(0.0f, 100.0f);
    }

    printf("Student data updated successfully.\n");
}

//---------------------------------------------------------------------------

/*This function receives a first and a last name of a student, finds him in 
the database, and prints all the data saved about him.*/
void get_student_data(const char* first_name, const char* last_name) {
    struct student* currStudent = find_student(first_name, last_name);

    if(currStudent == NULL){
        printf("Student was not found! \n");
        return;
    }
    
    printf("\n\n");
    printf("First_name: %s \n", currStudent->first_name);
    printf("Last_name: %s\n", currStudent->last_name);
    printf("Level: %d\n", currStudent->level);
    printf("Class: %d\n", currStudent->class);
    printf("Cellphone number: %s\n", currStudent->cellphone);
    printf("Grades:");
    for (int i = 0; i < COURSESLEN; i++) {
        printf( "%s: ", courses[i]);
        printf("%.2f ", currStudent->grades[i]);
    }
    printf("\n\n");  
    
}

//---------------------------------------------------------------------------

/*This function receives a first_name and a last_name pointers, asks the 
user for both of the first_name and last_name in order to update the 
pointers.*/
void get_first_and_last_name(char* first_name, char* last_name) {
    printf("Enter the first name of the student: ");
    scanf("%s", first_name);

    printf("Enter the last name of the student: ");
    scanf("%s", last_name);
}

//---------------------------------------------------------------------------

/*This function displays all operations that could be done upon our data base, 
asks the user for his choice and operates the wanted operation.*/
void task_handler(int choice) {
    switch (choice) {
        case 1:
            display_students_to_file();
            break;
        case 2:
            add_new_student();
            break;
        case 3:{
                char first_name[NAMELEN];
                char last_name[NAMELEN];
                get_first_and_last_name(first_name, last_name);

                update_student(first_name, last_name);
                break;
            }
        case 4:{
                char first_name[NAMELEN];
                char last_name[NAMELEN];
                get_first_and_last_name(first_name, last_name);


                delete_student(first_name, last_name);
                break;
            }
        case 5:{
                char first_name[NAMELEN];
                char last_name[NAMELEN];
                get_first_and_last_name(first_name, last_name);

                get_student_data(first_name, last_name);
                break;
            }
        case 6:
                get_statistics();
                break;
        case 7:
            printf("Exiting the program\n");
            exit(0);
        default:
            printf("Invalid choice. Please select a valid option.\n");
    }
}

//---------------------------------------------------------------------------

/*This function displays the menu of statistical operations that could be done 
on our data base, it asks the user for his choice, and accordingly, operates 
the wanted operation.*/
void get_statistics(){
    while (1) {
                display_statistics();
                int statChoice;
                printf("Enter your choice: ");
                scanf("%d", &statChoice);
                clear_input_buffer();

                switch (statChoice) {
                    case 1:
                        get_deans_list();
                        break;
                    case 2:
                        get_students_to_leave_soon();
                        break;
                    case 3:
                        get_average_by_course_and_level();
                        break;
                    case 4:
                        printf("Exiting statistics menu.\n");
                        return; 
                    default:
                        printf("Invalid choice. Please select a valid option.\n");
                        break;
                }
            }
}

//---------------------------------------------------------------------------

/*This function displays the 10 students who have the highest grades in a specific
course, in all of the levels. It does so first displaying all available courses so the
user can choose which course he wants to look at, after a course was chosen, the function
loops through all levels and classes, search for the top 10 students in that course. Finally
it sorts them from highest to lowest, and prints them onto the console.*/
void get_deans_list() {
    printf("Available Courses:\n");
    for (int i = 0; i < COURSES_SIZE; i++) {
        printf("%d. %s\n", i + 1, courses[i]);
    }

    int courseChoice;
    printf("Enter the course number to view the Dean's List: ");
    courseChoice = get_valid_integer_input(1, COURSES_SIZE);

    printf("Dean's List for Course: %s\n", courses[courseChoice - 1]);
    printf("------------------------------\n");

    struct student* topStudents[LEVELS][10] = { NULL };

    for (int level = 1; level < LEVELS; level++) {
        for (int class = 1; class < CLASSES; class++) {
            struct student* currentStudent = S.DB[level][class];
            while (currentStudent != NULL) {
                if (currentStudent->level == level) {
                    int insertIndex = -1;
                    for (int i = 0; i < 10; i++) {
                        if (topStudents[level][i] == NULL) {
                            insertIndex = i;
                            break;
                        } else if (currentStudent->grades[courseChoice - 1] >
                                   topStudents[level][i]->grades[courseChoice - 1]) {
                            insertIndex = i;
                            break;
                        }
                    }

                    if (insertIndex >= 0) {
                        for (int i = 9; i > insertIndex; i--) {
                            topStudents[level][i] = topStudents[level][i - 1];
                        }
                        topStudents[level][insertIndex] = currentStudent;
                    }
                }
                currentStudent = currentStudent->next;
            }
        }
    }

    for (int level = 1; level < LEVELS; level++) {
        printf("Level %d:\n", level);
        int numToDisplay = 0;
        for (int i = 0; i < 10; i++) {
            if (topStudents[level][i] != NULL) {
                printf("Student: %s %s - Grade: %.2f\n", topStudents[level][i]->first_name,
                       topStudents[level][i]->last_name, topStudents[level][i]->grades[courseChoice - 1]);
                numToDisplay++;
            }
        }

        if (numToDisplay == 0) {
            printf("No students in this level for the selected course.\n");
        }
    }

    for (int level = 1; level < LEVELS; level++) {
        for (int i = 0; i < 10; i++) {
            topStudents[level][i] = NULL;
        }
    }
}

//---------------------------------------------------------------------------

/*This function displays all students who are about to leave, it basically goes
through all classes in the 12th grade and prints them onto the console.*/
void get_students_to_leave_soon() {
    printf("Students Leaving Soon (Level 12):\n");
    for (int class = 0; class < CLASSES; class++) {
        struct student* currentStudent = S.DB[12][class];
        while (currentStudent != NULL) {
            printf("%s %s\n", currentStudent->first_name, currentStudent->last_name);
            currentStudent = currentStudent->next;
        }
    }
    printf("\n");
}

//---------------------------------------------------------------------------

/*This function gets the average grade of each course in each level, it basically
loops through each course, then loops through each level counting the number of
students in the level and their total grades, then finally prints the average 
grade of each course in each level.*/
void get_average_by_course_and_level() {
    printf("Course Averages by Level:\n");
    const char* myConstArrayData[COURSES_SIZE] = COURSES;

    for (int course = 0; course < COURSES_SIZE; course++) {
        printf("Course: %s\n", myConstArrayData[course]);
        printf("--------------------------\n");

        for (int level = 1; level < LEVELS; level++) {
            float total = 0;
            int count = 0;

            for (int class = 0; class < CLASSES; class++) {
                struct student* currentStudent = S.DB[level][class];
                while (currentStudent != NULL) {
                    if (currentStudent->level == level) {
                        total += currentStudent->grades[course];
                        count++;
                    }
                    currentStudent = currentStudent->next;
                }
            }

            if (count > 0) {
                printf("Level %d: Average=%.2f\n", level, total / count);
            } else {
                printf("Level %d: No students in this level.\n", level);
            }
        }
        printf("\n");
    }
}

//---------------------------------------------------------------------------

/*This function iterates over all students in the linked list freeing each
one of them.*/
void clear_list() {
    for (int level = 1; level < LEVELS; level++) {
        for (int class = 0; class < CLASSES; class++) {
            struct student* currentStudent = S.DB[level][class];
            while (currentStudent != NULL) {
                struct student* temp = currentStudent;
                currentStudent = currentStudent->next;
                free(temp);
            }
            S.DB[level][class] = NULL; 
        }
    }
}


//---------------------------------------------------------------------------

/*Main function, inititates the data base, handles all operations of the application
and finally before terminating the program it clears the linked list.*/
int main(){
    init_db();
    show_tasks_list();

    clear_list();
    return 0;
}



