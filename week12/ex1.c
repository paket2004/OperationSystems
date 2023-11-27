#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

// Function to print keyboard events
void print_event(const struct input_event *event) {
    const char *event_type = "";
  switch (event->type) {
        case EV_KEY:
            event_type = "KEY";
            break;
        default:
            // Ignore other event types except EV_KEY
            return;
    }
  const char *action = "";
  // Copy current event to the action varialbe
    if (event->value == 0) {
        action = "RELEASED";
    } else if (event->value == 1) {
        action = "PRESSED";
    } else if (event->value == 2) {
        action = "REPEATED";
    }
  // Formated output
    printf("%s %s 0x%04x (%d)\n", event_type, action, event->code, event->code);
}

int main() {
    // Path to the keyboard device on my system
    const char *dev_path = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
  // Open the keyboard device
  int fd = open(dev_path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening device");
        exit(EXIT_FAILURE);
    }
  // Available shortcuts for this program
    printf("Available shortcuts: 'P'+'E', 'C'+'A'+'P', and 'I + L + O + S'\n");
  // Structure to hold input events
    struct input_event event;
  // Variables to prodive functionality of the shorcuts
    int button_p = 0;
    int button_e = 0;
    int button_c = 0;
    int button_a = 0;
    int button_i = 0;
    int button_l = 0;
    int button_o = 0;
    int button_s = 0;

    while (1) {
        // Read input events from the keyboard device
        ssize_t bytesRead = read(fd, &event, sizeof(struct input_event));
    if (bytesRead == -1) {
            perror("Error reading event");
            break;
        } else if (bytesRead == sizeof(struct input_event)) {
            // Check if the event is a key event AND is it either pressed, repeated, or released to perform further actions
            if (event.type == EV_KEY && (event.value == 0 || event.value == 1 || event.value == 2)) {
                // Print the key event
                print_event(&event);
        // Check for pressing the choosed key and assign special value for that variable
                if (event.code == KEY_P) {
                    button_p = event.value;
                } else if (event.code == KEY_E) {
                    button_e = event.value;
                } else if (event.code == KEY_C) {
                    button_c = event.value;
                } else if (event.code == KEY_A) {
                    button_a = event.value;
                } else if (event.code == KEY_O) {
                    button_o = event.value;
                } else if (event.code == KEY_I) {
                    button_i = event.value;
                } else if (event.code == KEY_L) {
                    button_l = event.value;
                } else if (event.code == KEY_S) {
                    button_s = event.value;
                }
        // Check for the available shortcuts.
                if (button_p && button_e) {
                    printf("Shortcut: P + E -> \"I passed the Exam!\"\n");
                } else if (button_c && button_a && button_p) {
                    printf("Shortcut: C + A + P -> Get some cappuccino!\n");
                } else if (button_i && button_l && button_o && button_s) {
                    printf("Shortcut: I + L + O + S -> I love OS!\n");
                }
            }
        }
    }
  // Close the keyboard device
    close(fd);
    return 0;
}
