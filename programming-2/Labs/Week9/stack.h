//
// Created by giley on 4/18/2026.
//

#ifndef STACK_H
#define STACK_H

typedef struct {
    int *data;
    int size;
} stack;

int push(stack *st, int value);
int pop(stack *st, int *result);
int peek(stack *st, int *result);
int duplicate(stack *st);
void swap(stack *st);
void debug_stack(stack st);

#endif //STACK_H
