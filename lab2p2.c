#include <stdio.h>
#include <string.h>

struct dob
{
    int day;
    int month;
    int year;
};

struct student_info
{
    int roll_no;
    char name[50];
    float CGPA;
    struct dob age;
};

void displayByValue(struct student_info s)
{
    printf("\nUsing Call by Value:\n");
    printf("Roll No: %d\n", s.roll_no);
    printf("Name: %s\n", s.name);
    printf("CGPA: %.2f\n", s.CGPA);
    printf("Date of Birth: %02d-%02d-%04d\n", s.age.day, s.age.month, s.age.year);
}

void displayByAddress(struct student_info *s)
{
    printf("\nUsing Call by Address:\n");
    printf("Roll No: %d\n", s->roll_no);
    printf("Name: %s\n", s->name);
    printf("CGPA: %.2f\n", s->CGPA);
    printf("Date of Birth: %02d-%02d-%04d\n", s->age.day, s->age.month, s->age.year);
}

int main()
{
    struct student_info s;

    printf("Enter Roll No: ");
    scanf("%d", &s.roll_no);

    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);

    printf("Enter CGPA: ");
    scanf("%f", &s.CGPA);

    printf("Enter Date of Birth (dd mm yyyy): ");
    scanf("%d %d %d", &s.age.day, &s.age.month, &s.age.year);

    displayByValue(s);
    displayByAddress(&s);

    return 0;
}