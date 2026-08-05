#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main() {
    stack st = {NULL, 0};
    int choice;
    int value;

    while (1) {
        printf("\n1 - Push        4 - Swap\n");
        printf("2 - Pop         5 - Duplicate\n");
        printf("3 - Peek        9 - DEBUG\n");
        printf("0 - Exit\n> ");

        scanf("%d", &choice);

        switch (choice) {
            case 1: // Push
                printf("Enter value: ");
                scanf("%d", &value);

                if (!push(&st, value)) {
                    printf("ERROR: Stack overflow!\n");
                }
                break;

            case 2: // Pop
                if (!pop(&st, &value)) {
                    printf("ERROR: Stack underflow!\n");
                } else {
                    printf("Popped: %d\n", value);
                }
                break;

            case 3: // Peek
                if (!peek(&st, &value)) {
                    printf("ERROR: Stack underflow!\n");
                } else {
                    printf("Top: %d\n", value);
                }
                break;

            case 4: // Swap
                if (st.size < 2) {
                    printf("ERROR: Not enough elements to swap!\n");
                } else {
                    swap(&st);
                }
                break;

            case 5: {
                int res = duplicate(&st);
                if (res == 0) {
                    printf("ERROR: Cannot duplicate!\n");
                } else if (res == -1) {
                    printf("ERROR: Stack overflow!\n");
                }
                break;
            }

            case 9: // Debug
                debug_stack(st);
                break;

            case 0: // Exit
                free(st.data);
                return 0;

            default:
                printf("Invalid choice\n");
        }
    }
}
