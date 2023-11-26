#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

void print_event(const struct input_event *ev) {
    const char *event_type = "";

    switch (ev->type) {
        case EV_KEY:
            event_type = "KEY";
            break;
        default:
            // Ignore other event types
            return;
    }

    const char *action = "";

    if (ev->value == 0) {
        action = "RELEASED";
    } else if (ev->value == 1) {
        action = "PRESSED";
    } else if (ev->value == 2) {
        action = "REPEATED";
    }

    printf("%s %s 0x%04x (%d)\n", event_type, action, ev->code, ev->code);
}

int main() {
    const char *dev_path = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";

    int fd = open(dev_path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening device");
        exit(EXIT_FAILURE);
    }

    printf("Listening for keyboard events. Press 'E' and 'X' simultaneously to exit.\n");

    struct input_event ev;

    int e_pressed = 0;
    int x_pressed = 0;

    while (1) {
        ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));

        if (bytesRead == -1) {
            perror("Error reading event");
            break;
        } else if (bytesRead == sizeof(struct input_event)) {
            if (ev.type == EV_KEY && (ev.value == 0 || ev.value == 1 || ev.value == 2)) {
                // Print only PRESSED, REPEATED, and RELEASED events
                print_event(&ev);

                // Check for 'E' and 'X' pressed simultaneously to exit
                if (ev.code == KEY_E) {
                    e_pressed = ev.value;
                } else if (ev.code == KEY_X) {
                    x_pressed = ev.value;
                }

                if (e_pressed && x_pressed) {
                    printf("Exiting...\n");
                    break;
                }
            }
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}
