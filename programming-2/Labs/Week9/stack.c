//
// Created by giley on 4/18/2026.
//

#include <stdlib.h>
#include "stack.h"

#include <stdio.h>

int push(stack *st, int value) {
    int *temp = realloc(st->data, (st->size + 1) * sizeof(int));
    if (temp == NULL) {
        printf("Memory allocation failed!\n");
        return 0;
    }

    st->data = temp;
    st->data[st->size] = value;
    st->size++;

    return 1;
}

int pop(stack *st, int *result) {
    if (st->size == 0) {
        printf("Stack underflow!\n");
        return 0;
    }

    *result = st->data[st->size - 1];
    st->size--;

    if (st->size == 0) {
        free(st->data);
        st->data = NULL;
    } else {
        st->data = realloc(st->data, (st->size) * sizeof(int));
    }

    return 1;
}

int peek(stack *st, int *result) {
    if (st->size == 0) {
        printf("Stack underflow!\n");
        return 0;
    }

    *result = st->data[st->size - 1];
    return 1;
}

void debug_stack(stack st)
{
    printf("DEBUG\n");
    printf("\tStack size: %d\n", st.size);
    printf("\tData pointer: %p\n", st.data);

    if (st.size != 0 && st.data == NULL)
    {
        printf("Sanity check failed!\n");
        printf("Stack is NULL, but size is not 0\n");
        return;
    }

    if (st.size == 0 && st.data != NULL)
    {
        printf("Sanity check failed!\n");
        printf("Stack data is not NULL, but size is 0\n");
        return;
    }

    printf("\tValues: ");
    for (int i = st.size - 1; i >= 0; i--)
    {
        printf("%d ", st.data[i]);
    }
    printf("\n\n");
}

int duplicate(stack *st) {
    int value;

    if (!peek(st, &value)) {
        return 0;   // empty
    }

    if (!push(st, value)) {
        return -1;  // overflow
    }

    return 1;       // success
}

void swap(stack *st) {
    int a, b;

    if (st->size < 2) {
        printf("Not enough elements to swap!\n");
        return;
    }

    pop(st, &a);
    pop(st, &b);

    push(st, a);
    push(st, b);
}