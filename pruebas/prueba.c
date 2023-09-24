#include <cs50.h>
#include <ctype.h>
#include <stdio.h>

int main(void){
    string answer = get_string('What is your name ?');
    printf('Hello , %s\n',answer);
}
