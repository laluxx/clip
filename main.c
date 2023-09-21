#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define TEMP_FILE "/tmp/output.mkv"
#define FFMPEG_CMD_FORMAT "ffmpeg -video_size 1920x1080 -framerate 25 -f x11grab -i :0.0 -f alsa -ac 2 -i default %s -y &"
#define OUTPUT_DIR "~/Videos/clips/"
#define CMD_MAX_LEN 512

void save_clip() {
    char filepath[CMD_MAX_LEN];
    char cmd[CMD_MAX_LEN];
    time_t now;
    struct tm *tm_info;

    time(&now);
    tm_info = localtime(&now);

    if (!tm_info) {
        perror("Error retrieving local time");
        return;
    }

    if (strftime(filepath, sizeof(filepath), OUTPUT_DIR "%Y%m%d%H%M%S_clip.mkv", tm_info) == 0) {
        fprintf(stderr, "Error formatting time string\n");
        return;
    }

    if (snprintf(cmd, sizeof(cmd), "mv %s %s", TEMP_FILE, filepath) >= sizeof(cmd)) {
        fprintf(stderr, "Command buffer overflow\n");
        return;
    }

    if (system(cmd) == -1) {
        perror("Error executing command");
    }
}

int main() {
    Display *display;
    Window root;
    XEvent ev;
    char cmd[CMD_MAX_LEN];

    if (snprintf(cmd, sizeof(cmd), FFMPEG_CMD_FORMAT, TEMP_FILE) >= sizeof(cmd)) {
        fprintf(stderr, "Command buffer overflow\n");
        return 1;
    }

    if (!(display = XOpenDisplay(NULL))) {
        fprintf(stderr, "Unable to open display\n");
        return 1;
    }

    root = DefaultRootWindow(display);
    XGrabKey(display, XKeysymToKeycode(display, XK_space), ControlMask, root, True, GrabModeAsync, GrabModeAsync);
    XSelectInput(display, root, KeyPressMask);

    // Start initial recording
    if (system(cmd) == -1) {
        perror("Error executing command");
        XCloseDisplay(display);
        return 1;
    }

    while (1) {
        XNextEvent(display, &ev);
        if (ev.type == KeyPress) {
            printf("Ctrl + Space pressed\n");
            // Save the clip and restart the recording
            save_clip();
            if (system(cmd) == -1) {
                perror("Error executing command");
            }
        }
    }

    XCloseDisplay(display);
    return 0;
}










// kinda work
/* #include <X11/Xlib.h> */
/* #include <X11/keysym.h> */
/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <unistd.h> */
/* #include <time.h> */

/* #define FFMPEG_CMD "ffmpeg -video_size 1920x1080 -framerate 25 -f x11grab -i :0.0 -f alsa -ac 2 -i default %s -y &" */
/* #define OUTPUT_DIR "~/Videos/clips/" */
/* #define CMD_MAX_LEN 512 */

/* void save_clip() { */
/*     char filepath[CMD_MAX_LEN]; */
/*     char cmd[CMD_MAX_LEN]; */
/*     time_t now; */
/*     struct tm *tm_info; */

/*     time(&now); */
/*     tm_info = localtime(&now); */
/*     if (!tm_info) { */
/*         perror("Error retrieving local time"); */
/*         return; */
/*     } */

/*     if (strftime(filepath, sizeof(filepath), OUTPUT_DIR "%Y%m%d%H%M%S_clip.mkv", tm_info) == 0) { */
/*         fprintf(stderr, "Error formatting time string\n"); */
/*         return; */
/*     } */

/*     // Construct the command to save the clip */
/*     if (snprintf(cmd, sizeof(cmd), FFMPEG_CMD, filepath) >= sizeof(cmd)) { */
/*         fprintf(stderr, "Command buffer overflow\n"); */
/*         return; */
/*     } */

/*     if (system(cmd) == -1) { */
/*         perror("Error executing command"); */
/*     } */
/* } */

/* int main() { */
/*     Display *display; */
/*     Window root; */
/*     XEvent ev; */

/*     if (!(display = XOpenDisplay(NULL))) { */
/*         fprintf(stderr, "Unable to open display\n"); */
/*         return 1; */
/*     } */

/*     root = DefaultRootWindow(display); */
/*     XGrabKey(display, XKeysymToKeycode(display, XK_space), ControlMask, root, True, GrabModeAsync, GrabModeAsync); */
/*     XSelectInput(display, root, KeyPressMask); */

/*     // Start initial recording */
/*     save_clip(); */

/*     while (1) { */
/*         XNextEvent(display, &ev); */
/*         if (ev.type == KeyPress) { */
/*             printf("Ctrl + Space pressed\n"); */
/*             // Save the clip and restart the recording */
/*             save_clip(); */
/*         } */
/*     } */

/*     // In case of program termination by external signals, close the display properly. */
/*     XCloseDisplay(display); */
/*     return 0; */
/* } */
