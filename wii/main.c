#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

// Standard Wii Homebrew boilerplate
static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

// Macro definitions from your original code
#define NEXT break
#define guard(n) asm volatile("#" #n)

// Your original benchmark logic, moved into its own function
void run_benchmark() {
    static int prog[] = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5};
    int *ip = prog;
    int count = 100000000;

    for (;;) {
        switch (*ip++) {
            case 0:
                guard(0);
                NEXT;
            case 1:
                guard(1);
                NEXT;
            case 2:
                guard(2);
                NEXT;
            case 3:
                guard(3);
                NEXT;
            case 4:
                guard(4);
                NEXT;
            case 5:
                if (count > 0) {
                    count--;
                    ip = prog;
                    NEXT;
                } else {
                    // Instead of exit(), we just return from the function
                    return;
                }
            /* The rest is to get gcc to make a realistic switch statement */
            case 12:
            case 8:
                count = 25;
                NEXT;
            case 10:
                count--;
                NEXT;
        }
    }
}

// Main function for the Wii application
int main(int argc, char **argv) {
    // 1. Initialize video and console
    VIDEO_Init();
    WPAD_Init();
    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    // 2. Print a start message
    printf("\n\nHello Wii! Preparing to run benchmark...\n");
    printf("Press the HOME button to exit at any time.\n\n");
    
    // Give the user a moment to read
    // sleep(2);

    printf("Starting benchmark (100,000,000 iterations)...\n");

    // 3. Run your code
    run_benchmark();
    
    // 4. Print a finish message
    printf("Benchmark finished!\n");
    printf("The application will now loop. Press HOME to exit.\n");

    // 5. Loop forever, waiting for the user to press the HOME button
    while (1) {
        // Scan for controller input
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);

        // If HOME button is pressed, exit the loop
        if (pressed & WPAD_BUTTON_HOME) {
            break;
        }

        // Wait for the next vertical blank
        VIDEO_WaitVSync();
    }

    // The program will exit cleanly back to the Homebrew Channel
    return 0;
}
