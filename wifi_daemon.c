#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define CHECK_INTERVAL 10
#define LOG_FILE "/tmp/wifi_daemon.log"

volatile sig_atomic_t stop = 0;

void handle_signal(int sig) {
    stop = 1;
}

void log_message(const char *message) {
    FILE *log_fp = fopen(LOG_FILE, "a");
    if (log_fp) {
        fprintf(log_fp, "%s\n", message);
        fclose(log_fp);
    }
}

void send_notification(const char *message) {
    char command[512];
    snprintf(command, sizeof(command), "notify-send \"%s\"", message);
    system(command);
}

void send_best_wifi_reminder() {
    FILE *fp;
    char line[1024];
    char best_network[256] = "";
    int best_signal = -100;

    fp = popen("nmcli -t -f SSID,SIGNAL dev wifi 2>/dev/null", "r");
    if (fp == NULL) {
        log_message("Failed to get Wi-Fi networks.");
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        char network_name[256] = "";
        int signal = -1;

        if (sscanf(line, "%255[^:]:%d", network_name, &signal) == 2) {
            if (signal > best_signal) {
                best_signal = signal;
                strncpy(best_network, network_name, sizeof(best_network) - 1);
                best_network[sizeof(best_network) - 1] = '\0';
            }
        }
    }

    pclose(fp);

    if (best_signal > -100 && strlen(best_network) > 0) {
        char notification_message[512];
        snprintf(notification_message, sizeof(notification_message),
                 "Reminder: The best Wi-Fi network is: %s (Signal: %d)", best_network, best_signal);

        send_notification(notification_message);
        log_message(notification_message);
    } else {
        send_notification("Reminder: No Wi-Fi networks found.");
        log_message("No Wi-Fi networks found.");
    }
}

void daemonize() {
    pid_t pid, sid;

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    sid = setsid();
    if (sid < 0) exit(EXIT_FAILURE);
    if (chdir("/") < 0) exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main() {
    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);

    daemonize();
    log_message("Wi-Fi monitoring daemon started.");

    while (!stop) {
        send_best_wifi_reminder();
        sleep(CHECK_INTERVAL);
    }

    log_message("Wi-Fi monitoring daemon stopping.");
    return 0;
}